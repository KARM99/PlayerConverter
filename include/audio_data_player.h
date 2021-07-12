#ifndef PLAYERCONVERTER_SRC_PLAYER_AUDIO_DATA__PLAYER_H_
#define PLAYERCONVERTER_SRC_PLAYER_AUDIO_DATA__PLAYER_H_

#include "audio_data.h"

#include "portaudio.h"

class AudioDataPlayer {
private:
    AudioData ad;
    PaStream *stream;
    int index = 0;

    static constexpr int FRAMES_PER_BUFFER = 64;
public:
    AudioDataPlayer(AudioData&&);
    void play();
private:
    int paCallbackMethod(const void *inputBuffer,
                               void *outputBuffer,
                               unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags);
    static int paCallback(const void *inputBuffer,
                                void *outputBuffer,
                                unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData);
};

#endif
