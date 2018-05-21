#define tINTEGER 257
#define tFLOAT 258
#define tIDENTIFIER 259
#define tSTRING 260
#define tSWEEPING 261
#define tIF 262
#define tPOST 263
#define tTWEET 264
#define tRETURN 265
#define tSTOP 266
#define tREAD 267
#define tAGAIN 268
#define tSMALL 269
#define tHUGE 270
#define tNEWS 271
#define tFAKE 272
#define tUSE 273
#define tPUBLIC 274
#define tDEFINE 275
#define tUSES 276
#define tBY 277
#define tTO 278
#define tPROCEDURE 279
#define tTHEN 280
#define tDO 281
#define tASSIGN 282
#define tFROM 283
#define tFUNCTION 284
#define tON 285
#define tCELL 286
#define tAT 287
#define tAS 288
#define tNULL 289
#define tBLOCKEND 290
#define tBLOCKBEGIN 291
#define tFOR 292
#define tINITIALLY 293
#define tELSIF 294
#define tINPUT 295
#define tELSE 296
#define tOR 297
#define tAND 298
#define tNOT 299
#define tEQUALS 300
#define tBELOW 301
#define tABOVE 302
#define tPLUS 303
#define tMINUS 304
#define tTIMES 305
#define tOVER 306
#define tMODULUS 307
#define tUNARY 308
#define tENDERECO 309
#define tOBJECTS 310
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
  int                   i;  /* integer value */
  std::string          *s;  /* symbol name or string literal */
  float                 f;
  cdk::basic_node      *node; /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  gr8::block_node      *block;
  basic_type           *typed;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
