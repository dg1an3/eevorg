MODULE FracGen;

IMPORT
    ASCII, IO, FIO, MATHLIB, Graph, Graphics, TimeDate, Str;

CONST
    SquareResolution  =  150;

    Iterations = 255;

TYPE
    COMPLEX = RECORD
        R  :  LONGREAL;
        I  :  LONGREAL;
    END (* COMPLEX *);

    Square  =  RECORD
        C1, C2    :  COMPLEX;    (* C1 = upper left; C2 = lower right *)
        FileName  :  ARRAY [ 0..11 ] OF CHAR;
        Data      :  ARRAY [ 0..SquareResolution-1 ],
                         [ 0..SquareResolution-1 ] OF SHORTCARD;
    END (* Square *);



PROCEDURE Abs( x : COMPLEX ) : LONGREAL;

BEGIN
    RETURN( MATHLIB.Sqrt( x.R*x.R+x.I*x.I ) );
END Abs;


PROCEDURE Mult( x, y : COMPLEX ) : COMPLEX;

VAR
    Result : COMPLEX;
BEGIN
    Result.R := x.R*y.R - x.I*y.I;
    Result.I := x.R*y.I + x.I*y.R;
    RETURN( Result );
END Mult;


PROCEDURE CalcSquare( VAR S : Square );
(* Body of process to calculate data for square S *)

VAR
    CountR, CountI, IterCount  :  CARDINAL;
    x  :  COMPLEX;
    ZX, ZY, ZXY  : LONGREAL;
    T  :  TimeDate.Time;
    Str  :  ARRAY [ 0..16 ] OF CHAR;

BEGIN

    TimeDate.GetTime( T );

    Graph.InitEGA;
    Graph.GraphMode;
    Graphics.SetPalette( Graphics.PaletteType( 7,
                                8, 1, 9,
                                40, 5, 45,
                                32, 4, 36,
                                48, 6, 54,
                                16, 2, 18 ) );

    WITH S DO

        IO.WrStr( FileName );
        IO.WrLngReal( C1.R, 10, 8 );
        IO.WrLngReal( C1.I, 10, 8 );
        IO.WrLn;

        FOR CountR := 0 TO SquareResolution-1 DO

            x.R := LONGREAL( CountR )/LONGREAL( SquareResolution )*
                ( C2.R-C1.R ) + C1.R;
            IO.WrChar( ASCII.cr );
            IO.WrCard( CountR, 10 );

            FOR CountI := 0 TO SquareResolution-1 DO

                x.I := LONGREAL( CountI )/LONGREAL( SquareResolution )*
                    ( C2.I-C1.I ) + C1.I;
                ZX := 0.0;
                ZY:=0.0;
                IterCount := 0;
                WHILE ( IterCount < Iterations ) & ( ZX*ZX+ZY*ZY < 8.0 ) DO
                    ZXY := 2.0 * ZX * ZY ;
                    ZX := ZX * ZX - ZY * ZY + x.R ;
                    ZY := ZXY + x.I ;
                    INC( IterCount );
                END (* WHILE *);
                Data[ CountR, CountI ] := SHORTCARD( IterCount );
                Graph.Plot( CountR+30, CountI+100, IterCount MOD 16 );
            END (* FOR *);

        END (* FOR *);
        TimeDate.TimeToString( T, Str );
        IO.WrStr( '   ' );
        IO.WrStr( Str );
        IO.WrStr( '-->' );
        TimeDate.GetTime( T );
        TimeDate.TimeToString( T, Str );
        IO.WrStr( Str );
        IO.WrStr( '    ' );
        IO.WrLn;

    END (* WITH *);

END CalcSquare;


PROCEDURE SaveSquare( VAR S : Square );
(* Saves square data to proper file *)
VAR
    DataFile  :  FIO.File;
BEGIN

    DataFile := FIO.Create( S.FileName );
    FIO.WrBin( DataFile, S, SIZE( S ) );
    FIO.Close( DataFile );

END SaveSquare;


PROCEDURE LoadSquare( VAR S : Square );
(* Loads square data from proper file *)
VAR
    DataFile  :  FIO.File;
    Count  :  CARDINAL;
BEGIN

    DataFile := FIO.Open( S.FileName );
    Count := FIO.RdBin( DataFile, S, SIZE( S ) );
    FIO.Close( DataFile );

END LoadSquare;


PROCEDURE GenSquareFile( Name :  ARRAY OF CHAR;
                         C1R, C1I, C2R, C2I  :  LONGREAL );
VAR
    S  :  Square;
