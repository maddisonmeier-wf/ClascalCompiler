/* error.h
 *
 * Implements functions that output error messages
 *
 */

#include "error.h"


/* ----------------------------------------------------------------------- 
 * Outputs an error message
 * ----------------------------------------------------------------------- 
 */
void error(int line_number, char *e)
{
  if (line_number == -1)
    printf("ERROR:%s\n", e);
  else
    printf("ERROR:%d:%s\n", line_number, e);

  /* FIXME: The following crashes when const char pointers are passed in */
  /* free(e); */
  error_flag = 1;
}



/* ----------------------------------------------------------------------- 
 * Checks if str is set to NULL; if so, it initializes it and copies in it
 * the string "(null)" to imitate the correct libc behaviour in linux's 
 * glibc. The Solaris libc is so naive.
 * ----------------------------------------------------------------------- 
 */
void error_check_not_null(char **str)
{
  if (*str == NULL) {
    *str = (char *) malloc (strlen("(null)") + 1);
    CHECK_MEM_ERROR((*str))
    strcpy(*str, "(null)");
  }
    
}


/* ----------------------------------------------------------------------- 
 * Outputs an error message if an impossible/defective state was reached
 * ----------------------------------------------------------------------- 
 */
void error_unknown(int line_number)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  sprintf(e, ERROR_UNKNOWN);
  error(line_number, e);
}



/* -----------------------------------------------------------------------
 * Outputs an error message if there was an internal compiler error
 * -----------------------------------------------------------------------
 */
void error_compiler_error(int line_number, char *s)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&s);

  sprintf(e, "Internal compiler error: %s", s);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if a datatype is already defined
 * ----------------------------------------------------------------------- 
 */
void error_type_already_defined(int line_number, char *t, int line_number_prev)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&t);
  sprintf(e, "Data type \"%s\" is already defined at line %d", t, line_number_prev);
  error(line_number, e);
}


void error_circular_extends(int line_number, char *id, char *p_id)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&id);
  error_check_not_null(&p_id);
  sprintf(e, "Circular extend declaration exists with classes \"%s\", \"%s\", on line number: %d", id, p_id, line_number);
  error(line_number, e);
}

void error_extends_itself(int line_number, char *id)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&id);
  sprintf(e, "Class \"%s\" cannot extend itself, on line number: %d", id, line_number);
  error(line_number, e);
}


void error_constructor_cannot_have_return_type(int line_number, char *id)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&id);
  sprintf(e, "Class \"%s\" constructor cannot have a return type : %d", id, line_number);
  error(line_number, e);
}

/* ----------------------------------------------------------------------- 
 * Outputs an error message if a datatype is not defined
 * ----------------------------------------------------------------------- 
 */
void error_type_not_defined(int line_number, char *t)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&t);
  sprintf(e, "Data type \"%s\" is not defined", t);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if the datatypes t1 and t2 don't match
 * ----------------------------------------------------------------------- 
 */
void error_type_mismatch(int line_number, char *t1, char *t2)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&t1);
  error_check_not_null(&t2);
  sprintf(e, "Type mismatch between \"%s\" and \"%s\"", t1, t2);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if the class c has been already declared
 * ----------------------------------------------------------------------- 
 */
void error_class_already_declared(int line_number, char *c, int line_number_prev)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&c);
  sprintf(e, "Class \"%s\" already declared at line %d", c, line_number_prev);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if the variable v has been already declared
 * ----------------------------------------------------------------------- 
 */
void error_variable_already_declared(int line_number, char *v, int line_number_prev)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&v);
  sprintf(e, "Variable \"%s\" already declared at line %d", v, line_number_prev);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if the function f has been already declared
 * ----------------------------------------------------------------------- 
 */
void error_function_already_declared(int line_number, char *f, int line_number_prev)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&f);
  sprintf(e, "Function \"%s\" already declared at line %d", f, line_number_prev);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if the variable v has not been declared
 * ----------------------------------------------------------------------- 
 */
void error_variable_not_declared(int line_number, char *v)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&v);
  sprintf(e, "Variable \"%s\" not declared", v);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if the function f has not been declared
 * ----------------------------------------------------------------------- 
 */
void error_function_not_declared(int line_number, char *f)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&f);
  sprintf(e, "Function \"%s\" not declared", f);
  error(line_number, e);
}


/* ----------------------------------------------------------------------- 
 * Outputs an error message if the variable name v is invalid
 * ----------------------------------------------------------------------- 
 */
void error_variable_name_invalid(int line_number, char *v)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&v);
  sprintf(e, "Invalid variable name \"%s\"", v);
  error(line_number, e);
}

/* ----------------------------------------------------------------------- 
 * Outputs an error message if the function name f is invalid
 * ----------------------------------------------------------------------- 
 */
