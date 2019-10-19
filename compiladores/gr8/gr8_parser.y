%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;  /* integer value */
  std::string          *s;  /* symbol name or string literal */
  float                 f;
  cdk::basic_node      *node; /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  gr8::block_node      *block;
  basic_type           *typed;
};

%token <i> tINTEGER
%token <f> tFLOAT
%token <s> tIDENTIFIER tSTRING
%token tSWEEPING tIF tPOST tTWEET tRETURN tSTOP tREAD tAGAIN tSMALL tHUGE tNEWS tFAKE tUSE tPUBLIC tDEFINE tUSES tBY tTO tPROCEDURE tTHEN tDO tASSIGN tFROM tFUNCTION tON tCELL tAT tAS tNULL tBLOCKEND tBLOCKBEGIN tFOR tINITIALLY tELSIF tINPUT 

%nonassoc tELSE tIF

%left tOR
%left tAND 
%nonassoc tNOT
%right tEQUALS
%left tBELOW tABOVE
%left tPLUS tMINUS
%left tTIMES tOVER tMODULUS
%nonassoc tUNARY tENDERECO tOBJECTS "(" ")" "," 

%type <node> function declaration  function variable instruction cond iter X
%type <typed> type small_sequence huge_sequence fake_sequence
%type <sequence> arguments arguments_define variables declarations instructions expr2 file
%type <expression> expr
%type <lvalue> lval 
%type <block> block 
%type <s> concatenacao


%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%


file : declarations                                                                 {compiler->ast($$ = $1);} 
     |                                                                              {$$ = nullptr;}
     ;

block : tBLOCKBEGIN declarations instructions tBLOCKEND                             {$$ = new gr8::block_node(LINE,$2,$3);}
      | tBLOCKBEGIN declarations tBLOCKEND                                          {$$ = new gr8::block_node(LINE,$2,nullptr);}
      | tBLOCKBEGIN instructions tBLOCKEND                                          {$$ = new gr8::block_node(LINE,nullptr,$2);}
      | tBLOCKBEGIN tBLOCKEND                                                       {$$ = new gr8::block_node(LINE,nullptr,nullptr);}
      ;

declarations : declarations declaration                                             {$$ = new cdk::sequence_node(LINE,$2,$1);}
             | declaration                                                          {$$ = new cdk::sequence_node(LINE, $1);}
             ;

declaration : variable                                                              {$$ = $1;}
            | function                                                              {$$ = $1;}
            ;

function : type tFUNCTION tIDENTIFIER arguments                                {$$ = new gr8::func_declare_node(LINE,0,$1,$3,$4);}
         | tPUBLIC type tFUNCTION tIDENTIFIER arguments                        {$$ = new gr8::func_declare_node(LINE,1,$2,$4,$5);}
         | tUSE type tFUNCTION tIDENTIFIER arguments                           {$$ = new gr8::func_declare_node(LINE,2,$2,$4,$5);}
         | tPROCEDURE tIDENTIFIER arguments                                    {$$ = new gr8::func_declare_node(LINE,0,nullptr,$2,$3);}
         | tPUBLIC tPROCEDURE tIDENTIFIER arguments                            {$$ = new gr8::func_declare_node(LINE,1,nullptr,$3,$4);}
         | tUSE tPROCEDURE tIDENTIFIER arguments                               {$$ = new gr8::func_declare_node(LINE,2,nullptr,$3,$4);}
         | tDEFINE type tFUNCTION tIDENTIFIER arguments_define tAS block          {$$ = new gr8::func_define_node(LINE,0,$2,$4,$5,$7);}
         | tDEFINE tPUBLIC type tFUNCTION tIDENTIFIER arguments_define tAS block  {$$ = new gr8::func_define_node(LINE,1,$3,$5,$6,$8);}
         | tDEFINE tUSE type tFUNCTION tIDENTIFIER arguments_define tAS block     {$$ = new gr8::func_define_node(LINE,2,$3,$5,$6,$8);}
         | tDEFINE tPROCEDURE tIDENTIFIER arguments_define tAS block         {$$ = new gr8::func_define_node(LINE,0,nullptr,$3,$4,$6);}
         | tDEFINE tUSE tPROCEDURE tIDENTIFIER arguments_define tAS block    {$$ = new gr8::func_define_node(LINE,2,nullptr,$4,$5,$7);}
         | tDEFINE tPUBLIC tPROCEDURE tIDENTIFIER arguments_define tAS block {$$ = new gr8::func_define_node(LINE,1,nullptr,$4,$5,$7);}
         ;

