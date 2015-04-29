#ifndef TANGENT_H
#define TANGENT_H

#include "Point.h"
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>
#include <mutex>
#include <chrono>
using namespace std;

enum position {ABOVE,BELOW,ON};

inline position relativePosition( Point P0, Point P1, Point P2 );
inline bool isAbove(Point P, Point Vi, Point Vj);
inline bool isBelow(Point P, Point Vi, Point Vj);
inline bool isOn(Point P, Point Vi, Point Vj);

inline bool isTangentPoint(vector<Point> V, int k, int indexRange_Size, Point P);
inline bool isLPoint(vector<Point> V, int k, int indexRange_Size, Point P);
inline bool isRPoint(vector<Point> V, int k, int indexRange_Size, Point P);

void tangentBetweenTwoUpperHulls( int m, vector<Point> &V, int n, vector<Point> &W, vector<int> t1, vector<int> t2 );

#endif