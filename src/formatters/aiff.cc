#include <string>
#include <fstream>

#include "../include/audio_data.h"


#define AIFF_CHAR       char
#define AIFF_UCHAR      unsigned char
#define AIFF_SHORT      std::int16_t
#define AIFF_USHORT     std::uint16_t
#define AIFF_LONG       std::int32_t
#define AIFF_ULONG      std::uint32_t
#define AIFF_EXTENDED   long double

static_assert(sizeof(AIFF_CHAR) == 1);
static_assert(sizeof(AIFF_UCHAR) == 1);
static_assert(sizeof(AIFF_SHORT) == 2);
static_assert(sizeof(AIFF_USHORT) == 2);
static_assert(sizeof(AIFF_LONG) == 4);
static_assert(sizeof(AIFF_ULONG) == 4);
static_assert(sizeof(AIFF_EXTENDED) == 16); // only 10 are important


typedef struct {
    AIFF_CHAR id[4];
} AIFF_ID;

typedef struct {
    AIFF_ID             FORM;
    AIFF_LONG           ck_size;

    AIFF_ID             AIFF;

    AIFF_ID             COMM;

    AIFF_LONG           comm_ck_size;
    AIFF_SHORT          num_channels;
    AIFF_ULONG          number_sample_frames;
    AIFF_SHORT          bits_per_sample;
    AIFF_EXTENDED       sample_rate;

    AIFF_ID             DATA;
    AIFF_LONG           data_ck_size;
    AIFF_ULONG          offset;
    AIFF_ULONG          block_size;    
} AIFF_HEADER;

void ReadID(std::ifstream &file, char *dest) {
    file.read(dest, sizeof(AIFF_ID));
}

void WriteId(std::ofstream &file, const char *src) {
    file.write(src, sizeof(AIFF_ID));
}

template<typename T>
void ReadRev(std::ifstream &file, T *dest) {
    if (sizeof(T) == 16) {
        for (size_t i = 9; i + 1 != 0; i--) {
            file.read(reinterpret_cast<char*>(dest) + i, 1);
        }
    } else {
        for (size_t i = sizeof(T) - 1; i + 1 != 0; i--) {
            file.read(reinterpret_cast<char*>(dest) + i, 1);
        }
    }
}

void ReadRev(std::ifstream &file, char* dest, std::size_t sz) {
    for (size_t i = sz - 1; i + 1 != 0; i--) {
        file.read(dest + i, 1);
    }
}

template<typename T>
void WriteRev(std::ofstream &file, const T *src) {
    if (sizeof(T) == 16) {
        for (std::size_t i = 9; i + 1 != 0; i--) {
            file.write(reinterpret_cast<const char*>(src) + i, 1);
        }
    } else {
        for (std::size_t i = sizeof(T) - 1; i + 1 != 0; i--) {
            file.write(reinterpret_cast<const char*>(src) + i, 1);
        }
    }
}

void WriteRev(std::ofstream &file, const char *src, std::size_t sz) {
    for (std::size_t i = sz - 1; i + 1 != 0; i--) {
        file.write(src + i, 1);
    }
}

AIFF_HEADER ReadHeader(std::ifstream &file) {
    AIFF_HEADER header;
    ReadID(file, header.FORM.id);
    ReadRev(file, &header.ck_size);
    ReadID(file, header.AIFF.id);
    ReadID(file, header.COMM.id);
    ReadRev(file, &header.comm_ck_size);
    ReadRev(file, &header.num_channels);
    ReadRev(file, &header.number_sample_frames);
    ReadRev(file, &header.bits_per_sample);
    ReadRev(file, &header.sample_rate);
    ReadID(file, header.DATA.id);
    ReadRev(file, &header.data_ck_size);
    ReadRev(file, &header.offset);
    ReadRev(file, &header.block_size);
    return header;
}

void WriteHeader(const AIFF_HEADER &header, std::ofstream &file) {
    WriteId(file, header.FORM.id);
    WriteRev(file, &header.ck_size);
    WriteId(file, header.AIFF.id);
    WriteId(file, header.COMM.id);
    WriteRev(file, &header.comm_ck_size);
    WriteRev(file, &header.num_channels);
    WriteRev(file, &header.number_sample_frames);
    WriteRev(file, &header.bits_per_sample);
    WriteRev(file, &header.sample_rate);
    WriteId(file, header.DATA.id);
    WriteRev(file, &header.data_ck_size);
    WriteRev(file, &header.offset);
    WriteRev(file, &header.block_size);
}

extern "C" AudioData AudioRead(const char *filename) {
    std::ifstream file {filename, std::ios::binary};
    AIFF_HEADER header {ReadHeader(file)};
    AudioData ad(
        header.num_channels,
        static_cast<std::uint32_t>(header.sample_rate),
        header.bits_per_sample,
        8 * (header.data_ck_size - 2 * sizeof(AIFF_LONG)) / header.num_channels / header.bits_per_sample
    );
    std::size_t frame_size  = header.bits_per_sample / 8;
    for (std::size_t offset = 0; offset < ad.data.size(); offset += frame_size) {
        ReadRev(file, &ad.data[offset], frame_size);
    }
    return ad;
}

extern "C" void AudioWrite(const AudioData &ad, const char *filename) {
    AIFF_HEADER header {
        {'F', 'O', 'R', 'M'},
        // sizeof(AIFF_ID) * 3 + sizeof(AIFF_LONG) * 2 + sizeof(AIFF_ULONG) * 3 + sizeof(AIFF_SHORT) * 2 + 10ul + std::uint64_t{ad.data.size()}, // instead of sizeof(AIFF_EXTENDED)
        46 + ad.data.size(),
        {'A', 'I', 'F', 'F'},
        {'C', 'O', 'M', 'M'},
        18,
        ad.num_channels,
        ad.number_samples,
        ad.bits_per_sample,
        static_cast<AIFF_EXTENDED>(ad.sample_rate),
        {'S', 'S', 'N', 'D'},
        ad.data.size() + sizeof(AIFF_ULONG) * 2,
        0,
        0
    };


    std::ofstream file {filename, std::ios::binary};
    WriteHeader(header, file);

    std::size_t frame_size  = header.bits_per_sample / 8;
    for (std::size_t offset = 0; offset < ad.data.size(); offset += frame_size) {
        WriteRev(file, &ad.data[offset], frame_size);
    }
}
