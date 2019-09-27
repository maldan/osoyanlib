#pragma once

#define DEFAULT_ARRAY_STRUCT(TYPE) TYPE list;\
size_t length;\
size_t allocated;

#define ARRAY_REMOVE_AT(A, TYPE) size_t len = A->length - (at + 1);\
memmove(A->list + at, A->list + at + amount, len * sizeof(TYPE));\
A->length -= amount

#define RESIZE_ARRAY_IF_NEED(A, S, EL_SIZE) if (A->length + S > A->allocated) {\
size_t _minToAdd34cdf33435_Sdfsd4x = (A->length + S) - A->allocated; \
A->list = MEMORY_REALLOCATE(A->list, (_minToAdd34cdf33435_Sdfsd4x + A->allocated) * 2 * sizeof(EL_SIZE));\
A->allocated *= 2;\
A->allocated += _minToAdd34cdf33435_Sdfsd4x;\
}

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

#define INIT_DEFAULT_LIST_SIZE(A, TYPE) A->list = MEMORY_ALLOCATE(sizeof(TYPE) * 4);\
A->length = 0;\
A->allocated = 4
