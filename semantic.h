/*
* Maddison Meier and Derrick Rahbar Compilers fall 2014
*
*
*/


/*
 * semantic.h
 *
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H


void semantic_analysis(struct program_t *p);
void validate_assignment_statement(struct statement_table_t* statement, 
                                   struct attribute_table_t* attr_hash_table, 
                                   struct function_declaration_t *statement_func);

void validate_statement_sequence(struct statement_table_t* statement,
                                 struct attribute_table_t* attr_hash_table, 
                                 struct function_declaration_t *statement_func);

void validate_if_statement(struct statement_table_t* statement, 
                           struct attribute_table_t* attr_hash_table, 
                           struct function_declaration_t *statement_func);


void validate_while_statement(struct statement_table_t* statement,
                              struct attribute_table_t* attr_hash_table, 
                              struct function_declaration_t *statement_func);


void validate_print_statement(struct statement_table_t* statement, 
                              struct attribute_table_t* attr_hash_table, 
                              struct function_declaration_t *statement_func);

struct statement_table_t *create_statement_table(struct statement_t *statement);


void eval_statement(struct statement_t *statement,
                    struct attribute_table_t* attr_hash_table, 
                    struct function_declaration_t *statement_func);

struct expression_data_t* get_expr_expr_data(struct expression_t *expr, 
                                             struct attribute_table_t* attr_hash_table, 
                                             struct function_declaration_t *statement_func, 
                                             int line_number);

struct expression_data_t* get_obj_inst_expr_data(struct object_instantiation_t *obj_inst, 
                                                 struct attribute_table_t* attr_hash_table, 
                                                 struct function_declaration_t *statement_func, 
                                                 int line_number);

struct expression_data_t* get_va_expr_data(struct variable_access_t* va, 
                                           struct attribute_table_t *attr_hash_table, 
                                           struct function_declaration_t *func, 
                                           int line_number);

struct expression_data_t* get_simple_expr_expr_data(struct simple_expression_t *simple_expression, 
                                                    struct attribute_table_t* attr_hash_table, 
                                                    struct function_declaration_t *statement_func, 
                                                    int line_number);

struct expression_data_t* get_term_expr_data(struct term_t *term, 
                                             struct attribute_table_t* attr_hash_table, 
                                             struct function_declaration_t *statement_func,
                                             int line_number);


struct expression_data_t* get_factor_expr_data(struct factor_t *factor, 
                                               struct attribute_table_t* attr_hash_table, 
                                               struct function_declaration_t *statement_func,
                                               int line_number);


struct expression_data_t* get_primary_expr_data(struct primary_t* primary, 
                                                struct attribute_table_t* attr_hash_table, 
                                                struct function_declaration_t *statement_func,
                                                int line_number);


struct expression_data_t* get_func_des_expr_data(struct function_designator_t *func_des, 
                                                 struct attribute_table_t* attr_hash_table, 
                                                 struct function_declaration_t *statement_func, 
                                                 int line_number);


int structurally_equivalent(struct expression_data_t *expr_1, struct expression_data_t *expr_2);


int check_for_equivalence(struct expression_data_t *expr_1, struct expression_data_t *expr_2);

int check_class_equivalence(struct expression_data_t *expr_1, struct expression_data_t *expr_2);

int formal_param_lists_equal(struct formal_parameter_section_list_t *fpl_1, struct formal_parameter_section_list_t *fpl_2);

int formal_param_section_equal(struct formal_parameter_section_t *fps_1, struct formal_parameter_section_t *fps_2);

int check_array_equivalence(struct expression_data_t *expr_1, struct expression_data_t *expr_2);

int is_primitive(char *id);

int check_primitive_equivalence(char *t1, char *t2);

void check_param_list_against_function(struct actual_parameter_list_t *apl, 
                                       struct formal_parameter_section_list_t *fpl, 
                                       char *id,
                                       struct attribute_table_t* attr_hash_table, 
                                       struct function_declaration_t *statement_func,
                                       int line_number);

int has_this(char *id);

char* get_class_id_from_this(char *id);


struct expression_data_t* evaluate_va_identifier(struct variable_access_t *va, 
                                                 struct attribute_table_t *attr_hash_table,
                                                 struct function_declaration_t *func, 
                                                 int line_number);


struct expression_data_t* evaluate_va_function_var(char *id, 
                                                   struct attribute_table_t* attr_hash_table, 
                                                   struct function_declaration_t *func, 
                                                   int line_number);

struct expression_data_t* evaluate_va_class_var(char *id, struct attribute_table_t *attr_hash_table, int line_number);

struct expression_data_t* convert_td_to_expr_data(struct type_denoter_t *type, struct expression_data_t *expr, int line_number);


struct expression_data_t *evaluate_va_indexed_var(struct variable_access_t *var, 
                                                  struct attribute_table_t *attr_hash_table,
                                                  struct function_declaration_t *func, 
                                                  int line_number);

struct expression_data_t* get_iel_expr_data(struct index_expression_list_t *iel, 
                                            struct attribute_table_t *attr_hash_table,
                                            struct function_declaration_t *func, 
                                            int line_number);

struct expression_data_t* evaluate_va_attribute_designator(struct variable_access_t *va, 
                                                           struct attribute_table_t *attr_hash_table,
                                                           struct function_declaration_t *func, 
                                                           int line_number);

struct expression_data_t *evaluate_va_method_designator(struct variable_access_t *va, 
                                                        struct attribute_table_t *attr_hash_table,
                                                        struct function_declaration_t *func, 
                                                        int line_number);

struct attribute_table_t* get_class_attr_table(char *id, int line_number);

struct expression_data_t* make_call(struct primary_t* primary, 
                                                struct attribute_table_t* attr_hash_table, 
                                                struct function_declaration_t *statement_func,
                                                int line_number);


#endif /* SEMANTIC_H */
