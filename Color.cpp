#include "Color.h"

UINT Random(UINT max)
{
	return rand() % (max + 1);
}

COLORREF GetRandomColor()
{
	return RGB(Random(255), Random(255), Random(255));
}

COLORREF GetNextColor(COLORREF tempColor, UINT dir)
{
	COLORREF c = tempColor;
	if (dir == 1)
	{
		if (c == 0xffffff)
			c = 0x000000;
		else c += 0x000005;
	}
	else
	{
		if (c == 0x000000)
			c = 0xffffff;
		else c -= 0x000005;
	}
	return c;
}
