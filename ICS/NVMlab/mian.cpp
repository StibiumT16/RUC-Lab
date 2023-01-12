#include "mian.h"

#include <fstream>
#include <iostream>
#include <libpmemobj.h>
#include <map>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

const int K = 16;
const int V = 128;
const int S = 32;
const int N = 1E6 + 10;  

POBJ_LAYOUT_BEGIN (stibiumt);
POBJ_LAYOUT_ROOT (stibiumt, struct root);
POBJ_LAYOUT_TOID (stibiumt, struct node);
POBJ_LAYOUT_TOID (stibiumt, struct queue);
POBJ_LAYOUT_END  (stibiumt);

bool do_not_dump = false;

struct node{
    char ch[S][K + V];
};

struct queue{
    int pos;
    TOID(struct node) lst[];

    int push(PMEMobjpool *pop, const string &key, const string &val){
        int i = pos / S, j = pos % S; 
        TX_BEGIN(pop){
            if(!j){
                TOID(struct node) tmp = TX_NEW(struct node);
                memcpy(D_RW(tmp)->ch[0], (key + val).c_str(), K + V);
                lst[i] = tmp;
            }
            else{
                memcpy(D_RW(lst[i])->ch[j], (key + val).c_str(), K + V);
            }
            ++pos; 
        }TX_END
        return 0;
    } 

};

struct root{
    TOID(struct queue) queue;
};

map<string, string> state;

static int qconstruct(PMEMobjpool *pop, void *ptr, void *arg){
    struct queue *q = (struct queue *)ptr;
    q->pos = 0;
    pmemobj_persist(pop, q, sizeof(*q));
    return 0;
}

static int qnew(PMEMobjpool *pop, TOID(struct queue) *q, int nentries){
	return POBJ_ALLOC(pop, q, struct queue, sizeof(struct queue) 
        + nentries * sizeof(TOID(struct node)), qconstruct, &nentries);
}

static inline int file_exists(char const *file) { return access(file, F_OK); }

void mian(std::vector<std::string> args){
    auto filename = args[0].c_str();
    PMEMobjpool *pop;
    if (file_exists(filename) != 0)
        pop = pmemobj_create(filename, POBJ_LAYOUT_NAME (stibiumt), N * 256 , 0666);
    else{
        pop = pmemobj_open(filename, POBJ_LAYOUT_NAME(stibiumt));
        do_not_dump = true;
    }
    if (pop == NULL){
        std::cout << filename << std::endl;
        perror("pmemobj_create");
        return;
    }

    TOID(struct root) root = POBJ_ROOT(pop, struct root);
    struct root *rootp = D_RW(root);
    
    Query q = nextQuery();
    switch (q.type) {
        case Query::SET:    
            qnew(pop, &rootp->queue, N);        
            D_RW(rootp->queue)->push(pop, q.key, q.value);
            break;
        case Query::GET:
            for(int k = 0; k < D_RO(rootp->queue)->pos; ++k){
                int i = k / S, j = k % S;
                    string kv(D_RO(D_RO(rootp->queue)->lst[i])->ch[j]);
                    state[kv.substr(0, 16)] = kv.substr(16, 128);
            }
            if(state.count(q.key))
                q.callback(state[q.key]);
            else 
                q.callback("-");
            break;
        case Query::NEXT:
            for(int k = 0; k < D_RO(rootp->queue)->pos; ++k){
                int i = k / S, j = k % S;
                    string kv(D_RO(D_RO(rootp->queue)->lst[i])->ch[j]);
                    state[kv.substr(0, 16)] = kv.substr(16, 128);
            }
            if(auto it = state.upper_bound(q.key); it!= state.end())
                q.callback(it->first);
            else 
                q.callback("-");
            break;

        default:
            throw std::invalid_argument(std::to_string(q.type));
    }

    
    while (1) {
        q = nextQuery();
        switch (q.type) {
        case Query::SET:        
            D_RW(rootp->queue)->push(pop, q.key, q.value);
            break;
        case Query::GET:
            if(state.count(q.key))
                q.callback(state[q.key]);
            else 
                q.callback("-");
            break;
        case Query::NEXT:
            if(auto it = state.upper_bound(q.key); it!= state.end())
                q.callback(it->first);
            else 
                q.callback("-");
            break;

        default:
            throw std::invalid_argument(std::to_string(q.type));
        }
    }

    pmemobj_close(pop);
}
