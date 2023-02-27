#include "lock_manager.h"

std::list<LockManager::LockRequest>::iterator LockManager::GetFirstWaitRqt_RECORD(const Rid &rid, int tab_fd) {
    LockRequestQueue* lock_queue = &lock_table_[LockDataId(tab_fd, rid, LockDataType::RECORD)];
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (!iter->granted_) break;
    return iter;
}  

std::list<LockManager::LockRequest>::iterator LockManager::GetFirstWaitRqt_TABLE(int tab_fd) {
    LockRequestQueue* lock_queue = &lock_table_[LockDataId(tab_fd, LockDataType::TABLE)];
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (!iter->granted_) break;
    return iter;
}  


/**
 * 申请行级读锁
 * @param txn 要申请锁的事务对象指针
 * @param rid 加锁的目标记录ID
 * @param tab_fd 记录所在的表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockSharedOnRecord(Transaction *txn, const Rid &rid, int tab_fd) {
    // Todo:
    // 1. 通过mutex申请访问全局锁表
    // 2. 检查事务的状态
    // 3. 查找当前事务是否已经申请了目标数据项上的锁，如果存在则根据锁类型进行操作，否则执行下一步操作
    // 4. 将要申请的锁放入到全局锁表中，并通过组模式来判断是否可以成功授予锁
    // 5. 如果成功，更新目标数据项在全局锁表中的信息，否则阻塞当前操作
    // 提示：步骤5中的阻塞操作可以通过条件变量来完成，所有加锁操作都遵循上述步骤，在下面的加锁操作中不再进行注释提示
    std::unique_lock<std::mutex> lock(latch_);
    if (txn->GetState() == TransactionState::ABORTED) return false;
    if (txn->GetIsolationLevel() == IsolationLevel::READ_UNCOMMITTED) {
        txn->SetState(TransactionState::ABORTED);
        return false;
    }
    if (txn->GetState() == TransactionState::SHRINKING) {
        txn->SetState(TransactionState::ABORTED);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
        return false;
    }
    LockRequestQueue *lock_queue = &lock_table_[LockDataId(tab_fd, rid, LockDataType::RECORD)];
    lock_queue->request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::SHARED));
    while (GetFirstWaitRqt_RECORD(rid, tab_fd)->txn_id_ != txn->GetTransactionId() || lock_queue->is_waiting_ || lock_queue->IX_lock_num_ || lock_queue->upgrading_ )
        lock_queue->cv_.wait(lock);
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (iter->txn_id_ == txn->GetTransactionId())  break;
    if (txn->GetState() == TransactionState::ABORTED) {
        lock_queue->request_queue_.erase(iter);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::DEADLOCK_PREVENTION);
        return false;
    }
    iter->granted_ = true;
    lock_queue->shared_lock_num_++;
    txn->SetState(TransactionState::GROWING);
    txn->GetLockSet()->emplace(LockDataId(tab_fd, rid, LockDataType::RECORD));
    return true;
}

/**
 * 申请行级写锁
 * @param txn 要申请锁的事务对象指针
 * @param rid 加锁的目标记录ID
 * @param tab_fd 记录所在的表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockExclusiveOnRecord(Transaction *txn, const Rid &rid, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    if (txn->GetState() == TransactionState::ABORTED) return false;
    if (txn->GetState() == TransactionState::SHRINKING){
        txn->SetState(TransactionState::ABORTED);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
        return false;
    }
    LockRequestQueue *lock_queue = &lock_table_[LockDataId(tab_fd, rid, LockDataType::RECORD)];
    lock_queue->request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::EXLUCSIVE));
    while (lock_queue->request_queue_.front().txn_id_ != txn->GetTransactionId())
        lock_queue->cv_.wait(lock);
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (iter->txn_id_ == txn->GetTransactionId()) break;
    if (txn->GetState() == TransactionState::ABORTED) {
        lock_queue->request_queue_.erase(iter);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::DEADLOCK_PREVENTION);
        return false;
    }
    iter->granted_ = true;
    lock_queue->is_waiting_ = true;
    txn->SetState(TransactionState::GROWING);
    txn->GetLockSet()->emplace(LockDataId(tab_fd, rid, LockDataType::RECORD));
    return true;
}

/**
 * 申请表级读锁
 * @param txn 要申请锁的事务对象指针
 * @param tab_fd 目标表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockSharedOnTable(Transaction *txn, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    if (txn->GetState() == TransactionState::ABORTED) return false;
    if (txn->GetIsolationLevel() == IsolationLevel::READ_UNCOMMITTED) {
        txn->SetState(TransactionState::ABORTED);
        return false;
    }
    if (txn->GetState() == TransactionState::SHRINKING) {
        txn->SetState(TransactionState::ABORTED);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
        return false;
    }
    LockRequestQueue *lock_queue = &lock_table_[LockDataId(tab_fd, LockDataType::TABLE)];
    lock_queue->request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::SHARED));
    while (GetFirstWaitRqt_TABLE(tab_fd)->txn_id_ != txn->GetTransactionId() || lock_queue->is_waiting_ || lock_queue->upgrading_ || lock_queue->IX_lock_num_ )
        lock_queue->cv_.wait(lock);
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (iter->txn_id_ == txn->GetTransactionId())  break;
    if (txn->GetState() == TransactionState::ABORTED) {
        lock_queue->request_queue_.erase(iter);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::DEADLOCK_PREVENTION);
        return false;
    }
    iter->granted_ = true;
    lock_queue->shared_lock_num_++;
    txn->SetState(TransactionState::GROWING);
    txn->GetLockSet()->emplace(LockDataId(tab_fd, LockDataType::TABLE));
    return true;
}

/**
 * 申请表级写锁
 * @param txn 要申请锁的事务对象指针
 * @param tab_fd 目标表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockExclusiveOnTable(Transaction *txn, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    if (txn->GetState() == TransactionState::ABORTED) return false;
    if (txn->GetState() == TransactionState::SHRINKING){
        txn->SetState(TransactionState::ABORTED);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
        return false;
    }
    LockRequestQueue *lock_queue = &lock_table_[LockDataId(tab_fd, LockDataType::TABLE)];
    lock_queue->request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::EXLUCSIVE));
    while (lock_queue->request_queue_.front().txn_id_ != txn->GetTransactionId()) 
        lock_queue->cv_.wait(lock);  
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (iter->txn_id_ == txn->GetTransactionId()) break;
    if (txn->GetState() == TransactionState::ABORTED) {
        lock_queue->request_queue_.erase(iter);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::DEADLOCK_PREVENTION);
        return false;
    }
    iter->granted_ = true;
    lock_queue->is_waiting_ = true;
    txn->SetState(TransactionState::GROWING);
    txn->GetLockSet()->emplace(LockDataId(tab_fd, LockDataType::TABLE));
    return true;
}

/**
 * 申请表级意向读锁
 * @param txn 要申请锁的事务对象指针
 * @param tab_fd 目标表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockISOnTable(Transaction *txn, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    if (txn->GetState() == TransactionState::ABORTED) return false;
    if (txn->GetIsolationLevel() == IsolationLevel::READ_UNCOMMITTED) {
        txn->SetState(TransactionState::ABORTED);
        return false;
    }
    if (txn->GetState() == TransactionState::SHRINKING) {
        txn->SetState(TransactionState::ABORTED);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
        return false;
    }
    LockRequestQueue *lock_queue = &lock_table_[LockDataId(tab_fd, LockDataType::TABLE)];
    lock_queue->request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::INTENTION_SHARED));
    while (GetFirstWaitRqt_TABLE(tab_fd)->txn_id_ != txn->GetTransactionId() || lock_queue->is_waiting_ || lock_queue->upgrading_ )
        lock_queue->cv_.wait(lock);
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (iter->txn_id_ == txn->GetTransactionId())  break;
    if (txn->GetState() == TransactionState::ABORTED) {
        lock_queue->request_queue_.erase(iter);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::DEADLOCK_PREVENTION);
        return false;
    }
    iter->granted_ = true;
    lock_queue->shared_lock_num_++;
    txn->SetState(TransactionState::GROWING);
    txn->GetLockSet()->emplace(LockDataId(tab_fd, LockDataType::TABLE));
    return true;
}

/**
 * 申请表级意向写锁
 * @param txn 要申请锁的事务对象指针
 * @param tab_fd 目标表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockIXOnTable(Transaction *txn, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    if (txn->GetState() == TransactionState::ABORTED) return false;
    if (txn->GetState() == TransactionState::SHRINKING) {
        txn->SetState(TransactionState::ABORTED);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
        return false;
    }
    LockRequestQueue *lock_queue = &lock_table_[LockDataId(tab_fd, LockDataType::TABLE)];
    lock_queue->request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::INTENTION_EXCLUSIVE));
    while (lock_queue->request_queue_.front().txn_id_ != txn->GetTransactionId() || lock_queue->shared_lock_num_)
        lock_queue->cv_.wait(lock);
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (iter->txn_id_ == txn->GetTransactionId()) break;
    if (txn->GetState() == TransactionState::ABORTED) {
        lock_queue->request_queue_.erase(iter);
        throw TransactionAbortException(txn->GetTransactionId(), AbortReason::DEADLOCK_PREVENTION);
        return false;
    }
    iter->granted_ = true;
    lock_queue->IX_lock_num_++;
    txn->SetState(TransactionState::GROWING);
    txn->GetLockSet()->emplace(LockDataId(tab_fd, LockDataType::TABLE));
    return true;
}

/**
 * 释放锁
 * @param txn 要释放锁的事务对象指针
 * @param lock_data_id 要释放的锁ID
 * @return 返回解锁是否成功
 */
