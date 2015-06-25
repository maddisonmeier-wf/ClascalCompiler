/*
* Maddison Meier and Derrick Rahbar Compilers fall 2014
*
*
*/


/*
 * main.c
 *
 * Implements an object oriented pascal compiler
 */



extern "C"
{
    
  #include "shared.h"
  #include "symtab.h"
  #include "rulefuncs.h"
   extern struct program_t *program;

  extern void yyparse();
}

#include "code_gen.h"

/* Flags if any errors occured */
int error_flag = 0;

/* Holds the command-line arguments */
struct args_t cmdArgs;

/* This is the data structure we are left with (the parse tree) after
   yyparse() is done . */



/* ----------------------------------------------------------------------- 
 * Prints a usage banner
 * ----------------------------------------------------------------------- 
 */
void print_usage()
{
  printf("%s: Usage: %s [-v] [-exitsem] [-O] [-ir] [-free] [-fcf] [-fvn] [-fgre] [--help]\n",
    PROGRAMNAME, PROGRAMNAME);
}


/* ----------------------------------------------------------------------- 
 * parses the command-line arguments to main
 * ----------------------------------------------------------------------- 
 */
void parse_command_line_arguments(int argc, char **argv, struct args_t *args)
{
  int i = 1;

  if (argc < 1) {
    print_usage();
    exit(1);
  }

  memset(args, 0, sizeof(struct args_t));

  while (i < argc) {
    if (strcmp(argv[i], "-v") == 0 ) {
      args->verbose = 1;
    } else if (strcmp(argv[i], "--help") == 0 ||
         strcmp(argv[i], "-h") == 0 ) { 
      print_usage();
      exit(3);
    } else if (strcmp(argv[i], "-exitsem") == 0) {
      args->exit_after_sem = 1;
    } else if (strcmp(argv[i], "-ir") == 0) {
      args->printIr = 1;
    } else if (strcmp(argv[i], "-lir") == 0) {
      args->printLir = 1;
    } else if (strcmp(argv[i], "-fcf") == 0) {
      args->optimize_cf = 1;
    } else if (strcmp(argv[i], "-free") == 0) {
      args->optimize_ree = 1;
    } else if (strcmp(argv[i], "-fvn") == 0) {
      args->optimize_vn = 1;
    } else if (strcmp(argv[i], "-fgre") == 0) {
      args->optimize_gre = 1;
    } else if (strcmp(argv[i], "-O") == 0) {
      args->optimize_cf = 1;
      args->optimize_ree = 1;
      args->optimize_vn = 1;
      args->optimize_gre = 1;
    } else {
      fprintf(stderr, "Invalid argument '%s'\n", argv[i]);
      print_usage();
      exit(1);
    }
    i++;
  }

}



/* -----------------------------------------------------------------------
 * main function, where all hell breaks loose
 * ----------------------------------------------------------------------- 
 */
int main(int argc, char **argv)
{
  parse_command_line_arguments(argc, argv, &cmdArgs);

  usrdef_init();
  symtab_init();
  /* begin parsing */
  yyparse();
  
  /* If there were parsing errors, exit. */
  exit_on_errors();

  //print_class_table(program->cl->class_node_list);
  code_generation(program);
  /* If there were errors during semantic analysis, exit. */
  exit_on_errors();

  /* If we should only perform semantic analysis, exit */
  if (cmdArgs.exit_after_sem == 1) {
    exit(0);
  }

  if (cmdArgs.verbose == 1) {
    /* print the user defined data types */
    printf("USER DEFINED DATA TYPES:\n");
    printf("------------------------\n");
    usrdef_print();
    
    /* print the symbol table*/
    printf("\n\n");
    printf("SYMBOL TABLE:\n");
    printf("-------------\n");
    symtab_print(0);
  }

  /* Simple, wasn't it ?!! */

  return 0;
}
