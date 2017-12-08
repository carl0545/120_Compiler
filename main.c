#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "symtable.h"
#include "type.h"

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

    yyparse();
    treeprint(root, 0);

    fprintf(stdout, "Populating Symbol Table...\n");
    rootCpy = root;
    initGlobal();
    populateSymbolTable(rootCpy);




    fprintf(stdout, "Performing Type Checking...\n");
    init_type();
    type_check(rootCpy);

    //fprintf(stdout, "Setting the size\n");
    //ht_set_size(global);



  }


return 0;
}
