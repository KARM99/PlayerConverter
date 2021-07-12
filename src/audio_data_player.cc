#include "audio_data_player.h"


#include <iostream>
#include <exception>
#include <unordered_map>

#include "portaudio.h"

#include "audio_data.h"

#define db(x) std::cerr << (#x) << " = " << (x) << std::endl;

AudioDataPlayer::AudioDataPlayer(AudioData&& ad)
    : ad(std::move(ad)),
      stream(nullptr),
      index(0)
{
}

void AudioDataPlayer::play() {
    if (Pa_Initialize() != paNoError) {
        throw std::exception();
    }

    const std::unordered_map<std::int32_t, PaSampleFormat> format{
        {8, paInt8},
        {16, paInt16},
        {24, paInt24},
        {32, paInt32},
    };

    PaError error = Pa_OpenDefaultStream(&stream,
                                         0,
                                         ad.num_channels,
                                         format.at(ad.bits_per_sample),
                                         ad.sample_rate,
                                         FRAMES_PER_BUFFER,
                                         paCallback,
                                         this);

    if (error != paNoError) {
        throw std::runtime_error("Error while trying to play the audio.");
    }

    if (Pa_StartStream(stream) != paNoError) {
        throw std::runtime_error("Error while trying to play the audio.");
    }

    while (Pa_IsStreamActive(stream)) {
        Pa_Sleep(100);
    }

    if (Pa_CloseStream(stream) != paNoError) {
        throw std::runtime_error("Error while trying to play the audio.");
    }

    if (Pa_Terminate() != paNoError) {
        throw std::runtime_error("Error while trying to play the audio.");
    }
}

int AudioDataPlayer::paCallbackMethod(const void *input_buffer,
                                            void *output_buffer,
                                            unsigned long frames_per_buffer,
                                            const PaStreamCallbackTimeInfo* time_info,
                                            PaStreamCallbackFlags status_flags)
{
    (void) time_info; /* Prevent unused variable warnings. */
    (void) status_flags;
    (void) input_buffer;

    std::uint8_t* out = reinterpret_cast<std::uint8_t*>(output_buffer);
    for (int i = 0; i < frames_per_buffer * ad.num_channels; i++) {
        if (index == ad.TotalSamples()) {
            return paComplete;
        }
        ad.ReadSample(index++, out);
    }
    return paContinue;
}

int AudioDataPlayer::paCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{
    return reinterpret_cast<AudioDataPlayer*>(userData)->paCallbackMethod(inputBuffer,
                                                                          outputBuffer,
                                                                          framesPerBuffer,
                                                                          timeInfo,
                                                                          statusFlags);
}

