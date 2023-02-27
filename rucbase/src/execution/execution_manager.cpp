#include "execution_manager.h"

#include "executor_delete.h"
#include "executor_index_scan.h"
#include "executor_insert.h"
#include "executor_nestedloop_join.h"
#include "executor_projection.h"
#include "executor_seq_scan.h"
#include "executor_update.h"
#include "index/ix.h"
#include "record_printer.h"

TabCol QlManager::check_column(const std::vector<ColMeta> &all_cols, TabCol target) {
    if (target.tab_name.empty()) {
        // Table name not specified, infer table name from column name
        std::string tab_name;
        for (auto &col : all_cols) {
            if (col.name == target.col_name) {
                if (!tab_name.empty()) {
                    throw AmbiguousColumnError(target.col_name);
                }
                tab_name = col.tab_name;
            }
        }
        if (tab_name.empty()) {
            throw ColumnNotFoundError(target.col_name);
        }
        target.tab_name = tab_name;
    } else {
        // Make sure target column exists
        if (!(sm_manager_->db_.is_table(target.tab_name) &&
              sm_manager_->db_.get_table(target.tab_name).is_col(target.col_name))) {
            throw ColumnNotFoundError(target.tab_name + '.' + target.col_name);
        }
    }
    return target;
}

std::vector<ColMeta> QlManager::get_all_cols(const std::vector<std::string> &tab_names) {
    std::vector<ColMeta> all_cols;
    for (auto &sel_tab_name : tab_names) {
        // 这里db_不能写成get_db(), 注意要传指针
        const auto &sel_tab_cols = sm_manager_->db_.get_table(sel_tab_name).cols;
        all_cols.insert(all_cols.end(), sel_tab_cols.begin(), sel_tab_cols.end());
    }
    return all_cols;
}

std::vector<Condition> QlManager::check_where_clause(const std::vector<std::string> &tab_names,
                                                     const std::vector<Condition> &conds) {
    auto all_cols = get_all_cols(tab_names);
    // Get raw values in where clause
    std::vector<Condition> res_conds = conds;
    for (auto &cond : res_conds) {
        // Infer table name from column name
        cond.lhs_col = check_column(all_cols, cond.lhs_col);
        if (!cond.is_rhs_val) {
            cond.rhs_col = check_column(all_cols, cond.rhs_col);
        }
        TabMeta &lhs_tab = sm_manager_->db_.get_table(cond.lhs_col.tab_name);
        auto lhs_col = lhs_tab.get_col(cond.lhs_col.col_name);
        ColType lhs_type = lhs_col->type;
        ColType rhs_type;
        if (cond.is_rhs_val) {
            cond.rhs_val.init_raw(lhs_col->len);
            rhs_type = cond.rhs_val.type;
        } else {
            TabMeta &rhs_tab = sm_manager_->db_.get_table(cond.rhs_col.tab_name);
            auto rhs_col = rhs_tab.get_col(cond.rhs_col.col_name);
            rhs_type = rhs_col->type;
        }
        if (lhs_type != rhs_type) {
            throw IncompatibleTypeError(coltype2str(lhs_type), coltype2str(rhs_type));
        }
    }
    return res_conds;
}

int QlManager::get_indexNo(std::string tab_name, std::vector<Condition> curr_conds) {
    int index_no = -1;
    TabMeta &tab = sm_manager_->db_.get_table(tab_name);
    for (auto &cond : curr_conds) {
        if (cond.is_rhs_val && cond.op != OP_NE) {
            // If rhs is value and op is not "!=", find if lhs has index
            auto lhs_col = tab.get_col(cond.lhs_col.col_name);
            if (lhs_col->index) {
                // This column has index, use it
                index_no = lhs_col - tab.cols.begin();
                break;
            }
        }
    }
    return index_no;
}

void QlManager::insert_into(const std::string &tab_name, std::vector<Value> values, Context *context) {
    // lab3 task3 Todo
    // make InsertExecutor
    // call InsertExecutor.Next()
    auto insert_executor = std::make_unique<InsertExecutor>(sm_manager_, tab_name, values, context);
    insert_executor->Next();
    // lab3 task3 Todo end
}

