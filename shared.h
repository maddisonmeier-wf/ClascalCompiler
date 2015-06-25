/*
* Maddison Meier and Derrick Rahbar Compilers fall 2014
*
*
*/


/* shared.h
 *
 * Contains function definitions of all shared subroutines
 */

#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VA_SCOPE_MAIN 1

#define SCOPE_FV 0
#define SCOPE_NFV 1

#define ASSIGNMENT_STATEMENT 0
#define STATEMENT_SEQUENCE 1
#define IF_STATEMENT 2
#define WHILE_STATEMENT 3
#define PRINT_STATEMENT 4

#define K 1024
#define MAX_ERROR_SIZE 1 * K
#define MAX_NEW_CHAR_SIZE 255
#define MAX_USRDEF_HASHKEYS 8
#define MAX_SYMTAB_HASHKEYS 26
#define PROGRAMNAME "opc"

#define PRIMITIVE_TYPE_NAME_INTEGER "integer"
#define PRIMITIVE_TYPE_NAME_BOOLEAN "boolean"
#define PRIMITIVE_TYPE_NAME_POINTER "pointer"
#define PRIMITIVE_TYPE_NAME_UNKNOWN "unknown"

#define BOOLEAN_VALUE_TRUE "True"
#define BOOLEAN_VALUE_FALSE "False"

#define TRUE 1
#define FALSE 0

#define ENTITY_CLASS 0
#define ENTITY_FUNCTION 1
#define ENTITY_VARIABLE 2

#define MULOP_STAR 0
#define MULOP_SLASH 1
#define MULOP_MOD 2
#define MULOP_AND 3
#define MULOP_NONE -1

#define SIGN_PLUS 1
#define SIGN_MINUS -1 

#define ADDOP_PLUS 0
#define ADDOP_MINUS 1
#define ADDOP_OR 2
#define ADDOP_NONE -1 

#define RELOP_EQUAL 0
#define RELOP_NOTEQUAL 1
#define RELOP_LT 2
#define RELOP_GT 3
#define RELOP_LE 4
#define RELOP_GE 5
#define RELOP_NONE -1

#define EXPRESSION_DATA_INTEGER 1
#define EXPRESSION_DATA_BOOLEAN 2
#define EXPRESSION_DATA_REAL 3


/* Macro that checks for a malloc error */
#define CHECK_MEM_ERROR(name) {if (name == NULL) { \
				 printf("Memory allocation error\n"); \
				 exit(1); \
				} \
                              }

/* Macro that iterates to the end of a linked list whose next
 * element pointer is named "next"
 */
#define GOTO_END_OF_LIST(name) {while (name->next != NULL) \
                                  name = name->next; \
                               }

/* ================================================================
 * Datatype declarations
 * ================================================================
 */

extern int error_flag;

struct args_t {
  /** Print the user defined data types and symbol table */
  int verbose;  
  /** Exit after performing semantic analysis */
  int exit_after_sem;  
  /** Print the Intermediate Representation */
  int printIr;
  /** Print the Lower-level Intermediate Representation */
  int printLir;
  /** Apply constant folding */
  int optimize_cf;
  /** Apply redundant expression elimination */
  int optimize_ree;
  /** Apply value numbering */
  int optimize_vn;
  /** Apply global redundancy elimination */
  int optimize_gre;
};


struct identifier_list_t;
struct identifier_list_t {
  char *id; 
  struct identifier_list_t *next;
};

struct unsigned_number_t;
struct range_t{
  struct unsigned_number_t *min;
  struct unsigned_number_t *max;
};

struct array_type_t{
  struct range_t *r;
  struct type_denoter_t *td;
};

  union type_data_union {
    struct array_type_t *at;
    struct class_list_t *cl;
    char *id;
  };