BEGIN
    Str.Copy( S.FileName, Name );
    S.C1.R := C1R;
    S.C1.I := C1I;
    S.C2.R := C2R;
    S.C2.I := C2I;
    CalcSquare( S );
    SaveSquare( S );
END GenSquareFile;


(* VAR
    z : COMPLEX;
    x, w, r, a, b, astep, bstep : LONGREAL;
    Total, Lyap : LONGREAL;
    N : CARDINAL;
    Data : FIO.File; *)
VAR
    S  :  Square;
(*    Data, Print : FIO.File;
    astep, bstep, a, b : LONGREAL;
    N, M, aint, bint : CARDINAL;
    S : ARRAY [ 0..21 ] OF CHAR;
    T : ARRAY [ 0..12 ] OF CHAR;
    OK : BOOLEAN;
    Key : CHAR;
    DataBuf : ARRAY [ 0..480 ] OF BYTE;
    Color : SHORTCARD;
    AtBit, AtByte : CARDINAL; *)


VAR
    Key  :  CHAR;

CONST
    StepSize = 0.00078125;

BEGIN

(*    Data := FIO.Create( 'man.dat' );

    Graph.InitEGA;
    Graph.GraphMode;
    Graphics.SetPalette( Graphics.PaletteType( 7,
                                8, 1, 9,
                                40, 5, 45,
                                32, 4, 36,
                                48, 6, 54,
                                16, 2, 18 ) );

    a := -0.2;
    astep := 0.025/1280.0;
    bstep := 0.025/1280.0;

    WHILE a < -0.175 DO

        b := -0.7;

        WHILE b < -0.675 DO

            z.R := a;
            z.I := b;
            N := Iterate( z );

(*            IO.WrLngReal( a, 3, 10 );
            IO.WrLngReal( b, 3, 10 );
            IO.WrCard( N, 8 );
            IO.WrLn; *)
            Graph.Plot( TRUNC((a+0.2)/astep/2.0), TRUNC((b+0.7)/bstep/2.0), ((N MOD 64) DIV 4) );

(*            FIO.WrLngReal( Data, a, 6, 10 );
            FIO.WrLngReal( Data, b, 6, 10 ); *)
            FIO.WrBin( Data, BYTE( N ), 1 );
(*            FIO.WrLn( Data ); *)

            b := b+bstep;

        END (* WHILE *);

        a := a+astep;

    END (* WHILE *);
    FIO.Close( Data );

    Key := IO.RdKey(); *)

    GenSquareFile( 'SQ1714.FRC', -0.225+StepSize*16.0, -0.7-StepSize*12.0,
                                 -0.225+StepSize*17.0, -0.7-StepSize*11.0 );
    GenSquareFile( 'SQ1814.FRC', -0.225+StepSize*17.0, -0.7-StepSize*12.0,
                                 -0.225+StepSize*18.0, -0.7-StepSize*11.0 );
    GenSquareFile( 'SQ1914.FRC', -0.225+StepSize*18.0, -0.7-StepSize*12.0,
                                 -0.225+StepSize*19.0, -0.7-StepSize*11.0 );
    GenSquareFile( 'SQ2014.FRC', -0.225+StepSize*19.0, -0.7-StepSize*12.0,
                                 -0.225+StepSize*20.0, -0.7-StepSize*11.0 );
    GenSquareFile( 'SQ2114.FRC', -0.225+StepSize*20.0, -0.7-StepSize*12.0,
                                 -0.225+StepSize*21.0, -0.7-StepSize*11.0 );
    GenSquareFile( 'SQ2214.FRC', -0.225+StepSize*21.0, -0.7-StepSize*12.0,
                                 -0.225+StepSize*22.0, -0.7-StepSize*11.0 );
    GenSquareFile( 'SQ2314.FRC', -0.225+StepSize*22.0, -0.7-StepSize*12.0,
                                 -0.225+StepSize*23.0, -0.7-StepSize*11.0 );
    GenSquareFile( 'SQ2414.FRC', -0.225+StepSize*23.0, -0.7-StepSize*12.0,
                                 -0.225+StepSize*24.0, -0.7-StepSize*11.0 );
    GenSquareFile( 'SQ1715.FRC', -0.225+StepSize*16.0, -0.7-StepSize*13.0,
                                 -0.225+StepSize*17.0, -0.7-StepSize*12.0 );
    GenSquareFile( 'SQ1815.FRC', -0.225+StepSize*17.0, -0.7-StepSize*13.0,
                                 -0.225+StepSize*18.0, -0.7-StepSize*12.0 );
    GenSquareFile( 'SQ1915.FRC', -0.225+StepSize*18.0, -0.7-StepSize*13.0,
                                 -0.225+StepSize*19.0, -0.7-StepSize*12.0 );
    GenSquareFile( 'SQ2015.FRC', -0.225+StepSize*19.0, -0.7-StepSize*13.0,
                                 -0.225+StepSize*20.0, -0.7-StepSize*12.0 );
    GenSquareFile( 'SQ2115.FRC', -0.225+StepSize*20.0, -0.7-StepSize*13.0,
                                 -0.225+StepSize*21.0, -0.7-StepSize*12.0 );
    GenSquareFile( 'SQ2215.FRC', -0.225+StepSize*21.0, -0.7-StepSize*13.0,
                                 -0.225+StepSize*22.0, -0.7-StepSize*12.0 );
    GenSquareFile( 'SQ2315.FRC', -0.225+StepSize*22.0, -0.7-StepSize*13.0,
                                 -0.225+StepSize*23.0, -0.7-StepSize*12.0 );
    GenSquareFile( 'SQ2415.FRC', -0.225+StepSize*23.0, -0.7-StepSize*13.0,
                                 -0.225+StepSize*24.0, -0.7-StepSize*12.0 );

