/*
 *-------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    SPD.h       Production String Constants
 *
 *  This revision:
 *
 *    2020 May 27 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2020 May 27 Paul Willmott Baseline.
 *
 *  Copyright (c) 2020 Paul C. L. Willmott. See license at end.
 *
 *-------------------------------------------------------------------------------
 */

#ifndef SPD_h
#define SPD_h

#define SPD_addOp           "\101\001"
#define SPD_andOp           "\101\002"
#define SPD_arbitaryText    "\101\003"
#define SPD_asmInstruction  "\101\004"
#define SPD_assumeDir       "\101\005"
#define SPD_assumeList      "\101\006"
#define SPD_assumeReg       "\101\007"
#define SPD_assumeRegister  "\101\010"
#define SPD_assumeSegReg    "\101\011"
#define SPD_assumeSegVal    "\101\012"
#define SPD_assumeVal       "\101\013"
#define SPD_bool            "\101\014"
#define SPD_byteRegister    "\101\015"
#define SPD_className       "\101\016"
#define SPD_commaOptNewline "\101\017"
#define SPD_constant        "\101\020"
#define SPD_constExpr       "\101\021"
#define SPD_coprocessor     "\101\022"
#define SPD_dataDecl        "\101\023"
#define SPD_dataDir         "\101\024"
#define SPD_dataItem        "\101\025"
#define SPD_dataType        "\101\026"
#define SPD_directive       "\101\027"
#define SPD_directiveList   "\101\030"
#define SPD_e01             "\101\031"
#define SPD_e02             "\101\032"
#define SPD_e03             "\101\033"
#define SPD_e04             "\101\034"
#define SPD_e05             "\101\035"
#define SPD_e06             "\101\036"
#define SPD_e07             "\101\037"
#define SPD_e08Op           "\101\040"
#define SPD_e08             "\101\041"
#define SPD_e09             "\101\042"
#define SPD_e10             "\101\043"
#define SPD_e11             "\101\044"
#define SPD_e12             "\101\045"
#define SPD_echoDir         "\101\046"
#define SPD_elseifBlock     "\101\047"
#define SPD_elseifList      "\101\050"
#define SPD_elseifStatement "\101\051"
#define SPD_elseBlock       "\101\052"
#define SPD_elseStatement   "\101\053"
#define SPD_endDir          "\101\054"
#define SPD_endifStatement  "\101\055"
#define SPD_endsDir         "\101\056"
#define SPD_equalDir        "\101\057"
#define SPD_equDir          "\101\060"
#define SPD_equId           "\101\061"
#define SPD_expr            "\101\062"
#define SPD_externDef       "\101\063"
#define SPD_externDir       "\101\064"
#define SPD_externList      "\101\065"
#define SPD_externType      "\101\066"
#define SPD_fpInstruction   "\101\067"
#define SPD_fpMnemonicOne   "\101\070"
#define SPD_fpMnemonicTwo   "\101\071"
#define SPD_fpMnemonicZero  "\101\072"
#define SPD_frameExpr       "\101\073"
#define SPD_generalDir      "\101\074"
#define SPD_gpRegister      "\101\075"
#define SPD_groupDir        "\101\076"
#define SPD_groupId         "\101\077"
#define SPD_ifBlock         "\101\100"
#define SPD_ifDir           "\101\101"
#define SPD_ifStatement     "\101\102"
#define SPD_immExpr         "\101\103"
#define SPD_initValue       "\101\104"
#define SPD_inSegDir        "\101\105"
#define SPD_inSegDirList    "\101\106"
#define SPD_inSegmentDir    "\101\107"
#define SPD_instPrefix      "\101\110"
#define SPD_instruction     "\101\111"
#define SPD_labelDef        "\101\112"
#define SPD_labelDir        "\101\113"
#define SPD_langType        "\101\114"
#define SPD_listDir         "\101\115"
#define SPD_listOption      "\101\116"
#define SPD_mapType         "\101\117"
#define SPD_memOption       "\101\120"
#define SPD_mnemonicOne     "\101\121"
#define SPD_mnemonicTwo     "\101\122"
#define SPD_mnemonicZero    "\101\123"
#define SPD_modelDir        "\101\124"
#define SPD_modelOpt        "\101\125"
#define SPD_modelOptList    "\101\126"
#define SPD_module          "\101\127"
#define SPD_mulOp           "\101\130"
#define SPD_nameDir         "\101\131"
#define SPD_nearfar         "\101\132"
#define SPD_offsetDir       "\101\133"
#define SPD_offsetDirType   "\101\134"
#define SPD_operand         "\101\135"
#define SPD_optionDir       "\101\136"
#define SPD_optionItem      "\101\137"
#define SPD_optionList      "\101\140"
#define SPD_orOp            "\101\141"
#define SPD_pageDir         "\101\142"
#define SPD_pageExpr        "\101\143"
#define SPD_pageLength      "\101\144"
#define SPD_pageWidth       "\101\145"
#define SPD_processor       "\101\146"
#define SPD_processorDir    "\101\147"
#define SPD_pubDef          "\101\150"
#define SPD_publicDir       "\101\151"
#define SPD_pubList         "\101\152"
#define SPD_qualifiedType   "\101\153"
#define SPD_register        "\101\154"
#define SPD_relOp           "\101\155"
#define SPD_scalarInstList  "\101\156"
#define SPD_segAlign        "\101\157"
#define SPD_segAttrib       "\101\160"
#define SPD_segDir          "\101\161"
#define SPD_segId           "\101\162"
#define SPD_segIdList       "\101\163"
#define SPD_segmentDef      "\101\164"
#define SPD_segmentDir      "\101\165"
#define SPD_segmentRegister "\101\166"
#define SPD_segOption       "\101\167"
#define SPD_segOptionList   "\101\170"
#define SPD_simpleSegDir    "\101\171"
#define SPD_sizeArg         "\101\172"
#define SPD_stackOption     "\101\173"
#define SPD_textEquDir      "\101\174"
#define SPD_textList        "\101\175"
#define SPD_textItem        "\101\176"
#define SPD_titleDir        "\101\177"
#define SPD_titleType       "\102\001"
#define SPD_type            "\102\002"


#endif /* SPD_h */

/*
 *------------------------------------------------------------------------------
 *  ASM286
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott
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

