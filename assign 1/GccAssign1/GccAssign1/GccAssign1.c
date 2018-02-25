/*
 * GccAssign1.c
 *
 * Created: 2/24/2018 1:06:55 PM
 *  Author: sawar1
 */ 


#include <avr/io.h>

int main()
{
	int *X = 0x0222;
	int *Y = 0x0400;
	int *Z = 0x0600;
	int a = 0;
	char sum = 0;
	char R16;
	char R17;
	char R18;
	char R19;
	
	for(a=0; a<300; a++)
	{
		sum = 0;
		sum = X;
		*X = sum;
		if(sum/5 == 0)
		{
			*Y = sum;
			Y++;
			R16 = sum;
			R17 = sum;
		}
		else
		{
			*Z = sum;
			Z++;
			R18 = sum;
			R19 = sum;
		}
		X++;
	}
}

