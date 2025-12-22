MODULE FracPlot;

IMPORT
    ASCII, IO, FIO, MATHLIB, Graph, Graphics, TimeDate, Str, Lib;
FROM Storage IMPORT ALLOCATE;

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


VAR
    S  :  ARRAY [ 1..24 ] OF POINTER TO Square;
    Key  :  CHAR;
    C1, C2, Line, Col, Color, N, At, AtBit : CARDINAL;
    S1, S2 : ARRAY [ 0..31 ] OF CHAR;
    CBuf, YBuf, MBuf : ARRAY [ 0..299 ] OF SHORTCARD;
    OK : BOOLEAN;
    PrnFile : FIO.File;

CONST
    StepSize = 0.00078125;


BEGIN

    Graph.InitEGA;
    Graph.GraphMode;
(*    Graphics.SetPalette( Graphics.PaletteType( 7,
                                8, 1, 9,
                                40, 5, 45,
                                32, 4, 36,
                                48, 6, 54,
                                16, 2, 18 ) ); *)

    FIO.IOcheck := TRUE;
(*    PrnFile := FIO.Open( 'SQ1.PRN' );
    FIO.WrStr( PrnFile, '*t300R*r-3U*b0M*r2Q' ); *)

    FOR C2 := 8 TO 24 DO
        NEW( S[ C2 ] );
    END (* FOR *);
    FOR C1 := 4 TO 15 DO
        FOR C2 := 8 TO 24 DO
(*            IO.WrStr( 'Preparing line ' );
            IO.WrCard( C1, 5 );
            IO.WrStr( '; loading square ' );
            IO.WrCard( C2, 5 );
            IO.WrLn; *)
            Str.CardToStr( LONGCARD(100*C2+C1+10000), S[ C2 ]^.FileName, 10, OK );
                (* S[ C2 ]^.FileName = '10000' to '11624' *)
            Str.Slice( S[ C2 ]^.FileName, S[ C2 ]^.FileName, 1, 4 );
                (* S[ C2 ]^.FileName = '0000' to '1624' *)
            Str.Concat( S[ C2 ]^.FileName, 'SQ', S[ C2 ]^.FileName );
                (* S[ C2 ]^.FileName = 'SQ0000' *)
            Str.Concat( S[ C2 ]^.FileName, S[ C2 ]^.FileName, '.FRC' );
                (* S[ C2 ]^.FileName = 'SQ0000.FRC' *)
            LoadSquare( S[ C2 ]^ );
        END (* FOR *);
        Str.CardToStr( LONGCARD( C1+100 ), S1, 10, OK );
        Str.Concat( S1, 'SQ', S1 );
            (* S1 = 'SQ1' *)
        Str.Concat( S1, S1, '.PRN' );
            (* S1 = 'SQ1.PRN' *)
        PrnFile := FIO.Create( S1 );

        FOR Line := 149 TO 0 BY -1 DO
            Lib.Fill( ADR( CBuf ), SIZE( CBuf ), 0 );
            Lib.Fill( ADR( YBuf ), SIZE( YBuf ), 0 );
            Lib.Fill( ADR( MBuf ), SIZE( MBuf ), 0 );
            At := 0;
            AtBit := 0;
            FOR C2 := 8 TO 24 DO
                FOR Col := 0 TO 149 DO
                    Color := CARDINAL( S[ C2 ]^.Data[ Col, Line ] );
                    IF Color = 255 THEN
                        Color := 0;
                    ELSE
                        N := 0;
                        IF MATHLIB.Sin( MATHLIB.Sqrt(LONGREAL( 255-Color ))
                            /8.0*3.1415 ) > LONGREAL(Lib.RAND()*2.0-1.0) THEN
                            N := N + 1;
                        END (* IF *);
                        IF MATHLIB.Sin( MATHLIB.Sqrt(LONGREAL( 255-Color ))
                            /4.0*3.1415 (* + 2.09 *) ) > LONGREAL(Lib.RAND()*2.0-1.0) THEN
                            N := N + 2;
                        END (* IF *);
                        IF MATHLIB.Sin( MATHLIB.Sqrt(LONGREAL( 255-Color ))
                            /2.0*3.1415 (* + 4.19 *) ) > LONGREAL(Lib.RAND()*2.0-1.0) THEN
                            N := N + 4;
                        END (* IF *);
                        Color := N;
                        Graph.Plot( ( C1*25 )-Line DIV 6, ( (C2-8)*20 )+Col DIV 7, 7-Color );
                    END (* IF *);
(*                    Color := TRUNC( MATHLIB.Sqrt( LONGREAL( Color ) ) * 16.0 ); *)
(*                    IF Line MOD 2 = Col MOD 2 THEN
                        Color := Color DIV 32;
                    ELSE
                        Color := Color MOD 8;
                    END (* IF *); *)
(*                    IF Color = 7 THEN Color := 2 END; *)
                    IF 0 IN BITSET( Color ) THEN
                        CBuf[ At ] := CBuf[ At ] + SHORTCARD(128 >> AtBit);
                    END (* IF *);
                    IF 1 IN BITSET( Color ) THEN
                        YBuf[ At ] := YBuf[ At ] + SHORTCARD(128 >> AtBit);
                    END (* IF *);
                    IF 2 IN BITSET( Color ) THEN
                        MBuf[ At ] := MBuf[ At ] + SHORTCARD(128 >> AtBit);
                    END (* IF *);
                    INC( AtBit );
                    IF AtBit = 8 THEN
                        IF CBuf[ At ] = 26 THEN
                            CBuf[ At ] := 18;
                        END (* IF *);
                        IF YBuf[ At ] = 26 THEN
                            YBuf[ At ] := 18;
                        END (* IF *);
                        IF MBuf[ At ] = 26 THEN
                            MBuf[ At ] := 18;
                        END (* IF *);
                        INC( At );
                        AtBit := 0;
                    END (* IF *);
                END (* FOR *);
            END (* FOR *);
(*            WHILE FIO.IOresult()#0 DO
            END (* WHILE *); *)
            FIO.WrStr( PrnFile, '*b300V' );
            FIO.WrBin( PrnFile, CBuf, SIZE( CBuf ) );
            FIO.WrStr( PrnFile, '*b300V' );
            FIO.WrBin( PrnFile, YBuf, SIZE( YBuf ) );
            FIO.WrStr( PrnFile, '*b300W' );
            FIO.WrBin( PrnFile, MBuf, SIZE( MBuf ) );
        END (* FOR *);
        FIO.Close( PrnFile );
    END (* FOR *);

    FIO.WrChar( PrnFile, 14C );

    FIO.Close( PrnFile );

END FracPlot.