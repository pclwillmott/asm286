.LIST
.CODE
fred:

#IF 1

#INCLUDE '/Users/paul/Documents/Projects/LEGACY/asm286/inc_example.ASM'

DB 'hello'
.NOLIST 
            
DB \
\
\
\
'fred' ; this is. a comment


echo typo some text\
some more text\
yet more text

.NOLIST
.LIST

#IF 1
polly:
#ELSEIF 0
polly3:
#ELSEIF 0
polly4:
NOP
#ELSEIF 2
DAA
#ELSE
polly2:
#ENDIF

#ENDIF

wally:

NOP
NOP
NOP

END fred

Everything after END is ignored!
