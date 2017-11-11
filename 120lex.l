/*	$Id: lexer.l,v 1.2 1997/11/19 15:13:15 sandro Exp $	*/

/*
 * Copyright (c) 1997 Sandro Sigala <ssigala@globalnet.it>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * ISO C++ lexical analyzer.
 *
 * Based on the ISO C++ draft standard of December '96.
 */

%{
#define STACKSIZE 2

#include <ctype.h>
#include <stdio.h>
//#include "token.h"
//#include "ytab.h"
#include "120gram.h"
//#include "parser.h"
#include "tree.h"
#include "stack.h"



int lineno = 1;
int saw_iostream, saw_string, saw_std;


static int yywrap(void);
static int createtoken(int);
static void skip_until_eol(void);
static void skip_comment(void);
static int check_identifier(char *);
void handle_include();
int typenametable_insert(char *s, int cat);
int typenametable_lookup(char *s);
StructState pop_stack(Stack*);
void push_stack(Stack*,int,char*);
void init_stack(Stack*);

token *t;
char *filename = "test";

Stack includeStack;
int initStackFlag = 0;



%}

intsuffix				([uU][lL]?)|([lL][uU]?)
fracconst				([0-9]*\.[0-9]+)|([0-9]+\.)
exppart					[eE][-+]?[0-9]+
floatsuffix				[fFlL]
chartext				([^'])|(\\.)
stringtext				([^"])|(\\.)

%%

"\n"					{ ++lineno; }
[\t\f\v\r ]+				{ /* Ignore whitespace. */ }

"/*"					{ skip_comment(); }
"//"					{ skip_until_eol(); }

"{"					{ return createtoken(LC); }
"<%"					{ return createtoken(LC); }
"}"					{ return createtoken(RC); }
"%>"					{ return createtoken(RC); }
"["					{ return createtoken(LB); }
"<:"					{ return createtoken(LB); }
"]"					{ return createtoken(RB); }
":>"					{ return createtoken(RB); }
"("					{ return createtoken(LP); }
")"					{ return createtoken(RP); }
";"					{ return createtoken(SM); }
":"					{ return createtoken(COLON); }
"..."					{ return createtoken(ELLIPSIS); }
"?"					{ return createtoken(QUEST); }
"::"					{ return createtoken(COLONCOLON); }
"."					{ return createtoken(DOT); }
".*"					{ return createtoken(DOTSTAR); }
"+"					{ return createtoken(PLUS); }
"-"					{ return createtoken(MINUS); }
"*"					{ return createtoken(MUL); }
"/"					{ return createtoken(DIV); }
"%"					{ return createtoken(MOD); }
"^"					{ return createtoken(ER); }
"xor"					{ return createtoken(ER); }
"&"					{ return createtoken(AND); }
"bitand"				{ return createtoken(AND); }
"|"					{ return createtoken(OR); }
"bitor"					{ return createtoken(OR); }
"~"					{ return createtoken(NOT); }
"compl"					{ return createtoken(NOT); }
"!"					{ return createtoken(BANG); }
"not"					{ return createtoken(BANG); }
"="					{ return createtoken(ASN); }
"<"					{ return createtoken(LT); }
">"					{ return createtoken(GT); }
"+="					{ return createtoken(PLASN); }
"-="					{ return createtoken(MIASN); }
"*="					{ return createtoken(MUASN); }
"/="					{ return createtoken(DIASN); }
"%="					{ return createtoken(MOASN); }
"^="					{ return createtoken(ERASN); }
"xor_eq"				{ return createtoken(ERASN); }
"&="					{ return createtoken(ANASN); }
"and_eq"				{ return createtoken(ANASN); }
"|="					{ return createtoken(ORASN); }
"or_eq"					{ return createtoken(ORASN); }
"<<"					{ return createtoken(SHL); }
">>"					{ return createtoken(SHR); }
"<<="					{ return createtoken(SLASN); }
">>="					{ return createtoken(SRASN); }
"=="					{ return createtoken(EQ); }
"!="					{ return createtoken(NE); }
"not_eq"				{ return createtoken(NE); }
"<="					{ return createtoken(LE); }
">="					{ return createtoken(GE); }
"&&"					{ return createtoken(ANDAND); }
"and"					{ return createtoken(ANDAND); }
"||"					{ return createtoken(OROR); }
"or"					{ return createtoken(OROR); }
"++"					{ return createtoken(INCOP); }
"--"					{ return createtoken(DECOP); }
","					{ return createtoken(CM); }
"->*"					{ return createtoken(ARROWSTAR); }
"->"					{ return createtoken(FOLLOW); }

"asm"					{ return createtoken(ASM); }
"auto"					{ return createtoken(AUTO); }
"bool"					{ return createtoken(BOOL); }
"break"					{ return createtoken(BREAK); }
"case"					{ return createtoken(CASE); }
"catch"					{ return createtoken(CATCH); }
"char"					{ return createtoken(CHAR); }
"class"					{ return createtoken(CLASS); }
"const"					{ return createtoken(CONST); }
"const_cast"				{ return createtoken(CONST_CAST); }
"continue"				{ return createtoken(CONTINUE); }
"default"				{ return createtoken(DEFAULT); }
"delete"				{ return createtoken(DELETE); }
"do"					{ return createtoken(DO); }
"double"				{ return createtoken(DOUBLE); }
"dynamic_cast"				{ return createtoken(DYNAMIC_CAST); }
"else"					{ return createtoken(ELSE); }
"enum"					{ return createtoken(ENUM); }
"explicit"				{ return createtoken(EXPLICIT); }
"export"				{ return createtoken(EXPORT); }
"extern"				{ return createtoken(EXTERN); }
"false"					{ return createtoken(FALSE); }
"float"					{ return createtoken(FLOAT); }
"for"					{ return createtoken(FOR); }
"friend"				{ return createtoken(FRIEND); }
"goto"					{ return createtoken(GOTO); }
"if"					{ return createtoken(IF); }
"inline"				{ return createtoken(INLINE); }
"int"					{ return createtoken(INT); }
"long"					{ return createtoken(LONG); }
"mutable"				{ return createtoken(MUTABLE); }
"namespace"				{ return createtoken(NAMESPACE); }
"new"					{ return createtoken(NEW); }
"operator"				{ return createtoken(OPERATOR); }
"private"				{ return createtoken(PRIVATE); }
"protected"				{ return createtoken(PROTECTED); }
"public"				{ return createtoken(PUBLIC); }
"register"				{ return createtoken(REGISTER); }
"reinterpret_cast"			{ return createtoken(REINTERPRET_CAST); }
"return"				{ return createtoken(RETURN); }
"short"					{ return createtoken(SHORT); }
"signed"				{ return createtoken(SIGNED); }
"sizeof"				{ return createtoken(SIZEOF); }
"static"				{ return createtoken(STATIC); }
"static_cast"				{ return createtoken(STATIC_CAST); }
"struct"				{ return createtoken(STRUCT); }
"switch"				{ return createtoken(SWITCH); }
"template"				{ return createtoken(TEMPLATE); }
"this"					{ return createtoken(THIS); }
"throw"					{ return createtoken(THROW); }
"true"					{ return createtoken(TRUE); }
"try"					{ return createtoken(TRY); }
"typedef"				{ fprintf(stderr, "TYPEDEF not supported"); exit(1);/*return createtoken(TYPEDEF);*/ }
"typeid"				{ return createtoken(TYPEID); }
"typename"				{ return createtoken(TYPENAME); }
"union"					{ return createtoken(UNION); }
"unsigned"				{ return createtoken(UNSIGNED); }
"using"					{ return createtoken(USING); }
"virtual"				{ return createtoken(VIRTUAL); }
"void"					{ return createtoken(VOID); }
"volatile"				{ return createtoken(VOLATILE); }
"wchar_t"				{ return createtoken(WCHAR_T); }
"while"					{ return createtoken(WHILE); }

[a-zA-Z_][a-zA-Z_0-9]*			{ return createtoken(check_identifier(yytext)); }

"0"[xX][0-9a-fA-F]+{intsuffix}?		{ return createtoken(ICON); }
"0"[0-7]+{intsuffix}?			{ return createtoken(ICON); }
[0-9]+{intsuffix}?			{ return createtoken(ICON); }

{fracconst}{exppart}?{floatsuffix}?	{ return createtoken(FCON); }
[0-9]+{exppart}{floatsuffix}?		{ return createtoken(FCON); }

"'"{chartext}*"'"			{ return createtoken(CCON); }
"L'"{chartext}*"'"			{ return createtoken(CCON); }

"\""{stringtext}*"\""			{ return createtoken(STRING); }
"L\""{stringtext}*"\""			{ return createtoken(STRING); }
"#include"[ ]*\"[^\\"]+\"  { init_stack(&includeStack); handle_include(); }
"#include <iostream>"			{ saw_iostream = 1; }
"#include <string>"			{ saw_string = 1; }
"using namespace std;"			{ saw_std = 1;
		  if (saw_string) typenametable_insert("string", CLASS_NAME);
		  if (saw_iostream) {
		  typenametable_insert("ifstream", CLASS_NAME);
			typenametable_insert("ofstream", CLASS_NAME);
			}
		}


.					{ fprintf(stderr, "%d: unexpected character `%c'\n", lineno, yytext[0]); exit(1); }

%%

static int
yywrap(void)
{
  if(includeStack.top <= 0){
	 return 1;
  }

  else{
  fclose(yyin);
  // Grab the previous input file from the state stack.
  yypop_buffer_state();
  StructState state = pop_stack(&includeStack);
  filename = state.name;
  lineno = state.line_num;
  return 0;

  }


}

/*
 * We use this routine instead a lex pattern because we don't need
 * to save the matched comment in the `yytext' buffer.
 */
static void
skip_comment(void)
{
	int c1, c2;

	c1 = input();
	c2 = input();

	while (c2 != EOF && !(c1 == '*' && c2 == '/')) {
		if (c1 == '\n')
			++lineno;
		c1 = c2;
		c2 = input();
	}
}

/*
 * See the previous comment for an explanation.
 */
static void
skip_until_eol(void)
{
	int c;

	while ((c = input()) != EOF && c != '\n')
		;
	++lineno;
}

/*
 * Type name table.
 * Replace this with a hash table, when you get a chance.
 */


struct typenametable_entry {
   char *name;
   int category;
   struct typenametable_entry *next;
   } *head;

int typenametable_lookup(char *s)
{
   struct typenametable_entry *tmp = head;
   while (tmp) {
      if (!strcmp(s, tmp->name)) return tmp->category;
      tmp = tmp->next;
   }
   return IDENTIFIER;
}

int typenametable_insert(char *s, int cat)
{
   struct typenametable_entry *tmp = head;
   while (tmp) {
      if (!strcmp(s, tmp->name)) {
         fprintf(stderr,
	    "warning, %s is already in the typename table as a %d\n",
	    s, tmp->category );
         return tmp->category;
         }
      tmp = tmp->next;
   }
   tmp = calloc(1, sizeof (struct typenametable_entry));
   if(!tmp) { fprintf(stderr, "tnti out of memory\n"); return -1; }
   tmp->name = strdup(s);
   tmp->category = cat;
   tmp->next = head;
   head = tmp;
   return 0;//check later, get rid of warning
}

static int
check_identifier(char *s)
{
	return typenametable_lookup(s);
}

static int createtoken(int cat)
{

  //free(yylval.node);

  yylval.node = (struct tree *) calloc(1, sizeof(struct tree));
  yylval.node->leaf = (token *) calloc(1, sizeof(token));



  yylval.node->leaf->category = cat;
  yylval.node->prodrule = cat;

  yylval.node->leaf->text = (char *) calloc(1, strlen(yytext)+1);
  strcpy(yylval.node->leaf->text, yytext);

  yylval.node->leaf->lineno = yylineno;

  //ival, dval, sval functionality
  if(cat == ICON){
    yylval.node->leaf->ival = atoi(yytext);
  }
  else if(cat == FCON){
    yylval.node->leaf->dval = atof(yytext);
  }
  else if(cat == STRING){
    yylval.node->leaf->sval = yytext;

  }

  yylval.node->leafCheck = 1;
  yylval.node->nkids = 0;
  //printf("NOOOOO2222: %d %d %s ", yylval.node->leaf->category, cat, yylval.node->leaf->text);



   return cat;
}

void handle_include()
{
    char *fname = strchr(yytext, '\"')+1;
    fname[strlen(fname)-1] = '\0';
    fname = strdup(fname);

    FILE* input_file = fopen(fname, "r");

    if(!input_file) {
        fprintf(stderr, "Unable to open include file %s: ", fname);
        perror("");
        exit(1);
    }
    // Push flex's internal buffer state.
    yypush_buffer_state(yy_create_buffer(input_file, YY_BUF_SIZE));
    // Push "auxiliary" file data.
    push_stack(&includeStack, lineno, "test2");
    filename = fname;
    lineno = 1;


}

StructState pop_stack(Stack *st){
  StructState ret;
  if(st->top == 0){
    printf("ERROR: Stack is empty");
    exit(1);
  }
  else{
    ret = st->stk[st->top];
    st->top--;
  }
  return ret;

}

void push_stack(Stack *st,int line,char* name){
  if(st->top < STACKSIZE){
    st->stk[st->top].name = name;
    st->stk[st->top].line_num = line;
    st->top++;

  }
  else{
  printf("ERROR: Include Stack is full!\n");
  }

}

void init_stack(Stack *st){
  if(initStackFlag == 0){
    st->top = 0;
    initStackFlag = 1;
  }
  else{
    return;
  }

}
