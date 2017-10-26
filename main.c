#include <stdio.h>
#include <stdlib.h>
//#include "ytab.h"
#include "tree.h"
#include "symtable.h"

extern int yyparse();
extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
extern token *t;
extern struct tree *root;
extern struct hashtable_s *curr;
extern struct hashtable_s *global;

//extern char *finame;

int main(int argc, char *argv[]) {
  struct tree *rootCpy;

  if(argc < 2){
		printf("no file given.\n");
		exit(1);
	}

  for(int i = 1; i < argc; i++){

    FILE *fp = fopen(argv[i], "r");
    yyin = fp;

    fprintf(stdout, "Opening: %s\n", argv[i]);

    int ntoken;


    //ntoken = yylex();
    //printf("test2");
    ntoken = yyparse();
    treeprint(root, 0);

    rootCpy = root;
    //initGlobal();
    //populateSymbolTable(rootCpy);





  }


return 0;
}
