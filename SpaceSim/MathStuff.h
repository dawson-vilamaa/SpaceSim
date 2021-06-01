#pragma once
#ifndef MATHSTUFF_H
#define MATHSTUFF_H

#include <math.h>
#include "Vectors.h"

#define DIMENSION 3

struct Line {
    double m[DIMENSION], b[DIMENSION];
};

float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}

float distance2D(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float dotProduct(vec3 u, vec3 v) {
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

double minPointToLineDistance(double linePoint1[], double linePoint2[], double point[]) {
    double v[2] = { 0,0 };
    double t;
    struct Line line;
    double a[3], b[3][2];
    double minDistanceVector[DIMENSION];
    double minDistance = 0;

    //Create the line along with a and b from video
    for (int i = 0; i < DIMENSION; i++) {
        line.m[i] = linePoint1[i] - linePoint2[i];
        line.b[i] = linePoint2[i];

        //a just matches slope of line
        a[i] = line.m[i];
        //b split into b[0] + b[1]t
        b[i][0] = line.b[i] - point[i];
        b[i][1] = line.m[i];
    }

    //dot product a.b
    for (int i = 0; i < DIMENSION; i++) {
        v[0] += a[i] * b[i][0];
        v[1] += a[i] * b[i][1];
    }

    //set v = 0 which is currently v0 + v1t = 0 which means t = -v0/v1
    t = (v[1] != 0) ? -v[0] / v[1] : 0;

    for (int i = 0; i < DIMENSION; i++) {
        minDistanceVector[i] = b[i][0] + b[i][1] * t;
        minDistance += pow(minDistanceVector[i], 2);
    };
    return sqrt(minDistance);
}

#endif