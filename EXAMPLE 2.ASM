IF 1
.MODEL SMALL
.STACK
.CODE
fred:
barry TEXTEQU '''FRED', 'GEO''RGE',
 'HARRY', % 1 + 2, ''''

lab1:
lab2::
IF DEFINED fred
IF 0
NOP
ELSEIF 2 EQ 0
DAA
DAA
DAA
ELSE
CLI
ENDIF
ENDIF
NOP
.DATA
.DATA?
.FARDATA
.FARDATA?
jerry EQU 42 * 10
jerry3 = 12
jerry3 = 13
john TEXTEQU barry, % 234

ECHO % jerry3 + 12
ENDIF
ECHO % 'ABCD'
ECHO 'TBYTE = ', % SIZEOF TBYTE
END

Everything after END is ignored!