variables : variables ',' variable                                                  {$$ = new cdk::sequence_node(LINE,$3,$1);}
          | variable                                                                {$$ = new cdk::sequence_node(LINE, $1);}
          ;

variable : type tIDENTIFIER                                               {$$ = new gr8::var_init_node(LINE,-1,$1,$2,nullptr);}
         | tUSE type tIDENTIFIER                                          {$$ = new gr8::var_init_node(LINE,1,$2,$3,nullptr);}
         | tPUBLIC type tIDENTIFIER                                       {$$ = new gr8::var_init_node(LINE,2,$2,$3,nullptr);}
         | type tIDENTIFIER '(' tINITIALLY expr ')'                       {$$ = new gr8::var_init_node(LINE,0,$1,$2,$5);}
         | tUSE type tIDENTIFIER '(' tINITIALLY expr ')'                  {$$ = new gr8::var_init_node(LINE,1,$2,$3,$6);}
         | tPUBLIC type tIDENTIFIER '(' tINITIALLY expr ')'               {$$ = new gr8::var_init_node(LINE,2,$2,$3,$6);}
         ;

type : tSMALL                                                                       {$$ = new basic_type(4,basic_type::TYPE_INT);} 
     | tHUGE                                                                        {$$ = new basic_type(8,basic_type::TYPE_DOUBLE);}
     | tSMALL small_sequence                                                        {$$ = new basic_type(4,basic_type::TYPE_POINTER); 
                                                                                     $$->_subtype = $2;}
     | tHUGE huge_sequence                                                          {$$ = new basic_type(4,basic_type::TYPE_POINTER); 
                                                                                     $$->_subtype = $2;}
     | fake_sequence                                                                {$$ = $1;}
     ;

small_sequence : tFAKE                                                               {$$ = new basic_type(4,basic_type::TYPE_INT);}  
               | tSMALL small_sequence                                               {$$ = new basic_type(4,basic_type::TYPE_POINTER);
                                                                                      $$->_subtype = $2; } 
               ;

huge_sequence : tFAKE                                                               {$$ = new basic_type(8,basic_type::TYPE_DOUBLE);}
              | tHUGE huge_sequence                                                 {$$ = new basic_type(4,basic_type::TYPE_POINTER);
                                                                                      $$->_subtype = $2;}
              ;

fake_sequence : tNEWS                                                               {$$ = new basic_type(4,basic_type::TYPE_STRING);}  
              | tFAKE fake_sequence                                                 {$$ = new basic_type(4,basic_type::TYPE_POINTER);
                                                                                     $$->_subtype = $2;}
              ;


instructions : instructions instruction                                             {$$ = new cdk::sequence_node(LINE, $2, $1);}
             | instruction                                                          {$$ = new cdk::sequence_node(LINE, $1);}
             ;

instruction : expr                                                                  {$$ = $1;}
            | tASSIGN expr tTO lval                                                 {$$ = new cdk::assignment_node(LINE,$4,$2);}
            | tTWEET expr                                                           {$$ = new gr8::print_node(LINE,$2,false);}
            | tPOST expr                                                            {$$ = new gr8::print_node(LINE,$2,true);}
            | tAGAIN                                                                {$$ = new gr8::again_node(LINE, 1);}
            | tAGAIN tINTEGER                                                       {$$ = new gr8::again_node(LINE, $2);}
            | tSTOP                                                                 {$$ = new gr8::stop_node(LINE, 1);}
            | tSTOP tINTEGER                                                        {$$ = new gr8::stop_node(LINE, $2);}
            | tRETURN                                                               {$$ = new gr8::return_node(LINE, nullptr);}
            | tRETURN expr                                                          {$$ = new gr8::return_node(LINE, $2);}
            | cond                                                                  {$$ = $1;}
            | iter                                                                  {$$ = $1;}
            | block                                                                 {$$ = $1;}     
            ;

