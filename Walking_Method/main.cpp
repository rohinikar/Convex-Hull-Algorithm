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
#include "tangent.h"


using namespace std;


//DECLARATION OF CLASSES AND FUNCTIONS
//class Point;
void createRandomPointObject(vector<Point> &P, int N);
void outputRandomPointObject(vector<Point> &P);
int computeUpperLowerHull(vector<Point> &P, int startPos, int endPos, vector<Point> &Hupper, vector<Point> &Hlower );
void computeConvexHullExtremePoints(vector<Point> &P, int startPos, int endPos, int* minMin, int* minMax, int* maxMin, int* maxMax);
void upperTangentComputationParallel(vector<Point> &H1upper,vector<Point> &H2upper,vector<Point> &H3upper,vector<Point> &H4upper,int N);
void lowerTangentComputationParallel(vector<Point> &H1lower,vector<Point> &H2lower,vector<Point> &H3lower,vector<Point> &H4lower,int N);
//void tangentBetweenTwoUpperHulls( int m, vector<Point> &V, int n, vector<Point> &W, vector<int> t1, vector<int> t2 );
void convexHullSequential(vector<Point> &P, int N);
void convexHullParallel( vector<Point> &P, int N);
//END OF DECLARATION OF CLASSES AND FUNCTIONS







//MAIN METHOD
int main()
{
	int N;
	vector<Point> P;
	
	cout <<"Enter the number of input points :" <<endl;
	cin >> N;

	//Creation of random points
	createRandomPointObject(P, N);

	//Output the generated random points
	outputRandomPointObject(P);

	cout <<"Clock ticks per second in the system : " << chrono::high_resolution_clock::period::den << endl;



	//SEQUENTIAL PART

	//Start calculating time
	auto start_time_sequential = chrono::high_resolution_clock::now();
	convexHullSequential(P,N);
	//Stop calculating time
	auto end_time_sequential = chrono::high_resolution_clock::now();
	
	

	//PARALLEL PART

	//Start calculating time
	auto start_time_parallel = chrono::high_resolution_clock::now();
	convexHullParallel(P,N);
	//Stop calculating time
	auto end_time_parallel = chrono::high_resolution_clock::now();



	cout<<"Time taken by Sequential Portion :" << chrono::duration_cast<chrono::milliseconds>(end_time_sequential - start_time_sequential).count() <<" milliseconds" <<endl;
	cout<<"Time taken by Parallel Portion :" << chrono::duration_cast<chrono::milliseconds>(end_time_parallel - start_time_parallel).count() <<" milliseconds" <<endl;
	
	
   	
} //End of main


//RANDOM GENERATION OF POINTS
void createRandomPointObject(vector<Point> &P, int N)
{
	float num1=0.0, num2=0.0;
	int lower, upper;
	
	cout <<"Enter lower bound :" << endl;
	cin >> lower;
	cout <<"Enter upper bound :" << endl;
	cin >> upper;
	srand(time(NULL));
	
	//Random generation of Points
	for (int i = 0; i < N; i++)
	{
		num1=lower+(float) (rand())/((float)(RAND_MAX/(upper-lower)));
		num2=lower+(float) (rand())/((float)(RAND_MAX/(upper-lower)));
		P.push_back(Point(num1, num2));
	}


	//Sort the generated random Points
	sort(P.begin(),P.end());

} // End of createRandomPointObject




//OUTPUT RANDOMLY GENERATED POINTS IF NUMBER OF POINTS IS LESS THAN 25, ELSE DISPLAY MESSAGE
void outputRandomPointObject(vector<Point> &P)
{
	if (P.size() > 25)
	{
		cout <<"The number of points is too large to be displayed! " << endl;
	}
	else
	{
	    for(int i=0; i<P.size(); i=i+1)
	       {
		     cout<<"Point object value: " << P.at(i).x() <<"," << P.at(i).y() <<endl;
	       }
	}
} //End of outputRandomPointObject