void QlManager::delete_from(const std::string &tab_name, std::vector<Condition> conds, Context *context) {
    // Parse where clause
    conds = check_where_clause({tab_name}, conds);
    // Get all RID to delete
    std::vector<Rid> rids;
    // make scan executor
    std::unique_ptr<AbstractExecutor> scanExecutor;
    // lab3 task3 Todo
    // 根据get_indexNo判断conds上有无索引
    int index_no = get_indexNo(tab_name, conds);
    // 创建合适的scan executor(有索引优先用索引)
    if(~index_no) scanExecutor = std::make_unique<IndexScanExecutor>(sm_manager_, tab_name, conds, index_no, context);
    else  scanExecutor = std::make_unique<SeqScanExecutor>(sm_manager_, tab_name, conds, context);
    // lab3 task3 Todo end

    for (scanExecutor->beginTuple(); !scanExecutor->is_end(); scanExecutor->nextTuple()) {
        rids.push_back(scanExecutor->rid());
    }

    // lab3 task3 Todo
    // make deleteExecutor
    // call deleteExecutor.Next()
    auto delete_executor = std::make_unique<DeleteExecutor>(sm_manager_, tab_name, conds, rids, context);
    delete_executor->Next();
    // lab3 task3 Todo end
}

void QlManager::update_set(const std::string &tab_name, std::vector<SetClause> set_clauses,
                           std::vector<Condition> conds, Context *context) {
    TabMeta &tab = sm_manager_->db_.get_table(tab_name);
    // Parse where clause
    conds = check_where_clause({tab_name}, conds);
    // Get raw values in set clause
    for (auto &set_clause : set_clauses) {
        auto lhs_col = tab.get_col(set_clause.lhs.col_name);
        if (lhs_col->type != set_clause.rhs.type) {
            throw IncompatibleTypeError(coltype2str(lhs_col->type), coltype2str(set_clause.rhs.type));
        }
        set_clause.rhs.init_raw(lhs_col->len);
    }
    // Get all RID to update
    std::vector<Rid> rids;

    // lab3 task3 Todo
    // make scan executor
    int index_no = get_indexNo(tab_name, conds);
    std::unique_ptr<AbstractExecutor> table_scan;
    if(~index_no) table_scan = std::make_unique<IndexScanExecutor>(sm_manager_, tab_name, conds, index_no, context);
    else table_scan = std::make_unique<SeqScanExecutor>(sm_manager_, tab_name, conds, context);
    for (table_scan->beginTuple(); !table_scan->is_end(); table_scan->nextTuple()) {
        rids.push_back(table_scan->rid());
    }
    auto scanExecutor =  std::make_unique<SeqScanExecutor>(sm_manager_, tab_name, conds, context);
    for (scanExecutor->beginTuple(); !scanExecutor->is_end(); scanExecutor->nextTuple()){
        // 将rid存入rids
        rids.push_back(scanExecutor->rid());
        // make updateExecutor
        auto update_executor = std::make_unique<UpdateExecutor>(sm_manager_, tab_name, set_clauses, conds, rids, context);
        // call updateExecutor.Next()
        update_executor->Next();
    }
    // lab3 task3 Todo end
}

/**
 * @brief 表算子条件谓词生成
 *
 * @param conds 条件
 * @param tab_names 表名
 * @return std::vector<Condition>
 */
std::vector<Condition> pop_conds(std::vector<Condition> &conds, const std::vector<std::string> &tab_names) {
    auto has_tab = [&](const std::string &tab_name) {
        return std::find(tab_names.begin(), tab_names.end(), tab_name) != tab_names.end();
    };
    std::vector<Condition> solved_conds;
    auto it = conds.begin();
    while (it != conds.end()) {
        if (has_tab(it->lhs_col.tab_name) && (it->is_rhs_val || has_tab(it->rhs_col.tab_name))) {
            solved_conds.emplace_back(std::move(*it));
            it = conds.erase(it);
        } else {
            it++;
        }
    }
    return solved_conds;
}

/*
std::unique_ptr<AbstractExecutor> build(size_t l, size_t r,  std::vector<std::unique_ptr<AbstractExecutor>> &table_scan_executors) {
    if(l == r) return std::move(table_scan_executors[l]);
    size_t mid = (l + r) >> 1;
    return std::make_unique<NestedLoopJoinExecutor>(std::move(build(l, mid, table_scan_executors)), std::move(build(mid + 1, r, table_scan_executors)));
}
*/

