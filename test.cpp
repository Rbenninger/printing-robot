#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>

struct Point
{
	int x;
	int y;
};

struct Letter
{
		char letter;
		int width;
		int arrLength;
		Point points[3];
};

using namespace std;

int main()
{
	Letter let1 = {'A', 5, 3, {{1, 3}, {2, 5}, {3, 6}}};
	
	cout << let1.letter << let1.width << let1.arrLength << endl;
	cout << let1.points[0].x << endl;
	
	system("PAUSE");
	return EXIT_SUCCESS;
}
