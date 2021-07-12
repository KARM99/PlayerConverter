#include "audio_data.h"

#include <cstring>
#include <iostream>
#include <cstdint>

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
    this->data.resize(uint32_t{number_samples} * num_channels * BytesPerSample());
}

AudioData::AudioData(AudioData&& other)
  : num_channels(other.num_channels), 
    sample_rate(other.sample_rate),
    bits_per_sample(other.bits_per_sample),
    number_samples(other.number_samples),
    data(std::move(other.data))
{ }

std::uint8_t AudioData::BytesPerSample() const {
  return bits_per_sample / 8;
}

void AudioData::ReadSample(int index, std::uint8_t* &out) const{
  for (int j = 0; j < BytesPerSample(); j++) {
    *out++ = data[index * BytesPerSample() + j];
  }
}
std::uint32_t AudioData::TotalSamples() const {
  return number_samples * num_channels;
}
