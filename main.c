#include "PC_FileIO.c"

const double PAGE_WIDTH = 21.59;
const double SCALE = 0.5;
const int SMALL = 1;
const int LARGE = 3;
const int NEW = 3;
const int TOL = 5;
const int WRITE_POWER = 5;
const double BIG_RADIUS = 2.75;
const double SMALL_RADIUS = 1;
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
	double moved = nMotorEncoder[X_MOTOR]*180*PI*SMALL_RADIUS;
	return fabs(PAGE_WIDTH - moved);
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
	
	return width*SCALE;
}

void MovePen(Point loc)
{
	int direction = 0;
	if (loc.x != 0)
	{
		int curEnc = nMotorEncoder[X_MOTOR];
		if (loc.x < 0)
			direction = -1;
		else
			direction = 1;
			
		double EncLimit = fabs(loc.x*PI*SMALL_RADIUS/180);
		
		motor[X_MOTOR] = direction*WRITE_POWER;
		while(fabs(curEnc - nMotorEncoder[X_MOTOR]) < EncLimit)
		{}
		motor[X_MOTOR] = 0;
	}
	if (loc.y != 0)
	{
		int curEnc = nMotorEncoder[Y_LEFT];
		if (loc.y < 0)
			direction = -1;
		else
			direction = 1;
			
		double EncLimit = fabs(loc.y*PI*BIG_RADIUS/180);
			
		motor[Y_LEFT] = motor[Y_RIGHT] = direction*WRITE_POWER;
		while(fabs(curEnc - nMotorEncoder[Y_LEFT]) < EncLimit)
		{}
		motor[Y_LEFT] = motor[Y_RIGHT] = 0;
	}
}

void AddSmall()
{
	MovePen(Point(SMALL, 0));
}

void AddLarge()
{
	MovePen(Point(LARGE, 0));
}

bool NotSkew()
{
	return fabs(getGyroDegrees(GYRO)) < TOL;
}

void PauseTimer(int current)
{
	totalTime += current;
}

void PenUp()
{
	
}

void PenDown()
{
	
}

void PressEnter()
{
	while(!getButtonPress(buttonCenter))
	{}
	while(getButtonPress(buttonCenter))
	{}
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
	PauseTimer(time[T1]);
	do
	{
		DisplayString(1, "Place on a new piece of paper and press enter");
		PressEnter();
	} while(!OnPaper());
	
	time1[T1] = 0;
}

bool StartUp()
{
	istream fin = ("input.txt");
	if (OpenFile(fin))
	{
		SensorType[TOUCH] = SensorEV3_Touch;
		SensorType[GYRO] = SensorEV3_Gyro;
		SensorType[COLOR] = SensorEV3_Color;
		wait1Msec(50);
		SensorMode[GYRO] = modeEV3Gyro_RateAndAngle;
		SensorMode[COLOR] = modeEV3Color_Color;
		wait1Msec(50);
		
		do
		{
			DisplayString(1, "Place on a piece of paper and press enter");
			PressEnter();
		} while(!OnPaper());
		
		resetGyro(GYRO);
		time1[T1] = 0;
		
		return true;
	}
	else
		return false;
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
	
	for (int curPoint = 1; curPoint < Letters[charIndex].arrLength-1; curPoint++)
	{
		MovePen(Letters[charIndex].points[curPoint]);
	}
	
	PenUp();
	MovePen(letters[charIndex].points[letters[charIndex].arrLength-1]);
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
	if (StartUp())
	{
		
	}
	else
	{
		DisplayString(1, "Problem opening file. Press enter to end program");
	}
	PressEnter();
}