//CONVEX HULL COMPUTATION FOR SEQUENTIAL PART
void convexHullSequential(vector<Point> &P, int N)
{
	int a=0;
	vector<Point> Hupper,Hlower;
	a = computeUpperLowerHull(P,0,N-1,Hupper,Hlower);

	for(int i=0; i<Hupper.size(); i=i+1)
	{
		cout<<"(SEQUENTIAL) Convex Hull Point :" << Hupper.at(i).x() <<"," << Hupper.at(i).y() << endl;
	}

	for(int i=Hlower.size()-1; i>=0; i=i-1)
	{
		cout<<"(SEQUENTIAL) Convex Hull Point :" << Hlower.at(i).x() <<"," << Hlower.at(i).y() << endl;
	}
} //End of convexHullSequential


//CONVEX HULL COMPUTATION FOR PARALLEL PART
void convexHullParallel( vector<Point> &P, int N)
{
	int m=0,size=0,q=0,xyz=0;
	int i = 0, start = 0, end = 0, count = 0;
	vector< vector<Point> > HParallelUpper, HParallelLower;
	vector<Point> H1upper,H2upper,H3upper,H4upper;
	vector<Point> H1lower,H2lower,H3lower,H4lower;
	

	unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
	cout << "Concurent Threads Supported :" << concurentThreadsSupported <<endl;
	
	//SIZE OF EACH BLOCK OF POINTS
	size=N/concurentThreadsSupported;
	
	
	//CREATION OF THREADS FOR COMPUTATION OF UPPER AND LOWER HULL POINTS
	std::vector<std::thread> threads;
	threads.push_back(std::thread(computeUpperLowerHull,ref(P),0,size-1,ref(H1upper),ref(H1lower)));
	threads.push_back(std::thread(computeUpperLowerHull,ref(P),size,(2*size)-1,ref(H2upper),ref(H2lower)));
	threads.push_back(std::thread(computeUpperLowerHull,ref(P),(2*size),(3*size)-1,ref(H3upper),ref(H3lower)));
	threads.push_back(std::thread(computeUpperLowerHull,ref(P),(3*size),((4*size)-1+(N-(4*size))),ref(H4upper),ref(H4lower)));
	

	for(auto& thread : threads){
        thread.join();
    }

	
	//TANGENT COMPUTATION OF UPPER AND LOWER HULLS
	upperTangentComputationParallel(H1upper,H2upper,H3upper,H4upper,N);
	lowerTangentComputationParallel(H1lower,H2lower,H3lower,H4lower,N);
	
	
	

} // End of convexHullParallel


