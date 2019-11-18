/* example
struct Point
{
	public:
		int x;
		int y;
};

struct Letter
{
	public:
		char letter;
		double width;
		Point points[5];
};

ASCII values A=65 Z=90 .=46
in array A=0 Z=25 .=26
*/

#include "PC_FileIO.c"

const double PAGE_WIDTH = 21.59;
const double SCALE = 0.5;
const int SMALL = 1;
const int LARGE = 3;
const int NEW = 3;
const tmotor Y_LEFT = motorA;
const tmotor Y_RIGHT = motorD;
const tmotor X_MOTOR = motorB;
const tmotor Z_MOTOR = motorC;
const tsensor COLOR = S1;
const tsensor GYRO = S2;
const tsensor TOUCH = S3;

public int totalChar = 0;
public int totalTime = 0;

struct Point
{
	public:
		int x;
		int y;
		
		Point (int x0, int y0)
		{
			x = x0;
			y = y0;	
		}	
};

bool OpenFile(istream fin)
{
	
}

bool OnPaper()
{
	return SensorValue(COLOR) == (int)colorWhite;
}

double SpaceLeft()
{
	
}

double GetWidth(string word)
{
	double width = 0;
	
	for (int curChar = 0; curChar < strlen(word); curChar++)
	{
		int charIndex = -1;
		if (word[curChar] = ".")
			charIndex = 26;
		else if ((int)word[curChar] > 96 && (int)word[curChar] < 123) //lowercase
			charIndex = (int)word[curChar] - 96;
		else
			charIndex = (int)word[curChar] - 65;
			
		width += letters[charIndex].width;
		if (curChar != strlen(word) -1)
			width += SMALL;
	}
	
	return width;
}

void AddSmall()
{
	
}

void AddLarge()
{
	
}

bool NotSkew()
{
	
}

void MovePen(Point loc)
{
	
}

void PauseTimer(int current)
{
	
}

void PenUp()
{
	
}

void PenDown()
{
	
}

void PressEnter()
{
	
}

void ResetArm()
{
	motor[X_MOTOR] = 30;
	while (SensorValue[TOUCH] == 0)
	{}
	motor[X_MOTOR] = 0;
	
	nMotorEncoder[X_MOTOR] = 0;
}

void NewLine()
{
	ResetArm();
	MovePen(Point(0,-NEW));
}

void RefillPaper()
{
	
}

bool StartUp()
{
	
}

void ShutDown()
{
	PauseTimer(time1[T1]);
	ResetArm();
	
	DisplayString(1, "Total time: %f", totalTime/1000);
	DisplayString(2, "Total number of characters: %d", totalChar);
	
	PressEnter();
	//TODO: close file
}

void WriteLetter(char letter)
{
	int charIndex = -1;
	if (letter = ".")
		charIndex = 26;
	else if ((int)letter > 96 && (int)letter < 123) //lowercase
		charIndex = (int)letter - 96;
	else
		charIndex = (int)letter - 65;
		
	MovePen(letters[charIndex].points[0]);
	PenDown();
	
	for (int curPoint = 1; curPoint < Letters[charIndex].arrLength; curPoint++)
	{
		MovePen(Letters[charIndex].points[curPoint]);
	}
	
	PenUp();
}

bool TooLong(string word)
{
	return GetWidth(word) > PAGE_WIDTH;
}

int WriteWord(string word)
{
	for (int charIndex = 0; charIndex < strlen(word); char++)
	{
		WriteLetter(word[charIndex]);
		if (charIndex != strlen(word)-1)
			AddSmall();
	}
	AddLarge();
	
	return word.length;
}

task main()
{

}