#define TYPE_DENOTER_T_ARRAY_TYPE 1
#define TYPE_DENOTER_T_CLASS_TYPE 2
#define TYPE_DENOTER_T_IDENTIFIER 3
struct type_denoter_t{
  int type; /* 1 - array_type
	     * 2 - class_type
	     * 3 - base_type (integer, real, boolean)
	     */
  char *name;
  union type_data_union data;
};


/* ---------------------------------------------------------------- */



/* ----------------------------------------------------------------
 * Everything required for the variable_declaration_list
 * ----------------------------------------------------------------
 */
struct variable_declaration_t {
  struct identifier_list_t *il;
  struct type_denoter_t *tden;
  struct VarNode_c *var_list;
  int line_number;
};

struct variable_declaration_list_t; 
struct variable_declaration_list_t {
  struct variable_declaration_t *vd;
  struct variable_declaration_list_t *next;  
};

/* ---------------------------------------------------------------- */


/* ----------------------------------------------------------------
 * Everything required for the func_declaration_list
 * ----------------------------------------------------------------
 */
struct formal_parameter_section_t{
  struct identifier_list_t *il;
  char *id;
  int is_var;
};

struct formal_parameter_section_list_t{
  struct formal_parameter_section_t *fps;
  struct formal_parameter_section_list_t *next;
};

struct function_heading_t{
  char *id;
  char *res; 
  struct formal_parameter_section_list_t *fpsl; /* == formal_parameter_list */
};

struct function_declaration_t {
  struct function_heading_t *fh;
  struct function_block_t *fb;
  int line_number;
};

struct func_declaration_list_t;
struct func_declaration_list_t{
  struct function_declaration_t *fd;
  struct func_declaration_list_t *next;
};
/* ---------------------------------------------------------------- */



/* ----------------------------------------------------------------
 * Everything required for the statement_sequence
 * ----------------------------------------------------------------
 */
struct expression_t;
struct actual_parameter_t{
  struct expression_t *e1;
  struct expression_t *e2;
  struct expression_t *e3;
};

struct actual_parameter_list_t;
struct actual_parameter_list_t{
  struct actual_parameter_t *ap;
  struct actual_parameter_list_t *next;
};

struct function_designator_t{
  char *id;
  struct actual_parameter_list_t *apl;
};

/* This is a temporary data structure used to hold the value and type of
   an expression. It is included (inherited) by a bunch of other data
   structures */
struct expression_data_t{
  float val;
  char *type;
  struct array_type_t *array;
};

struct unsigned_number_t{
  int ui;
  struct expression_data_t *expr;
};

struct variable_access_t;
#define PRIMARY_T_VARIABLE_ACCESS 1
#define PRIMARY_T_UNSIGNED_CONSTANT 2
#define PRIMARY_T_FUNCTION_DESIGNATOR 3
#define PRIMARY_T_EXPRESSION 4
#define PRIMARY_T_PRIMARY 5
struct primary_t{
  int type; /* 1 - variable_access
	     * 2 - unsigned_constant
	     * 3 - function_designator
	     * 4 - expression
	     * 5 - primary (NOT boolean)
	     */
  union{
    struct variable_access_t *va; 
    struct unsigned_number_t *un; /* == unsigned_constant */
    struct function_designator_t *fd;
    struct expression_t *e;
    struct primary_t *next;
  }data;
  struct expression_data_t *expr;
};

struct factor_data_t{
    int sign;
    struct factor_t *next;
  };

#define FACTOR_T_SIGNFACTOR 1
#define FACTOR_T_PRIMARY 2
struct factor_t{
  int type; /* 1 - sign/factor
    * 2 - primary
    */
  union {
    struct factor_data_t *f;
    struct primary_t *p;
  }data;
  struct expression_data_t *expr;
};

struct term_t;
struct term_t{
  struct factor_t *f;
  int mulop;
  struct expression_data_t *expr;
  struct term_t *next;
};

