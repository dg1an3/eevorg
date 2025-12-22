/* Program 2.2-1 from Dynamical Systems and Fractals by Becker and Dorfler */
/*                      ------------------------------                       */
/* page 38 */

#include <stdio.h>
#include <graphics.h>

double feedback;
int maximalIteration = 120;
int XScreen = 640;
int YScreen = 350;
double left = 2.76;
double right = 2.78;
double top = 1.5;
double bottom = 0.0;

double f(double p, double k)
{
	return (p + k*p*(1-p));
}

int measlesValue()
{
	int i, range;
	double population;
	double deltaXperPixel;

	deltaXperPixel = (right-left)/(double)XScreen;
	for (range = 0; range <= XScreen; range++)
	{
		feedback = left + range*deltaXperPixel;
		population = 0.0001;
		for (i = 1; i <= maximalIteration; i++)
		{
			if (i > 60)
				putpixel((int)((feedback-left)*
					(double)XScreen/(right-left)),
					(int)((top-population)*
					(double)YScreen/(top-bottom)), 15);
			population = f(population, feedback);
		}
	}
	return 0;
}

main()
{
	/* request auto detection */
	int gdriver = DETECT, gmode, errorcode;
	int x, y;

	/* initialize graphics and local variables */
	initgraph(&gdriver, &gmode, "c:\\sys\\tc\\bgi");
	errorcode = graphresult();
	if (errorcode != grOk)  /* an error occurred */
	{
		printf("Graphics error: %s\n", grapherrormsg(errorcode));
		printf("Press any key to halt:");
		getch();
		exit(1); /* terminate with an error code */
	}

	/* return to graphics mode
	setgraphmode(getgraphmode()); */

	measlesValue();

	/* clean up */
	getch();
	closegraph();
	return 0;
}