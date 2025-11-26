#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    if(!track) return false;
    size_t index = findSlot(track.get()->get_title());
    if(index!=max_size){ //track with the smae title already exists
        slots[index].access(++access_counter); //update acceess time
        return false;
    }
    bool eviction = false;
    if(isFull()) {
        evictLRU();
        eviction = true; 
    }
    int emptySlot = findEmptySlot();
    //we must use rvalue for this bc we dleted copy constructors bc it damages all wrap idea.
    //so we use std::move (which showed in tirgul prsentation and took me only 20 minutes to find :))))
    //std::move convert lvalue aka track to rvalue in order for us to use move constructor of PointerWrapper as we should
    //store function already update access time to access counter and mark occupied flag
    //++ AC before storing it in order to keep LRU model
    slots[emptySlot].store(std::move(track),++access_counter); 
    return eviction;
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied()) return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * TODO: Implement the findLRUSlot() method for LRUCache
 */
size_t LRUCache::findLRUSlot() const {
    uint64_t min = access_counter;
    size_t index=max_size;
    for(size_t i=0;i<slots.size();i++){
       if(slots[i].isOccupied() && slots[i].getLastAccessTime()<min){
         min = slots[i].getLastAccessTime();
         index=i;
       }
    }
    return index;
}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}