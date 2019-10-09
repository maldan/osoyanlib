#include "../../include/list/bitset.h"

void ____bitset_init(struct BitSet *bitSet) {
    bitSet->list = MEMORY_ALLOCATE(sizeof(uint8_t) * 8);
    bitSet->length = 0;
    bitSet->allocated = 8 * 8; // 64 bit, 8 byte

    // INIT_DEFAULT_LIST_SIZE(bitSet, uint8_t);
}

void ____bitset_free(struct BitSet *bitSet) {
    MEMORY_FREE(bitSet->list)
    MEMORY_FREE(bitSet)
}

void bitset_put(struct BitSet *bitSet, uint64_t value, uint8_t size) {
    // 8 is uint64, so we allocate 8 byte, why? Because!
    // RESIZE_ARRAY_IF_NEED(bitSet, 8, uint8_t)

    // Try to allocate 8 byte
    if (bitSet->length + 64 > bitSet->allocated) {
        // size_t oldA = bitSet->allocated;
        bitSet->list = MEMORY_REALLOCATE(bitSet->list, bitSet->allocated * 2 * sizeof(uint8_t));
        bitSet->allocated *= 2;

        // Clear new memory
        memset(bitSet->list + bitSet->length, 0, bitSet->allocated - bitSet->length);
    }

    value &= (UINT64_MAX >> (sizeof(uint64_t) * 8u - size));
    value <<= (bitSet->length % 8u);
    uint64_t reminder = 0;
    uint64_t reminderSize = 0;
    if (value > 255) {
        reminder = (UINT64_MAX << 8u) & value;
        reminder >>= 8u;
        reminderSize = (bitSet->length % 8u) + size - 8u;
        size -= reminderSize;
        value &= 255u;
    }
    bitSet->list[bitSet->length / 8] += value;

    bitSet->position += size;
    bitSet->length += size;

    if (reminderSize > 0) {
        bitset_put(bitSet, reminder, reminderSize);
    }
}

bool bitset_get(struct BitSet *bitSet, size_t position) {
    return bitSet->list[position / 8] & (1u << (position % 8)) ?true :false;
}