struct simple_expression_t;
struct simple_expression_t{
  struct term_t *t;
  int addop;
  struct expression_data_t *expr;
  struct simple_expression_t *next;
};

struct expression_t{
  struct simple_expression_t *se1;
  int relop;
  struct simple_expression_t *se2;
  struct expression_data_t *expr;
};

struct index_expression_list_t;
struct index_expression_list_t{
  struct expression_t *e;
  struct index_expression_list_t *next;
  struct expression_data_t *expr;
};

struct indexed_variable_t{
  struct variable_access_t *va;
  struct index_expression_list_t *iel;
  struct expression_data_t *expr;
};

struct attribute_designator_t{
  struct variable_access_t *va;
  char *id;
};

struct method_designator_t{
  struct variable_access_t *va;
  struct function_designator_t *fd;
};

#define VARIABLE_ACCESS_T_IDENTIFIER 1
#define VARIABLE_ACCESS_T_INDEXED_VARIABLE 2
#define VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR 3
#define VARIABLE_ACCESS_T_METHOD_DESIGNATOR 4
struct variable_access_t{
  int type; /* 1 - identifier
	     * 2 - indexed_variable
	     * 3 - attribute_designator
	     * 4 - method_designator
	     */
  union{
    char *id;
    struct indexed_variable_t *iv;
    struct attribute_designator_t *ad;
    struct method_designator_t *md;
  }data;
  char *recordname;          /* This is a temporary field used to collect
				a verbose description of the data type
				that is validated */
  struct expression_data_t *expr;
};

struct object_instantiation_t{
  char *id;
  struct actual_parameter_list_t *apl;
};

struct assignment_statement_t{
  struct variable_access_t *va;
  struct expression_t *e;  
  struct object_instantiation_t *oe;
};

struct statement_t;
struct if_statement_t{
  struct expression_t *e;
  struct statement_t *s1;
  struct statement_t *s2;
};

struct while_statement_t{
  struct expression_t *e;
  struct statement_t *s;
};

struct print_statement_t{
  struct variable_access_t *va;
};

struct function_block_t{
  struct variable_declaration_list_t *vdl;
  struct statement_sequence_t *ss;
};

 union statement_union{
        struct assignment_statement_t *as;
        struct statement_sequence_t *ss;
        struct if_statement_t *is;
        struct while_statement_t *ws;
        struct print_statement_t *ps;
  };

struct statement_sequence_t;
#define STATEMENT_T_ASSIGNMENT 1
#define STATEMENT_T_SEQUENCE 2
#define STATEMENT_T_IF 3
#define STATEMENT_T_WHILE 4
#define STATEMENT_T_PRINT 5
struct statement_t {
  int type; /* 1 - assignment_statement
	     * 2 - statement_sequence
	     * 3 - if_statement
	     * 4 - while_statement
	     * 5 - print_statement
	     */

  union statement_union data;
  int line_number;
};

struct statement_sequence_t{
  struct statement_t *s;
  struct statement_sequence_t *next;
  int line_number;
};



/* ---------------------------------------------------------------- */



/* ----------------------------------------------------------------
 * Everything required for the program
 * ----------------------------------------------------------------
 */
struct program_heading_t {
  char *id;
  struct identifier_list_t *il;
};

struct class_identification_t {
  char *id;
  struct class_extend_t *extend;
  int line_number;
};

struct class_extend_t {
  char *id;
  struct class_list_t *extend_class;
};

struct class_block_t{
  struct variable_declaration_list_t *vdl; /* == variable_declaration_part */
  struct func_declaration_list_t *fdl;
  struct statement_table_t *statement_hash_table; /* Circular? */
  struct attribute_table_t *attribute_hash_table;
  int class_func_num;
  int class_var_num;
};

struct class_list_t;
struct class_list_t {
  struct class_identification_t *ci;
  struct class_block_t *cb;
  struct ClassNode_c *class_node_list;
  struct class_list_t *next;
};

