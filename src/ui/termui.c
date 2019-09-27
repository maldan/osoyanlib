#include "../../include/ui/termui.h"


static size_t p = -1;
static char carr[] = {'.', ':', '_', '|', '=', ')', '-'};
// static uint8_t *termui_param_buffer = 0;
// static struct Vector *termui_style_list = 0;

struct TermUiStyleNode *termui_new_style_node() {
    struct TermUiStyleNode *node = MEMORY_ALLOCATE_STRUCT(TermUiStyleNode);
    INIT_STRING(node->color, "")
    INIT_STRING(node->background, "")
    INIT_STRING(node->bold, "")
    return node;
}

struct TermUiNode *termui_new_node() {
    struct TermUiNode *node = MEMORY_ALLOCATE_STRUCT(TermUiNode);
    INIT_STRING(node->id, "")
    INIT_STRING(node->blockType, "")
    INIT_DICT(node->styles)
    INIT_DICT(node->attributes)
    INIT_WSTRING(node->content, "")
    INIT_VECTOR(node->nodes, TermUiNode *)

    return node;
}

void termui_destroy_style_node(struct TermUiStyleNode *node) {
    DESTROY_STRING(node->color)
    DESTROY_STRING(node->background)
    DESTROY_STRING(node->bold)
    MEMORY_FREE(node);
}

void termui_parse_styles(struct TermUiNode *node, char *styleList) {
    size_t len = strlen(styleList);
    bool isPropNameMode = true;

    NEW_STRING(tempPropName)
    NEW_STRING(tempPropValue)

    for (size_t i = 0; i < len; ++i) {
        char c = styleList[i];

        // End of parameter name
        if (c == ':') {
            isPropNameMode = false;
            i += 1;
            continue;
        }
        if (c == ';') {
            isPropNameMode = true;
            i += 1;

            dict_add(node->styles, tempPropName->list, tempPropValue->list);
            string_clear(tempPropName);
            string_clear(tempPropValue);

            continue;
        }

        // Put to prop name
        if (isPropNameMode) string_put_char(tempPropName, c);
        else string_put_char(tempPropValue, c);
    }
}

