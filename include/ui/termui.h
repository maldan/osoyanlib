#pragma once

#include <wchar.h>
#include "../geom/rect.h"
#include "../string/string.h"
#include "../string/wstring.h"
#include "../list/dict.h"
#include "../list/map.h"

struct TermUiDocument {
    EQU_MAP(idList);
    EQU_VECTOR(styleNodes);
    struct TermUiNode *root;
    wchar_t *screenBuffer;
    uint32_t *paramBuffer;
    struct Rectangle screen;
};

struct TermUiNode {
    // Base
    EQU_STRING(id);
    EQU_STRING(elementType);

    // Params
    EQU_DICT(styles);
    EQU_DICT(attributes);
    struct Rectangle area;
    uint16_t contentOffsetY;

    char testChar;

    // Text content
    EQU_WSTRING(content);

    // Children
    EQU_VECTOR(nodes);
};

struct TermUiDocument *termui_create_document();
struct TermUiNode *termui_create_node();

void termui_render(struct TermUiDocument *document);

void termui_destroy_node(struct TermUiNode *node);
void termui_destroy_document(struct TermUiDocument *document);