struct program_t {
  struct program_heading_t *ph;
  struct class_list_t *cl;
  struct class_table_t *class_hash_table;
};



/* ----------------------------------------------------------------
 * Function declarations
 * ----------------------------------------------------------------
 */
int makekey(char* lexeme, int max_hashkeys);
char * tolowerr(char *s);
void print_tabs(int numOfTabs);
char *inttostring(int value);
char *longtostring(long value);
struct actual_parameter_t *set_actual_parameter(struct expression_t *e1, struct expression_t *e2, struct expression_t *e3);
struct actual_parameter_list_t *set_actual_parameter_list(struct actual_parameter_t *ap, struct actual_parameter_list_t *next);
struct array_type_t *set_array_type(struct range_t *r, struct type_denoter_t *td);
struct assignment_statement_t *set_assignment_statement(struct variable_access_t *va, struct expression_t *e, struct object_instantiation_t *oe);
struct attribute_designator_t *set_attribute_designator(struct variable_access_t *va, char *id, int line_number);
void check_compatibility(char* type1, char* type2);
struct class_block_t *set_class_block(struct variable_declaration_list_t *vdl, struct func_declaration_list_t *fdl);
struct class_identification_t *set_class_identification(char *id, char *extend, int line_number);
struct class_list_t *set_class_list(struct class_identification_t* ci, struct class_block_t *cb, struct class_list_t *next);
struct expression_t *set_expression(struct simple_expression_t* se1, int relop, struct simple_expression_t* se2, int line_number);
struct expression_data_t *set_expression_data(float val, char *type);
struct factor_t* set_factor_t_sign_factor(int sign, struct factor_t* f, int line_number);
struct factor_t* set_factor_t_primary(struct primary_t* p);
struct factor_data_t* set_factor_data(int sign, struct factor_t* next);
struct formal_parameter_section_t *set_formal_parameter_section(struct identifier_list_t *il, char *id, int is_var);
struct formal_parameter_section_list_t *set_formal_parameter_section_list(struct formal_parameter_section_t *fps, struct formal_parameter_section_list_t *next);
struct function_block_t *set_function_block(struct variable_declaration_list_t *vdl, struct statement_sequence_t *ss);
struct function_declaration_t *set_function_declaration(struct function_heading_t *fh, struct function_block_t *fb, int line_number);
struct func_declaration_list_t *set_func_declaration_list(struct function_declaration_t *fd, struct func_declaration_list_t *next);
struct function_designator_t *set_function_designator(char * id, struct actual_parameter_list_t * apl);
struct function_heading_t *set_function_heading(char *id, char *res, struct formal_parameter_section_list_t *fpsl);
struct identifier_list_t *set_identifier_list(char *id, struct identifier_list_t* next);
struct if_statement_t *set_if_statement(struct expression_t *e, struct statement_t *s1, struct statement_t *s2);
struct index_expression_list_t *set_index_expression_list(struct expression_t *e, struct index_expression_list_t *next, struct expression_data_t *expr);
struct indexed_variable_t *set_indexed_variable(struct variable_access_t *va, struct index_expression_list_t *iel, int line_number);
struct method_designator_t *set_method_designator(struct variable_access_t *va, struct function_designator_t *fd, int line_number);
struct object_instantiation_t *set_object_instantiation(char *id, struct actual_parameter_list_t *apl);
struct primary_t *set_primary_t_variable_access(struct variable_access_t *va);
struct primary_t* set_primary_t_unsigned_constant(struct unsigned_number_t *un);
struct primary_t* set_primary_t_function_designator(struct function_designator_t *fd);
struct primary_t* set_primary_t_expression(struct expression_t *e);
struct primary_t* set_primary_t_primary(struct primary_t *p, int line_number);
struct print_statement_t *set_print_statement(struct variable_access_t *va);
struct program_t *set_program(struct program_heading_t *ph, struct class_list_t *cl);
struct program_heading_t *set_program_heading(char *id, struct identifier_list_t *il);
struct range_t *set_range(struct unsigned_number_t *min, struct unsigned_number_t *max, int line_number);
int * set_sign(int sign);
struct simple_expression_t *set_simple_expression(struct term_t *t, int addop, struct simple_expression_t *next, int line_number);
struct statement_t *set_statement_assignment(struct assignment_statement_t *as, int line_number);
struct statement_t *set_statement_statement_sequence(struct statement_sequence_t *ss, int line_number);
struct statement_t *set_statement_if(struct if_statement_t *is, int line_number);
struct statement_t *set_statement_while(struct while_statement_t *ws, int line_number);
struct statement_t *set_statement_print(struct print_statement_t *ps, int line_number);
struct statement_sequence_t *set_statement_sequence(struct statement_t *s, struct statement_sequence_t *next, int line_number);
struct term_t *set_term(struct factor_t *f, int mulop, struct term_t *term, int line_number);
struct type_denoter_t *set_type_denoter_array(char *name, struct array_type_t *at);
struct type_denoter_t *set_type_denoter_class(char *name, struct class_list_t *cl);
struct type_denoter_t *set_type_denoter_id(char *name, char *id);
struct unsigned_number_t *set_unsigned_number(int ui, struct expression_data_t* expr);
struct variable_access_t *set_variable_access_id(char *id, char *recordname, struct expression_data_t *expr);
struct variable_access_t *set_variable_access_indexed_variable(struct indexed_variable_t *iv, char *record_name, struct expression_data_t *expr);
struct variable_access_t *set_variable_access_attribute_designator(struct attribute_designator_t *ad);
struct variable_access_t *set_variable_access_method_designator(struct method_designator_t *md, char *recordname, struct expression_data_t *expr);
struct variable_declaration_t *set_variable_declaration(struct identifier_list_t *il, struct type_denoter_t *tden, int line_number);
struct variable_declaration_list_t *set_variable_declaration_list(struct variable_declaration_t *vd, struct variable_declaration_list_t *next);
struct while_statement_t *set_while_statement(struct expression_t *e, struct statement_t *s);
void create_class_hash_table(struct class_list_t *class_list);
void add_class_to_cht(struct class_list_t *class_list);
void create_statement_hash_table(struct func_declaration_list_t *func_dec_list);
void add_statement_list_to_sht(struct function_declaration_t *func_dec);
int* create_attribute_hash_table(struct func_declaration_list_t *func_dec_list, struct variable_declaration_list_t *var_dec_list);
int add_class_attrs_to_aht(struct variable_declaration_list_t *var_dec_list, int type, struct function_declaration_t *dummy_func_dec);
void parse_param_section(struct formal_parameter_section_t *param_section, int scope, struct function_declaration_t *func);
int parse_var_dec(struct variable_declaration_t *var_dec, int scope, struct function_declaration_t *func, int order);
int add_class_funcs_to_aht(struct func_declaration_list_t *func_dec_list, struct function_declaration_t *dummy_func_dec, int order);
void add_func_params_to_aht(struct formal_parameter_section_list_t *param_list, int scope, struct function_declaration_t *func);
void add_func_var_to_aht(struct variable_declaration_list_t *var_dec_list, int scope, struct function_declaration_t *func);
struct type_denoter_t* generate_type_denoter(char* return_type);
void add_class_to_hash_table(struct class_table_t *class_t);
void add_statement_to_hash_table(struct statement_table_t *statement);
void add_attribute_to_hash_table(struct attribute_table_t *attr, int entity_type);
int check_against_reserved_words(char* id, int line_number, int entity_type);
void attribute_hash_table_error(struct attribute_table_t *item_ptr, struct attribute_table_t *failed_attr);
struct class_table_t* create_class_node(struct class_list_t *class_list);
struct statement_table_t* create_statement_node(struct statement_t *stat, struct function_declaration_t *function, int line_number);
char* format_attr_id(char id[], int id_length);
void print_statement_hash_table(struct statement_table_t *stat);
void find_undefined_extends(struct class_list_t *class_list);
void check_extend_attributes(struct class_list_t *base_class, struct class_list_t *parent_class);
void check_id_against_var_dec_list(char *id, struct class_list_t *parent_class, int line_number);
struct class_table_t* find_hash_object(char *class_list);
void check_class_constructors(struct class_table_t *class_hash_table);
char* create_attribute_key(char* id, int scope, char *function_id);
struct expression_data_t* check_real_or_integer(struct expression_data_t *expr_1, struct expression_data_t *expr_2, int line_number);
struct expression_data_t* check_boolean(struct expression_data_t *expr_1, struct expression_data_t *expr_2, int line_number);
void check_for_same_name_vars(char *class_id, struct attribute_table_t *attr_hash_table);
void validate_class_attribute_types(struct class_table_t *class_hash_table);
void validate_type_denoter(struct type_denoter_t *td, struct class_table_t *class_hash_table, int line_number);
void check_for_class_existence(char *id, struct class_table_t *class_hash_table, int line_number);
void print_hash_table(struct attribute_table_t* attr);
void print_class_hash_table(struct class_table_t* class_table);
int is_boolean(char *id);
int is_integer(char *id);
int is_real(char *id);
int is_array(char *id);
int is_true(char *id);
int is_false(char *id);
struct expression_data_t* identify_primitive_data(char *id);
struct VarNode_c* create_var_list(struct identifier_list_t *il, struct type_denoter_t *tden);
struct VarNode_c* concat_var_lists(struct VarNode_c *base, struct variable_declaration_list_t *vdl);
struct ClassNode_c* create_and_concat(struct class_identification_t *ci, struct class_block_t *cb, struct class_list_t *next);
struct ClassNode_c* create_ClassNode_c(struct class_identification_t *ci, struct class_block_t *cb, struct class_list_t *next);
struct ClassNode_c* look_up_class(char *id, struct class_list_t *cl);
int calc_class_size(struct VarNode_c *attr_list, struct class_list_t *class_list);
int is_primitive(char* id);
char* create_id_char(char* id);
void exit_on_errors();
void print_class_table(struct ClassNode_c *classes);
int calc_var_size(struct type_denoter_t *tden);
char* get_var_type(struct type_denoter_t *tden);


