/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    execute.c       Execute a production
 *
 *  This revision:
 *
 *    2020 April 11 Paul Willmott Relative Jumps added.
 *
 *  Revision History:
 *
 *    2020 April 10 Paul Willmott ORG and $ added.
 *
 *    2019 November 17 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm286.h"

enum {
  SUCCESS = 1,
  FAIL    = 0,
} ;

struct ifContext {
  int satisfied;
  int assemble;
};


struct ifContext ifStack[MAX_IF_STACK];

int no_print = 0;

int execute( ptree_node_t *ptree, int pass, int lineno )
{
  
  unsigned int arg_index ;
  
//  extern unsigned int current_position, dep_count;
  
  static enum SegAlign segAlign = SA_PARA;
  static enum CombineType combineType = CT_PRIVATE;
  static char *className = NULL;
  static int readonly = 0;
  static int combine_at = 0;
  
  extern int no_print ;
  
/*
 *-----------------------------------------------------------------------------
 */
  
/*
 * Only process productions.
 */
  
  if ( ptree->production_id < PRODUCTION_OFFSET )  {
    return SUCCESS ;
  }
  
/*
 * Handle Coditional Compilation
 */
  
  switch (ptree->production_id) {
    case PRD_ifStatement:
    case PRD_ppifStatement:
      if (ifCount == MAX_IF_STACK) {
        errno = ERR_TOO_MANY_NESTED_IFS;
        return FAIL;
      }
      int parent_assemble = (ifCount == 0) || (ifStack[ifCount-1].assemble);
      ifCount++;
      if (parent_assemble) {
        ifStack[ifCount-1].assemble = 1;
        if ( ! execute ( ptree->args[ 1 ], pass, -1 ) ) {
          return FAIL ;
        }
      }
      ifStack[ifCount-1].assemble = parent_assemble && (ptree->args[1]->value.i != 0);
      ifStack[ifCount-1].satisfied = ifStack[ifCount-1].assemble || (! parent_assemble);
      break;
    case PRD_elseifStatement:
    case PRD_ppelseifStatement:
      ifStack[ifCount-1].assemble = 1;
      if ( ! execute ( ptree->args[ 1 ], pass, -1 ) ) {
        return FAIL ;
      }
      ifStack[ifCount-1].assemble = (!ifStack[ifCount-1].satisfied) && (ptree->args[1]->value.i != 0);
      ifStack[ifCount-1].satisfied = ifStack[ifCount-1].satisfied || ifStack[ifCount-1].assemble;
      break;
    case PRD_elseStatement:
    case PRD_ppelseStatement:
      ifStack[ifCount-1].assemble = (!ifStack[ifCount-1].satisfied) ;
      ifStack[ifCount-1].satisfied = 1;
      break;
    case PRD_endifStatement:
    case PRD_ppendifStatement:
      ifCount--;
      break;
  }
  
  assembleIt = (ifCount == 0) || (ifStack[ifCount-1].assemble);
  
/*
 * Evaluate all the nodes from the bottom up and left to right.
 */
  
  for ( arg_index = 0; arg_index < ptree->num_args; arg_index++ ) {
    if ( ! execute ( ptree->args[ arg_index ], pass, lineno ) ) {
      return FAIL ;
    }
  }

/*
 * Skip non-action productions.
 */
  
  if ( ! assembleIt || ptree->exec_type == XT_IGNORE ) {
    return SUCCESS ;
  }
  
/*
 * If production is not an action production then drop through
 * value of the first argument.
 */
  
  if ( ptree->exec_type == XT_DROP ) {
    
    ptree->value_type = ptree->args[ 0 ]->value_type ;
    
    if ( ptree->value_type == TOK_STRING ) {
      ptree->value.s = NULL ;
      if ( ! setstr ( &ptree->value.s, ptree->args[ 0 ]->value.s ) ) {
        return FAIL ;
      }
      
    }
    else {
      CPYVALUE( ptree->value, ptree->args[ 0 ]->value ) ;
    }
    
    return SUCCESS ;
    
  }
  
/*
 * Jump Table
 */

//  int result, i1 = 0, i2 = 0;
  char *str ;
  int pass1 = (pass == 0);
  int pass2 = (pass == 1);
  unsigned int pos;

  unsigned char op2 = 0;
  
  unsigned char opcode[6] ;
  int opcode_count = 0;
  
  int literal[2] ;
  int literal_word[2] ;
  int literal_count = 0;
  
  extern FILE *ppFP2;
  extern int ppModified;
  
  int disp = 0;

  switch ( ptree->production_id ) {
     
    case PRD_lineDir:
    {
      if (add_src_file(ptree->args[2]->value.s, &cur_file)) {
        return FAIL;
      }
      cur_line = ptree->args[1]->value.i; 
      if (pass == -1) {
  //      fprintf(ppFP2, "LINE %i %s\n", ptree->args[1]->value.i, ptree->args[2]->value.s);
      }
      break;
    }
    case PRD_ppItem:
    {
      switch (ptree->variant) {
        case 0:
          fprintf(ppFP2, " ");
          break;
        case 2:
          fprintf(ppFP2, "\n");
          break;
        case 3:
        case 4:
          fprintf(ppFP2, "%s", ptree->args[0]->value.s);
          break;
      }
      break;
    }
    case PRD_ppIncludeDir:
    {
      int last_file = cur_file;
      int last_line = cur_line;
      int lineno = 1;
      fputs("#INCLUDE\n", ppFP2);
      if (ptree->variant == 0) {
        FILE *fp = NULL;
        if ((fp = fopen(ptree->args[1]->value.s, "r")) == NULL) {
          errno = ERR_FILE_OPEN_ERROR;
          return FAIL;
        }
        char buffer[512];
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
          fprintf(ppFP2,"LINE %i '%s'\n", lineno++, ptree->args[1]->value.s );
          fputs(buffer, ppFP2);
        }
        ppModified = -1;
        fclose(fp);
        fprintf(ppFP2,"LINE %i %s\n", last_line-1, src_file[last_file].filename);
        fputs("#INCLUDE\n", ppFP2);
      }
      break;
    }
    case PRD_ppTokenSequence:
    {
      fprintf(ppFP2, "%s\n", ptree->args[0]->value.s);
      break;
    }
    case PRD_nameDir:
    {
      if (module_name != NULL) {
        free(module_name);
      }
      setstr(&module_name, ptree->args[1]->value.s);
      break;
    }
    case PRD_listDir:
    {
      if (pass2) {
        list = ptree->args[0]->variant == 0;
        printf("LIST = %i\n", list);
      }
      break;
    }
    case PRD_titleDir: {
      switch (ptree->args[0]->variant) {
        case 0:
          setstr(&title, ptree->args[1]->value.s);
          break;
        case 1:
          setstr(&subtitle, ptree->args[1]->value.s);
          break;
      }
      break;
    }
    case PRD_assumeSegReg:
    {
      if (set_assume(ptree->args[0]->value.i, ptree->args[2]->value.s)) {
        return FAIL;
      }
      break;
    }
    case PRD_segmentRegister:
    {
      ptree->value.i = ptree->variant;
      ptree->value_type = TOK_INTEGERDEC;
      break;
    }
    case PRD_groupDir:
    {
      if (set_segment_group_from_list(ptree->args[0]->value.s)) {
        return FAIL;
      }
      break;
    }
    case PRD_segIdList:
    {
      int idx = (ptree->variant == 0) ? 2 : 0;
      if (add_to_segid_list(ptree->args[idx]->value.s)) {
        return FAIL;
      }
      break;
    }
    case PRD_segDir:
    {
      char *name = NULL;
      int access = 0;
      enum SegAlign align;
      enum CombineType combine;
      int at = 0;
      char *class = NULL;
      char *group = NULL;
      close_all_segments();
      switch (ptree->variant) {
        case 0: // code id
          name = ptree->args[1]->value.s;
        case 1: // code
          if (name == NULL) {
            name = "_TEXT";
          }
          align = SA_WORD;
          combine = CT_PUBLIC;
          class = "CODE";
          if (get_model() == MODEL_TINY) {
            group = "DGROUP";
          }
          break;
        case 2: // data?
          name = "_BSS";
          align = SA_WORD;
          combine = CT_PUBLIC;
          class = "BSS";
          group = "DGROUP";
          break;
        case 3: // data
          name = "_DATA";
          align = SA_WORD;
          combine = CT_PUBLIC;
          class = "DATA";
          group = "DGROUP";
          break;
        case 4: // const
          name = "CONST";
          align = SA_WORD;
          combine = CT_PUBLIC;
          class = "CONST";
          group = "DGROUP";
          break;
        case 5: // fardata? id
          name = ptree->args[1]->value.s;
        case 6: // fardata?
          if (name == NULL) {
            name = "FAR_BSS";
          }
          align = SA_PARA;
          combine = CT_PRIVATE;
          class = "FAR_BSS";
          break;
        case 7: // fardata id
          name = ptree->args[1]->value.s;
        case 8: // fardata
          if (name == NULL) {
            name = "FAR_DATA";
          }
          align = SA_PARA;
          combine = CT_PRIVATE;
          class = "FAR_DATA";
          break;
        case 9: // stack expr
          at = ptree->args[1]->value.i;
        case 10: // stack
          if (get_model() == MODEL_TINY) {
            errno = ERR_INVALID_DIRECTIVE;
            return FAIL;
          }
          if (at == 0) {
            at = 1024;
          }
          name = "STACK";
          group = "DGROUP";
          align = SA_PARA;
          combine = CT_STACK;
          class = name;
          break;
      }
      
      if (open_segment_with_attributes(name, access, align, combine, at, class)) {
        return FAIL;
      }
      if (group != NULL) {
        set_segment_group(name, group);
      }
      if (strcmp(name,"_TEXT") == 0) {
        set_assume(SR_CS, name);
      }
      else {
        set_assume(SR_CS, "ERROR");
        set_assume(SR_DS, name);
        set_assume(SR_ES, name);
      }
      if (get_model() != MODEL_TINY) {
        set_assume(SR_SS, "STACK");
      }
      else {
        set_assume(SR_SS, "DGROUP");
      }

      break;
    }
    case PRD_assumeDir:
    {
      for (int i = 0; i < 4; i++) {
        if (set_assume(i, "NOTHING")) {
          return FAIL;
        }
      }
      break;
    }
    case PRD_segOption:
    {
      readonly = -1;
      break;
    }
    case PRD_className:
    {
      setstr(&className, ptree->args[0]->value.s);
      break;
    }
    case PRD_segAttrib:
    {
      combineType = (ptree->variant == 5) ? CT_PUBLIC : ptree->variant;
      if (combineType == CT_AT) {
        combine_at = ptree->args[1]->value.i;
      }
      break;
    }
    case PRD_segAlign:
    {
      segAlign = ptree->variant;
      break;
    }
    case PRD_type:
    {
      ptree->value.i = ptree->args[0]->variant;
      ptree->value_type = TOK_INTEGERDEC;
      break;
    }
    case PRD_equalDir:
    {
      if (set_variable(ptree->args[0]->value.s, DT_SDWORD, ptree->args[2]->value.i)) {
        return FAIL;
      }
      break;
    }
    case PRD_equDir:
    {
      if (set_constant(ptree->args[0]->value.s, DT_SDWORD, ptree->args[2]->value.i)) {
        return FAIL;
      }
      break;
    }
    case PRD_textEquDir:
    {
      if (set_strconstant(ptree->args[0]->value.s, ptree->args[2]->value.s)) {
        return FAIL;
      }
      break;
    }
    case PRD_textList:
    {
      unsigned long l, l1 = strlen(ptree->args[0]->value.s), l2 = strlen(ptree->args[2]->value.s);
      l = l1 + l2;
      char *t;
      if ((t = (char *) malloc(l+1)) == NULL) {
        errno = ERR_OUT_OF_MEMORY;
        return FAIL;
      }
      strcpy(t, ptree->args[0]->value.s);
      strcat(t, ptree->args[2]->value.s);
      ptree->value.s = t;
      ptree->value_type = TOK_STRING;
      break;
    }
    case PRD_textItem:
    {
      char buffer[32];
      switch (ptree->variant) {
        case 0:
          sprintf(buffer, "%i", ptree->args[1]->value.i) ;
          setstr(&ptree->value.s, buffer);
          ptree->value_type = TOK_STRING;
          break;
        case 2: // id
          if (get_strsymbol_value(ptree->args[0]->value.s, &ptree->value.s)) {
            return FAIL;
          }
          break;
      }
      break;
    }
    case PRD_bool:
      ptree->value_type = TOK_INTEGERDEC;
      ptree->value.i = (ptree->variant == 0) ? 0 : -1;
      break;
    case PRD_endDir:
    {
      main_routine = ptree->args[1]->value.i;
      list = 0;
      break;
    }
    case PRD_modelOpt:
    {
      switch (ptree->variant) {
        case 0: // Language Type
          language_type = ptree->args[0]->variant;
          break;
        case 1: // Stack
          stack_distance = ptree->args[0]->variant;
          break;
      }
      break;
    }
    case PRD_e12:
    {
      ptree->value.i = ptree->args[1]->value.i;
      ptree->value_type = ptree->args[1]->value_type;
      break;
    }
    case PRD_e11:
    {
      switch (ptree->variant) {
        case 2: // sizeof
        {
          switch (ptree->args[1]->variant) {
            case 0: // type
            {
              ptree->value.i = datatype_size(ptree->args[1]->value.i);
              break;
            }
            case 1: // identifier
            {
              if (get_array_size(ptree->args[1]->value.s, &ptree->value.i)) {
                return FAIL;
              }
              break;
            }
          }
          ptree->value_type = TOK_INTEGERDEC;
          break;
        }
        case 3: // lengthof
        {
          if (get_array_size(ptree->args[1]->value.s, &ptree->value.i)) {
            return FAIL;
          }
          ptree->value_type = TOK_INTEGERDEC;
          break;
        }
        case 12: // string
        {
          unsigned int dw = 0;
          char *s = ptree->args[0]->value.s;
          for (int i = 0; *s && i < 4; i++, s++) {
            dw |= (unsigned int)*s << (i << 3);
          }
          ptree->value.i = dw;
          ptree->value_type = TOK_INTEGERDEC;
          break;
        }
        case 4: // defined
        {
          ptree->value_type = TOK_INTEGERDEC;
          ptree->value.i = (get_symbol_index(ptree->args[1]->value.s) == -1) ? 0 : -1;
          break;
        }
        case 6: // $
        {
          unsigned int value;
          
          if (get_current_position(&value)) {
            return FAIL;
          }
          ptree->value_type = TOK_INTEGERDEC;
          ptree->value.i = value;
          break;
        }
        case 11: // identifier
        {
          int value;
          if (get_symbol_value(ptree->args[0]->value.s, &value)) {
            return FAIL;
          }
          ptree->value.i = value;
          ptree->value_type = TOK_INTEGERDEC;
          break;
        }
      }
      break;
    }
    case PRD_e09:
    {
      switch (ptree->variant) {
        case 2: // type
        {
          if (get_element_length(ptree->args[1]->value.s, &ptree->value.i)) {
            return FAIL;
          }
          ptree->value_type = TOK_INTEGERDEC;
          break;
        }
      }
      break;
    }
    case PRD_modelDir:
    {
      if (set_model(ptree->args[1]->variant)) {
        return FAIL;
      }
      break;
    }
    case PRD_mnemonicZero:
    {
      const unsigned char simple[][3] = {
        {1, 0x37, 0x00}, // AAA
        {2, 0xd5, 0x0a}, // AAD
        {2, 0xd4, 0x0a}, // AAM
        {1, 0x3f, 0x00}, // AAS
        {1, 0x98, 0x00}, // CBW
        {1, 0xf8, 0x00}, // CLC
        {1, 0xfc, 0x00}, // CLD
        {1, 0xfa, 0x00}, // CLI
        {2, 0x0f, 0x06}, // CLTS
        {1, 0xf5, 0x00}, // CMC
        {1, 0x99, 0x00}, // CWD
        {1, 0x27, 0x00}, // DAA
        {1, 0x2f, 0x00}, // DAS
        {1, 0xf4, 0x00}, // HLT
        {1, 0xce, 0x00}, // INTO
        {1, 0xcf, 0x00}, // IRET
        {1, 0x9f, 0x00}, // LAHF
        {1, 0xc9, 0x00}, // LEAVE
        {1, 0x90, 0x00}, // NOP
        {1, 0x61, 0x00}, // POPA
        {1, 0x9d, 0x00}, // POPF
        {1, 0x60, 0x00}, // PUSHA
        {1, 0x9c, 0x00}, // PUSHF
        {1, 0xcb, 0x00}, // RET <- NOT THE WHOLE STORY
        {1, 0x9e, 0x00}, // SAHF
        {1, 0xf9, 0x00}, // STC
        {1, 0xfd, 0x00}, // STD
        {1, 0xfb, 0x00}, // STI
        {1, 0x9b, 0x00}, // WAIT
      };
      for (int i = 1; i < simple[ptree->variant][0] + 1; i++) {
        opcode[opcode_count++] = simple[ptree->variant][i];
      }
      break;
    }
    case PRD_pubDef:
    {
      if (set_symbol_visibility(ptree->args[(ptree->variant == 0) ? 1 : 0]->value.s, VS_PUBLIC)) {
        errno = ERR_IDENTIFIER_NOT_FOUND;
        return FAIL;
      }
      break;
    }
    case PRD_labelDef:
    {
      if (pass1) {
        enum Distance dist;
        char *t;
        setstr(&t,ptree->args[0]->value.s);
        unsigned long l = strlen(t);
        if (t[l-2] == ':') {
          dist = DIST_FAR;
          l -= 2;
        }
        else {
          dist = DIST_NEAR;
          l--;
        }
        t[l] = '\0' ;
        if (get_current_position(&pos)) {
          return FAIL;
        }
        if (add_label(t, dist, pos)) {
          return FAIL;
        }
        free(t);
      }
      break;
    }
    case PRD_segmentDir:
    {
      int res = open_segment_with_attributes(ptree->args[0]->value.s, readonly, segAlign, combineType, combine_at, className);
      readonly = 0;
      segAlign = SA_PARA;
      combineType = CT_PRIVATE;
      combine_at = 0;
      if (className != NULL) {
        free(className);
        className = NULL;
      }
      if (res) {
        return FAIL;
      }
      break;
    }
    case PRD_endsDir:
      if (close_segment(ptree->args[0]->value.s)) {
        return FAIL;
      }
      break;
    case PRD_processorDir:
    {
      if (ptree->variant == 0) {
        int priority[] = { 0, 1, 3, 2 }; 
        processor = priority[ptree->args[0]->variant];
      }
      else {
        coprocessor = ptree->args[0]->variant;
      }
      int pmask[] = { 0x01, 0x03, 0x07, 0x87 };
      int cpmask[] = { 0x00, 0x01, 0x05 };
      if (set_variable("@Cpu", DT_WORD, cpmask[coprocessor] << 8 | pmask[processor])) {
        return FAIL;
      }
      break;
    }
    case PRD_echoDir:
      if (pass2) {
        printf("%s\n", ptree->args[1]->value.s);
      }
      break;
    case PRD_e07:
    {
      int result = ptree->args[1]->value.i;
      ptree->value_type = TOK_INTEGERDEC;
      ptree->value.i = (ptree->args[0]->variant == 0) ? result : -1 * result;
      break;
    }
    case PRD_e08:
    {
      int result = ptree->args[1]->value.i;
      ptree->value_type = TOK_INTEGERDEC;
      ptree->value.i = (ptree->args[0]->variant == 0) ? result >> 8: result & 0xff;
      break;
    }
    case PRD_e03:
    {
      ptree->value_type = TOK_INTEGERDEC;
      ptree->value.i = ~ptree->args[1]->value.i;
      break;
    }
    case PRD_e01:
    case PRD_e02:
    case PRD_e04:
    case PRD_e05:
    case PRD_e06:
    {
      int i1 = ptree->args[0]->value.i;
      int i2 = ptree->args[2]->value.i;
      int result;
      switch (ptree->args[1]->args[0]->production_id) {
        case TOK_MULTIPLY:
          result = i1 * i2;
          break;
        case TOK_DIVIDE:
          result = i1 / i2;
          break;
        case TOK_MOD:
          result = i1 % i2;
          break;
        case TOK_SHL:
          result = i1 << i2;
          break;
        case TOK_SHR:
          result = i1 >> i2;
          break;
        case TOK_PLUS:
          result = i1 + i2;
          break;
        case TOK_MINUS:
          result = i1 - i2;
          break;
        case TOK_AND:
          result = i1 & i2;
          break;
        case TOK_OR:
          result = i1 | i2;
          break;
        case TOK_XOR:
          result = i1 ^ i2;
          break;
        case TOK_EQ:
          result = (i1 == i2) ? -1 : 0;
          break;
        case TOK_NE:
          result = (i1 != i2) ? -1 : 0;
          break;
        case TOK_LT:
          result = (i1 < i2) ? -1 : 0;
          break;
        case TOK_GT:
          result = (i1 > i2) ? -1 : 0;
          break;
        case TOK_LE:
          result = (i1 <= i2) ? -1 : 0;
          break;
        case TOK_GE:
          result = (i1 >= i2) ? -1 : 0;
          break;
        default:
          result = 0;
          break;
      }
      ptree->value_type = TOK_INTEGERDEC;
      ptree->value.i = result;
      break;
    }
  }

  // Do the code deposits
  
  int do_print = pass2 && ptree->production_id != PRD_lineDir; // && ( opcode_count > 0 || literal_count > 0);
  
  if (do_print) {
    if (print_src_start()) {
      return FAIL;
    }
  }
  
  if (dep_disp(opcode, opcode_count, disp, pass, lineno)) {
    return FAIL;
  }
  
  for (int i = 0; i < literal_count; i++) {
    if (literal_word[i]) {
      if (depw((unsigned short)literal[i], pass, lineno)) {
        return FAIL;
      }
    }
    else {
      if (dep((unsigned char)literal[i], pass, lineno)) {
        return FAIL;
      }
    }
  }
  
  if (do_print) {
    if (print_src_end()) {
      return FAIL;
    }
  }

  return SUCCESS ;
  
