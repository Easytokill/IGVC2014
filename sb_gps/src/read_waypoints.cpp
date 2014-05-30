#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "read_waypoints.h"

using namespace std;

double* ReturnWaypoints() 
{
	string output;
	int count = 0;
	int array_size;
	double* waypoints_array;
	ifstream waypoints_file ("waypoints.txt");
	if (waypoints_file.is_open())
	{
		while (getline(waypoints_file, output))
		{
			if (count == 0) {
				array_size = atoi(output.c_str())*2;
				waypoints_array = new double [array_size];
				//cout << array_size << "\n";
				count++;
			}
			
			else { 
				waypoints_array[count-1] = atof(output.c_str());
				count++;
				//cout << waypoints_array[count-1] << "\n"; 
			}
		}
		waypoints_file.close();
	}

	else
	{
		cout << "Unable to open file";
	}

	return waypoints_array;
}

int main() {
	double* x = ReturnWaypoints();
	int size = sizeof(x);
	int i = 0;
	cout << size << "\n";
	while ( i < size*2 ) { cout << x[i] << "\n"; i++; }
	return 0;
}
