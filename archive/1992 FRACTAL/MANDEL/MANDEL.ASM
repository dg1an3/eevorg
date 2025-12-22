LambdaX   DQ ?
LambdaY   DT ?
Iter      DW ?
ZX        D
          WAIT FINIT
          WAIT FLD LambdaX    ; Lambda(x)
          WAIT FLD LambdaY    ; Lambda(yi)
          WAIT FLDZ           ; Z(x) = 0
          WAIT FLDZ           ; Z(yi) = 0i
          MOV CX,Iter         ; Number of iterations

IterLoop: WAIT FLD ST(0)      ; Z(y)                               23
          WAIT FMUL ST(0)     ; Z(y)*Z(y)                          141
          WAIT FDECSTP        ;                                    12
          WAIT FMUL ST(1)     ; Z(y)*Z(x)                          141
          WAIT FIMUL TWO      ; Z(y)*Z(x)*2                        139
          WAIT FADD ST(2)     ; Z(y)*Z(x)*2 + Lambda(y)            88
          WAIT FDECSTP        ;                                    12
          WAIT FMUL ST(0)     ; Z(x)*Z(x)                          141
          WAIT FSUB ST(6)     ; Z(x)*Z(x) - Z(y)*Z(y)              88
          WAIT FADD ST(2)     ; Z(x)*Z(x) - Z(y)*Z(y) + Lambda(x)  88
          WAIT FINCSTP        ;                                    12
          LOOP IterLoop       ;                                    17

          WAIT FMUL ST(0)
          WAIT DECSTP
          WAIT FMUL ST(0)
          WAIT FADD ST(7)
          WAIT FICOM FOUR