expr2 : expr2 "," expr                                                              {$$ = new cdk::sequence_node(LINE, $3, $1);}
      | expr                                                                        {$$ = new cdk::sequence_node(LINE, $1);}
      ; 

cond : tIF expr tTHEN block  %prec tIF                                              {$$ = new gr8::if_node(LINE,$2,$4);}
     | tIF expr tTHEN block X  %prec tELSE                                          {$$ = new gr8::if_else_node(LINE,$2,$4,$5);}
     ;
     
X : tELSIF expr tTHEN block                                                         {$$ = new gr8::if_node(LINE,$2,$4);}
  | tELSE block                                                                     {$$ = $2;}
  | tELSIF expr tTHEN block X                                                       {$$= new gr8::if_else_node(LINE,$2,$4,$5);}
  ;

iter : tSWEEPING lval tFROM expr tTO expr tDO block       {$$ = new gr8::sweeping_node(LINE,$4,$6,new cdk::integer_node(LINE,1),$2,$8);}
     | tSWEEPING lval tFROM expr tTO expr tBY expr tDO block                        {$$ = new gr8::sweeping_node(LINE,$4,$6,$8,$2,$10);}
     ;

arguments : /* empty */                                                             {$$ = nullptr;}
          | tUSES variables                                                         {$$ = $2;}
          ;

arguments_define : /* empty */                                                      {$$ = nullptr;}
                 | tON variables                                                    {$$ = $2;}
                 ;

expr : tINTEGER                           { $$ = new cdk::integer_node(LINE, $1); }
     | tFLOAT                             { $$ = new cdk::double_node(LINE, $1);}
     | concatenacao                       { $$ = new cdk::string_node(LINE, $1);}
     | tMINUS expr %prec tUNARY           { $$ = new cdk::neg_node(LINE, $2); }
     | tPLUS expr %prec tUNARY            { $$ = new gr8::identity_node(LINE, $2);}
     | lval tENDERECO %prec tUNARY        { $$ = new gr8::pos_indication_node(LINE, $1);}
     | expr tTIMES expr                   { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr tOVER expr                    { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr tMODULUS expr                 { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr tPLUS expr                    { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr tMINUS expr                   { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr tBELOW expr                   { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr tABOVE expr                   { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tEQUALS expr                  { $$ = new cdk::eq_node(LINE, $1, $3); }
     | tNOT expr                          { $$ = new cdk::not_node(LINE, $2);}
     | expr tAND expr                     { $$ = new cdk::and_node(LINE, $1, $3);}
     | expr tOR expr                      { $$ = new cdk::or_node(LINE, $1, $3);}
     | '(' expr ')'                       { $$ = $2; }
     | lval                               { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
     | lval '=' expr                      { $$ = new cdk::assignment_node(LINE, $1, $3); }
     | tINPUT                             { $$ = new gr8::read_node(LINE);}
     | expr tOBJECTS                      { $$ = new gr8::objects_node(LINE,$1);}
     | tUSE expr2 tFOR tIDENTIFIER        { $$ = new gr8::func_call_node(LINE,$4,$2);}
     | tDO tIDENTIFIER                    { $$ = new gr8::func_call_node(LINE,$2,nullptr);}
     | tNULL                              { $$ = new gr8::null_node(LINE);}
     ;

concatenacao : tSTRING                    {$$ = $1;}
             | concatenacao tSTRING       {$$ = new std::string(*$1 + *$2);}
             ;

lval : tIDENTIFIER %prec tUNARY           { $$ = new cdk::identifier_node(LINE, $1);}
     | tCELL expr tAT expr %prec tUNARY   { $$ = new gr8::index_node(LINE, $2, $4);}
     ;
%%
