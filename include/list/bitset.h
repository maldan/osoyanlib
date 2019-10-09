#pragma once

#include <stdint.h>
#include <math.h>
#include "list.h"
#include "../memory/memory.h"

#define NEW_BITSET(X) struct BitSet *X = MEMORY_ALLOCATE_STRUCT(BitSet); ____bitset_init(X);
#define INIT_BITSET(X) (X) = MEMORY_ALLOCATE_STRUCT(BitSet); ____bitset_init(X);
#define EQU_BITSET(X) struct BitSet *X
#define DESTROY_BITSET(X) ____bitset_free(X);

#define BITSET_GET(bitSet, position) bitSet->list[position / 8] & (1u << (position % 8)) ?true :false

struct BitSet {
    DEFAULT_ARRAY_STRUCT(uint8_t *)
    size_t position;
};

void ____bitset_init(struct BitSet *bitSet);

void ____bitset_free(struct BitSet *bitSet);

void bitset_put(struct BitSet *bitSet, uint64_t value, uint8_t size);

bool bitset_get(struct BitSet *bitSet, size_t position);