/*
 * Finished
 */
  
}

/*
 *-----------------------------------------------------------------------------
 * This routine sets a string value.
 *-----------------------------------------------------------------------------
 */

int setstr
(
 char **str,
 char *value
)
{
  
  /*
   *-----------------------------------------------------------------------------
   */
  
  if ( ( (*str) = ( char * ) malloc ( strlen ( value ) + 1 ) ) == NULL ) {
    errno = ERR_OUT_OF_MEMORY;
    return FAIL ;
  }
  
  strcpy ( (*str), value ) ;
  
  return SUCCESS ;
  
  /*
   * Finished
   */
  
}

int dep_disp(unsigned char opcodes[], int opcode_count, int disp, int pass, int lineno)
{
  if (dep_opcodes(opcodes, opcode_count, pass, lineno)) {
    return 1;
  }
  if (opcode_count > 1) {
    switch ((opcodes[1] >> 6) & 0x03) {
      case 0x01:
        return dep((unsigned char)disp, pass, lineno);
      case 0x02:
        return depw((unsigned short)disp, pass, lineno);
    }
  }
  return 0;
}

unsigned char rm_disp_mod(ptree_node_t *ptree, int *disp, int *w)
{
  unsigned char opcode2 = 0;
  *w = 0x01;
  switch (ptree->variant) {
    case 0:
    case 1:
      *w = ptree->args[0]->variant;
      if (ptree->variant == 0) {
        *disp = ptree->args[3]->value.i;
        if (is_word(*disp)) {
          opcode2 = 0x02 << 6;
        }
        else {
          opcode2 = 0x01 << 6;
        }
      }
      opcode2 |= ptree->args[1]->variant;
      break;
    case 2:
    case 3:
      if (ptree->variant == 2) {
        *disp = ptree->args[2]->value.i;
        if (is_word(*disp)) {
          opcode2 = 0x02 << 6;
        }
        else {
          opcode2 = 0x01 << 6;
        }
      }
      opcode2 |= ptree->args[0]->variant;
      break;
  }
  return opcode2;
}

