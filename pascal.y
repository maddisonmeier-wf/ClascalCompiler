%{
/*
 * grammar.y
 *
 * Pascal grammar in Yacc format, based originally on BNF given
 * in "Standard Pascal -- User Reference Manual", by Doug Cooper.
 * This in turn is the BNF given by the ANSI and ISO Pascal standards,
 * and so, is PUBLIC DOMAIN. The grammar is for ISO Level 0 Pascal.
 * The grammar has been massaged somewhat to make it LALR.
 */

/*
* Maddison Meier and Derrick Rahbar Compilers fall 2014
*
*
*/


#include "shared.h"
#include "rulefuncs.h"

  int yylex(void);
  void yyerror(const char *error);

  extern char *yytext;          /* yacc text variable */
  extern int line_number;       /* Holds the current line number; specified
				   in the lexer */
  struct program_t *program;    /* points to our program */
%}

%token AND ARRAY ASSIGNMENT CLASS COLON COMMA DIGSEQ
%token DO DOT DOTDOT ELSE END EQUAL EXTENDS FUNCTION
%token GE GT IDENTIFIER IF LBRAC LE LPAREN LT MINUS MOD NEW NOT
%token NOTEQUAL OF OR PBEGIN PLUS PRINT PROGRAM RBRAC
%token RPAREN SEMICOLON SLASH STAR THEN
%token VAR WHILE

%type <tden> type_denoter
%type <id> result_type
%type <id> identifier
%type <idl> identifier_list
%type <fdes> function_designator
%type <apl> actual_parameter_list
%type <apl> params
%type <ap> actual_parameter
%type <vd> variable_declaration
%type <vdl> variable_declaration_list
%type <r> range
%type <un> unsigned_integer
%type <fpsl> formal_parameter_section_list
%type <fps> formal_parameter_section
%type <fps> value_parameter_specification
%type <fps> variable_parameter_specification
%type <va> variable_access
%type <as> assignment_statement
%type <os> object_instantiation
%type <ps> print_statement
%type <e> expression
%type <s> statement
%type <ss> compound_statement
%type <ss> statement_sequence
%type <ss> statement_part
%type <is> if_statement
%type <ws> while_statement
%type <e> boolean_expression
%type <iv> indexed_variable
%type <ad> attribute_designator
%type <md> method_designator
%type <iel> index_expression_list
%type <e> index_expression
%type <se> simple_expression
%type <t> term
%type <f> factor
%type <op> sign
%type <p> primary
%type <un> unsigned_constant
%type <un> unsigned_number
%type <at> array_type
%type <cb> class_block
%type <vdl> variable_declaration_part
%type <fdl> func_declaration_list
%type <funcd> function_declaration
%type <fb> function_block
%type <fh> function_heading
%type <id> function_identification
%type <fpsl> formal_parameter_list
%type <cl> class_list
%type <ci> class_identification
%type <program> program
%type <ph> program_heading
%type <op> relop
%type <op> addop
%type <op> mulop

%union {
  struct type_denoter_t *tden;
  char *id;
  struct identifier_list_t *idl;
  struct function_designator_t *fdes;
  struct actual_parameter_list_t *apl;
  struct actual_parameter_t *ap;
  struct variable_declaration_list_t *vdl;
  struct variable_declaration_t *vd;
  struct range_t *r;
  struct unsigned_number_t *un;
  struct formal_parameter_section_list_t *fpsl;
  struct formal_parameter_section_t *fps;
  struct variable_access_t *va;
  struct assignment_statement_t *as;
  struct object_instantiation_t *os;
  struct print_statement_t *ps;
  struct expression_t *e;
  struct statement_t *s;
  struct statement_sequence_t *ss;
  struct if_statement_t *is;
  struct while_statement_t *ws;
  struct indexed_variable_t *iv;
  struct attribute_designator_t *ad;
  struct method_designator_t *md;
  struct index_expression_list_t *iel;
  struct simple_expression_t *se;
  struct term_t *t;
  struct factor_t *f;
  int *i;
  struct primary_t *p;
  struct array_type_t *at;
  struct class_block_t *cb;
  struct func_declaration_list_t *fdl;
  struct function_declaration_t *funcd;
  struct function_block_t *fb;
  struct function_heading_t *fh;
  struct class_identification_t *ci;
  struct class_list_t *cl;
  struct program_t *program;
  struct program_heading_t *ph;
  int op;
}

%%

program : program_heading semicolon class_list DOT
	{
        program = set_program($1, $3);
        return 1; 
	}
 ;

program_heading : PROGRAM identifier
	{
        $$ = set_program_heading($2, NULL);
	}
 | PROGRAM identifier LPAREN identifier_list RPAREN
	{
        $$ = set_program_heading($2, $4);
	}
 ;

identifier_list : identifier_list comma identifier
        {
            check_id_duplicate($1, $3, line_number);

            $$ = set_identifier_list($3, $1);
        }
 | identifier
        {
            $$ = set_identifier_list($1, NULL);
        }
 ;

