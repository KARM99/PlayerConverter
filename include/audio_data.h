#ifndef PLAYERCONVERTER_SRC_AUDIO_DATA_H_
#define PLAYERCONVERTER_SRC_AUDIO_DATA_H_

#include <cstdint>
#include <vector>
#include <memory>
#include <cstdint>


class AudioData {
public:

    std::uint16_t num_channels;
    std::uint32_t sample_rate;
    std::uint32_t bits_per_sample;
    std::uint32_t number_samples;

    std::vector<std::uint8_t> data;

    AudioData(std::uint16_t, std::uint32_t, std::uint32_t, std::uint32_t);
    AudioData(AudioData&&);

    std::uint8_t BytesPerSample() const;
    std::uint32_t TotalSamples() const;

    void ReadSample(int index, std::uint8_t* &out) const;
};

#endif
