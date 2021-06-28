#ifndef PLAYERCONVERTER_SRC_AUDIO_DATA_H_
#define PLAYERCONVERTER_SRC_AUDIO_DATA_H_

#include <cstdint>
#include <vector>
#include <memory>

class AudioData {
public:

    std::uint16_t num_channels;
    std::uint32_t sample_rate;
    std::uint32_t bits_per_sample;
    std::uint32_t number_samples;

    std::vector<char> data;

    AudioData(std::uint16_t, std::uint32_t, std::uint32_t, std::uint32_t);
};

#endif
