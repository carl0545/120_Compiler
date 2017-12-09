#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "symtable.h"
#include "type.h"
#include "intermediate.h"


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

    char name[100];

    strcpy(name, argv[i]);


    for(int k = 0; k < strlen(name); k++){
      if(name[k] == '.'){
        name[k+1] = 'i';
        name[k+2] = 'c';
        name[k+3] = '\0';
        break;
      }

    }

    FILE *interF = fopen(name, "w");

    fprintf(interF, "\n");





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

    fprintf(stdout, "Setting the size\n");


    init_imd_globals();
    ht_set_size(global, &global_r_counter, &local_r_counter , &param_r_counter, GLOBAL_H);
    gen_first(rootCpy);
    gen_follows(rootCpy);
    gen_place(rootCpy);
    gen_temp(rootCpy);

    codeGen(rootCpy);

    print_main(rootCpy->code, interF);

    //treeprint(rootCpy, 0);

    printf("PRINTING SYMBOL TABLE SIZE\n\n\n");
    printSize(global);


    fclose(fp);
    fclose(interF);
  }


return 0;
}
