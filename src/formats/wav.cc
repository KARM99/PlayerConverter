#include <string>
#include <fstream>
#include <iostream>

#include "audio_format.h"
#include "audio_data.h"

// http://soundfile.sapp.org/doc/WaveFormat/
// the names with capitals should always be constant
struct WAV_HEADER {
    char                RIFF[4];            // Contains the letters "RIFF" in ASCII form
    std::uint32_t       chunk_size;         // 4 + (8 + SubChunk1Size[16]) + (8 + SubChunk2Size)  
    char                WAVE[4];            // Contains the letters "WAVE"     
    char                FMT[4];             // FMT header       
    std::uint32_t       subchunk_1_size;    // Size of the fmt chunk                                
    std::uint16_t       audio_format;       // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
    std::uint16_t       num_of_chan;        // Number of channels 1=Mono 2=Sterio                   
    std::uint32_t       sample_rate;        // Sampling Frequency in Hz                             
    std::uint32_t       bytes_per_sec;      // bytes per second 
    std::uint16_t       block_align;        // 2=16-bit mono, 4=16-bit stereo 
    std::uint16_t       bits_per_sample;    // Number of bits per sample      
    char                DATA[4];            // "data"  string   
    std::uint32_t       subchunk_2_size;    // Sampled data length
};


class WAV : public AudioFormat {
public:
    AudioData AudioRead(const char* filename) {
        WAV_HEADER header;
        std::ifstream file {filename, std::ios::binary};

        file.read(reinterpret_cast<char*>(&header), sizeof(header));

        AudioData ad(header.num_of_chan,
                    header.sample_rate,
                    header.bits_per_sample,
                    header.subchunk_2_size / header.num_of_chan / (header.bits_per_sample >> 3));
        file.read(reinterpret_cast<char*>(&ad.data[0]), ad.data.size());
        return ad;
    }

    void AudioWrite(const AudioData& ad, const char* filename) {
        WAV_HEADER header {
            {'R', 'I', 'F', 'F'},
            uint32_t{36} + uint32_t{ad.data.size()},
            {'W', 'A', 'V', 'E'},
            {'f', 'm', 't', ' '} ,
            16,
            1,
            ad.num_channels,
            ad.sample_rate,
            std::uint32_t{ad.sample_rate} * uint32_t{ad.num_channels} * uint32_t{ad.bits_per_sample >> 3},
            ad.num_channels * (ad.bits_per_sample >> 3),
            ad.bits_per_sample,
            {'d', 'a', 't', 'a'},
            ad.data.size()
        };
        std::ofstream file {filename, std::ios::binary};
        file.write(reinterpret_cast<char*>(&header), sizeof(header));
        file.write(reinterpret_cast<const char*>(&ad.data[0]), ad.data.size());
    }
};

extern "C" std::unique_ptr<AudioFormat> create() {
    return std::make_unique<WAV>();
}
