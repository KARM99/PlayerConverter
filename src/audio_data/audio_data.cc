#include "../include/audio_data.h"

#include <cstring>
#include <iostream>

#define db(x) std::cerr << (#x) << " = " << (x) << std::endl;

AudioData::AudioData(std::uint16_t num_channels,
                     std::uint32_t sample_rate,
                     std::uint32_t bits_per_sample,
                     std::uint32_t number_samples)
    : num_channels(num_channels), 
      sample_rate(sample_rate),
      bits_per_sample(bits_per_sample),
      number_samples(number_samples)
{
    this->data.resize(uint32_t{number_samples} * num_channels * (bits_per_sample >> 3));
}
