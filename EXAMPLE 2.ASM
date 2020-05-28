IF 1
code SEGMENT
fred:
barry TEXTEQU '''FRED', 'GEO''RGE',
 'HARRY', % 1 + 2, ''''

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
jerry EQU 42 * 10
jerry3 = 12
jerry3 = 13
john TEXTEQU barry

ECHO % jerry3 + 12
code ENDS
ENDIF
ECHO % 'ABCD'
ECHO 'TBYTE = ', % SIZEOF TBYTE
END

Everything after END is ignored!
