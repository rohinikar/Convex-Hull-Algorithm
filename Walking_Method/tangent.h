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



inline float orient( Point P0, Point P1, Point P2 );
void tangentBetweenTwoUpperHulls( int m, vector<Point> &V, int n, vector<Point> &W, int* t1, int* t2 );
void tangentBetweenTwoLowerHulls( int m, vector<Point> &V, int n, vector<Point> &W, int* t1, int* t2 );

#endif