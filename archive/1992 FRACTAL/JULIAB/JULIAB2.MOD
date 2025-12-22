MODULE JuliaB;

IMPORT Graph, MATHLIB, IO, Lib;

CONST
    P = 8;
    Delay = 87;

VAR
    X1, Y1, X2, Y2  :  ARRAY [ 0..P ] OF LONGREAL;
    AA, BA  :  ARRAY [ 0..Delay ] OF LONGREAL;
    X, Y, R, T, OA, OB, A, B, A1, B1, AS, BS, R1  :  LONGREAL;
    S, J, N, M, At  :  CARDINAL;
    K : CHAR;

PROCEDURE DrawPlot;
BEGIN
    A1 := (0.25-A) / 2.0;
    B1 := -B / 2.0;
    R1 := MATHLIB.Sqrt( A1*A1 + B1*B1 );

    X1[ 0 ] := 0.5+MATHLIB.Sqrt( R1+A1 );
    Y1[ 0 ] := MATHLIB.Sqrt( R1-A1 );
    IF ( B > 0.0 ) THEN
        Y1[ 0 ] := -Y1[ 0 ];
    END (* IF *);
    Graph.Plot( TRUNC( (X1[ 0 ]+2.667)*119.81),
        TRUNC( (Y1[ 0 ]+2.000)*119.66), Lib.RANDOM( 15 ) );
    S := 1;
    FOR J := S TO P DO
        X := X1[ J-1 ];
        Y := Y1[ J-1 ];
        R := MATHLIB.Sqrt( (X-A)*(X-A)+(Y-B)*(Y-B) )/2.0;
        T := (X-A) / 2.0;
        X1[ J ] := MATHLIB.Sqrt( R+T );
        X2[ J ] := -X1[ J ];
        Y1[ J ] := MATHLIB.Sqrt( R-T );
        IF Y<B THEN
            Y1[ J ] := -Y1[ J ];
        END (* IF *);
        Y2[ J ] := -Y1[ J ];
        Graph.Plot( TRUNC( (X1[ J ]+2.667)*119.81),
            TRUNC( (Y1[ J ]+2.000)*119.66), J+14-P );
        Graph.Plot( TRUNC( (X2[ J ]+2.667)*119.81),
            TRUNC( (Y2[ J ]+2.000)*119.66), J+14-P );
    END (* FOR *);
    FOR M := 1 TO (* 2047 *) (1 << (P-1))-1 DO
        S := P;
        N := M;
        WHILE (N MOD 2) = 0 DO
            N := N DIV 2;
            DEC( S );
        END (* WHILE *);
        X1[ S-1 ] := X2[ S-1 ];
        Y1[ S-1 ] := Y2[ S-1 ];
        FOR J := S TO P DO
            X := X1[ J-1 ];
            Y := Y1[ J-1 ];
            R := MATHLIB.Sqrt( (X-A)*(X-A)+(Y-B)*(Y-B) )/2.0;
            T := (X-A) / 2.0;
            X1[ J ] := MATHLIB.Sqrt( R+T );
            X2[ J ] := -X1[ J ];
            Y1[ J ] := MATHLIB.Sqrt( R-T );
            IF Y<B THEN
                Y1[ J ] := -Y1[ J ];
            END (* IF *);
            Y2[ J ] := -Y1[ J ];
            Graph.Plot( TRUNC( (X1[ J ]+2.667)*119.81),
                TRUNC( (Y1[ J ]+2.000)*119.66), S+14-P );
            Graph.Plot( TRUNC( (X2[ J ]+2.667)*119.81),
                TRUNC( (Y2[ J ]+2.000)*119.66), S+14-P );
        END (* FOR *);
    END (* FOR *);

END DrawPlot;


PROCEDURE ErasePlot;
VAR
    A, B  :  LONGREAL;