void error_function_name_invalid(int line_number, char *f)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&f);
  sprintf(e, "Invalid function name \"%s\"", f);
  error(line_number, e);
}

/* ----------------------------------------------------------------------- 
 * Outputs an error message if the class name c is invalid
 * ----------------------------------------------------------------------- 
 */
void error_class_name_invalid(int line_number, char *c)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&c);
  sprintf(e, "Invalid class name \"%s\"", c);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if the record r has no field named f
 * ----------------------------------------------------------------------- 
 */
void error_class_has_no_such_field(int line_number, char *c, char *f)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&c);
  error_check_not_null(&f);
  sprintf(e, "Class \"%s\" has no field named \"%s\"", c, f);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if the base datatype is not the same as the
 * target
 * ----------------------------------------------------------------------- 
 */
void error_datatype_is_not(int line_number, char *base, char *target)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&base);
  error_check_not_null(&target);
  sprintf(e, "Datatype \"%s\" is not \"%s\"", base, target);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if an indexed variable is not an array
 * ----------------------------------------------------------------------- 
 */
void error_indexed_variable_not_an_array(int line_number, char *v)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&v);
  sprintf(e, "Indexed variable \"%s\" is not an array", v);
  error(line_number, e);
}



/* ----------------------------------------------------------------------- 
 * Outputs an error message if there are too many (more than one) signs
 * in a factor
 * ----------------------------------------------------------------------- 
 */
void error_too_many_signs(int line_number)
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  sprintf(e, "Too many signs");
  error(line_number, e);
}


/* -----------------------------------------------------------------------
 * Outputs an error message if the program does not have a class named
 * the same as the program
 * ----------------------------------------------------------------------- 
 */
void error_missing_program_class()
{
  char *e;

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  sprintf(e, "Missing program class");
  error(-1, e);
}


/* -----------------------------------------------------------------------
 * Outputs an error message if a class tries to extend a class that
 * does not exist
 * -----------------------------------------------------------------------
 */
void error_extending_missing_class(int line_number, char *this_class, char *base_class)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&this_class);
  error_check_not_null(&base_class);
  sprintf(e, "Class \"%s\" attempted to extend class \"%s\" which does not exist", this_class, base_class);
  error(line_number, e);
}



/* -----------------------------------------------------------------------
 * Outputs an error message if a class tries to extend a class that
 * does not exist
 * -----------------------------------------------------------------------
 */
void error_class_not_base_class(int line_number, char *this_class, char *base_class)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&this_class);
  error_check_not_null(&base_class);
  sprintf(e, "Class \"%s\" is not a base class of \"%s\" in the assignment", base_class, this_class);
  error(line_number, e);
}


/* -----------------------------------------------------------------------
 * Outputs an error message if an array variable index is outside the
 * array's boundaries
 * -----------------------------------------------------------------------
 */
void error_array_index_out_of_bounds(int line_number, long index, long min, long max)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  sprintf(e, "Array index %ld is out of the range (%ld,%ld)", index, min, max);
  error(line_number, e);
}


/* -----------------------------------------------------------------------
 * Outputs an error message if an array declaration contains an
 * invalid range
 * -----------------------------------------------------------------------
 */
void error_array_range_invalid(int line_number, long min, long max)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  sprintf(e, "Invalid array range (%ld,%ld)", min, max);
  error(line_number, e);
}



/* -----------------------------------------------------------------------
 * Outputs an error message if a variable has the same name as the
 * function it belongs to.
 * -----------------------------------------------------------------------
 */
void error_variable_name_reserved_for_function_return_value(int line_number, char *v)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&v);

  sprintf(e, "Variable name \"%s\" is reserved for use as a function return value", v);
  error(line_number, e);
}



/* -----------------------------------------------------------------------
 * Outputs an error message if the return value of a function is set
 * on the right hand side of on expression
 * -----------------------------------------------------------------------
 */
void error_cannot_set_return_value_on_right_hand_side(int line_number, char *v)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&v);

  sprintf(e, "Variable name \"%s\" is reserved for use as a function return value", v);
  error(line_number, e);
}



/* -----------------------------------------------------------------------
 * Outputs an error message if the return value of a function was not
 * set
 * -----------------------------------------------------------------------
 */
void error_function_return_value_not_set(int line_number, char *f)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&f);

  sprintf(e, "No return value set for function \"%s\"", f);
  error(line_number, e);
}



/* -----------------------------------------------------------------------
 * Outputs an error message if the index value of an array is not an
 * integer
 * -----------------------------------------------------------------------
 */
void error_array_index_is_not_integer(int line_number, char *v)
{
  char *e; 

  e = (char *) malloc(MAX_ERROR_SIZE);
  CHECK_MEM_ERROR(e)

  error_check_not_null(&v);

  sprintf(e, "Array index not an integer for variable \"%s\"", v);
  error(line_number, e);
}