class_list: class_list class_identification PBEGIN class_block END
	{
        check_for_extend($1, $2);
        $$ = set_class_list($2, $4, $1);
	}
 | class_identification PBEGIN class_block END
	{
        $$ = set_class_list($1, $3, NULL);
	}
 ;

class_identification : CLASS identifier
	{
        $$ = set_class_identification($2, NULL, line_number);
	}
| CLASS identifier EXTENDS identifier
	{
        $$ = set_class_identification($2, $4, line_number);
	}
;

class_block:
 variable_declaration_part
 func_declaration_list
	{
        $$ = set_class_block($1, $2);
	}
 ;

type_denoter : array_type
	{       
        $$ = set_type_denoter_array("array", $1);
	}
 | identifier
	{
        if(is_prim_data_type($1) == 1)
        {$$ = set_type_denoter_id($1, $1);}
        else
        {$$ = set_type_denoter_class($1, NULL);}
        
	}
 ;

array_type : ARRAY LBRAC range RBRAC OF type_denoter
	{
        $$ = set_array_type($3, $6);
	}
 ;

range : unsigned_integer DOTDOT unsigned_integer
	{
        $$ = set_range($1, $3, line_number);
	}
 ;

variable_declaration_part : VAR variable_declaration_list semicolon
	{
        $$ = $2;
	}
 |
	{
        $$ = NULL;
	}
 ;

variable_declaration_list : variable_declaration_list semicolon variable_declaration
	{
        $$ = set_variable_declaration_list($3, $1);
	}
 | variable_declaration
	{
        $$ = set_variable_declaration_list($1, NULL);
	}

 ;

variable_declaration : identifier_list COLON type_denoter
	{
        $$ = set_variable_declaration($1, $3, line_number);
	}
 ;

func_declaration_list : func_declaration_list semicolon function_declaration
	{
        $$ = set_func_declaration_list($3, $1);
	}
 | function_declaration
	{
        $$ = set_func_declaration_list($1, NULL);
	}
 |
	{
        $$ = NULL;
	}
 ;

formal_parameter_list : LPAREN formal_parameter_section_list RPAREN 
	{
        $$ = $2;
	}
;
formal_parameter_section_list : formal_parameter_section_list semicolon formal_parameter_section
	{
        $$ = set_formal_parameter_section_list($3, $1);
	}
 | formal_parameter_section
	{
        $$ = set_formal_parameter_section_list($1, NULL);
	}
 ;

formal_parameter_section : value_parameter_specification
 | variable_parameter_specification
 ;

value_parameter_specification : identifier_list COLON identifier
	{
        $$ = set_formal_parameter_section($1, $3, 0);
	}
 ;

variable_parameter_specification : VAR identifier_list COLON identifier
	{
        $$ = set_formal_parameter_section($2, $4, 1);
	}
 ;

function_declaration : function_identification semicolon function_block
	{
        $$ = set_function_declaration(set_function_heading($1,"_VOID", NULL), $3, line_number);
	}
 | function_heading semicolon function_block
	{
        $$ = set_function_declaration($1, $3, line_number);
	}
 ;

function_heading : FUNCTION identifier COLON result_type
	{
        $$ = set_function_heading($2, $4, NULL);
	}
 | FUNCTION identifier formal_parameter_list COLON result_type
	{
        $$ = set_function_heading($2, $5, $3);
	}
 ;

result_type : identifier ;

function_identification : FUNCTION identifier
	{
        $$ = $2;
	}
;

function_block : 
  variable_declaration_part
  statement_part
	{
        $$ = set_function_block($1, $2);
	}
;

statement_part : compound_statement
 ;

compound_statement : PBEGIN statement_sequence END
	{
        $$ = $2;
	}
 ;

statement_sequence : statement
	{
        $$ = set_statement_sequence($1, NULL, line_number);
	}
 | statement_sequence semicolon statement
	{
        $$ = set_statement_sequence($3, $1, line_number);
	}
 ;

statement : assignment_statement
	{
        $$ = set_statement_assignment($1, line_number);
	}
 | compound_statement
	{
        $$ = set_statement_statement_sequence($1, line_number);
	}
 | if_statement
	{
        $$ = set_statement_if($1, line_number);
	}
 | while_statement
	{
        $$ = set_statement_while($1, line_number);
	}
 | print_statement
        {
            $$ = set_statement_print($1, line_number);
        }
 ;

while_statement : WHILE boolean_expression DO statement
	{
        $$ = set_while_statement($2, $4);
	}
 ;

if_statement : IF boolean_expression THEN statement ELSE statement
	{
        $$ = set_if_statement($2, $4, $6);
	}
 ;

assignment_statement : variable_access ASSIGNMENT expression
	{
        $$ = set_assignment_statement($1, $3, NULL);
	}
 | variable_access ASSIGNMENT object_instantiation
	{
        $$ = set_assignment_statement($1, NULL, $3);
	}
 ;

object_instantiation: NEW identifier
	{
        $$ = set_object_instantiation($2, NULL);
	}
 | NEW identifier params
	{
        $$ = set_object_instantiation($2, $3);
	}
;

print_statement : PRINT variable_access
        {
            $$ = set_print_statement($2);
        }
