#include <math.h>
#include <graphics.h>
#include <complex.h>

// Graphing f(z) = L*exp(z) iterated over L

main()
{
	int graphdriver = DETECT;
	int graphmode = DETECT;
	initgraph(&graphdriver, &graphmode, "C:\\TC\\LIB");

	double L_real = 0.0;
	double L_imag = 0.0;
	for (L_real = 0.0; L_real < 1.0; L_real += (1.0/640.0))
		for (L_imag = 0.0; L_imag < 1.0; L_imag += (1.0/350.0))
		{
			int count;
			complex z = 0.0;
			for (count = 0; count < 1000; count++)
				z = complex(L_real, L_imag)*exp(z);
			putpixel((int)L_real*640, (int)L_imag*350,
				(int)abs(z));
		}
}