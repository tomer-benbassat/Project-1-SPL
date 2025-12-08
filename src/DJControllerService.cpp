#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}


/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(cache.contains(track.get_title())){ //HIT
        cache.get(track.get_title());//access_counter++
        return 1;
    }
    else{ //MISS
        //clone returns pointerWrapper
        PointerWrapper<AudioTrack> clone = track.clone();
        if(!clone){
            //I wrote the error from the assigment forum.
            //in the instructions they say to print: "[ERROR] Track: "<title>" failed to clone", i feel like its the same
            std::cout << "[DJControllerService] Error: Track cloning failed for" << track.get_title() << "/n";
            return 0;
        }
        clone->load();
        clone->analyze_beatgrid();
        bool eviction = cache.put(std::move(clone)); //reminder: the only way to create PW is by rvalue
        //now new PW owns track and move constructor we implemented make clone rvalue and make it null --> deleted when out of scope :)))))
        if(eviction) { return -1; }
    }
    return 0;
}



void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    AudioTrack* track = cache.get(track_title);
    if(!track){ return nullptr; }
    return track; 
}