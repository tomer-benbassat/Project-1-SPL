#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
      waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

AudioTrack::~AudioTrack() {
    // TODO: Implement the destructor
    //deleted the pointer field
    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    #endif
    delete [] waveform_data;
}

AudioTrack::AudioTrack(const AudioTrack& other): title(other.title),artists(other.artists),
duration_seconds(other.duration_seconds),bpm(other.bpm),waveform_size(other.waveform_size),
waveform_data(nullptr)
{
    // TODO: Implement the copy constructor
    //done. allocate memory for pointer field
    #ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
    #endif
    double* clone = new double[waveform_size];
        for (size_t i = 0; i < waveform_size; i++) {
           clone[i] = other.waveform_data[i];
        }
    waveform_data = clone;
}

AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    // TODO: Implement the copy assignment operator
    //done. delete current memory for pointer field + allocate new memory and deep copy from other.
    #ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    #endif
    if(&other!=this){
        title=other.title;
        artists=other.artists;
        duration_seconds=other.duration_seconds;
        bpm=other.bpm;
        waveform_size=other.waveform_size;
        delete[] waveform_data; //delete current data
         //assign new data using deep copy for pointer field
        waveform_data = new double[waveform_size];
        for (size_t i = 0; i < waveform_size; i++) {
            waveform_data[i] = other.waveform_data[i];
        }

    }
    return *this;
}
/*make sure to use std::move on expensive to create objects*/
AudioTrack::AudioTrack(AudioTrack&& other) noexcept : title(std::move(other.title)),
artists(std::move(other.artists)),duration_seconds(other.duration_seconds),bpm(other.bpm),
waveform_size(other.waveform_size),
//steal rvalue memory:
waveform_data(other.waveform_data) {
    // TODO: Implement the move constructor
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif
    //empty the source
    other.waveform_data=nullptr;
    other.waveform_size=0;
}

AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    // TODO: Implement the move assignment operator

    #ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    #endif
    if(&other!=this){
        //deallocate
        delete[] waveform_data;
        /*use std::move on expensive to create stractures (String,Vector)*/
        title=std::move(other.title);
        artists=std::move(other.artists);
        duration_seconds=other.duration_seconds;
        bpm=other.bpm;
        waveform_size=other.waveform_size;
        /*copy the pointer "steal"*/
        waveform_data = other.waveform_data;
        //make sure old one cant access it "empty"
        other.waveform_data=nullptr;
        other.waveform_size=0;
    }
    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}