//COMPUTE THE UPPER & LOWER HULLS FOR A GIVEN SET OF POINTS
int computeUpperLowerHull(vector<Point> &P, int startPos, int endPos, vector<Point> &Hupper, vector<Point> &Hlower )
{
	
	
    int    botUpper=0, topUpper=(-1);   // indices for bottom and top of the upper stack
	int    botLower=0, topLower=(-1);   // indices for bottom and top of the lower stack
	int    minMin = 0, minMax = 0, maxMin = 0, maxMax = 0;
	int    i;
    
	// Get the indices of points with min x-coord and min|max y-coord
    

	
	//COMPUTATION OF POINT INDICES OF EXTREME POINTS OF CONVEX HULL
	computeConvexHullExtremePoints(P,startPos,endPos,&minMin,&minMax,&maxMin,&maxMax);
	
	
    	
	// DEGENERATE CASE WHEN x-coords OF ALL POINTS ARE THE SAME 
    if (minMax == endPos) {       
		++topUpper;
        Hupper.push_back(P.at(minMin));
        if (P.at(minMax).y() != P.at(minMin).y()) // a  nontrivial segment
		{
			++topUpper;
            Hupper.push_back(P.at(minMax));
		}
		++topUpper;
        Hupper.push_back(P.at(minMin));            // add polygon endpoint
        return topUpper+1;
    }

    
	
    // COMPUTATION OF LOWER HULL AND UPPER HULL USING STACKS Hlower AND Hupper RESPECTIVELY
	++topUpper;
    Hupper.push_back(P.at(minMin));      //add leftmost point onto Hupper to start with
	++topLower;
    Hlower.push_back(P.at(minMin));      //add leftmost point onto Hlower to start with
    
	//SCAN ALL POINTS STARTING FROM LEFTMOST TO FIND UPPER AND LOWER HULL POINTS SIMULTANEOUSLY
	i = minMax;
    while (++i <= maxMin)
    {

        
		//IGNORE POINTS WHICH LIE ON THE LINE JOINING P[minMin] AND P[maxMin]
        if (orient( P.at(minMin), P.at(maxMin), P.at(i))== 0 && i < maxMin) // the lower line joins P[minMin]  with P[maxMin] which divides the upper and lower convex hull
            continue;  



		//COMPUTE THE UPPER CONVEX HULL POINTS
		else
		{
			if (orient( P.at(minMin), P.at(maxMin), P.at(i))>0 && i <= maxMin)
			{
				while (topUpper > 0)         // there are at least 2 points on the stack
				{
                    if (orient(  Hupper.at(topUpper-1), Hupper.at(topUpper), P.at(i)) <0) // test if  P[i] is left of the line at the stack top
                       break;         // P[i] is a new hull  vertex
			        else
			          {
				       Hupper.pop_back();
                       topUpper--;         // pop top point off  stack
			          }
				}
				++topUpper;
                Hupper.push_back(P.at(i));        // push P[i] onto stack
			}
			



			//COMPUTE THE LOWER CONVEX HULL POINTS
			else
			{
               while (topLower > 0)         // there are at least 2 points on the stack
                 {
					 if (orient(  Hlower.at(topLower-1), Hlower.at(topLower), P.at(i))>0) // test if  P[i] is left of the line at the stack top
                         break;         // P[i] is a new hull  vertex
                      else
			            {
				         Hlower.pop_back();
                         topLower--;         // pop top point off  stack
			            }
                 }
		     ++topLower;
             Hlower.push_back(P.at(i));        // push P[i] onto stack
		   }
		
		
		}
    }



	//IF maxMin NOT EQUAL TO maxMax
	if (maxMin!=maxMax)
	{
		++topUpper;
        Hupper.push_back(P.at(maxMax));    // push P[maxmax] onto stack containing upper hull points
	}
	
	++topUpper;
    Hupper.push_back(P.at(maxMin));    // push rightmost point onto stack containing upper hull points

    return topUpper+1;
	
} //End of computeUpperLowerHull



//COMPUTE THE EXTREME POINTS OF CONVEX HULL
void computeConvexHullExtremePoints(vector<Point> &P, int startPos, int endPos, int* minMin, int* minMax, int* maxMin, int* maxMax)
{
	
	float xmin = P.at(startPos).x();
	float xmax = P.at(endPos).x();
	int i;

	*minMin = startPos;
	*maxMax = endPos;

    for (i=startPos+1; i<=endPos; i++)
        if (P.at(i).x() != xmin) break;
    *minMax = i-1;

	
    for (i=endPos-1; i>=startPos; i--)
        if (P.at(i).x() != xmax) break;
    *maxMin = i+1;
	
} //End of computeConvexHullExtremePoints


