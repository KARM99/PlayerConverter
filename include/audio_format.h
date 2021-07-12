#include "audio_data.h"

#include <memory>

class AudioFormat {
public:
    virtual AudioData AudioRead(const char* path) = 0;
    virtual void AudioWrite(const AudioData& ad, const char* path) = 0;
};

typedef std::unique_ptr<AudioFormat> create_t();
