#include <stdio.h>
#include "\sys\tc\cul\culproto.h"

double population;
double feedback;
int maximalIteration;

double f(double p, double k)
{
	return (p + k*p*(1-p));
}

int measlesValue()
{
	int i;

	for (i = 1; i <= maximalIteration; i++)
	{
		population = f(population, feedback);
		printf("After %i iterations p has the value %5.4lf\n",
			i, population);
		sound1((int)(population*220), 1);
	}
	return 0;
}

main()
{
	printf("Calculation of Measles Values\n");
	printf("=============================\n");

	printf("Initial population p (0 to 1) -> ");
	scanf("%lf", &population);

	printf("\nFeedback parameter k (0 to 3) -> ");
	scanf("%lf", &feedback);

	printf("\nMaximum Iterations            -> ");
	scanf("%i", &maximalIteration);

	measlesValue();

	return 0;
}