#ifndef SIM_CAIROCOLOR_H
#define SIM_CAIROCOLOR_H

class CairoColor
{
private:
	float cred;
	float cgreen;
	float cblue;
public:
	CairoColor();
	CairoColor(float r, float g, float b);
	CairoColor(int r, int g, int b);
	float red();
	float green();
	float blue();	      
};
#endif /* SIM_CAIROCOLOR_H */
