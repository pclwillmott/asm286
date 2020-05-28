/*
 *-------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    PRD.h       Production Integer Constants
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

#ifndef PRD_h
#define PRD_h

PRD_addOp           = 8128,
PRD_andOp           = 8129,
PRD_arbitaryText    = 8130,
PRD_asmInstruction  = 8131,
PRD_assumeDir       = 8132,
PRD_assumeList      = 8133,
PRD_assumeReg       = 8134,
PRD_assumeRegister  = 8135,
PRD_assumeSegReg    = 8136,
PRD_assumeSegVal    = 8137,
PRD_assumeVal       = 8138,
PRD_bool            = 8139,
PRD_byteRegister    = 8140,
PRD_className       = 8141,
PRD_commaOptNewline = 8142,
PRD_constant        = 8143,
PRD_constExpr       = 8144,
PRD_coprocessor     = 8145,
PRD_dataDecl        = 8146,
PRD_dataDir         = 8147,
PRD_dataItem        = 8148,
PRD_dataType        = 8149,
PRD_directive       = 8150,
PRD_directiveList   = 8151,
PRD_e01             = 8152,
PRD_e02             = 8153,
PRD_e03             = 8154,
PRD_e04             = 8155,
PRD_e05             = 8156,
PRD_e06             = 8157,
PRD_e07             = 8158,
PRD_e08Op           = 8159,
PRD_e08             = 8160,
PRD_e09             = 8161,
PRD_e10             = 8162,
PRD_e11             = 8163,
PRD_e12             = 8164,
PRD_echoDir         = 8165,
PRD_elseifBlock     = 8166,
PRD_elseifList      = 8167,
PRD_elseifStatement = 8168,
PRD_elseBlock       = 8169,
PRD_elseStatement   = 8170,
PRD_endDir          = 8171,
PRD_endifStatement  = 8172,
PRD_endsDir         = 8173,
PRD_equalDir        = 8174,
PRD_equDir          = 8175,
PRD_equId           = 8176,
PRD_expr            = 8177,
PRD_externDef       = 8178,
PRD_externDir       = 8179,
PRD_externList      = 8180,
PRD_externType      = 8181,
PRD_fpInstruction   = 8182,
PRD_fpMnemonicOne   = 8183,
PRD_fpMnemonicTwo   = 8184,
PRD_fpMnemonicZero  = 8185,
PRD_frameExpr       = 8186,
PRD_generalDir      = 8187,
PRD_gpRegister      = 8188,
PRD_groupDir        = 8189,
PRD_groupId         = 8190,
PRD_ifBlock         = 8191,
PRD_ifDir           = 8192,
PRD_ifStatement     = 8193,
PRD_immExpr         = 8194,
PRD_initValue       = 8195,
PRD_inSegDir        = 8196,
PRD_inSegDirList    = 8197,
PRD_inSegmentDir    = 8198,
PRD_instPrefix      = 8199,
PRD_instruction     = 8200,
PRD_labelDef        = 8201,
PRD_labelDir        = 8202,
PRD_langType        = 8203,
PRD_listDir         = 8204,
PRD_listOption      = 8205,
PRD_mapType         = 8206,
PRD_memOption       = 8207,
PRD_mnemonicOne     = 8208,
PRD_mnemonicTwo     = 8209,
PRD_mnemonicZero    = 8210,
PRD_modelDir        = 8211,
PRD_modelOpt        = 8212,
PRD_modelOptList    = 8213,
PRD_module          = 8214,
PRD_mulOp           = 8215,
PRD_nameDir         = 8216,
PRD_nearfar         = 8217,
PRD_offsetDir       = 8218,
PRD_offsetDirType   = 8219,
PRD_operand         = 8220,
PRD_optionDir       = 8221,
PRD_optionItem      = 8222,
PRD_optionList      = 8223,
PRD_orOp            = 8224,
PRD_pageDir         = 8225,
PRD_pageExpr        = 8226,
PRD_pageLength      = 8227,
PRD_pageWidth       = 8228,
PRD_processor       = 8229,
PRD_processorDir    = 8230,
PRD_pubDef          = 8231,
PRD_publicDir       = 8232,
PRD_pubList         = 8233,
PRD_qualifiedType   = 8234,
PRD_register        = 8235,
PRD_relOp           = 8236,
PRD_scalarInstList  = 8237,
PRD_segAlign        = 8238,
PRD_segAttrib       = 8239,
PRD_segDir          = 8240,
PRD_segId           = 8241,
PRD_segIdList       = 8242,
PRD_segmentDef      = 8243,
PRD_segmentDir      = 8244,
PRD_segmentRegister = 8245,
PRD_segOption       = 8246,
PRD_segOptionList   = 8247,
PRD_simpleSegDir    = 8248,
PRD_sizeArg         = 8249,
PRD_stackOption     = 8250,
PRD_textEquDir      = 8251,
PRD_textList        = 8252,
PRD_textItem        = 8253,
PRD_titleDir        = 8254,
PRD_titleType       = 8255,
PRD_type            = 8256,

#endif /* PRD_h */

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