void termui_real_parse(struct TermUiDocument *document, struct WString *content, struct TermUiNode *node, size_t *offset) {
    size_t len = content->length; //strlen(content);
    size_t startTagIndex = 0;
    size_t endTagIndex = 0;

    bool isTagNameOpen = false;
    bool isNeedEndOfTag = false;
    bool isAttributeMode = false;
    bool isAttributeValueMode = false;
    bool isTagHasAttr = false;
    bool isHexMode = false;
    struct TermUiNode *tempNode = 0;

    NEW_STRING(tempAttrName)
    NEW_STRING(tempAttrValue)

    for (size_t i = *offset; i < len; ++i) {
        wchar_t c = content->list[i];

        /*if (isHexMode) {
            string_put_char(tempNode->content, c);
            isHexMode = false;
            continue;
        }*/

        if (isAttributeValueMode) {
            if (c == '"') {
                isAttributeValueMode = false;
                if (strcmp(tempAttrName->list, "style") == 0) {
                    termui_parse_styles(tempNode, tempAttrValue->list);
                }
                if (strcmp(tempAttrName->list, "id") == 0) {
                    map_add(document->idList, tempAttrValue->list, tempNode);
                }
                dict_add(tempNode->attributes, tempAttrName->list, tempAttrValue->list);
                string_clear(tempAttrName);
                string_clear(tempAttrValue);
                continue;
            }
            string_put_char(tempAttrValue, c);
            continue;
        }

        if (isAttributeMode) {
            if (c == '=') {
                isAttributeMode = false;
                isAttributeValueMode = true;
                i++;
                continue;
            }
            if (c < 'a' || c > 'z') {
                LOGGER_HALT(__FILE__, __LINE__, "Attr name must be sequence of a-z characters %zu", i)
            }

            string_put_char(tempAttrName, c);
        }

        if (isTagNameOpen) {
            if (c == '>') {
                if (!isTagHasAttr) endTagIndex = i;
                if (isNeedEndOfTag) {
                    *offset = i;
                    return;
                }
                // New tag
                // tempNode->blockType = chars_substr(content, startTagIndex, endTagIndex);
                string_put(tempNode->blockType, wchars_to_chars(wchars_substr(content->list, startTagIndex, endTagIndex)));

                // Add this node to base node
                vector_push(node->nodes, tempNode);

                isTagNameOpen = false;
                isNeedEndOfTag = !isNeedEndOfTag;
                continue;
            }
            if (c == ' ') {
                if (!isTagHasAttr) endTagIndex = i;
                isTagHasAttr = true;
                isAttributeMode = true;
                continue;
            }
            if (c < 'a' || c > 'z') {
                LOGGER_HALT(__FILE__, __LINE__, "Tag must be sequence of a-z characters %zu", i)
            }
            continue;
        }

        if (c == '<') {
            if (isNeedEndOfTag) {
                if (content->list[i + 1] != '/') {
                    termui_real_parse(document, content, tempNode, &i);
                } else {
                    isTagNameOpen = true;
                    startTagIndex = i + 1;
                    i++;
                }
            } else {
                isTagNameOpen = true;
                startTagIndex = i + 1;

                // New tag
                tempNode = termui_new_node();
            }

            continue;
        }

        if (c == '\n') continue;
        if (c == '\t') continue;
        if (c == '\r') continue;
        if (c == ' ') {
            while (true) {
                i++;
                c = content->list[i];
                if (c != ' ') {
                    c = content->list[i - 1];
                    i -= 1;
                    break;
                }
            }
        }

        /*if (c == '\\' && content[i + 1] == 'x') {
            i++;
            isHexMode = true;
            continue;
        }*/

        wstring_put_wchar(tempNode->content, c);
    }
}

struct TermUiDocument *termui_parse(struct WString *content) {
    // Init document
    struct TermUiDocument *document = MEMORY_ALLOCATE_STRUCT(TermUiDocument);
    INIT_MAP(document->idList, struct TermUiNode *)
    INIT_VECTOR(document->styleNodes, struct TermUiStyleNode *)

    // Set document size
    struct winsize winSize = console_get_window_size();
    document->width = winSize.ws_col;
    document->height = winSize.ws_row;

    // Allocate buffers
    document->buffer = MEMORY_ALLOCATE(document->width * document->height * sizeof(wchar_t));
    document->paramBuffer = MEMORY_ALLOCATE(document->width * document->height);

    // Root node
    struct TermUiNode *rootNode = termui_new_node();
    string_put(rootNode->blockType, "root");

    // Real parse
    size_t offset = 0;
    termui_real_parse(document, content, rootNode, &offset);

    // Root of document
    document->root = rootNode;

    return document;
}

struct TermUiDocument *termui_parse_file(char *path) {
    EQU_BLOB(X) = file_get_contents(path);
    if (!X) return 0;
    NEW_WSTRING(X2)
    wstring_put(X2, (char *)X->list);
    DESTROY_BLOB(X)
    return termui_parse(X2);
}

struct String *termui_css_color_to_xterm_background(char *color) {
    NEW_STRING(out)

    if (strcmp(color, "yellow") == 0) string_put(out, ANSI_BACKGROUND_YELLOW);
    else if (strcmp(color, "green") == 0) string_put(out, ANSI_BACKGROUND_GREEN);
    else if (strcmp(color, "blue") == 0) string_put(out, ANSI_BACKGROUND_BLUE);
    else if (strcmp(color, "red") == 0) string_put(out, ANSI_BACKGROUND_RED);
    else if (color[0] == '#') {
        struct ColorRgbInt x = color_hex_to_rgb(color + 1);
        string_add(out, "\x1b[48;2;%d;%d;%dm", x.r, x.g, x.b);
    }

    return out;
}

struct String *termui_css_color_to_xterm_color(char *color) {
    NEW_STRING(out)

