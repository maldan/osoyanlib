#include "../../include/audio/wave.h"

struct WaveSound *wave_allocate(float length) {
    size_t sampleRate = 44100;
    size_t numChannels = 2;
    size_t bitsPerSample = 16;
    size_t byteRate = sampleRate * numChannels * bitsPerSample / 8;
    size_t sampleSize = (size_t)ceilf((float)byteRate * length) + sizeof(struct WaveHeader) + 4;

    // Allocate data
    char *buffer = MEMORY_ALLOCATE(sizeof(struct WaveHeader) + sampleSize);

    // Header
    struct WaveHeader *header = (struct WaveHeader *) buffer;
    header->chunkID = 0x46464952;
    header->chunkSize = sampleSize + sizeof(struct WaveHeader) - 8;
    header->format = 0x45564157;
    header->subChunk1ID = 0x20746d66;
    header->subChunk1Size = 16;
    header->audioFormat = 1;
    header->numChannels = numChannels;
    header->sampleRate = sampleRate;
    header->byteRate = byteRate;
    header->blockAlign = 4;
    header->bitsPerSample = bitsPerSample;
    header->subChunk2ID = 0x61746164;
    header->subChunk2Size = sampleSize;

    // Wave sound
    struct WaveSound *waveSound = MEMORY_ALLOCATE_STRUCT(WaveSound);
    waveSound->header = (struct WaveHeader *)buffer;
    waveSound->data = buffer + sizeof(struct WaveHeader);
    waveSound->length = sampleSize;
    waveSound->dataLength = sampleSize - sizeof(struct WaveHeader);

    return waveSound;
}
