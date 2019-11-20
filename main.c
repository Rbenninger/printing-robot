#include "PC_FileIO.c"

const string fileName = "test.txt";
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

int totalChar;
int totalTime;

bool OpenFile() //opens file and makes sure it exists
{
	TFileHandle fin;
	bool fileOkay = openReadPC(fin, fileName);
	return fileOkay;
}

int GetIndex(char letter) //returns index of letter in the array
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

bool OnPaper() //checks to make sure the robot is on paper
{
	return SensorValue(COLOR) == (int)colorWhite;
}

double SpaceLeft() //returns the space left in a line in cm
{
	double moved = nMotorEncoder[X_MOTOR]*180*PI*SMALL_RADIUS;
	return abs(PAGE_WIDTH - moved);
}

double GetWidth(string word) // gets the width of a word in cm
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

void MovePen(Point loc)//moves pen to a relative point
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

void AddSmall() //adds a small space between letters
{
	MovePen({SMALL, 0});
}

void AddLarge() //adds a large space between words
{
	MovePen({LARGE, 0});
}

bool NotSkew() //checks to make sure the robot is aligned
{
	return abs(getGyroDegrees(GYRO)) < TOL;
}

void PauseTimer(int current) //adds the current time to the total
{
	totalTime += current;
}

void PenUp() //lifts pen off the page
{

}

void PenDown() //puts the pen on the page
{

}

void PressEnter() //waits for the user to press and release the enter button
{
	while(!getButtonPress(buttonCenter))
	{}
	while(getButtonPress(buttonCenter))
	{}
}

void ResetArm() //moves the arm all the way to the left
{
	motor[X_MOTOR] = 30;
	while (SensorValue[TOUCH] == 0)
	{}
	motor[X_MOTOR] = 0;

	nMotorEncoder[X_MOTOR] = 0;
}

void NewLine() //resets the arm and moves down a line
{
	ResetArm();
	MovePen({0,-NEW});
}

void RefillPaper() //handles event when the robot reaches the end of the page
{
	PauseTimer(time[T1]);
	do
	{
		DisplayString(1, "Place on a new piece of paper and press enter");
		PressEnter();
	} while(!OnPaper());

	ResetArm();
	time1[T1] = 0;
}

bool StartUp() //handles start up
{
	if (OpenFile())
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
		totalTime = 0;
		totalChar = 0;
		time1[T1] = 0;

		return true;
	}
	else
		return false;
}

void ShutDown() //handles shutdown
{
	PauseTimer(time1[T1]);
	ResetArm();

	DisplayString(1, "Printing complete");
	DisplayString(2, "Total time: %f", totalTime/1000);
	DisplayString(3, "Total number of characters: %d", totalChar);

	closeFilePC(fileName);
}

void WriteLetter(char letter) //follows the path for a letter
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

bool TooLong(string word) //checks to see if the word needs to be split
{
	return GetWidth(word) > PAGE_WIDTH;
}

int WriteLongWord(string word) //adds hyphens where needed while writing a word
{
	int length = 0;
	for (int curChar = 0; curChar < strlen(word); curChar++)
	{
		int charIndex = GetIndex(word[curChar]);
		double width = (SMALL + letter[charIndex].width + letter[27].width)*SCALE;

		if (SpaceLeft() < width && curChar != strlen(word) -1)
		{
			WriteLetter('-');
			if (!OnPaper())
				RefillPaper();
			else
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

int WriteWord(string word) //writes a word
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
		string word = "";
		while (readTextPC(fin, word))
		{
			if (!NotSkew())
			{
				do
				{
					DisplayString(1, "Align robot on paper and press enter");
					PressEnter();
				} while (!OnPaper());
				resetGyro(GYRO);
			}

			double width = GetWidth(word);
			if (width > SpaceLeft() && !TooLong(word)) //not enough space left on the current line
			{
				if (!OnPaper())
					RefillPaper();
				else
					NewLine();
			}

			if (TooLong(word))
				totalChar += WriteLongWord(word);
			else
				totalChar += WriteWord(word);
		}
		ShutDown();
	}
	else
	{
		DisplayString(1, "Problem opening file. Press enter to end program");
	}
	PressEnter();
}