int cmp(std::vector<std::string> a, std::vector<std::string> b, const std::vector<int>& orders, const std::vector<ColType>& types, const std::vector<size_t> & pos){
    for(size_t i = 0; i < pos.size(); ++i){
        auto order = orders[i];
        if(types[pos[i]] == TYPE_INT){
            auto x = std::stoi(a[pos[i]]), y = std::stoi(b[pos[i]]);
            if(x == y) continue;
            if(!order) return  x < y ? -1 : 1; //ASC
            else  return x > y ? -1 : 1; //DESC
        } 
        else if (types[pos[i]] == TYPE_FLOAT){
            auto x = std::stof(a[pos[i]]), y = std::stof(b[pos[i]]);
            if(x == y) continue;
            if(!order) return  x < y ? -1 : 1; //ASC
            else  return x > y ? -1 : 1; //DESC
        }
        else{
            auto x = a[pos[i]], y = b[pos[i]];
            if(x == y) continue;
            if(!order) return  x < y ? -1 : 1; //ASC
            else  return x > y ? -1 : 1; //DESC
        }
    }
    return 0;
}

/**
 * @brief select plan 生成
 *
 * @param sel_cols select plan 选取的列
 * @param tab_names select plan 目标的表
 * @param conds select plan 选取条件
 */
