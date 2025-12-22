Program Fractals;

(*$U+*)

Type
  Complex = record
    R, I  :   Real;
  End; (* Complex *)

Procedure AddComplex
( X, Y  : Complex;
  Var Z : Complex );
Begin
  Z.R:=X.R+Y.R;
  Z.I:=X.I+Y.I;
End; (* AddComplex *)

Procedure MultComplex
( X, Y  : Complex;
  Var Z : Complex );
Begin
  Z.R:= X.R * Y.R - X.I * Y.I;
  Z.I:= X.R * Y.I - X.I * Y.R;
End; (* MultComplex *)

Function AbsComplex
( X : Complex ) : Real;
Begin
  AbsComplex:=SqRt(X.R*X.R + X.I*X.I);
End; (* AbsComplex *)

Function Dynamic
( Z, Lambda  : Complex;
  Iterations : Integer ) : Real;
Var
  NextZ : Complex;
  Count : Integer;
Begin
  NextZ.R:=Z.R;
  NextZ.I:=Z.I;
  For Count:=0 to Iterations do begin
    MultComplex(NextZ,NextZ,NextZ);
    AddComplex(NextZ,Lambda,NextZ);
  End; (* For *)
  Dynamic:=Abs(AbsComplex(NextZ)-AbsComplex(Z));
End; (* Dynamic *)

Var
  RealMin, RealMax, RealStep : Real;
  ImagMin, ImagMax, ImagStep : Real;

Procedure PlotPoint
( Z     : Complex;
  Color : Integer );
Begin
  Plot(Trunc((Z.R+RealMin)*RealStep),Trunc((Z.I+ImagMin)*ImagStep),
    Color);
End; (* PlotPoint *)

Var
  Z, Lambda  : Complex;
  Iterations : Integer;

Begin
  GraphColorMode;
  RealMin:=0.0; RealMax:=1.0; RealStep:=(RealMax-RealMin)/320.0;
  ImagMin:=0.0; ImagMax:=1.0; ImagStep:=(ImagMax-ImagMin)/200.0;
  Lambda.R:=0.26; Lambda.I:=1.42;
  Iterations:=100;
  Z.R:=RealMin;
  While (Z.R<RealMax) do begin
    Z.I:=ImagMin;
    While (Z.I<ImagMax) do begin
      PlotPoint(Z,Trunc(Dynamic(Z,Lambda,Iterations)/100.0) mod 4);
      Z.I:=Z.I+ImagStep;
    End; (* While *)
    Z.R:=Z.R+RealStep;
  End; (* While *)
End.



End.