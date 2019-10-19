%option c++ prefix="gr8_scanner_" outfile="gr8_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <stack>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include <iostream>
#include "gr8_parser.tab.h"

std::stack<int> blocks;

// don't change this
#define yyerror LexerError
%}
%x X_STRING	X_COMMENT X_IGNORE X_SPECIAL
%%

"plus"				   return tPLUS; 
"minus"				   return tMINUS; 
"?"					   return tENDERECO;
"objects"			   return tOBJECTS; 

"times"				   return tTIMES; 
"over"				   return tOVER;
"modulus"			   return tMODULUS;

"below"				   return tBELOW; 
"above"				   return tABOVE; 

"equals"			   return tEQUALS;  

"not"				   return tNOT; 
"and"				   return tAND;
"or"				   return tOR;
	
"sweeping"             return tSWEEPING;  
"if"                   return tIF;  
"else"                 return tELSE;
"elsif"				   return tELSIF;  
"post"                 return tPOST; 
"tweet"                return tTWEET; 
"return"			   return tRETURN;
"stop"				   return tSTOP; 
"again"				   return tAGAIN; 
"small"				   return tSMALL; 
"huge"				   return tHUGE; 
"news"				   return tNEWS; 
"fake"				   return tFAKE; 
"initially"			   return tINITIALLY;
"use"				   return tUSE; 
"public" 			   return tPUBLIC;
"define"			   return tDEFINE; 
"uses"				   return tUSES; 
"by"				   return tBY; 
"to"				   return tTO; 
"procedure"			   return tPROCEDURE;
"then"				   return tTHEN; 
"do"				   return tDO; 
"assign"			   return tASSIGN; 
"from"				   return tFROM;  
"function"			   return tFUNCTION; 
"on"				   return tON;
"cell"				   return tCELL;
"at"			       return tAT;
"null"				   return tNULL;
"as"				   return tAS;
"for"				   return tFOR;
"input"				   return tINPUT;
")"					   return *yytext;
"("					   return *yytext;
","					   return *yytext;

[A-Za-z][A-Za-z0-9_]*  {yylval.s = new std::string(yytext); return tIDENTIFIER;}	

\"                        {yy_push_state(X_STRING); yylval.s = new std::string("");}
<X_STRING>\"              {yy_pop_state();return tSTRING;}
<X_STRING>\\\"            *yylval.s += yytext + 1;
<X_STRING>"~0[A-Za-z ]*$" { yy_push_state(X_IGNORE); }
<X_STRING>"~"             { yy_push_state(X_SPECIAL); }            
<X_STRING>.               { *yylval.s += yytext; }
<X_STRING>\n              yyerror("newline in string");

<X_SPECIAL>t           { *yylval.s += "\t"; yy_pop_state();}
<X_SPECIAL>n           { *yylval.s += "\n"; yy_pop_state();}
<X_SPECIAL>r           { *yylval.s += "\r"; yy_pop_state();}
<X_SPECIAL>[0-6]{1,3}  { *yylval.s += (char) strtol(yytext, nullptr, 7); yy_pop_state(); }
<X_SPECIAL>\"		   { yyless(0);yy_pop_state();}
<X_SPECIAL>.		   { yy_pop_state();}

<X_IGNORE>\"		   {yy_pop_state(); yyless(0);}
<X_IGNORE>.			   ;			   		   

0[0-6]*                {yylval.i = strtol(yytext, nullptr, 7); return tINTEGER;}
[1-9][0-9]*            {yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;}

[0-9]+"."[0-9]+|"."[0-9]+  { yylval.f = strtof(yytext, nullptr); return tFLOAT; }

[1-9]+"E"[1-9]+		  				 { yylval.f = strtof(yytext, nullptr); return tFLOAT; }
[0-9]*"."[0-9]+"E"[1-9][0-9]*	     { yylval.f = strtof(yytext, nullptr); return tFLOAT; }
[0-9]+"."[0-9]+"e-"[0-9]+  			 { yylval.f = strtof(yytext, nullptr); return tFLOAT; }
"."[0-9]"+e-"[0-9]+					 { yylval.f = strtof(yytext, nullptr); return tFLOAT; }
[0-9]+"."[0-9]+"e+"[0-9]+  			 { yylval.f = strtof(yytext, nullptr); return tFLOAT; }
"."[0-9]+"e+"[0-9]+ 				 { yylval.f = strtof(yytext, nullptr); return tFLOAT; }

"!!".*$         	   ;

"<<"				   yy_push_state(X_COMMENT);
<X_COMMENT>"<<"		   yy_push_state(X_COMMENT);
<X_COMMENT>">>"		   yy_pop_state();
<X_COMMENT>.|\n 	   ;


"...\n"				   ;

"\n"" "*|"\n""\t"*   	{
						if(blocks.size() == 0){blocks.push(0);}
						if((yyleng-1) > blocks.top()){
							blocks.push(yyleng-1);
							return tBLOCKBEGIN;
						}

						 if((yyleng-1) < blocks.top()){
							blocks.pop();
							if((yyleng-1) > blocks.top())
								yyerror("wrong identation");
							else{
								yyless(0);
								return tBLOCKEND;
							}
						}}

<<EOF>>					 		{if(blocks.size() == 0)
									return 0;

								if(blocks.top() > 0){
									blocks.pop();

									return tBLOCKEND;
								} else{
									return 0;
								}}
" "*|"\t"*					;
.                      {std::cout<< yytext <<std::endl;yyerror("Unknown character");}
%%
