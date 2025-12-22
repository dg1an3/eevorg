MODULE JuliaC;

IMPORT Graph, Graphics, MATHLIB, IO, Lib, Storage;

CONST
    P = 9;
    Delay = 27;

TYPE

    PointRecord  =  RECORD
        V  :  ARRAY [ 0..2 ] OF SHORTCARD;
    END (* PointRecord *);

    PlotRecord  =  RECORD
        A, B, AS, BS  :  LONGREAL;
        Point  :  ARRAY [ 0..1024 ] OF PointRecord;
    END (* PlotRecord *);

PROCEDURE CalcPlotRecord( VAR PR : PlotRecord );

VAR
    X1, Y1, X2, Y2  :  ARRAY [ 0..P ] OF LONGREAL;
    AA, BA  :  ARRAY [ 0..Delay ] OF LONGREAL;
    X, Y, R, T, OA, OB, (* A, B, *) A1, B1, (* AS, BS, *) R1  :  LONGREAL;
    S, J, N, M, At  :  CARDINAL;
    K : CHAR;
    L : LONGCARD;
    PlotCount  :  CARDINAL;

BEGIN
    WITH PR DO
        A1 := (0.25-A) / 2.0;
        B1 := -B / 2.0;
        R1 := MATHLIB.Sqrt( A1*A1 + B1*B1 );

        X1[ 0 ] := 0.5+MATHLIB.Sqrt( R1+A1 );
        Y1[ 0 ] := MATHLIB.Sqrt( R1-A1 );
        IF ( B > 0.0 ) THEN
            Y1[ 0 ] := -Y1[ 0 ];
        END (* IF *);
        PlotCount := 0;
        L := LONGCARD( (X1[ 0 ]+2.667)*119.81)*16384 +
            LONGCARD( (Y1[ 0 ]+2.000)*87.253)*32 + LONGCARD( Lib.RANDOM( 15 ) );
        PR.Point[ PlotCount ].V[0] := SHORTCARD( L MOD 256 );
        PR.Point[ PlotCount ].V[1] := SHORTCARD(( L DIV 256 ) MOD 256 );
        PR.Point[ PlotCount ].V[2] := SHORTCARD(( L DIV 65536 ) MOD 256 );
        INC( PlotCount );

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
            L := LONGCARD( (X1[ J ]+2.667)*119.81)*16384 +
                LONGCARD( (Y1[ J ]+2.000)*87.253)*32 + LONGCARD((J+4) MOD 15)+1;
            PR.Point[ PlotCount ].V[0] := SHORTCARD( L MOD 256 );
            PR.Point[ PlotCount ].V[1] := SHORTCARD(( L DIV 256 ) MOD 256 );
            PR.Point[ PlotCount ].V[2] := SHORTCARD(( L DIV 65536 ) MOD 256 );
            INC( PlotCount );
            L := LONGCARD( (X2[ J ]+2.667)*119.81)*16384 +
                LONGCARD( (Y2[ J ]+2.000)*87.253)*32 + LONGCARD((J+4) MOD 15)+1;
            PR.Point[ PlotCount ].V[0] := SHORTCARD( L MOD 256 );
            PR.Point[ PlotCount ].V[1] := SHORTCARD(( L DIV 256 ) MOD 256 );
            PR.Point[ PlotCount ].V[2] := SHORTCARD(( L DIV 65536 ) MOD 256 );
            INC( PlotCount );
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
                L := LONGCARD( (X1[ J ]+2.667)*119.81)*16384 +
                    LONGCARD( (Y1[ J ]+2.000)*87.253)*32 + LONGCARD((S+4) MOD 15)+1;
                PR.Point[ PlotCount ].V[0] := SHORTCARD( L MOD 256 );
                PR.Point[ PlotCount ].V[1] := SHORTCARD(( L DIV 256 ) MOD 256 );
                PR.Point[ PlotCount ].V[2] := SHORTCARD(( L DIV 65536 ) MOD 256 );
                INC( PlotCount );
                L := LONGCARD( (X2[ J ]+2.667)*119.81)*16384 +
                    LONGCARD( (Y2[ J ]+2.000)*87.253)*32 + LONGCARD((S+4) MOD 15)+1;
                PR.Point[ PlotCount ].V[0] := SHORTCARD( L MOD 256 );
                PR.Point[ PlotCount ].V[1] := SHORTCARD(( L DIV 256 ) MOD 256 );
                PR.Point[ PlotCount ].V[2] := SHORTCARD(( L DIV 65536 ) MOD 256 );
                INC( PlotCount );
            END (* FOR *);
        END (* FOR *);
    END (* WITH *);