//UPPER TANGENT COMPUTATION FOR PARALLEL VERSION
void upperTangentComputationParallel(vector<Point> &H1upper,vector<Point> &H2upper,vector<Point> &H3upper,vector<Point> &H4upper,int N)
{
	vector<int> H1LeftTangentPoint,H2LeftTangentPoint,H3LeftTangentPoint,H4LeftTangentPoint;
	vector<int> H1RightTangentPoint,H2RightTangentPoint,H3RightTangentPoint,H4RightTangentPoint;
	vector<int> lUpper,rUpper;
	int t1,t2;

	
	//Compute tangents between each pair of upper hulls computed earlier
	tangentBetweenTwoUpperHulls(H1upper.size(),H1upper,H2upper.size(),H2upper,&t1,&t2);
	H1RightTangentPoint.push_back(t1);
	H2LeftTangentPoint.push_back(t2);
	tangentBetweenTwoUpperHulls(H1upper.size(),H1upper,H3upper.size(),H3upper,&t1,&t2);
	H1RightTangentPoint.push_back(t1);
	H3LeftTangentPoint.push_back(t2);
	tangentBetweenTwoUpperHulls(H1upper.size(),H1upper,H4upper.size(),H4upper,&t1,&t2);
	H1RightTangentPoint.push_back(t1);
	H4LeftTangentPoint.push_back(t2);
	tangentBetweenTwoUpperHulls(H2upper.size(),H2upper,H3upper.size(),H3upper,&t1,&t2);
	H2RightTangentPoint.push_back(t1);
	H3LeftTangentPoint.push_back(t2);
	tangentBetweenTwoUpperHulls(H2upper.size(),H2upper,H4upper.size(),H4upper,&t1,&t2);
	H2RightTangentPoint.push_back(t1);
	H4LeftTangentPoint.push_back(t2);
	tangentBetweenTwoUpperHulls(H3upper.size(),H3upper,H4upper.size(),H4upper,&t1,&t2);
	H3RightTangentPoint.push_back(t1);
	H4LeftTangentPoint.push_back(t2);
	

	H1LeftTangentPoint.push_back(0);
	H4RightTangentPoint.push_back(H4upper.size()-1);

	//Sort the tangent points obtained for each upper hull
	sort(H1RightTangentPoint.begin(),H1RightTangentPoint.end());
	sort(H2RightTangentPoint.begin(),H2RightTangentPoint.end());
	sort(H3LeftTangentPoint.begin(),H3LeftTangentPoint.end());
	sort(H4LeftTangentPoint.begin(),H4LeftTangentPoint.end());
	
	//Compute l[i]s
	lUpper.push_back(H1LeftTangentPoint[0]);
	lUpper.push_back(H2LeftTangentPoint[0]);
	lUpper.push_back(H3LeftTangentPoint[H3LeftTangentPoint.size()-1]);
	lUpper.push_back(H4LeftTangentPoint[H4LeftTangentPoint.size()-1]);
	
	//Compute r[i]s
	rUpper.push_back(H1RightTangentPoint[0]);
	rUpper.push_back(H2RightTangentPoint[0]);
	rUpper.push_back(H3RightTangentPoint[0]);
	rUpper.push_back(H4RightTangentPoint[0]);

	

	//Output the Convex Hull Points
	while(lUpper[0]<=rUpper[0])
	{
		cout<<"(PARALLEL) Convex Hull Point :" << H1upper.at(lUpper[0]).x() <<"," << H1upper.at(lUpper[0]).y() << endl;
		lUpper[0]=lUpper[0]+1;
	}
	while(lUpper[1]<=rUpper[1])
	{
		cout<<"(PARALLEL) Convex Hull Point :" << H2upper.at(lUpper[1]).x() <<"," << H2upper.at(lUpper[1]).y() << endl;
		lUpper[1]=lUpper[1]+1;
	}
	while(lUpper[2]<=rUpper[2])
	{
		cout<<"(PARALLEL) Convex Hull Point :" << H3upper.at(lUpper[2]).x() <<"," << H3upper.at(lUpper[2]).y() << endl;
		lUpper[2]=lUpper[2]+1;
	}
	while(lUpper[3]<=rUpper[3])
	{
		cout<<"(PARALLEL) Convex Hull Point :" << H4upper.at(lUpper[3]).x() <<"," << H4upper.at(lUpper[3]).y() << endl;
		lUpper[3]=lUpper[3]+1;
	}
} //End of upperTangentComputationParallel


