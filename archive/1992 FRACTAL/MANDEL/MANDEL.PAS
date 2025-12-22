Program Mandelbrot;

(*$U+*)

Const
  Iterations = 100;
  Xmin  =  -1.0;
  Xsize =   2.0;
  Ymin  =  -1.0;
  Ysize =   2.0;

Var
  LambdaX,
  LambdaY,
  Xstep,
  Ystep,
  X, Y, XY : Real;
  AtX, AtY,
  Count    : Integer;


Begin
  GraphColorMode;
  LambdaX:=Xmin;
  Xstep:=Xsize/320.0;
  LambdaY:=Ymin;
  Ystep:=Ysize/200.0;
  For AtY:=199 downto 0 do begin
    For AtX:=0 to 319 do begin
      X:=0.0; Y:=0.0;
      While (X*X+Y*Y<4.0) and (Count<100) do begin
        GotoXY(1,1); Write(Count:5);
        XY:=2*X*Y;
        X:=X*X-Y*Y+LambdaX;
        Y:=XY+LambdaY;
        Count:=Count+1;
      End; (* While *)
      Plot(AtX,AtY,Trunc(X*X+Y*Y/4.0));
      LambdaX:=LambdaX+Xstep;
    End; (* For *)
    LambdaY:=LambdaY+Ystep;
  End; (* For *)
End. (* Mandelbrot *)