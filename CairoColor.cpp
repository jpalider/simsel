#include "CairoColor.h"

CairoColor::CairoColor()
{
	cred = 0.f;
	cgreen = 0.f;
	cblue = 0.f;
}

// CairoColor::CairoColor(float r, float g, float b)
// {
// 	cred = r;
// 	cgreen = g;
// 	cblue = b;
// }

CairoColor::CairoColor(int r, int g, int b)
{
	cred = (float)r / 255;
	cgreen = (float)g / 255;
	cblue = (float)b / 255;
}

float CairoColor::red()
{
	return cred;
}

float CairoColor::green()
{
	return cgreen;
}

float CairoColor::blue()
{
	return cblue;
}