//LOWER TANGENT COMPUTATION FOR PARALLEL VERSION
void lowerTangentComputationParallel(vector<Point> &H1lower,vector<Point> &H2lower,vector<Point> &H3lower,vector<Point> &H4lower,int N)
{
	vector<int> H1LeftTangentPoint,H2LeftTangentPoint,H3LeftTangentPoint,H4LeftTangentPoint;
	vector<int> H1RightTangentPoint,H2RightTangentPoint,H3RightTangentPoint,H4RightTangentPoint;
	vector<int> lLower,rLower;
	int t3,t4;

	
	//Compute tangents between each pair of upper hulls computed earlier
	tangentBetweenTwoLowerHulls(H1lower.size(),H1lower,H2lower.size(),H2lower,&t3,&t4);
	H1RightTangentPoint.push_back(t3);
	H2LeftTangentPoint.push_back(t4);
	tangentBetweenTwoLowerHulls(H1lower.size(),H1lower,H3lower.size(),H3lower,&t3,&t4);
	H1RightTangentPoint.push_back(t3);
	H3LeftTangentPoint.push_back(t4);
	tangentBetweenTwoLowerHulls(H1lower.size(),H1lower,H4lower.size(),H4lower,&t3,&t4);
	H1RightTangentPoint.push_back(t3);
	H4LeftTangentPoint.push_back(t4);
	tangentBetweenTwoLowerHulls(H2lower.size(),H2lower,H3lower.size(),H3lower,&t3,&t4);
	H2RightTangentPoint.push_back(t3);
	H3LeftTangentPoint.push_back(t4);
	tangentBetweenTwoLowerHulls(H2lower.size(),H2lower,H4lower.size(),H4lower,&t3,&t4);
	H2RightTangentPoint.push_back(t3);
	H4LeftTangentPoint.push_back(t4);
	tangentBetweenTwoLowerHulls(H3lower.size(),H3lower,H4lower.size(),H4lower,&t3,&t4);
	H3RightTangentPoint.push_back(t3);
	H4LeftTangentPoint.push_back(t4);
	

	H1LeftTangentPoint.push_back(0);
	H4RightTangentPoint.push_back(H4lower.size()-1);

	//Sort the tangent points obtained for each upper hull
	sort(H1RightTangentPoint.begin(),H1RightTangentPoint.end());
	sort(H2RightTangentPoint.begin(),H2RightTangentPoint.end());
	sort(H3LeftTangentPoint.begin(),H3LeftTangentPoint.end());
	sort(H4LeftTangentPoint.begin(),H4LeftTangentPoint.end());
	
	//Compute l[i]s
	lLower.push_back(H1LeftTangentPoint[0]);
	lLower.push_back(H2LeftTangentPoint[0]);
	lLower.push_back(H3LeftTangentPoint[H3LeftTangentPoint.size()-1]);
	lLower.push_back(H4LeftTangentPoint[H4LeftTangentPoint.size()-1]);
	
	//Compute r[i]s
	rLower.push_back(H1RightTangentPoint[0]);
	rLower.push_back(H2RightTangentPoint[0]);
	rLower.push_back(H3RightTangentPoint[0]);
	rLower.push_back(H4RightTangentPoint[0]);

	

	//Output the Convex Hull Points
	while(lLower[0]<=rLower[0])
	{
		cout<<"(PARALLEL) Convex Hull Point :" << H1lower.at(lLower[0]).x() <<"," << H1lower.at(lLower[0]).y() << endl;
		lLower[0]=lLower[0]+1;
	}
	while(lLower[1]<=rLower[1])
	{
		cout<<"(PARALLEL) Convex Hull Point :" << H2lower.at(lLower[1]).x() <<"," << H2lower.at(lLower[1]).y() << endl;
		lLower[1]=lLower[1]+1;
	}
	while(lLower[2]<=rLower[2])
	{
		cout<<"(PARALLEL) Convex Hull Point :" << H3lower.at(lLower[2]).x() <<"," << H3lower.at(lLower[2]).y() << endl;
		lLower[2]=lLower[2]+1;
	}
	while(lLower[3]<=rLower[3])
	{
		cout<<"(PARALLEL) Convex Hull Point :" << H4lower.at(lLower[3]).x() <<"," << H4lower.at(lLower[3]).y() << endl;
		lLower[3]=lLower[3]+1;
	}
} //End of upperTangentComputationParallel


























 