    if (strcmp(color, "yellow") == 0) string_put(out, ANSI_COLOR_YELLOW);
    else if (strcmp(color, "green") == 0) string_put(out, ANSI_COLOR_GREEN);
    else if (strcmp(color, "blue") == 0) string_put(out, ANSI_COLOR_BLUE);
    else if (strcmp(color, "red") == 0) string_put(out, ANSI_COLOR_RED);
    else if (color[0] == '#') {
        struct ColorRgbInt x = color_hex_to_rgb(color + 1);
        string_add(out, "\x1b[38;2;%d;%d;%dm", x.r, x.g, x.b);
    }

    return out;
}

void termui_real_render(struct TermUiDocument *document, struct TermUiNode *node,
        uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t skipLines) {
    uint16_t amountOfBlock = node->nodes->length;
    uint16_t nodeWidth = width;
    uint16_t nodeHeight = height;
    uint16_t marginLeft = 0;
    uint16_t marginRight = 0;
    uint16_t paddingLeft = 0;
    uint16_t paddingRight = 0;
    uint16_t paddingTop = 0;
    uint16_t paddingBottom = 0;
    uint32_t scrollTop = 0;
    bool isDraw = true;
    bool isHorizontal = true;

    // Set node data
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    node->skipLines = skipLines;

    if (dict_value_is(node->styles, "visibility", "hidden")) {
        isDraw = false;
    }

    // Styles
    if (dict_has_key(node->styles, "scroll-top")) {
        scrollTop = dict_get_int(node->styles, "scroll-top");
        skipLines += scrollTop;
    }
    if (dict_has_key(node->styles, "margin-left")) {
        marginLeft = dict_get_int(node->styles, "margin-left");
        width -= marginLeft;
    }
    if (dict_has_key(node->styles, "margin-right")) {
        marginRight = dict_get_int(node->styles, "margin-right");
        width -= marginRight;
    }

    // Padding
    if (dict_has_key(node->styles, "padding-left"))
        paddingLeft = dict_get_int(node->styles, "padding-left");
    if (dict_has_key(node->styles, "padding-right"))
        paddingRight = dict_get_int(node->styles, "padding-right");
    if (dict_has_key(node->styles, "padding-top"))
        paddingTop = dict_get_int(node->styles, "padding-top");
    if (dict_has_key(node->styles, "padding-bottom"))
        paddingBottom = dict_get_int(node->styles, "padding-bottom");

    struct TermUiStyleNode *tempStyle = 0;
    uint8_t styleId = 0;
    if (dict_has_key(node->styles, "color")
        || dict_has_key(node->styles, "background")
        || dict_has_key(node->styles, "font-weight")) {
        tempStyle = termui_new_style_node();
        vector_push(document->styleNodes, tempStyle);
        styleId = document->styleNodes->length;
    }
    if (tempStyle && dict_has_key(node->styles, "color")) {
        EQU_STRING(X) = termui_css_color_to_xterm_color(dict_get(node->styles, "color"));
        string_put(tempStyle->color, X->list);
        DESTROY_STRING(X)
    }
    if (tempStyle && dict_has_key(node->styles, "background")) {
        EQU_STRING(X) = termui_css_color_to_xterm_background(dict_get(node->styles, "background"));
        string_put(tempStyle->background, X->list);
        DESTROY_STRING(X)
    }
    if (tempStyle && dict_has_key(node->styles, "font-weight")) {
        string_put(tempStyle->bold, ANSI_BOLD);
    }

    // Main content
    if (strcmp(node->blockType->list, "root") != 0) {
        size_t contentLength = node->content->length;
        size_t contentCharId = -1;

        if (isDraw) {
            // Draw area
            for (uint16_t j = 0; j < height; ++j) {
                for (uint16_t i = 0; i < width; ++i) {
                    size_t position = (x + i + marginLeft) % document->width + (y + j) % document->height * document->width;
                    if (position > document->width * document->height) exit(2);
                    document->buffer[position] = ' '; //carr[p];
                    document->paramBuffer[(x + i + marginLeft) % document->width + (y + j) % document->height * document->width] = styleId;
                }
            }

            // End of style
            for (uint16_t j = 0; j < height; ++j) {
                document->paramBuffer[(x + width - 1 + marginRight) % document->width + (y + j) % document->height * document->width] = 255;
            }

            // Draw content
            bool isEndOfContent = false;
            bool isSkipFirstSpace = true;
            uint16_t lineReminder;
            uint16_t maxLineWidth = (width - paddingLeft - paddingRight);
            uint16_t totalLoopHeight = (uint16_t)((height - paddingTop - paddingBottom) + skipLines);
            uint16_t totalLoopWidth = (uint16_t)(width - paddingLeft - paddingRight);

            for (uint16_t j = 0; j < totalLoopHeight; ++j) {
                for (uint16_t i = 0; i < totalLoopWidth; ++i) {
                    lineReminder = (width - paddingLeft - paddingRight) - i;

                    // Offset counter
                    contentCharId++;
                    if (contentCharId >= contentLength) {
                        isEndOfContent = true;
                        break;
                    }

                    // Check if word is fit to line
                    if (node->content->list[contentCharId] != ' ') {
                        // Word can't fit in any line
                        if (wchars_word_length(node->content->list + contentCharId) > maxLineWidth) {
                            // j++;
                            contentCharId += wchars_word_length(node->content->list + contentCharId);
                            continue;
                        }
                        if (wchars_word_length(node->content->list + contentCharId) > lineReminder) {
                            i = -1;
                            j++;
                            contentCharId -= 1;
                            if (skipLines > 0) {
                                skipLines--;
                                j--;
                            }
                            continue;
                        }
                    }

                    // Calculate position
                    size_t position = (x + i + marginLeft + paddingLeft) % document->width + (y + j + paddingTop) % document->height * document->width;

                    if (y + j > y + height - 1) {
                        isEndOfContent = true;
                        break;
                    }

                    if (isSkipFirstSpace && node->content->list[contentCharId] != ' ') {
                        isSkipFirstSpace = false;
                    }
                    if (isSkipFirstSpace && node->content->list[contentCharId] == ' ') {
                        i -= 1;
                        continue;
                    }

                    // Set content
                    if (skipLines <= 0) document->buffer[position] = node->content->list[contentCharId];
                }
                if (skipLines > 0) {
                    skipLines--;
                    j--;
                }
                if (isEndOfContent) break;
                isSkipFirstSpace = true;
            }
        }
    }

    // Child size
    int16_t blockWidthList[amountOfBlock];
    int16_t blockWidthOffset[amountOfBlock + 1];
    int16_t blockHeightList[amountOfBlock];
    int16_t blockHeightOffset[amountOfBlock + 1];

    blockWidthOffset[0] = 0;
    blockHeightOffset[0] = 0;

    uint16_t totalOffsetX = 0;
    uint16_t totalOffsetY = 0;

    // Temp amount and width
    size_t tempAmountOfBlock = amountOfBlock;
    size_t tempWidth = width;
    size_t tempHeight = height;
    size_t lastFlexBlock = 0;

    // Changed flex direction
    if (dict_value_is(node->styles, "flex-direction", "column")) {
        isHorizontal = false;

        // Calculate default
        for (size_t k = 0; k < node->nodes->length; ++k) {
            // Width
            blockWidthList[k] = width;
            blockWidthOffset[k + 1] = 0;

            // Height
            blockHeightList[k] = height;
            blockHeightOffset[k + 1] = height + totalOffsetY;
            totalOffsetY += height;
        }

        // Calculate non flex blocks
        for (size_t k = 0; k < node->nodes->length; ++k) {
            struct TermUiNode *tempNode = node->nodes->list[k];
            if (dict_has_key(tempNode->styles, "height")) {
                blockHeightList[k] = dict_get_int(tempNode->styles, "height");
                tempAmountOfBlock -= 1;
                tempHeight -= blockHeightList[k];
            }
        }

        // Calculate flex blocks
        for (size_t k = 0; k < node->nodes->length; ++k) {
            struct TermUiNode *tempNode = node->nodes->list[k];
            if (!dict_has_key(tempNode->styles, "height")) {
                blockHeightList[k] = tempHeight / tempAmountOfBlock;
            }
        }

        // Calculate final offset
        totalOffsetY = 0;
        for (size_t k = 0; k < node->nodes->length; ++k) {
            blockHeightOffset[k + 1] = blockHeightList[k] + totalOffsetY;
            totalOffsetY += blockHeightList[k];
        }
    } else {
        // Calculate default
        for (size_t k = 0; k < node->nodes->length; ++k) {
            // Width
            blockWidthList[k] = width;
            blockHeightOffset[k + 1] = width + totalOffsetX;
            totalOffsetX += width;

            // Height
            blockHeightList[k] = height;
            blockHeightOffset[k + 1] = 0;
        }

        // Calculate non flex blocks
        for (size_t k = 0; k < node->nodes->length; ++k) {
            struct TermUiNode *tempNode = node->nodes->list[k];
            if (dict_has_key(tempNode->styles, "width")) {
                blockWidthList[k] = dict_get_int(tempNode->styles, "width");
                tempAmountOfBlock -= 1;
                tempWidth -= blockWidthList[k];
            }
        }

        // Calculate flex blocks
        for (size_t k = 0; k < node->nodes->length; ++k) {
            struct TermUiNode *tempNode = node->nodes->list[k];
            if (!dict_has_key(tempNode->styles, "width")) {
                blockWidthList[k] = tempWidth / tempAmountOfBlock;
                lastFlexBlock = k;
            }
        }

        // Calculate final offset
        totalOffsetX = 0;
        for (size_t k = 0; k < node->nodes->length; ++k) {
            blockWidthOffset[k + 1] = blockWidthList[k] + totalOffsetX;
            totalOffsetX += blockWidthList[k];
        }
    }

    if (amountOfBlock > 1 && isHorizontal) {
        size_t totalWidth = width;
        size_t currentWidth = 0;
        for (size_t k = 0; k < node->nodes->length; ++k)
            currentWidth += blockWidthList[k];
        size_t reminderWidth = totalWidth - currentWidth;
        blockWidthList[lastFlexBlock] += reminderWidth;
        blockWidthOffset[lastFlexBlock + 1] += reminderWidth;
    }
    // Draw nodes
    // uint16_t offsetX = 0;
    // uint16_t offsetY = 0;

    if (!isHorizontal) {
        for (size_t k = 0; k < node->nodes->length; ++k) {
            // Height
            blockHeightOffset[k] -= scrollTop;
        }
    }

    // Draw child block
    for (size_t k = 0; k < node->nodes->length; ++k) {
        int16_t heightReminder = blockHeightList[k];
        int32_t tempSkipLine = 0;
        if (y + blockHeightOffset[k] + blockHeightList[k] > y + height) {
            heightReminder = blockHeightList[k] + ((y + height) - (y + blockHeightOffset[k] + blockHeightList[k]));
            if (heightReminder <= 0) break;
            blockHeightList[k] = heightReminder;
        }
        if (blockHeightOffset[k] < 0) {
            int16_t originalHeight = blockHeightList[k];
            blockHeightList[k] += blockHeightOffset[k];
            blockHeightOffset[k] = 0;
            if (blockHeightList[k] < 0) continue;
            tempSkipLine = originalHeight - blockHeightList[k];
            // PRINT(originalHeight - blockHeightList[k])
            // exit(1);
        }

        termui_real_render(document, node->nodes->list[k],
                x + blockWidthOffset[k] + marginLeft,
                y + blockHeightOffset[k],
                blockWidthList[k],
                blockHeightList[k], tempSkipLine);
        // if (isHorizontal) offsetX += 1;
        // else offsetY += 1;
    }
}

