/* error.h
 *
 * Contains function definitions of common error output function
 *
 */

#ifndef ERROR_H
#define ERROR_H

#include "stdio.h"
#include "shared.h"

#define ERROR_UNKNOWN "An Unknown error has occured"


/* helper functions */
void error(int line_number, char *e);
void error_check_not_null(char **str);


/* generic */
void error_unknown(int line_number);
void error_compiler_error(int line_number, char *s);


/* rulefuncs related */
void error_type_already_defined(int line_number, char *t, int line_number_prev);
void error_type_not_defined(int line_number, char *t);
void error_type_mismatch(int line_number, char *t1, char *t2);
void error_class_already_declared(int line_number, char *c, int line_number_prev);
void error_variable_already_declared(int line_number, char *v, int line_number_prev);
void error_function_already_declared(int line_number, char *f, int line_number_prev);
void error_variable_not_declared(int line_number, char *v);
void error_function_not_declared(int line_number, char *f);
void error_variable_name_invalid(int line_number, char *v);
void error_class_name_invalid(int line_number, char *c);
void error_function_name_invalid(int line_number, char *f);
void error_record_has_no_such_field(int line_number, char *r, char *f);
void error_datatype_is_not(int line_number, char *base, char *target);
void error_indexed_variable_not_an_array(int line_number, char *v);
void error_too_many_signs(int line_number);
void error_missing_program_class();
void error_extending_missing_class(int line_number, char *this_class, char *base_class);
void error_class_not_base_class(int line_number, char *this_class, char *base_class);
void error_array_index_out_of_bounds(int line_number, long index, long min, long max);
void error_array_range_invalid(int line_number, long min, long max);
void error_variable_name_reserved_for_function_return_value(int line_number, char *v);
void error_cannot_set_return_value_on_right_hand_side(int line_number, char *v);
void error_function_return_value_not_set(int line_number, char *f);
void error_array_index_is_not_integer(int line_number, char *v);
void error_circular_extends(int line_number, char *id, char *p_id);
void error_extends_itself(int line_number, char *id);
void error_constructor_cannot_have_return_type(int line_number, char *id);



#endif
