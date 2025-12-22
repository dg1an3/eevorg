/* MODULE JuliaB; */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graphics.h>

#define P 8
#define Delay 3

double X1[P], Y1[P], X2[P], Y2[P];
double AA[Delay], BA[Delay];
double X, Y, R, T, OA, OB, A, B, A1, B1, AS, BS, R1;
unsigned S, J, N, M, At;
char K;
/* Color : Graphics.PaletteType; */

void DrawPlot()
{
    A1 = (0.25-A) / 2.0;
    B1 = -B / 2.0;
    R1 = sqrt( A1*A1 + B1*B1 );

    X1[0] = 0.5+sqrt( R1+A1 );
    Y1[0] = sqrt( R1-A1 );
    if (B > 0.0)
	Y1[0] = -Y1[0];
    putpixel( (int)( (X1[ 0 ]+2.667*4)*119.81/2),
	(int)( (Y1[ 0 ]+2.000*4)*87.253/2), random( 15 ) );
    S = 1;
    for (J=S;J<=P;J++)
    {
	X = X1[ J-1 ];
	Y = Y1[ J-1 ];
	R = sqrt( (X-A1)*(X-A1)+(Y-B1)*(Y-B1) )/2.0;
	T = (X-A1) / 2.0;
	X1[ J ] = sqrt( R+T );
	X2[ J ] = -X1[ J ];
	Y1[ J ] = sqrt( R-T );
	if ( Y<B1 )
	    Y1[ J ] = -Y1[ J ];
	Y2[ J ] = -Y1[ J ];
	putpixel( (int)( (X1[ J ]+2.667*4)*119.81/2),
	    (int)( (Y1[ J ]+2.000*4)*87.253/2), ((J+4) % 15)+1 );
	putpixel( (int)( (X2[ J ]+2.667*4)*119.81/2),
	    (int)( (Y2[ J ]+2.000*4)*87.253/2), ((J+4) % 15)+1 );
    }/* for */
    for (M=1;M<=(1<<(P-1));M++)
	/* M := 1 TO (* 2047 *) (1 << (P-1))-1 DO */
    {
	S = P;
	N = M;
	while (N % 2==0)
	{
	/* WHILE (N MOD 2) = 0 DO */
	    N = N/2;
	    S--;
	}/* while */
	X1[ S-1 ] = X2[ S-1 ];
	Y1[ S-1 ] = Y2[ S-1 ];
	for (J=S;J<=P;J++)
	{
	/* FOR J := S TO P DO */
	    X = X1[ J-1 ];
	    Y = Y1[ J-1 ];
	    R = sqrt( (X-A1)*(X-A1)+(Y-B1)*(Y-B1) )/2.0;
	    T = (X-A1) / 2.0;
	    X1[ J ] = sqrt( R+T );
	    X2[ J ] = -X1[ J ];
	    Y1[ J ] = sqrt( R-T );
	    if (Y<B1)
		Y1[ J ] = -Y1[ J ];
	    Y2[ J ] = -Y1[ J ];
	    putpixel( (int)( (X1[ J ]+2.667*4)*119.81/2) % 640,
		(int)( (Y1[ J ]+2.000*4)*87.253/2) % 349, ((S+4) % 15)+1 );
	    putpixel( (int)( (X2[ J ]+2.667*4)*119.81/2) % 640,
		(int)( (Y2[ J ]+2.000*4)*87.253/2) % 349, ((S+4) % 15)+1 );
	}/* for */
    }/* for */

}/* DrawPlot */


