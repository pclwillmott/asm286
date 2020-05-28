IF 1
code SEGMENT
fred:
ECHO '''FRED', 'GEO''RGE',
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
code ENDS
ENDIF


END

Everything after END is ignored!
