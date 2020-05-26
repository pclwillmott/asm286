.MODEL LARGE
; hello
.286P
.287
ECHOGood Day!
ECHO = 1 + 1
ECHO = NOT 0
ECHO = 1 OR 2
ECHO = 0ffffh
ECHO = 377O
ECHO = 101010B
ECHO = HIGH 0eeffh
ECHO = LOW 0eeffh
code SEGMENT
ECHO NOP
ECHO NOP
ERIC: NOP
data SEGMENT
FRED:: DAA
data ENDS
GG: NOP
fred: NOP
PUBLIC    FRED
ECHO = @Model
ECHO = $
ECHO = -@Cpu
ECHO = - -1

;F2XM1
ECHO NOP and some more
code ENDS



END

Everything after END is ignored!