void ErasePlot()
{
    double A, B;

    A = AA[ At ];
    B = BA[ At ];
    A1 = (0.25-A) / 2.0;
    B1 = -B / 2.0;
    R1 = sqrt( A1*A1 + B1*B1 );

    X1[ 0 ] = 0.5+sqrt( R1+A1 );
    Y1[0] = sqrt( R1-A1 );
    if (B > 0.0)
	Y1[0] = -Y1[0];
    putpixel( (int)( (X1[ 0 ]+2.667*4)*119.81/2),
	(int)( (Y1[ 0 ]+2.000*4)*87.253/2), 0);
    S = 1;
    for (J=S;J<=P;J++)
    {
	X = X1[ J-1 ];
	Y = Y1[ J-1 ];
	R = sqrt( (X-A1)*(X-A1)+(Y-B1)*(Y-B1) )/2.0;
	T = (X-A1) / 2.0;
	X1[ J ] = sqrt( R+T );
	X2[ J ] = -X1[ J ];
	Y1[ J ] = sqrt( R-T );
	if ( Y<B1 )
	    Y1[ J ] = -Y1[ J ];
	Y2[ J ] = -Y1[ J ];
	putpixel( (int)( (X1[ J ]+2.667*4)*119.81/2),
	    (int)( (Y1[ J ]+2.000*4)*87.253/2), 0);
	putpixel( (int)( (X2[ J ]+2.667*4)*119.81/2),
	    (int)( (Y2[ J ]+2.000*4)*87.253/2), 0);
    }/* for */
    for (M=1;M<=(1<<(P-1));M++)
	/* M := 1 TO (* 2047 *) (1 << (P-1))-1 DO */
    {
	S = P;
	N = M;
	while (N % 2==0)
	{
	/* WHILE (N MOD 2) = 0 DO */
	    N = N/2;
	    S--;
	}/* while */
	X1[ S-1 ] = X2[ S-1 ];
	Y1[ S-1 ] = Y2[ S-1 ];
	for (J=S;J<=P;J++)
	{
	/* FOR J := S TO P DO */
	    X = X1[ J-1 ];
	    Y = Y1[ J-1 ];
	    R = sqrt( (X-A1)*(X-A1)+(Y-B1)*(Y-B1) )/2.0;
	    T = (X-A1) / 2.0;
	    X1[ J ] = sqrt( R+T );
	    X2[ J ] = -X1[ J ];
	    Y1[ J ] = sqrt( R-T );
	    if (Y<B)
		Y1[ J ] = -Y1[ J ];
	    Y2[ J ] = -Y1[ J ];
	    putpixel( (int)( (X1[ J ]+2.667*4)*119.81/2),
		(int)( (Y1[ J ]+2.000*4)*87.253/2), 0);
	    putpixel( (int)( (X2[ J ]+2.667*4)*119.81/2),
		(int)( (Y2[ J ]+2.000*4)*87.253/2), 0);
	}/* for */
    }/* for */

}/* ErasePlot */


void main(void)
{
    int graph_driver, graph_mode;
    int r1, r2;

    #define next_A_B B = B + BS;\
	BS = BS - (((double)rand())*0.06*B/32768.0) + \
		-(((double)rand())*0.03*BS/32768.0 - 0.015) + \
		((double)rand())*0.08/32768.0 - 0.04; \
	A = A + AS; \
	AS = AS - (((double)rand())*0.06*A/32768.0) + \
		-(((double)rand())*0.03*AS/32768.0 - 0.015) + \
		((double)rand())*0.08/32768.0 - 0.04

    randomize();

    for (;;)
    {
	randomize();
	graph_driver = DETECT;
	graph_mode = DETECT;
	initgraph(&graph_driver, &graph_mode, "");


	A = ((double)rand())*0.5/32768.0 - 0.25;
	B = ((double)rand())*0.5/32768.0 - 0.25;
	AS = ((double)rand())*0.5/32768.0 - 0.25;
	BS = ((double)rand())*0.5/32768.0 - 0.25;

/*	for (At = 0; At<=Delay; At++)
	{
	    DrawPlot();

	    AA[ At ] = A;
	    BA[ At ] = B;

	    next_A_B;
	};   /* for */ */

	At = 0;
	for (;;)
	{
	    ErasePlot();
	    AA[ At ] = A;
	    BA[ At ] = B;

	    DrawPlot();
	    next_A_B;
		/*
	    if ( ++At >= Delay ) At = 0; */

	}
    }

}/* main */