void QlManager::select_from(std::vector<TabCol> sel_cols, const std::vector<std::string> &tab_names,
                            std::vector<Condition> conds, std::vector<TabCol> order_cols, std::vector<int> orders,int top, Context *context) {
    // Parse selector
    auto all_cols = get_all_cols(tab_names);
    if (sel_cols.empty()) {
        // select all columns
        for (auto &col : all_cols) {
            TabCol sel_col = {.tab_name = col.tab_name, .col_name = col.name};
            sel_cols.push_back(sel_col);
        }
    } else {
        // infer table name from column name
        for (auto &sel_col : sel_cols) {
            sel_col = check_column(all_cols, sel_col);  //列元数据校验
        }
    }
    // Parse where clause
    conds = check_where_clause(tab_names, conds);
    // Scan table , 生成表算子列表tab_nodes
    
    std::vector<std::unique_ptr<AbstractExecutor>> table_scan_executors(tab_names.size());
    for (size_t i = 0; i < tab_names.size(); i++) {
        auto curr_conds = pop_conds(conds, {tab_names.begin(), tab_names.begin() + i + 1});
        int index_no = get_indexNo(tab_names[i], curr_conds);
        // lab3 task2 Todo
        // 根据get_indexNo判断conds上有无索引
        // 创建合适的scan executor(有索引优先用索引)存入table_scan_executors
        if(~index_no) table_scan_executors[i] = std::make_unique<IndexScanExecutor>(sm_manager_, tab_names[i], curr_conds, index_no, context);
        else  table_scan_executors[i] = std::make_unique<SeqScanExecutor>(sm_manager_, tab_names[i], curr_conds, context);
        // lab3 task2 Todo end
    }
    assert(conds.empty());
    std::unique_ptr<AbstractExecutor> executorTreeRoot = std::move(table_scan_executors.back());

    // lab3 task2 Todo
    // 构建算子二叉树
    // 逆序遍历tab_nodes为左节点, 现query_plan为右节点,生成joinNode作为新query_plan 根节点
    // 生成query_plan tree完毕后, 根节点转换成投影算子
    for (size_t i = tab_names.size() - 2; i != (size_t) -1; --i) 
        executorTreeRoot = std::make_unique<NestedLoopJoinExecutor>(std::move(table_scan_executors[i]), std::move(executorTreeRoot));
    executorTreeRoot = std::make_unique<ProjectionExecutor>(std::move(executorTreeRoot), sel_cols);
    // lab3 task2 Todo End
    //std::unique_ptr<AbstractExecutor> executorTreeRoot = build(0, table_scan_executors.size() - 1, table_scan_executors);
    //executorTreeRoot = std::make_unique<ProjectionExecutor>(std::move(executorTreeRoot), sel_cols);

    // Column titles
    std::vector<std::string> captions;
    captions.reserve(sel_cols.size());
    for (auto &sel_col : sel_cols)  captions.push_back(sel_col.col_name);

    // Print header
    RecordPrinter rec_printer(sel_cols.size());
    rec_printer.print_separator(context);
    rec_printer.print_record(captions, context);
    rec_printer.print_separator(context);

    // Print records
    size_t num_rec = 0;
    std::vector<ColType> record_types;
    for (auto &col : executorTreeRoot->cols()) 
        record_types.push_back(col.type);
        
   // 执行query_plan
    std::vector<std::vector <std::string> > records;
    for (executorTreeRoot->beginTuple(); !executorTreeRoot->is_end(); executorTreeRoot->nextTuple()) {
        auto Tuple = executorTreeRoot->Next();
        std::vector<std::string> columns;
        for (auto &col : executorTreeRoot->cols()) {
            std::string col_str;
            char *rec_buf = Tuple->data + col.offset;
            if (col.type == TYPE_INT) {
                col_str = std::to_string(*(int *)rec_buf);
            } else if (col.type == TYPE_FLOAT) {
                col_str = std::to_string(*(float *)rec_buf);
            } else if (col.type == TYPE_STRING) {
                col_str = std::string((char *)rec_buf, col.len);
                col_str.resize(strlen(col_str.c_str()));
            }
            columns.push_back(col_str);
        }
        records.push_back(columns);
        num_rec++;
    }
    size_t pos;
    
    //quick sort
    if(!order_cols.empty()){
        std::vector<size_t> poslist;
        for(size_t k =0 ; k < order_cols.size(); ++k){
            auto order_col = order_cols[k];
            for(pos = 0; pos < captions.size(); ++pos)
                if(captions[pos] == order_col.col_name){
                    poslist.push_back(pos);
                    break;
                }
        }

        std::function<void(int, int)> Sort = [&](int l, int r){
            auto m = records[(l + r) >> 1];
            int i = l,j = r;
            while(i < j){
                while(cmp(records[i], m, orders, record_types, poslist) < 0) i++;
                while(cmp(records[j], m, orders, record_types, poslist) > 0) j--;
                if(i <= j) swap(records[i], records[j]), i++, j--;
            }
            if(l < j) Sort(l, j);
            if(i < r) Sort(i, r);
        };

        Sort(0, records.size() - 1);
    }

    /*
    //brute force sort
    for(size_t k =0 ; k < order_cols.size(); ++k){
        auto order_col = order_cols[k];
        int ordertype = orders[k];
        for(pos = 0; pos < captions.size(); ++pos)
            if(captions[pos] == order_col.col_name)
                break;
        if(pos == captions.size()) continue;
        ColType type = record_types[pos];
        for(size_t i = 0; i < records.size(); ++i)
            for(size_t j = i + 1; j < records.size(); ++j){
                //ASC
                if(!ordertype){
                    if(type == TYPE_INT){
                        if(std::stoi(records[i][pos]) > std::stoi(records[j][pos])) 
                            swap(records[i], records[j]);
                    }
                    else if (type == TYPE_FLOAT){
                         if(std::stof(records[i][pos]) > std::stof(records[j][pos])) 
                            swap(records[i], records[j]);
                    }
                    else {
                        if(records[i][pos] > records[j][pos]) 
                            swap(records[i], records[j]);
                    }
                } else { //DESC
                    if(type == TYPE_INT){
                        if(std::stoi(records[i][pos]) < std::stoi(records[j][pos])) 
                            swap(records[i], records[j]);
                    }
                    else if (type == TYPE_FLOAT){
                         if(std::stof(records[i][pos]) < std::stof(records[j][pos])) 
                            swap(records[i], records[j]);
                    }
                    else {
                        if(records[i][pos] < records[j][pos]) 
                            swap(records[i], records[j]);
                    }
                }
            }
    }
    */

    for(pos = 0; pos < records.size() && (!~top || pos < (size_t)top); ++pos)
        rec_printer.print_record(records[pos], context);

    // Print footer
    rec_printer.print_separator(context);
    // Print record count
    RecordPrinter::print_record_count(pos, context);
}
