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

struct Letter
{
	public:
		char letter;
		int width;
		int arrLength;
		Point points[10]
		
		Letter(char let, int wid, int arrLen, Point Points[10])
		{
			letter = let;
			width = wid;
			points = Points;
			arrLength = arrLen;
		}
};

Letter letters[27] = [
Letter("A", 7, [Point(0,0), ]),
Letter("B", ),
Letter("C", ),
Letter("D", ),
Letter("E", ),
Letter("F", ),
Letter("G", ),
Letter("H", ),
Letter("I", ),
Letter("J", ),
Letter("K", ),
Letter("L", ),
Letter("M", ),
Letter("N", ),
Letter("O", ),
Letter("P", ),
Letter("Q", ),
Letter("R", ),
Letter("S", ),
Letter("T", ),
Letter("U", ),
Letter("V", ),
Letter("W", ),
Letter("X", ),
Letter("Y", ),
Letter("Z", ),
Letter(".", )
]