struct VarNode_c {
  char *name;
  int is_global;
  int offset;
  int size;
  char *type;
  struct VarNode_c *next;
  struct array_type_t* array;
};

struct ClassNode_c {
  char *name;
  struct ClassNode_c *parent;
  struct VarNode_c *attributes;
  struct ClassNode_c *next;
  int size;
};

struct attribute_table_t{
    struct type_denoter_t *type;
    int line_number;
    int is_func;
    struct formal_parameter_section_list_t *params;
    int scope;
    struct function_declaration_t *function;
    struct expression_data_t *expr;
    char id[30];
    int order;
    int id_length;
    char string_key[61];  
};

struct statement_table_t{
    int type;
    int line_number;
    struct function_declaration_t *function;
    
    union statement_union* statement_data;
  char string_key[31 + sizeof(union statement_union*)]; /*key for this statement*/
};

struct class_table_t {
    struct attribute_table_t *attribute_hash_table;
    struct class_table_t *extend;
    int line_number;
    int class_func_num;
    int class_var_num;
    struct statement_table_t *statement_hash_table;
    struct class_list_t *class_list;
    char *id;
};

struct attribute_table_t* create_attribute_node(char* id,
                                                struct type_denoter_t *type,
                                                int line_number, 
                                                int scope, 
                                                int is_func, 
                                                struct formal_parameter_section_list_t *params, 
                                                struct function_declaration_t *function,
                                                int order);







#endif

