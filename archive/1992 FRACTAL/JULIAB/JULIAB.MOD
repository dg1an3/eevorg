MODULE JuliaB;

IMPORT Graph, Graphics, MATHLIB, IO, Lib;

CONST
    P = 9;
    Delay = 27;

VAR
    X1, Y1, X2, Y2  :  ARRAY [ 0..P ] OF LONGREAL;
    AA, BA  :  ARRAY [ 0..Delay ] OF LONGREAL;
    X, Y, R, T, OA, OB, A, B, A1, B1, AS, BS, R1  :  LONGREAL;
    S, J, N, M, At  :  CARDINAL;
    K : CHAR;
    Color : Graphics.PaletteType;

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
        TRUNC( (Y1[ 0 ]+2.000)*87.253), Lib.RANDOM( 15 ) );
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
            TRUNC( (Y1[ J ]+2.000)*87.253), ((J+4) MOD 15)+1 );
        Graph.Plot( TRUNC( (X2[ J ]+2.667)*119.81),
            TRUNC( (Y2[ J ]+2.000)*87.253), ((J+4) MOD 15)+1 );
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
                TRUNC( (Y1[ J ]+2.000)*87.253), ((S+4) MOD 15)+1 );
            Graph.Plot( TRUNC( (X2[ J ]+2.667)*119.81),
                TRUNC( (Y2[ J ]+2.000)*87.253), ((S+4) MOD 15)+1 );
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
        TRUNC( (Y1[ 0 ]+2.000)*87.253), 0 );
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
            TRUNC( (Y1[ J ]+2.000)*87.253), 0 );
        Graph.Plot( TRUNC( (X2[ J ]+2.667)*119.81),
            TRUNC( (Y2[ J ]+2.000)*87.253), 0 );
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
                TRUNC( (Y1[ J ]+2.000)*87.253), 0 );
            Graph.Plot( TRUNC( (X2[ J ]+2.667)*119.81),
                TRUNC( (Y2[ J ]+2.000)*87.253), 0 );
        END (* FOR *);
    END (* FOR *);

END ErasePlot;


BEGIN
    Graph.InitEGA;

    Color[ 0 ] := 0;
    FOR At := 1 TO 15 DO
        Color[ At ] := BYTE( Lib.RANDOM( 64 ) );
    END (* FOR *);
    Graphics.SetPalette( Color );

    LOOP

        Lib.RANDOMIZE;
        Graph.GraphMode;
        Graphics.SetFunc( Graphics.clr, 0 );

        A := LONGREAL( Lib.RAND() )*3.0 - 1.5;
        B := LONGREAL( Lib.RAND() )*3.0 - 1.5;
        AS := LONGREAL( Lib.RAND() )*0.2 - 0.1;
        BS := LONGREAL( Lib.RAND() )*0.2 - 0.1;

        FOR At := 0 TO Delay DO

            Graphics.SetFunc( Graphics.clr, 0 );
            DrawPlot;
            Color[ Lib.RANDOM( 15 )+1 ] := BYTE( Lib.RANDOM( 64 ) );
            Graphics.SetPalette( Color );
            AA[ At ] := A;
            BA[ At ] := B;

            A := A + AS;
            AS := AS - LONGREAL( Lib.RAND() )*A*0.06
                - LONGREAL( Lib.RAND() )*0.03*AS + 0.015
                + LONGREAL( Lib.RAND() )*0.08 - 0.04;
            B := B + BS;
            BS := BS - LONGREAL( Lib.RAND() )*B*0.06
                - LONGREAL( Lib.RAND() )*0.03*BS + 0.015
                + LONGREAL( Lib.RAND() )*0.08 - 0.04;

        END (* FOR *);

        At := 0;
        REPEAT

            Graphics.SetFunc( Graphics.clr, 0 );
            Graphics.SetPlane( {0,1,2,3} );
            ErasePlot;
            AA[ At ] := A;
            BA[ At ] := B;

            Graphics.SetFunc( Graphics.xor, 0 );
            DrawPlot;
            Color[ Lib.RANDOM( 15 )+1 ] := BYTE( Lib.RANDOM( 64 ) );
            Graphics.SetPalette( Color );
            A := A + AS;
            AS := AS - LONGREAL( Lib.RAND() )*A*0.06
                - LONGREAL( Lib.RAND() )*0.03*AS + 0.015
                + LONGREAL( Lib.RAND() )*0.08 - 0.04;
            B := B + BS;
            BS := BS - LONGREAL( Lib.RAND() )*B*0.06
                - LONGREAL( Lib.RAND() )*0.03*BS + 0.015
                + LONGREAL( Lib.RAND() )*0.08 - 0.04;

            INC( At );
            IF At > Delay THEN At := 0 END;

        UNTIL IO.KeyPressed();
        K := IO.RdKey();

    END (* LOOP *);


END JuliaB.

