#pragma once

struct Rectangle {
    int x;
    int y;
    int width;
    int height;
};

struct Rectangle rect_add(struct Rectangle r1, struct Rectangle r2);
