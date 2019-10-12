#pragma once

#include "../memory/memory.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct WaveHeader {
    uint32_t chunkID;       // 0x46464952 "RIFF" in little endian
    uint32_t chunkSize;     // 4 + (8 + subChunk1Size) + (8 + subChunk2Size)
    uint32_t format;        // 0x45564157 "WAVE" in little endian

    uint32_t subChunk1ID;   // 0x20746d66 "fmt " in little endian
    uint32_t subChunk1Size; // 16 for PCM
    uint16_t audioFormat;   // 1 for PCM, 3 fot EEE floating point , 7 for μ-law
    uint16_t numChannels;   // 1 for mono, 2 for stereo
    uint32_t sampleRate;    // 8000, 22050, 44100, etc...
    uint32_t byteRate;      // sampleRate * numChannels * bitsPerSample/8
    uint16_t blockAlign;    // numChannels * bitsPerSample/8
    uint16_t bitsPerSample; // number of bits (8 for 8 bits, etc...)

    uint32_t subChunk2ID;   // 0x61746164 "data" in little endian
    uint32_t subChunk2Size; // numSamples * numChannels * bitsPerSample/8 (this is the actual data size in bytes)
};

struct WaveSound {
    struct WaveHeader *header;
    char *data;
    size_t length;
    size_t dataLength;
};

struct WaveSound *wave_allocate(float length);
