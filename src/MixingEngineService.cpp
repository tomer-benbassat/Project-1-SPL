#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 * Done
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 * Done
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...\n";
    for(int i=0;i<2;i++){
        if(decks[i]){
            delete decks[i]; //that demonstarte how mixingEngine owns tracks of both decks therfore has to delete it to endure no memory leak =)
            decks[i]=nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
/////////DONE
 //my note:
 //again we see this class owns the tracks loaded to the deck and it shows in the memory managment
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "=== Loading Track to Deck ===\n";
    PointerWrapper<AudioTrack> clone = track.clone();
    if(!clone){
        std::cout << "[ERROR] Track: "<< track.get_title() <<" failed to clone\n";
        return -1;
    }
    int target_deck;
    if(!decks[0]&&!decks[1]){ //first
        target_deck=0;
    }
    else{//subsequent
     target_deck = 1-active_deck;
    }
    std::cout << "[Deck Switch] Target deck: " << target_deck << "\n";
    if(decks[target_deck]){
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }
    clone->load();
    clone->analyze_beatgrid();
    if(decks[active_deck] && auto_sync && !can_mix_tracks(clone)){
        sync_bpm(clone);
    }
    //important line below!
    //release clone ownership from PW and transfer it to decks@
    decks[target_deck] = clone.release(); 
    std::cout << "[Load Complete] '" << track.get_title() << "' is now loaded on deck " << target_deck << "\n";
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << target_deck << "\n";
    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 ********DONE
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(!decks[active_deck]){return false;}
    if(!track){return false;}
    int active_deck_bpm = decks[active_deck]->get_bpm();
    int input_bpm = track->get_bpm();
    if(std::abs(active_deck_bpm-input_bpm) <= bpm_tolerance){ return true;}
    return false;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    int original_bpm = track->get_bpm();
    int active_deck_bpm = decks[active_deck]->get_bpm();
    if(decks[active_deck]&&track){
        int new_bpm = ((original_bpm + active_deck_bpm)/2);
        track->set_bpm(new_bpm);
        std::cout << "[Sync BPM] Syncing BPM from "<< original_bpm << " to " << new_bpm << "\n";
    }
    else{
        //added only to match the given output.....
        if(!decks[0] || !decks[1])
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty.";
    }
}


