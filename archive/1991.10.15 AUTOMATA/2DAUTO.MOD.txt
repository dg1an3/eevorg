MODULE TwoDAuto;

IMPORT Graph, Lib;

(* Two-dimensional binary automaton *)

CONST
    FieldWidth = 320;
    FieldHeight = 200;

TYPE
    CellRow = SET OF [ 0..FieldWidth ];

    FieldRec   = RECORD
        Cells  :  ARRAY [ 0..FieldHeight ] OF CellRow;
        Rule   :  BITSET;
    END (* Field *);

VAR
    Field, NewField;

BEGIN
    Graph.InitEGA;
    Graph.GraphMode;

    Lib.Fill( ADR(Field), SIZEOF(Field), 0 );
    LOOP
        (* Draw the field while recalculating*)
        FOR CountRow := 0 TO FieldHeight DO
            FOR CountCol := 0 TO FieldWidth DO
                WITH Field DO
                    IF (CountCol IN Cells[CountRow]) THEN
                        Plot( CountCol, CountRow, 15 );
                    ELSE
                        Plot( CountCol, CountRow, 0 );
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
        END (* FOR *);
        Lib.Move( ADR(NewField.Cells), ADR(Field.Cells), SIZEOF(Field.Cells) );
    END (* LOOP *);
