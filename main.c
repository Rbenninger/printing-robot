#include "PC_FileIO.c"

const double PAGE_WIDTH = 21.59; //cm
const double SCALE = 0.15; //10 units of points are equal to 1.5 cm
const int SMALL = 1; //small space between letters
const int LARGE = 5; //large space between words
const int NEW = 5; //space between lines
const int TOL = 5; //robot can be up to 5deg skew
const int WRITE_POWER = 5; //writing speed
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
	int x;
	int y;
};

bool OpenFile(string fileName)
{
	TFileHandle fin;
	bool fileOkay = openReadPC(fin, fileName);
	return fileOkay;
}

int GetIndex(char letter)
{
		if (letter = '.')
			return 26;
		else if (letter = '-')
			return 27;
		else if ((int)letter > 96 && (int)letter < 123) //lowercase
			return (int)letter - 96;
		else if ((int)letter > 65 && (int)letter < 90)
			return (int)letter - 65;
		else
			return -1;
}

bool OnPaper()
{
	return SensorValue(COLOR) == (int)colorWhite;
}

string SubString(string word, int start, int length)
{
	string output = "";
	
	for (int cur = start; cur < strlen(word); cur++)
	{
		
	}
}

double SpaceLeft() //cm
{
	double moved = nMotorEncoder[X_MOTOR]*180*PI*SMALL_RADIUS;
	return abs(PAGE_WIDTH - moved);
}

double GetWidth(string word)
{
	double width = 0;
	
	for (int curChar = 0; curChar < strlen(word); curChar++)
	{
		int charIndex = GetIndex(word[curChar]);
		
		if (charIndex != -1)
		{
			width += letters[charIndex].width;
			if (curChar != strlen(word) -1)
				width += SMALL;
		}	
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
			
		double EncLimit = abs(loc.x*PI*SMALL_RADIUS/180);
		
		motor[X_MOTOR] = direction*WRITE_POWER;
		while(abs(curEnc - nMotorEncoder[X_MOTOR]) < EncLimit)
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
		while(abs(curEnc - nMotorEncoder[Y_LEFT]) < EncLimit)
		{}
		motor[Y_LEFT] = motor[Y_RIGHT] = 0;
	}
}

void AddSmall()
{
	MovePen({SMALL, 0});
}

void AddLarge()
{
	MovePen({LARGE, 0});
}

bool NotSkew()
{
	return abs(getGyroDegrees(GYRO)) < TOL;
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
	MovePen({0,-NEW});
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
		
		ResetArm();
		nMotorEncoder[Y_LEFT] = 0;
		nMotorEncoder[X_MOTOR] = 0;
		resetGyro(GYRO);
		time1[T1] = 0;
		
		return true;
	}
	else
		return false;
}

void ShutDown(string fileName)
{
	PauseTimer(time1[T1]);
	ResetArm();
	
	DisplayString(1, "Total time: %f", totalTime/1000);
	DisplayString(2, "Total number of characters: %d", totalChar);
	
	PressEnter();
	closeFilePC(fileName);
}

void WriteLetter(char letter)
{
	int charIndex = GetIndex(letter);
	
	if (charIndex != -1)
	{
		MovePen(letters[charIndex].points[0]);
		PenDown();
	
		for (int curPoint = 1; curPoint < Letters[charIndex].arrLength-1; curPoint++)
		{
			MovePen(Letters[charIndex].points[curPoint]);
		}
	
		PenUp();
		MovePen(letters[charIndex].points[letters[charIndex].arrLength-1]);
	}
}

bool TooLong(string word)
{
	return GetWidth(word) > PAGE_WIDTH;
}

int WriteLong(string word)
{
	int length = 0;
	for (int curChar = 0; curChar < strlen(word); curChar++)
	{
		int charIndex = GetIndex(word[curChar]);
		double width = (SMALL + letter[charIndex].width + letter[27].width)*SCALE;
		
		if (SpaceLeft() < width && curChar != strlen(word) -1)
		{
			WriteLetter('-');
			NewLine();
			WriteLetter(word[curChar]);
			AddSmall();
			length += 2;
		}
		else
		{
			WriteLetter(word[curChar]);
			if (curChar != strlen(word) - 1)
				AddSmall();
			length++;
		}
	}
	
	return length;
}

int WriteWord(string word)
{
	for (int curChar = 0; curChar < strlen(word); curChar++)
	{
		WriteLetter(word[curChar]);
		if (curChar != strlen(word)-1)
			AddSmall();
	}
	AddLarge();
	
	return strlen(word);
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