END CalcPlotRecord;

PROCEDURE DrawPlot( PR : PlotRecord );

VAR
    Count, X, Y, Color  :  CARDINAL;
    L      :  LONGCARD;
BEGIN
    Graphics.SetFunc( Graphics.clr, 0 );
    FOR Count := 0 TO 2047 DO
        WITH PR.Point[ Count ] DO
            L := (LONGCARD(V[2])*256+LONGCARD(V[1]))*256+LONGCARD(V[0]);
            X := CARDINAL( L DIV 16384 );
            Y := CARDINAL(( L DIV 32 ) MOD 512);
            Color := CARDINAL( L MOD 16 );
            Graph.Plot( X, Y, Color );
        END (* WITH *);
    END (* FOR *);
END DrawPlot;

PROCEDURE ErasePlot( PR : PlotRecord );

VAR
    Count, X, Y, Color  :  CARDINAL;
    L      :  LONGCARD;
BEGIN
    Graphics.SetFunc( Graphics.clr, 0 );
    FOR Count := 0 TO 2047 DO
        WITH PR.Point[ Count ] DO
            L := (LONGCARD(V[2])*256+LONGCARD(V[1]))*256+LONGCARD(V[0]);
            X := CARDINAL( L DIV 16384 );
            Y := CARDINAL(( L DIV 32 ) MOD 512);
            Graph.Plot( X, Y, 0 );
        END (* WITH *);
    END (* FOR *);
END ErasePlot;

VAR
    PL  :  ARRAY [ 0..80 ] OF POINTER TO PlotRecord;
    Count, At, Q  :  CARDINAL;
    A, B, AS, BS  :  LONGREAL;
    Color  :  Graphics.PaletteType;
    K  :  CHAR;

BEGIN
    Graph.InitEGA;

    Color[ 0 ] := 0;
    FOR At := 1 TO 15 DO
        Color[ At ] := BYTE( Lib.RANDOM( 64 ) );
    END (* FOR *);
    Graphics.SetPalette( Color );

    Lib.RANDOMIZE;
    Graph.GraphMode;
    Graphics.SetFunc( Graphics.clr, 0 );

    A := LONGREAL( Lib.RAND() )*3.0 - 1.5;
    B := LONGREAL( Lib.RAND() )*3.0 - 1.5;
    AS := LONGREAL( Lib.RAND() )*0.2 - 0.1;
    BS := LONGREAL( Lib.RAND() )*0.2 - 0.1;

    Count := 0;
    LOOP
        IF Storage.Available( SIZE( PlotRecord ) ) THEN

            Storage.ALLOCATE( PL[ Count ], SIZE( PlotRecord ) );
            PL[ Count ]^.A := A;
            PL[ Count ]^.B := B;
            PL[ Count ]^.AS := AS;
            PL[ Count ]^.BS := BS;
            CalcPlotRecord( PL[ Count ]^ );
(*            DrawPlot( PL[ Count ]^ ); *)
            INC( Count );
            A := A + AS;
            AS := AS - LONGREAL( Lib.RAND() )*A*0.06
                - LONGREAL( Lib.RAND() )*0.03*AS + 0.015
                + LONGREAL( Lib.RAND() )*0.08 - 0.04;
            B := B + BS;
            BS := BS - LONGREAL( Lib.RAND() )*B*0.06
                - LONGREAL( Lib.RAND() )*0.03*BS + 0.015
                + LONGREAL( Lib.RAND() )*0.08 - 0.04;

        END (* WHILE *);

        FOR At := 0 TO Count-1 DO
            DrawPlot( PL[ At ]^ );
            ErasePlot( PL[ ( At+Count-(Count DIV 2 )-1 ) MOD Count ]^ );
            Q := Lib.RANDOM( 15 )+1;
            Color[ Q ] := ( Color[ Q ]+BYTE( Lib.RANDOM( 8 ) - 4 ) ) MOD BYTE( 64 );
            Graphics.SetPalette( Color );
        END (* FOR *);
    END (* LOOP *);

(*
    REPEAT

        Graphics.SetFunc( Graphics.clr, 0 );
        ErasePlot;
        AA[ At ] := A;
        BA[ At ] := B;

        Graphics.SetFunc( Graphics.clr, 0 );
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

    UNTIL IO.KeyPressed(); *)
    K := IO.RdKey();

END JuliaC.