(*  GenSquareFile( 'SQ1706.FRC', -0.225+StepSize*16.0, -0.7-StepSize*4.0,
                                 -0.225+StepSize*17.0, -0.7-StepSize*3.0 );
    GenSquareFile( 'SQ1806.FRC', -0.225+StepSize*17.0, -0.7-StepSize*4.0,
                                 -0.225+StepSize*18.0, -0.7-StepSize*3.0 );
    GenSquareFile( 'SQ1906.FRC', -0.225+StepSize*18.0, -0.7-StepSize*4.0,
                                 -0.225+StepSize*19.0, -0.7-StepSize*3.0 );
    GenSquareFile( 'SQ2006.FRC', -0.225+StepSize*19.0, -0.7-StepSize*4.0,
                                 -0.225+StepSize*20.0, -0.7-StepSize*3.0 );
    GenSquareFile( 'SQ2106.FRC', -0.225+StepSize*20.0, -0.7-StepSize*4.0,
                                 -0.225+StepSize*21.0, -0.7-StepSize*3.0 );
    GenSquareFile( 'SQ2206.FRC', -0.225+StepSize*21.0, -0.7-StepSize*4.0,
                                 -0.225+StepSize*22.0, -0.7-StepSize*3.0 );
    GenSquareFile( 'SQ2306.FRC', -0.225+StepSize*22.0, -0.7-StepSize*4.0,
                                 -0.225+StepSize*23.0, -0.7-StepSize*3.0 );
    GenSquareFile( 'SQ2406.FRC', -0.225+StepSize*23.0, -0.7-StepSize*4.0,
                                 -0.225+StepSize*24.0, -0.7-StepSize*3.0 );
    GenSquareFile( 'SQ1706.FRC', -0.225+StepSize*16.0, -0.7-StepSize*5.0,
                                 -0.225+StepSize*17.0, -0.7-StepSize*4.0 );
    GenSquareFile( 'SQ1807.FRC', -0.225+StepSize*17.0, -0.7-StepSize*5.0,
                                 -0.225+StepSize*18.0, -0.7-StepSize*4.0 );
    GenSquareFile( 'SQ1907.FRC', -0.225+StepSize*18.0, -0.7-StepSize*5.0,
                                 -0.225+StepSize*19.0, -0.7-StepSize*4.0 );
    GenSquareFile( 'SQ2007.FRC', -0.225+StepSize*19.0, -0.7-StepSize*5.0,
                                 -0.225+StepSize*20.0, -0.7-StepSize*4.0 );
    GenSquareFile( 'SQ2107.FRC', -0.225+StepSize*20.0, -0.7-StepSize*5.0,
                                 -0.225+StepSize*21.0, -0.7-StepSize*4.0 );
    GenSquareFile( 'SQ2207.FRC', -0.225+StepSize*21.0, -0.7-StepSize*5.0,
                                 -0.225+StepSize*22.0, -0.7-StepSize*4.0 );
    GenSquareFile( 'SQ2307.FRC', -0.225+StepSize*22.0, -0.7-StepSize*5.0,
                                 -0.225+StepSize*23.0, -0.7-StepSize*4.0 );
    GenSquareFile( 'SQ2407.FRC', -0.225+StepSize*23.0, -0.7-StepSize*5.0,
                                 -0.225+StepSize*24.0, -0.7-StepSize*4.0 );

    GenSquareFile( 'SQ0194.FRC', -0.225, -0.7+StepSize*7.0,
                                 -0.225+StepSize, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ0294.FRC', -0.225+StepSize, -0.7+StepSize*7.0,
                                 -0.225+StepSize*2.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ0394.FRC', -0.225+StepSize*2.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*3.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ0494.FRC', -0.225+StepSize*3.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*4.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ0594.FRC', -0.225+StepSize*4.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*5.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ0694.FRC', -0.225+StepSize*5.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*6.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ0794.FRC', -0.225+StepSize*6.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*7.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ0894.FRC', -0.225+StepSize*7.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*8.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ0994.FRC', -0.225+StepSize*8.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*9.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ1094.FRC', -0.225+StepSize*9.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*10.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ1194.FRC', -0.225+StepSize*10.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*11.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ1294.FRC', -0.225+StepSize*11.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*12.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ1394.FRC', -0.225+StepSize*12.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*13.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ1494.FRC', -0.225+StepSize*13.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*14.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ1594.FRC', -0.225+StepSize*14.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*15.0, -0.7+StepSize*8.0 );
    GenSquareFile( 'SQ1694.FRC', -0.225+StepSize*15.0, -0.7+StepSize*7.0,
                                 -0.225+StepSize*16.0, -0.7+StepSize*8.0 );
*)
(*
(*    S.FileName := 'SQ0117.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*15.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0217.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0317.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0417.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0517.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0617.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0717.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0817.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0917.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1017.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1117.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1217.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1317.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1417.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1517.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1617.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0118.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*16.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0218.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0318.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0418.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0518.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0618.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0718.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0818.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0918.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1018.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1118.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1218.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1318.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1418.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1518.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1618.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0119.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*17.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0219.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0319.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0419.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0519.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0619.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0719.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0819.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0919.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1019.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1119.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1219.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1319.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1419.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1519.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1619.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0120.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*18.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0220.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0320.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0420.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0520.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0620.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0720.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0820.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0920.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1020.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1120.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1220.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1320.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1420.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1520.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1620.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0121.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*19.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0221.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0321.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0421.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0521.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0621.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0721.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0821.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0921.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1021.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1121.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1221.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1321.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1421.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1521.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1621.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0122.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*20.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0222.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0322.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0422.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0522.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0622.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0722.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0822.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0922.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1022.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1122.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1222.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1322.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1422.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1522.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1622.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0123.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*21.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0223.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0323.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0423.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0523.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0623.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0723.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0823.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0923.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1023.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1123.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1223.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1323.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1423.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1523.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1623.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0124.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*22.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0224.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0324.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0424.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0524.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0624.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0724.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0824.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0924.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1024.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1124.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1224.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1324.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1424.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1524.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1624.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S ); *)

    S.FileName := 'SQ0125.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*23.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0225.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0325.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0425.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0525.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0625.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0725.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0825.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0925.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1025.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1125.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1225.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1325.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1425.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1525.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1625.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0126.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*24.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0226.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0326.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0426.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0526.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0626.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0726.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0826.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0926.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1026.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1126.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1226.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1326.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1426.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1526.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1626.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0127.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*25.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0227.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0327.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0427.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0527.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0627.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0727.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0827.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0927.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1027.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1127.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1227.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1327.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1427.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1527.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1627.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0128.FRC';
    S.C1.R := -0.225;
    S.C1.I := -0.7-(StepSize*26.0);
    S.C2.R := -0.22421875;
    S.C2.I := S.C1.I+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0228.FRC';
    S.C1.R := -0.22421875;
    S.C2.R := -0.2234375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0328.FRC';
    S.C1.R := -0.2234375;
    S.C2.R := -0.22265625;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0428.FRC';
    S.C1.R := -0.22265625;
    S.C2.R := -0.221875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0528.FRC';
    S.C1.R := -0.221875;
    S.C2.R := -0.22109375;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0628.FRC';
    S.C1.R := -0.22109375;
    S.C2.R := -0.2203125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0728.FRC';
    S.C1.R := -0.2203125;
    S.C2.R := -0.21953125;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0828.FRC';
    S.C1.R := -0.21953125;
    S.C2.R := -0.21875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ0928.FRC';
    S.C1.R := -0.21875;
    S.C2.R := -0.21796875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1028.FRC';
    S.C1.R := -0.21796875;
    S.C2.R := -0.2171875;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1128.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1228.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1328.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1428.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1528.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1628.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

(*    S.FileName := 'SQ1706.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1806.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ1906.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2006.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2106.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2206.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2306.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2406.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2506.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2606.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2706.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2806.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ2906.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ3006.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ3106.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S );

    S.FileName := 'SQ3206.FRC';
    S.C1.R := S.C1.R+StepSize;
    S.C2.R := S.C2.R+StepSize;
    CalcSquare( S );
    SaveSquare( S ); *) *)

END FracGen.