;

variable_access : identifier
	{
        $$ = set_variable_access_id($1, NULL, NULL);
	}
 | indexed_variable
	{
        $$ = set_variable_access_indexed_variable($1, NULL, NULL);
	}
 | attribute_designator
	{
        $$ = set_variable_access_attribute_designator($1);
	}
 | method_designator
	{
        $$ = set_variable_access_method_designator($1, NULL, NULL);
	}
 ;

indexed_variable : variable_access LBRAC index_expression_list RBRAC
	{
        $$ = set_indexed_variable($1, $3, line_number);
	}
 ;

index_expression_list : index_expression_list comma index_expression
	{
		/* We are assuming this will never run because we are not allowing expression lists greater than one expression */
        $$ = set_index_expression_list($3, $1, $3->expr);
	}
 | index_expression
	{
        $$ = set_index_expression_list($1, NULL, $1->expr);
	}
 ;

index_expression : expression ;

attribute_designator : variable_access DOT identifier
	{
        $$ = set_attribute_designator($1, $3, line_number);
	}
;

method_designator: variable_access DOT function_designator
	{
        $$ = set_method_designator($1, $3, line_number);
	}
 ;


params : LPAREN actual_parameter_list RPAREN 
	{
        $$ = $2;
	}
 ;

actual_parameter_list : actual_parameter_list comma actual_parameter
	{
        $$ = set_actual_parameter_list($3, $1);
	}
 | actual_parameter 
	{
        $$ = set_actual_parameter_list($1, NULL);
	}
 ;

actual_parameter : expression
	{
		/* We assume only this case should execute because a param list should only have an expression */
        $$ = set_actual_parameter($1, NULL, NULL);
	}
 | expression COLON expression
	{
        $$ = set_actual_parameter($1, $3, NULL);
	}
 | expression COLON expression COLON expression
	{
        $$ = set_actual_parameter($1, $3, $5);
	}
 ;

boolean_expression : expression ;

expression : simple_expression
	{
        $$ = set_expression($1, RELOP_NONE, NULL, line_number);
	}
 | simple_expression relop simple_expression
	{
        $$ = set_expression($1, $2, $3, line_number);
	}
 ;

simple_expression : term
	{
        $$ = set_simple_expression($1, ADDOP_NONE, NULL, line_number);
	}
 | simple_expression addop term
	{        
        $$ = set_simple_expression($3, $2, $1, line_number);
	}
 ;

term : factor
	{
        $$ = set_term($1, MULOP_NONE, NULL, line_number);
	}
 | term mulop factor
	{ 
        $$ = set_term($3, $2, $1, line_number);
	}
 ;

sign : PLUS
	{
        $$ = SIGN_PLUS;
	}
 | MINUS
	{
        $$ = SIGN_MINUS;
	}
 ;

factor : sign factor
	{
        $$ = set_factor_t_sign_factor($1, $2, line_number);
	}
 | primary 
	{
        $$ = set_factor_t_primary($1);
	}
 ;

primary : variable_access
	{
        $$ = set_primary_t_variable_access($1);
	}
 | unsigned_constant
	{
        $$ = set_primary_t_unsigned_constant($1);
	}
 | function_designator
	{
        $$ = set_primary_t_function_designator($1);
	}
 | LPAREN expression RPAREN
	{
        $$ = set_primary_t_expression($2);
	}
 | NOT primary
	{
        $$ = set_primary_t_primary($2, line_number);
	}
 ;

unsigned_constant : unsigned_number
 ;

unsigned_number : unsigned_integer ;

unsigned_integer : DIGSEQ
	{
        $$ = set_unsigned_number(atoi(yytext), set_expression_data(atoi(yytext), "VAR"));
	}
 ;

/* functions with no params will be handled by plain identifier */
function_designator : identifier params
	{
        $$ = set_function_designator($1, $2);
	}
 ;

addop: PLUS
	{
        $$ = ADDOP_PLUS;
	}
 | MINUS
	{
        $$ = ADDOP_MINUS;
	}
 | OR
	{
        $$ = ADDOP_OR;
	}
 ;

mulop : STAR
	{
        $$ = MULOP_STAR;
	}
 | SLASH
	{
        $$ = MULOP_SLASH;
	}
 | MOD
	{
        $$ = MULOP_MOD;
	}
 | AND
	{
        $$ = MULOP_AND;
	}
 ;

relop : EQUAL
	{
        $$ = RELOP_EQUAL;
	}
 | NOTEQUAL
	{
        $$ = RELOP_NOTEQUAL;
	}
 | LT
	{
        $$ = RELOP_LT;
	}
 | GT
	{
        $$ = RELOP_GT;
	}
 | LE
	{
        $$ = RELOP_LE;
	}
 | GE
	{
        $$ = RELOP_GE;
	}
 ;

identifier : IDENTIFIER
	{
        $$ = (char *)malloc(strlen(yytext)*sizeof(char)+1);
        strcpy($$,yytext);
	}
 ;

semicolon : SEMICOLON
 ;

comma : COMMA
 ;

%%
