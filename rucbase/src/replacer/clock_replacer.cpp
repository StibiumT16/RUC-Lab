#include "replacer/clock_replacer.h"

#include <algorithm>

ClockReplacer::ClockReplacer(size_t num_pages)
    : circular_{num_pages, ClockReplacer::Status::EMPTY_OR_PINNED}, hand_{0}, capacity_{num_pages} {
    // 成员初始化列表语法
    circular_.reserve(num_pages);
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    // Todo: try to find a victim frame in buffer pool with clock scheme
    // and make the *frame_id = victim_frame_id
    // not found, frame_id=nullptr and return false
    frame_id_t victim_frame_id = 0;
    bool chk = 0;
    for(size_t i = 1, cur = (hand_ + i) % capacity_; i <= capacity_; i++, cur = (hand_ + i) % capacity_){
        if(circular_[cur] == ClockReplacer::Status::ACCESSED){
            chk = 1;
            circular_[cur] = ClockReplacer::Status::UNTOUCHED;
        }
        else if(circular_[cur] == ClockReplacer::Status::UNTOUCHED){
            chk = 1;
            victim_frame_id = victim_frame_id ? victim_frame_id : cur;
        }
    }
    if(!chk){
        frame_id = nullptr;
        return false;
    }
    if(!victim_frame_id)
        for(size_t i = 1, cur = (hand_ + i) % capacity_; i <= capacity_; i++, cur = (hand_ + i) % capacity_){
            if(circular_[cur] == ClockReplacer::Status::UNTOUCHED){
                victim_frame_id = cur;
                break;
            }
        }
    circular_[(hand_ = *frame_id = victim_frame_id) % capacity_] = ClockReplacer::Status::EMPTY_OR_PINNED;
    return true;
}

void ClockReplacer::Pin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    // Todo: you can implement it!
    circular_[frame_id % capacity_] = ClockReplacer::Status::EMPTY_OR_PINNED;
}

void ClockReplacer::Unpin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    // Todo: you can implement it!
    circular_[frame_id % capacity_] = ClockReplacer::Status::ACCESSED;
}

size_t ClockReplacer::Size() {

    // Todo:
    // 返回在[arg0, arg1)范围内满足特定条件(arg2)的元素的数目
    // return all items that in the range[circular_.begin, circular_.end )
    // and be met the condition: status!=EMPTY_OR_PINNED
    // That is the number of frames in the buffer pool that storage page (NOT EMPTY_OR_PINNED)
    const std::lock_guard<mutex_t> guard(mutex_);
    size_t res = 0;
    for(auto x : circular_)  res += x != ClockReplacer::Status::EMPTY_OR_PINNED;
    return res;
}