BEGIN
    A := AA[ At ];
    B := BA[ At ];
    A1 := (0.25-A) / 2.0;
    B1 := -B / 2.0;
    R1 := MATHLIB.Sqrt( A1*A1 + B1*B1 );

    X1[ 0 ] := 0.5+MATHLIB.Sqrt( R1+A1 );
    Y1[ 0 ] := MATHLIB.Sqrt( R1-A1 );
    IF ( B > 0.0 ) THEN
        Y1[ 0 ] := -Y1[ 0 ];
    END (* IF *);
    Graph.Plot( TRUNC( (X1[ 0 ]+2.667)*119.81),
        TRUNC( (Y1[ 0 ]+2.000)*119.66), 0 );
    S := 1;
    FOR J := S TO P DO
        X := X1[ J-1 ];
        Y := Y1[ J-1 ];
        R := MATHLIB.Sqrt( (X-A)*(X-A)+(Y-B)*(Y-B) )/2.0;
        T := (X-A) / 2.0;
        X1[ J ] := MATHLIB.Sqrt( R+T );
        X2[ J ] := -X1[ J ];
        Y1[ J ] := MATHLIB.Sqrt( R-T );
        IF Y<B THEN
            Y1[ J ] := -Y1[ J ];
        END (* IF *);
        Y2[ J ] := -Y1[ J ];
        Graph.Plot( TRUNC( (X1[ J ]+2.667)*119.81),
            TRUNC( (Y1[ J ]+2.000)*119.66), 0 );
        Graph.Plot( TRUNC( (X2[ J ]+2.667)*119.81),
            TRUNC( (Y2[ J ]+2.000)*119.66), 0 );
    END (* FOR *);
    FOR M := 1 TO (* 2047 *) (1 << (P-1))-1 DO
        S := P;
        N := M;
        WHILE (N MOD 2) = 0 DO
            N := N DIV 2;
            DEC( S );
        END (* WHILE *);
        X1[ S-1 ] := X2[ S-1 ];
        Y1[ S-1 ] := Y2[ S-1 ];
        FOR J := S TO P DO
            X := X1[ J-1 ];
            Y := Y1[ J-1 ];
            R := MATHLIB.Sqrt( (X-A)*(X-A)+(Y-B)*(Y-B) )/2.0;
            T := (X-A) / 2.0;
            X1[ J ] := MATHLIB.Sqrt( R+T );
            X2[ J ] := -X1[ J ];
            Y1[ J ] := MATHLIB.Sqrt( R-T );
            IF Y<B THEN
                Y1[ J ] := -Y1[ J ];
            END (* IF *);
            Y2[ J ] := -Y1[ J ];
            Graph.Plot( TRUNC( (X1[ J ]+2.667)*119.81),
                TRUNC( (Y1[ J ]+2.000)*119.66), 0 );
            Graph.Plot( TRUNC( (X2[ J ]+2.667)*119.81),
                TRUNC( (Y2[ J ]+2.000)*119.66), 0 );
        END (* FOR *);
    END (* FOR *);

END ErasePlot;


BEGIN
    Graph.InitVGA;

    LOOP

        Lib.RANDOMIZE;

        Graph.GraphMode;

        A := LONGREAL( Lib.RAND() )*3.0 - 1.5;
        B := LONGREAL( Lib.RAND() )*3.0 - 1.5;
        AS := LONGREAL( Lib.RAND() )*0.2 - 0.1;
        BS := LONGREAL( Lib.RAND() )*0.2 - 0.1;

        FOR At := 0 TO Delay DO

            DrawPlot;

            AA[ At ] := A;
            BA[ At ] := B;

            A := A + AS;
            AS := AS - LONGREAL( Lib.RAND() )*A*0.02
                - LONGREAL( Lib.RAND() )*0.02*AS + 0.01
                + LONGREAL( Lib.RAND() )*0.04 - 0.02;
            B := B + BS;
            BS := BS - LONGREAL( Lib.RAND() )*B*0.02
                - LONGREAL( Lib.RAND() )*0.02*BS + 0.01
                + LONGREAL( Lib.RAND() )*0.04 - 0.02;

        END (* FOR *);

        At := 0;
        REPEAT

            ErasePlot;
            AA[ At ] := A;
            BA[ At ] := B;

            DrawPlot;
            A := A + AS;
            AS := AS - LONGREAL( Lib.RAND() )*A*0.02
                - LONGREAL( Lib.RAND() )*0.02*AS + 0.01
                + LONGREAL( Lib.RAND() )*0.04 - 0.02;
            B := B + BS;
            BS := BS - LONGREAL( Lib.RAND() )*B*0.02
                - LONGREAL( Lib.RAND() )*0.02*BS + 0.01
                + LONGREAL( Lib.RAND() )*0.04 - 0.02;

            INC( At );
            IF At > Delay THEN At := 0 END;

        UNTIL IO.KeyPressed();
        K := IO.RdKey();

    END (* LOOP *);


END JuliaB.

