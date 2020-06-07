    .LIST
.CODE
fred:

#IF 1

#INCLUDE '/Users/paul/Documents/Projects/LEGACY/asm286/inc_example.ASM'

DB 'hello'
             
DB \
\
\
\
'fred' ; this is. a comment






         #IF 1
polly:
#ELSEIF 0
polly3:
#ELSEIF 1
polly4:
#ELSE
polly2:
#ENDIF

#ENDIF

wally:

END fred

Everything after END is ignored!