int dep_opcodes(unsigned char opcodes[], int opcode_count, int pass, int lineno)
{
  for (int i = 0; i < opcode_count; i++) {
    if (dep(opcodes[i], pass, lineno)) {
      return 1;
    }
  }
  return 0;
}

int is_word(int value)
{
  return ( value < -128 || value > 255);
}

int add_src_file(char *filename, int *filenum)
{
  int index;
  
  for (index = 0; index < src_file_count; index++) {
    if (strcmp(src_file[index].filename, filename) == 0) {
      *filenum = index;
      return 0;
    }
  }
  
  *filenum = src_file_count;
  
  if (!src_file_count) {
    if ((src_file = (src_file_t *) malloc(sizeof(src_file_t))) == NULL) {
      errno = ERR_OUT_OF_MEMORY;
      return -1;
    }
  }
  else {
    if ((src_file = (src_file_t *) realloc(src_file, sizeof(src_file_t) * (src_file_count + 1))) == NULL) {
      errno = ERR_OUT_OF_MEMORY;
      return -1;
    }
  }
  
  src_file_t *ptr = &src_file[src_file_count++];
  
  setstr(&ptr->filename, filename);
  ptr->cur_line = 0;
  ptr->cur_pos = 0L;
  
  return 0;
  
}

/*
 *------------------------------------------------------------------------------
 *  ASM286
 *
 *  Copyright (c) 2019 Paul C. L. Willmott
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *------------------------------------------------------------------------------
 */
