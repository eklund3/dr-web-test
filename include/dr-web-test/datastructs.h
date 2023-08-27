#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <vector>

struct Point {
    Point(float _x, float _y) : x(_x), y (_y){};
    float x, y;
};

typedef std::vector<Point> Points;

#endif // DATASTRUCTS_H