void termui_render(struct TermUiDocument *document) {
    // Terminal changed size
    struct winsize winSize = console_get_window_size();
    if (document->width != winSize.ws_col || document->height != winSize.ws_row) {
        document->width = winSize.ws_col;
        document->height = winSize.ws_row;

        // Allocate buffers
        document->buffer = MEMORY_REALLOCATE(document->buffer, document->width * document->height * sizeof(wchar_t));
        document->paramBuffer = MEMORY_REALLOCATE(document->paramBuffer, document->width * document->height);
    }

    // Destroy old style nodes
    for (size_t k = 0; k < document->styleNodes->length; ++k)
        termui_destroy_style_node(document->styleNodes->list[k]);
    document->styleNodes->length = 0;

    // Add new null node
    struct TermUiStyleNode *tempStyleNode = termui_new_style_node();
    vector_push(document->styleNodes, tempStyleNode);

    // Start draw
    termui_real_render(document, document->root, 0, 0, document->width, document->height, 0);

    NEW_WSTRING(finalString)
    wstring_put(finalString, "\x1b[H");
    wstring_put(finalString, ANSI_HIDE_CURSOR);

    // Draw area
    struct TermUiStyleNode *lastStyleNode = 0;
    for (uint16_t j = 0; j < document->height; ++j) {
        for (uint16_t i = 0; i < document->width; ++i) {
            uint8_t styleId = document->paramBuffer[i % document->width + j % document->height * document->width];

            if (styleId != 0 && styleId != 255) {
                if (lastStyleNode != document->styleNodes->list[styleId - 1]) {
                    lastStyleNode = document->styleNodes->list[styleId - 1];

                    if (lastStyleNode->color->length)
                        wstring_put(finalString, lastStyleNode->color->list);
                    if (lastStyleNode->background->length)
                        wstring_put(finalString, lastStyleNode->background->list);
                    if (lastStyleNode->bold->length)
                        wstring_put(finalString, lastStyleNode->bold->list);
                }
            }
            if (styleId == 255) {
                if (lastStyleNode) {
                    if (lastStyleNode->color->length)
                        wstring_put(finalString, lastStyleNode->color->list);
                    if (lastStyleNode->background->length)
                        wstring_put(finalString, lastStyleNode->background->list);
                    if (lastStyleNode->bold->length)
                        wstring_put(finalString, lastStyleNode->bold->list);
                    lastStyleNode = 0;
                }
                wstring_put_wchar(finalString, document->buffer[i % document->width + j % document->height * document->width]);
                wstring_put(finalString, ANSI_COLOR_RESET);
            } else wstring_put_wchar(finalString, document->buffer[i % document->width + j % document->height * document->width]);
        }
    }

    // Print on screen
    wstring_put(finalString, ANSI_SHOW_CURSOR);

    //printf("2 %zu, %ls", finalString->length, document->buffer);
    //fputs(document->buffer, stdout);
    fputws(finalString->list, stdout);
    fflush(stdout);

    DESTROY_WSTRING(finalString)
    //MEMORY_FREE(buffer);
    //EMORY_FREE(termui_param_buffer);
    //for (size_t k = 0; k < termui_style_list->length; ++k) {
    //    termui_destroy_style_node(termui_style_list->list[k]);
    //}
    //DESTROY_VECTOR(termui_style_list);

    /*for (size_t i = 0; i < w.ws_col; ++i) {
        for (size_t j = 0; j < w.ws_row; ++j) {
            printf("1");
        }
    }*/
    // printf("\n");
}

struct TermUiNode *termui_get_node(struct TermUiDocument *document, char *id) {
    return map_get(document->idList, id);
}

void termui_set_css(struct TermUiNode *node, char *key, char *value) {
    dict_add(node->styles, key, value);
}

void termui_set_content(struct TermUiNode *node, char *content) {
    /*struct TermUiNode *node = map_get(document->idList, id);
    if (!node) {
        exit(1);
        return false;
    }*/
    string_clear(node->content);
    string_put(node->content, content);
}
