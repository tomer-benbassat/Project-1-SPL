#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>

//constructor
DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}

//==================addition:destructor======================
  DJLibraryService::~DJLibraryService(){
    //playlist alray has destructor
    //library
    for(size_t i=0;i<library.size();i++){
        delete library[i];
    }
    //library vector seleted by default
  }
//==================addition:destructor======================

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
//Done
//MYNOTE: notice this services own tracks therfore we allocated memory for tracks. 
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for(size_t i=0;i<library_tracks.size();i++){
        SessionConfig::TrackInfo current_info = library_tracks[i];
        if(current_info.type == "MP3"){
            library.push_back(new MP3Track(current_info.title,current_info.artists, current_info.duration_seconds,current_info.bpm, current_info.extra_param1, current_info.extra_param2));
            std::cout << "MP3Track created:" << current_info.extra_param1 << "kbps";
        }
        else{
            library.push_back(new WAVTrack(current_info.title,current_info.artists, current_info.duration_seconds,current_info.bpm, current_info.extra_param1, current_info.extra_param2));
            std::cout << "WAVTrack created:" << current_info.extra_param1 <<"HZ/" << current_info.extra_param2<< "bit";
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << "tracks loaded";
}




/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * HINT: Leverage Playlist's find_track method
 * Done
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}



//Done
//*********************Did it at 5AM so its better to test it well**************** 
void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
   std::cout << "[INFO] Loading playlist:" << playlist_name << "\n";
   playlist = Playlist(playlist_name);
   for(size_t i=0;i< track_indices.size();i++){
        size_t index = track_indices[i];
        if(index>library.size()||index < 1){
           std::cout << "[WARNING] Invalid track index:" << index << "\n"; 
        }
        else{//valid
          PointerWrapper<AudioTrack> clone = library[index-1]->clone();
          if(!clone){
            std::cout << "[ERROR] Track: "<< library[index-1]->get_title() <<" failed to clone\n";
          }
          else{
           clone->load();
           clone->analyze_beatgrid();
           //unwrap and transfer ownership
          playlist.add_track(clone.release());
         std:: cout << "Added '" << library[index-1]->get_title()<< "' to playlist '" << playlist_name << "' \n";
        }
      }
  } 
   std::cout << "[INFO] Playlist loaded: '" << playlist_name << "' "<< playlist.get_track_count() << "tracks\n";
}



/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> output;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for(int i=0;i<playlist.get_track_count();i++){
        output.push_back(tracks[i]->get_title());
    }
    return output; 
}
