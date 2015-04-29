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
#include <stdio.h>
using namespace std;


Point :: Point (float x = 0.0, float y = 0.0) {
 xval = x;
 yval = y;

}



	
	

        // Extractors.
float Point ::  x() { return xval; }

float Point ::  y() { return yval; }

bool ::operator<(Point i, Point j) {
return i.x() < j.x();

}