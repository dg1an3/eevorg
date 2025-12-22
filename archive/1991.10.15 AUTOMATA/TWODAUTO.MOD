MODULE TwoDAuto;

IMPORT Graph, Lib;

(* Two-dimensional binary automaton *)

CONST
    FieldWidth = 100;
    FieldHeight = 100;

TYPE
    CellRow = SET OF [ 0..FieldWidth ];

    FieldRec   = RECORD
        Cells  :  ARRAY [ 0..FieldHeight ] OF CellRow;
        Rule   :  BITSET;
    END (* Field *);

VAR
    Field, NewField : FieldRec;
    CountRow, CountCol  :  CARDINAL;

BEGIN
    Graph.InitEGA;
    Graph.GraphMode;

    Lib. RANDOMIZE;
    Lib.Fill( ADR(Field), SIZE(Field), 0 );
    Lib.Fill( ADR(NewField), SIZE(NewField), 0 );
    FOR CountRow := 0 TO 35 DO
        INCL( Field.Cells[ Lib.RANDOM( FieldHeight ) ],
            Lib.RANDOM( FieldWidth ) );
    END (* FOR *);
    Field.Rule := BITSET{ 2,9 };
    LOOP
        (* Draw the field while recalculating*)
        FOR CountRow := 0 TO FieldHeight DO
            FOR CountCol := 0 TO FieldWidth DO
                WITH Field DO
                    IF (CountCol IN Cells[CountRow]) THEN
                        Graph.Plot( CountCol*2, CountRow, 15 );
                        Graph.Plot( CountCol*2+1, CountRow, 15 );
                        (* Graph.Plot( CountCol*6, CountRow*3+1, 15 );
                        Graph.Plot( CountCol*6+1, CountRow*3+1, 15 ); *)
                    ELSE
                        Graph.Plot( CountCol*2, CountRow,  0 );
                        Graph.Plot( CountCol*2+1, CountRow, 0 );
                        (* Graph.Plot( CountCol*6, CountRow*3+1, 0 );
                        Graph.Plot( CountCol*6+1, CountRow*3+1, 0 ); *)
                    END (* IF *);
                    IF  CARDINAL(CountCol IN Cells[CountRow])+
                        CARDINAL((CountCol-1) IN Cells[CountRow])+
                        CARDINAL((CountCol+1) IN Cells[CountRow])+
                        CARDINAL(CountCol IN Cells[CountRow+1])+
                        CARDINAL((CountCol-1) IN Cells[CountRow+1])+
                        CARDINAL((CountCol+1) IN Cells[CountRow+1])+
                        CARDINAL(CountCol IN Cells[CountRow-1])+
                        CARDINAL((CountCol-1) IN Cells[CountRow-1])+
                        CARDINAL((CountCol+1) IN Cells[CountRow-1])
                        IN Rule THEN
                        INCL(NewField.Cells[CountRow],CountCol);
                    ELSE
                        EXCL(NewField.Cells[CountRow],CountCol);
                    END (* IF *);
                END (* WITH *);
            END (* FOR *);
            EXCL(NewField.Cells[CountRow],0);
            EXCL(NewField.Cells[CountRow],FieldWidth);
        END (* FOR *);
        Lib.Move( ADR(NewField.Cells), ADR(Field.Cells), SIZE(Field.Cells) );
        Lib.Fill( ADR(Field.Cells[0]), SIZE(Field.Cells[0]), 0 );
        Lib.Fill( ADR(Field.Cells[FieldHeight]), SIZE(Field.Cells[FieldHeight]), 0 );
    END (* LOOP *);
END TwoDAuto.