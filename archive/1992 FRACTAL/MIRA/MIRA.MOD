MODULE Mira;

IMPORT
    Graph, IO;

(* Sample values:
    A        B        X        Y
    0.7      0.9998   0.0      12.1     Pumpkin innards
    -0.4     1.0      4.0      0.0      Burr
    -0.4     1.0      20.0     0.0      Burr shell
    -0.2     1.0      10.0     0.0      7 petal Posie
    0.3      1.0      2.0      0.0      Round 5-star
    -0.48    0.93     4.0      0.0      Three point feather
*)

CONST
    A = -0.4;
    B = 1.0;
    P = 24000;
    C = 2.0-2.0*A;

    XScale = 9.0;
    XOfs   = 29.5;
    YScale = 6.0;
    YOfs   = 26.0;

PROCEDURE DrawPlot( X, Y  :  LONGREAL );

VAR
    U, W, (* X, Y, *) Z  :  LONGREAL;
    N, XI, YI        :  CARDINAL;

BEGIN
(*    X := 0.0;
    Y := 12.1; *)
    W := A*X+C*X*X/(1.0+X*X);
    FOR N := 0 TO P DO
        IF N > 100 THEN
            Graph.Plot( TRUNC( (X+XOfs)*XScale ),
                TRUNC( (Y+YOfs)*YScale ), (((N DIV 566)) MOD 9)+6 );
(*              XI := TRUNC( (X+XOfs)*XScale );
              YI := TRUNC( (Y+YOfs)*YScale );
              Graph.Plot( XI, YI, Graph.Point( XI, YI )+(N MOD 3) ); *)
        END (* FI *);
        Z := X;
        X := B*Y+W;
        U := X*X;
        W := A*X+C*U/(1.0+U);
        Y := W-Z;
    END (* ROF *);
END DrawPlot;


VAR
    K : CHAR;

BEGIN

    Graph.InitEGA;
    Graph.GraphMode;

    DrawPlot( 4.0, 0.0 );
    DrawPlot( 20.0, 0.0 );

    K := IO.RdKey();

END Mira.