bool LockManager::Unlock(Transaction *txn, LockDataId lock_data_id) {
    std::unique_lock<std::mutex> lock(latch_);
    LockRequestQueue *lock_queue = &lock_table_[lock_data_id];
    if(txn->GetLockSet()->find(lock_data_id) == txn->GetLockSet()->end()) return false;
    auto iter = lock_queue->request_queue_.begin();
    for (; iter != lock_queue->request_queue_.end(); ++iter) 
        if (iter->txn_id_ == txn->GetTransactionId()) break;
    LockMode mode = iter->lock_mode_;
    lock_queue->request_queue_.erase(iter);
    if (!((mode == LockMode::SHARED || mode == LockMode::INTENTION_SHARED) && txn->GetIsolationLevel() == IsolationLevel::READ_COMMITTED) && txn->GetState() == TransactionState::GROWING) 
        txn->SetState(TransactionState::SHRINKING);
    txn->GetLockSet()->erase(lock_data_id);
    if (mode == LockMode::SHARED || mode == LockMode::INTENTION_SHARED) {
        if (--lock_queue->shared_lock_num_ == 0) lock_queue->cv_.notify_all();
    } else if(mode == LockMode::EXLUCSIVE){ 
        lock_queue->is_waiting_ = false;
        lock_queue->cv_.notify_all();
    } else if(mode == LockMode::INTENTION_EXCLUSIVE){
        if(--lock_queue->IX_lock_num_ == 0) lock_queue->cv_.notify_all();
    }
    return true;
}