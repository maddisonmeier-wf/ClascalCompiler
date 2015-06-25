/*
* Maddison Meier and Derrick Rahbar Compilers fall 2014
*
*
*/

/* shared.c
 *
 * Implements all shared subroutines
 */

#include "shared.h"
#include "rulefuncs.h"
#include <string.h>

/* Global head to the attribute hash table */
struct attribute_table_t *attr_hash_table = NULL;

/*Global head to the statement hash table */
struct statement_table_t *stat_hash_table = NULL;

/*Global head to the class hash table */
struct class_table_t *class_hash_table = NULL;


/* ----------------------------------------------------------------------- 
 * Returns a hashkey value for the given lexeme
 * ----------------------------------------------------------------------- 
 */
int makekey(char* lexeme, int max_hashkeys)
{
  int len;
  int i;
  long charsum = 0;

  len = strlen(lexeme);
  for (i = 0; i < len; i++){
    charsum += lexeme[i];
  }

  return charsum % max_hashkeys;
}



/* ----------------------------------------------------------------------- 
 * Converts a string to lowercase
 * ----------------------------------------------------------------------- 
 */
char * tolowerr(char *s)
{
  int len;
  int i;
  char *new_t;

  if (s != NULL)
    len = strlen(s);
  else
    len = 0;

  new_t = (char *) malloc(len + 1); /* +1 for '\0' */

  for (i = 0; i < len; i++) {
    /* if an uppercase character */
    if (s[i] >= 65 && s[i] <=  90)
      new_t[i] = s[i] + 32;
    else
      new_t[i] = s[i];
  }

  new_t[len] = '\0';

  return new_t;
}



/* ----------------------------------------------------------------------- 
 * Prints the specified amount of tabs
 * ----------------------------------------------------------------------- 
 */
void print_tabs(int numOfTabs)
{
  int i = 0;
  while (i < numOfTabs) {
    printf("\t");
    i++;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns a string representation of the int value
 * ----------------------------------------------------------------------- 
 */
char *inttostring(int value)
{
  char *s;

  s = (char *) malloc(MAX_NEW_CHAR_SIZE);
  sprintf(s, "%d", value);

  return s;
}



/* ----------------------------------------------------------------------- 
 * Returns a string representation of the long value
 * ----------------------------------------------------------------------- 
 */
char *longtostring(long value)
{
  char *s;

  s = (char *) malloc(MAX_NEW_CHAR_SIZE);
  sprintf(s, "%ld", value);

  return s;
}

/*
Setter methods for structs making up the program 
*/

struct actual_parameter_t *set_actual_parameter(struct expression_t *e1, struct expression_t *e2, struct expression_t *e3)
{
    struct actual_parameter_t* ap = new_actual_parameter();
    ap->e1 = e1;
    ap->e2 = e2;
    ap->e3 = e3;

    return ap;
}

struct actual_parameter_list_t *set_actual_parameter_list(struct actual_parameter_t *ap, struct actual_parameter_list_t *next)
{
    struct actual_parameter_list_t* apl = new_actual_parameter_list();
    apl->ap = ap;
    apl->next = next;

    return apl;
}

struct array_type_t *set_array_type(struct range_t *r, struct type_denoter_t *td)
{
    struct array_type_t *at = new_array_type();
    at->r = r;
    at->td = td;

    return at;   
}

struct assignment_statement_t *set_assignment_statement(struct variable_access_t *va, struct expression_t *e, struct object_instantiation_t *oe)
{
    struct assignment_statement_t* as = new_assignment_statement();
    as->va = va;
    as->e = e;
    as->oe = oe;

    return as;
}

struct attribute_designator_t *set_attribute_designator(struct variable_access_t *va, char *id, int line_number)
{
    struct attribute_designator_t *ad = new_attribute_designator();
    ad->va = va;
    ad->id = (char *)malloc(strlen(id)*sizeof(char)+1);
    strcpy(ad->id, id);

    if(va->expr != NULL)
    {
        if( is_integer(va->expr->type) || is_boolean(va->expr->type) || is_real(va->expr->type) )
        {
            error_datatype_is_not(line_number, va->expr->type, "object");
        }
    }

    return ad;
}

void check_compatibility(char* type1, char* type2)
{
    
}

struct class_block_t *set_class_block(struct variable_declaration_list_t *vdl, struct func_declaration_list_t *fdl)
{
    struct class_block_t *cb = new_class_block();
    cb->vdl = vdl;
    cb->fdl = fdl;
    
    int* order_nums = create_attribute_hash_table(fdl, vdl);
    cb->class_var_num = order_nums[0];
    cb->class_func_num = order_nums[1];
    cb->attribute_hash_table = attr_hash_table;
    attr_hash_table = NULL; /*set global head back to null for next class*/
    
    create_statement_hash_table(fdl);
    cb->statement_hash_table = stat_hash_table;
    stat_hash_table = NULL; /*set global head back to null for next class */

    return cb;
}

struct class_identification_t *set_class_identification(char *id, char *extend, int line_number)
{
    struct class_identification_t *ci = new_class_identification();
    ci->id = (char *)malloc(strlen(id)+sizeof(char)+1);
    strcpy(ci->id, id);
    
    if(extend != NULL)
    {
      ci->extend = new_class_extend();
      ci->extend->id = (char *)malloc(strlen(extend)+1);
      strcpy(ci->extend->id, extend);
    }

    ci->line_number = line_number;

    return ci;

}

struct class_list_t *set_class_list(struct class_identification_t* ci, struct class_block_t *cb, struct class_list_t *next)
{
    struct class_list_t *cl = new_class_list();
    cl->ci = ci;
    cl->cb = cb;
    cl->next = next;
    cl->class_node_list = create_and_concat(ci, cb, next);

    return cl;
} 

struct expression_t *set_expression(struct simple_expression_t* se1, int relop, struct simple_expression_t* se2, int line_number)
{
    struct expression_t *e = new_expression();
    e->se1 = se1;
    e->relop = relop;
    e->se2 = se2;

    if(relop == RELOP_NONE)
    {
        /*just a simple expression given in se1*/
        e->expr = se1->expr;
    }
    else if(se1->expr != NULL && se2->expr != NULL) /*we can eval this exression*/
    {
        switch(relop)
        {
            case RELOP_EQUAL:
            case RELOP_NOTEQUAL:
                if(strcmp(se1->expr->type, se2->expr->type) == 0)
                {
                    e->expr = set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
                }
                else
                {
                    error_type_mismatch(line_number, se1->expr->type, se2->expr->type);
                }

            break;
            case RELOP_LT: /*only valid for integer and real*/
            case RELOP_GT:
            case RELOP_LE:
            case RELOP_GE:
                if((is_integer(se1->expr->type) || is_real(se1->expr->type)) && 
                   (is_integer(se2->expr->type) || is_real(se2->expr->type)))
                {
                    e->expr = set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
                }
                else
                {
                    /*bad type error*/
                    if(!(is_real(se1->expr->type) || is_integer(se1->expr->type)))
                    {
                        error_datatype_is_not(line_number, se1->expr->type, "real or integer");
                    }
                    if(!(is_real(se2->expr->type) || is_integer(se2->expr->type)))
                    {
                        error_datatype_is_not(line_number, se2->expr->type, "real or integer");
                    }

                    /*set to int to continue eval*/
                    e->expr = set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");   
                }
            
            break;
        }
    }

    return e;
}

struct expression_data_t *set_expression_data(float val, char *type)
{
    struct expression_data_t *ed = new_expression_data();
    ed->val = val;
    ed->type = (char*)malloc(sizeof(char)*strlen(type)+1);
    strcpy(ed->type, type);

    return ed;
}

////////////////////////////////////////////////////////////////////////////////////////
//FACTOR_T METHODS
////////////////////////////////////////////////////////////////////////////////////////

struct factor_t* set_factor_t_sign_factor(int sign, struct factor_t* f, int line_number)
{
    struct factor_t *fsf = new_factor();
    fsf->type = FACTOR_T_SIGNFACTOR;
    fsf->data.f = set_factor_data(sign, f);

    /*This would need to be a real or an integer to have a sign in from of it */
    if(f->expr != NULL)
    {
        if(is_integer(f->expr->type) || is_real(f->expr->type))
        {
            /*This is a valid type of sign factor*/
            fsf->expr = f->expr;
        }
        else
        {
            error_datatype_is_not(line_number, f->expr->type, "real or integer");

            /* set this to a valid type in order to continue eval*/
            fsf->expr = set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
        }
    }

    return fsf;

}

struct factor_t* set_factor_t_primary(struct primary_t* p)
{
    struct factor_t *fp = new_factor();
    fp->type = FACTOR_T_PRIMARY;
    fp->data.p = p;
    fp->expr = p->expr; /*if this is populated it will be the same for this factor*/

    return fp;
}

////////////////////////////////////////////////////////////////////////////////////////

struct factor_data_t* set_factor_data(int sign, struct factor_t* next)
{
    struct factor_data_t* fd = new_factor_data();
    fd->sign = sign;
    fd->next = next;

    return fd;
}

struct formal_parameter_section_t *set_formal_parameter_section(struct identifier_list_t *il, char *id, int is_var)
{
    struct formal_parameter_section_t *fps = new_formal_parameter_section();
    fps->il = il;
    fps->id = (char *)malloc(strlen(id)*sizeof(char)+1);
    strcpy(fps->id, id);
    fps->is_var = is_var;

    return fps;
}

struct formal_parameter_section_list_t *set_formal_parameter_section_list(struct formal_parameter_section_t *fps, struct formal_parameter_section_list_t *next)
{
    struct formal_parameter_section_list_t *fpsl = new_formal_parameter_section_list();
    fpsl->fps = fps;
    fpsl->next = next;

    return fpsl;
}

struct function_block_t *set_function_block(struct variable_declaration_list_t *vdl, struct statement_sequence_t *ss)
{
    struct function_block_t *fb = new_function_block();
    fb->vdl = vdl;
    fb->ss = ss;

    return fb;
}

struct function_declaration_t *set_function_declaration(struct function_heading_t *fh, struct function_block_t *fb, int line_number)
{
    struct function_declaration_t *fd = new_function_declaration();
    fd->fh = fh;
    fd->fb = fb;
    fd->line_number = line_number;

    return fd;
}

struct func_declaration_list_t *set_func_declaration_list(struct function_declaration_t *fd, struct func_declaration_list_t *next)
{
    struct func_declaration_list_t *fdl = new_func_declaration_list();
    fdl->fd = fd;
    fdl->next = next;

    return fdl;
}

struct function_designator_t *set_function_designator(char * id, struct actual_parameter_list_t * apl)
{
    struct function_designator_t *fd = new_function_designator();
    fd->id = (char *)malloc(strlen(id)*sizeof(char)+1);
    strcpy(fd->id, id);
    fd->apl = apl;

    return fd;
}

struct function_heading_t *set_function_heading(char *id, char *res, struct formal_parameter_section_list_t *fpsl)
{
    struct function_heading_t *fh = new_function_heading();
    fh->id = (char*)malloc(strlen(id)+1);
    strcpy(fh->id, id);
    fh->res = res;
    fh->fpsl = fpsl;

    return fh;
}

struct identifier_list_t *set_identifier_list(char *id, struct identifier_list_t* next)
{
    struct identifier_list_t *il = new_identifier_list();
    il->id = (char *)malloc(strlen(id)+1);
    strcpy(il->id, id);
    il->next = next;

    return il;
}

struct if_statement_t *set_if_statement(struct expression_t *e, struct statement_t *s1, struct statement_t *s2)
{
    struct if_statement_t *is = new_if_statement();
    is->e = e;
    is->s1 = s1;
    is->s2 = s2;

    return is;
}

struct index_expression_list_t *set_index_expression_list(struct expression_t *e, struct index_expression_list_t *next, struct expression_data_t *expr)
{
    struct index_expression_list_t *iel = new_index_expression_list();
    iel->e = e;
    iel->next = next;
    iel->expr = expr;

    return iel;
}

struct indexed_variable_t *set_indexed_variable(struct variable_access_t *va, struct index_expression_list_t *iel, int line_number)
{
    struct indexed_variable_t *iv = new_indexed_variable();
    iv->va = va;
    iv->iel = iel;
    
    /* We are assuming the index expression list only has one element */
    if(iel->expr != NULL)
    {
        if(!is_integer(iel->expr->type))
        {
            error_datatype_is_not(line_number, iel->expr->type, "integer");
        }
    }

    if(va->expr != NULL)
    {
        if(!is_array(va->expr->type))
        {
            error_datatype_is_not(line_number, va->expr->type, "array");
        }
    }
    return iv;
}

struct method_designator_t *set_method_designator(struct variable_access_t *va, struct function_designator_t *fd, int line_number)
{
    struct method_designator_t *md = new_method_designator();
    md->va = va;
    md->fd = fd;

    if(va->expr != NULL)
    {
        if( is_integer(va->expr->type) || is_boolean(va->expr->type) || is_real(va->expr->type) )
        {
            error_datatype_is_not(line_number, va->expr->type, "object");
        }
    }

    return md;
}

struct object_instantiation_t *set_object_instantiation(char *id, struct actual_parameter_list_t *apl)
{
    struct object_instantiation_t *oi = new_object_instantiation();
    oi->id = (char *)malloc(strlen(id)+sizeof(char)+1);
    strcpy(oi->id, id);
    oi->apl = apl;

    return oi;
}

////////////////////////////////////////////////////////////////////////////////////////
//PRIMARY_T METHODS
////////////////////////////////////////////////////////////////////////////////////////

struct primary_t *set_primary_t_variable_access(struct variable_access_t *va)
{
    struct primary_t *p = new_primary();
    p->type = PRIMARY_T_VARIABLE_ACCESS;
    p->data.va = va;
    /*This will be evaluated after parse by finding the type of the variable access*/

    return p;
}

struct primary_t* set_primary_t_unsigned_constant(struct unsigned_number_t *un)
{
    struct primary_t *p = new_primary();
    p->type = PRIMARY_T_UNSIGNED_CONSTANT;
    p->data.un = un;
    p->expr = un->expr; /*This will set the value to the int val and the type to int*/

    return p;
}

struct primary_t* set_primary_t_function_designator(struct function_designator_t *fd)
{
    struct primary_t *p = new_primary();
    p->type = PRIMARY_T_FUNCTION_DESIGNATOR;
    p->data.fd = fd;
    /*We do not know what the return val for this function is must eval after parse
     this will be the function return value of fd->id (find is hash table)*/

    return p;
}

struct primary_t* set_primary_t_expression(struct expression_t *e)
{
    struct primary_t *p = new_primary();
    p->type = PRIMARY_T_EXPRESSION;
    p->data.e = e;
    p->expr = e->expr; /*the type of this primary is whatever the expression type is*/

    return p;
}

struct primary_t* set_primary_t_primary(struct primary_t *p, int line_number)
{
    /*This must be a boolean type because this was preset with a NOT*/
    struct primary_t *pr = new_primary();
    pr->type = PRIMARY_T_PRIMARY;
    pr->data.next = p;

    if(p->expr != NULL)
    {
        if(strcmp(p->expr->type, "boolean") == 0)
        {
            /*we are good, this primary is a boolean perform NOT*/
            pr->expr = p->expr;
        }
        else
        {
            error_datatype_is_not(line_number, p->expr->type, "boolean");
            
            /*set the type to the correct value in order to continue eval*/
            pr->expr = set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
        }
    }
    
    /*if the expression was not set, then we need to wait until after parsing to eval*/

    return pr;
}

////////////////////////////////////////////////////////////////////////////////////////


struct print_statement_t *set_print_statement(struct variable_access_t *va)
{
    struct print_statement_t *ps = new_print_statement();
    ps->va = va;

    return ps;   
}

struct program_t *set_program(struct program_heading_t *ph, struct class_list_t *cl)
{
    struct program_t *p = new_program();
    p->ph = ph;
    p->cl = cl;
    return p;   
}

struct program_heading_t *set_program_heading(char *id, struct identifier_list_t *il)
{
    struct program_heading_t *ph = new_program_heading();
    ph->id = (char *)malloc(strlen(id)+sizeof(char)+1);
    strcpy(ph->id, id);
    ph->il = il;

    return ph;   
}

struct range_t *set_range(struct unsigned_number_t *min, struct unsigned_number_t *max, int line_number)
{
    if(min->ui > max->ui)
    {
        error_array_range_invalid(line_number, min->ui, max->ui);
    }
    struct range_t *r = new_range();
    r->min = min;
    r->max = max;

    return r;
}

int * set_sign(int sign)
{
  int * s = (int *)malloc(sizeof(int)+1);
  *s = sign;

  return s;
}

struct simple_expression_t *set_simple_expression(struct term_t *t, int addop, struct simple_expression_t *next, int line_number)
{
    struct simple_expression_t *se = new_simple_expression();
    se->t = t;
    se->addop = addop;
    se->next = next;

    if(addop == ADDOP_NONE) /*this is just a term*/
    {
        se->expr = t->expr;
    }
    else if(t->expr != NULL && next->expr != NULL) /*if both expr are populated we can eval*/
    {
        char *t_type = t->expr->type;
        char *n_type = next->expr->type;

        switch(addop)
        {
            case ADDOP_PLUS:
            case ADDOP_MINUS: /*only valid for real and integer*/
                if((is_integer(t_type) || is_real(t_type)) &&
                    (is_integer(n_type) || is_real(n_type)))
                {
                    if(is_real(t_type) || is_real(n_type))
                    {
                        se->expr = set_expression_data(EXPRESSION_DATA_REAL, "real");  
                    }
                    else
                    {
                        se->expr = set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
                    }
                }
                else
                {
                    if(!(is_integer(t_type) || is_real(t_type)))
                    {
                        error_datatype_is_not(line_number, t_type, "real or integer");
                    }
                    if(!(is_integer(n_type) || is_real(n_type)))
                    {
                        error_datatype_is_not(line_number, n_type, "real or integer");
                    }

                    /*set to int so we can continue parsing*/
                    se->expr = set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
                }
            break;
            case ADDOP_OR: /*only valid for boolean*/
                if(is_boolean(t_type) && is_boolean(n_type))
                {
                    se->expr = set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
                }
                else
                {
                    if(!is_boolean(t_type))
                    {
                        error_datatype_is_not(line_number, t_type, "boolean");
                    }
                    if(!is_boolean(n_type))
                    {
                        error_datatype_is_not(line_number, n_type, "boolean");
                    }

                    /*set to int so we can continue parsing*/
                    se->expr = set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
                }
            break;
        }
    }

    return se;
}

////////////////////////////////////////////////////////////////////////////////////////
//STATEMENT_T METHODS
////////////////////////////////////////////////////////////////////////////////////////

struct statement_t *set_statement_assignment(struct assignment_statement_t *as, int line_number)
{
    struct statement_t *s = new_statement();
    s->type = STATEMENT_T_ASSIGNMENT;
    s->data.as = as;
    s->line_number = line_number;

    return s;
}

struct statement_t *set_statement_statement_sequence(struct statement_sequence_t *ss, int line_number)
{
    struct statement_t *s = new_statement();
    s->type = STATEMENT_T_SEQUENCE;
    s->data.ss = ss;
    s->line_number = line_number;

    return s;
}

struct statement_t *set_statement_if(struct if_statement_t *is, int line_number)
{
    struct statement_t *s = new_statement();
    s->type = STATEMENT_T_IF;
    s->data.is = is;
    s->line_number = line_number;

    return s;
}

struct statement_t *set_statement_while(struct while_statement_t *ws, int line_number)
{
    struct statement_t *s = new_statement();
    s->type = STATEMENT_T_WHILE;
    s->data.ws = ws;
    s->line_number = line_number;

    return s;
}

struct statement_t *set_statement_print(struct print_statement_t *ps, int line_number)
{
    struct statement_t *s = new_statement();
    s->type = STATEMENT_T_PRINT;
    s->data.ps = ps;
    s->line_number = line_number;

    return s;
}

////////////////////////////////////////////////////////////////////////////////////////

struct statement_sequence_t *set_statement_sequence(struct statement_t *s, struct statement_sequence_t *next, int line_number)
{
    struct statement_sequence_t *ss = new_statement_sequence();
    ss->s = s;
    ss->next = next;
    ss->line_number = line_number;

    return ss;
}

struct term_t *set_term(struct factor_t *f, int mulop, struct term_t *term, int line_number)
{
    struct term_t *t = new_term();
    t->f = f;
    t->mulop = mulop;
    t->next = term;

    if(mulop == MULOP_NONE)
    {
        t->expr = f->expr; /*no next term, this is just a factor*/
    }
    /*if both expr are populated we can analyze this one*/
    else if(term->expr != NULL && f->expr != NULL)
    {
        char * f_type = f->expr->type;
        char * t_type = term->expr->type;

        /*analyze how to populate expr*/
        switch(mulop)
        {
            case MULOP_STAR: /*only valid for real and integer*/
            case MULOP_SLASH:
                   /*we know the type of both sides, therefore we can analyze*/
                   if((is_real(t_type) || is_integer(t_type)) &&  
                      (is_real(f_type) || is_integer(f_type)))
                      {
                        /*They are both either an integer or a real*/
                        
                        if(is_real(t_type) || is_real(f_type))
                        {
                            t->expr = set_expression_data(EXPRESSION_DATA_REAL, "real");
                        }
                        /*both must be integers*/
                        else
                        {
                            t->expr = set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
                        }
                      }
                      else
                      {
                        /*bad type error*/
                        if(!(is_real(f_type) || is_integer(f_type)))
                        {
                            error_datatype_is_not(line_number, f_type, "real or integer");
                        }
                        if(!(is_real(t_type) || is_integer(t_type)))
                        {
                            error_datatype_is_not(line_number, t_type, "real or integer");
                        }

                        /*set to int to continue eval*/
                        t->expr = set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
                      } 
            break;
            case MULOP_MOD: /*only valid for integer*/
                if(is_integer(f_type) && is_integer(t_type))
                {
                    t->expr = set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
                }
                else
                {
                    if(!is_integer(f_type))
                    {
                        error_datatype_is_not(line_number, f_type, "integer");
                    }
                    if(!is_integer(t_type))
                    {
                        error_datatype_is_not(line_number, t_type, "integer");
                    }
                    /*set to int to continue eval*/
                    t->expr = set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
                }

            break;
            case MULOP_AND: /*only valid for boolean*/
                if(is_boolean(f_type) && is_boolean(t_type))
                {
                    t->expr = set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
                }
                else
                {
                    if(!is_boolean(f_type))
                    {
                        error_datatype_is_not(line_number, f_type, "boolean");
                    }
                    if(!is_boolean(t_type))
                    {
                        error_datatype_is_not(line_number, t_type, "boolean");
                    }
                    /*set to int to continue eval*/
                    t->expr = set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
                }

            break;
        }
    }

    return t;
}

////////////////////////////////////////////////////////////////////////////////////////
//TYPE_DENOTER_T METHODS
////////////////////////////////////////////////////////////////////////////////////////

struct type_denoter_t *set_type_denoter_array(char *name, struct array_type_t *at)
{
    struct type_denoter_t *td = new_type_denoter();
    td->type = TYPE_DENOTER_T_ARRAY_TYPE;
    
    td->name = (char *)malloc(strlen(name)+sizeof(char)+1);
    strcpy(td->name, name);
    
    td->data.at = at;

    return td;
}

struct type_denoter_t *set_type_denoter_class(char *name, struct class_list_t *cl)
{
  struct type_denoter_t *td = new_type_denoter();
  td->type = TYPE_DENOTER_T_CLASS_TYPE;

  td->name = (char *)malloc(strlen(name)+sizeof(char)+1);
  strcpy(td->name, name);

  td->data.cl = cl;

  return td;
}

struct type_denoter_t *set_type_denoter_id(char *name, char *id)
{
    struct type_denoter_t *td = new_type_denoter();
    td->type = TYPE_DENOTER_T_IDENTIFIER;
    
    td->name = (char *)malloc(strlen(name)+sizeof(char)+1);
    strcpy(td->name, name);

    td->data.id = (char *)malloc(strlen(id)+sizeof(char)+1);
    strcpy(td->data.id, id);

    return td; 
}

////////////////////////////////////////////////////////////////////////////////////////

struct unsigned_number_t *set_unsigned_number(int ui, struct expression_data_t* expr)
{
    struct unsigned_number_t *un = new_unsigned_number();
    un->ui = ui;
    un->expr = expr;

    return un;
}

////////////////////////////////////////////////////////////////////////////////////////
//VARIABLE_ACCESS_T METHODS
////////////////////////////////////////////////////////////////////////////////////////

struct variable_access_t *set_variable_access_id(char *id, char *recordname, struct expression_data_t *expr)
{
    struct variable_access_t *va = new_variable_access();
    va->type = VARIABLE_ACCESS_T_IDENTIFIER;
    va->data.id = (char *)malloc(strlen(id)*sizeof(char)+1);
    strcpy(va->data.id, id);
    
    /* Info about this id will be known after parsing */
    return va;
}

struct variable_access_t *set_variable_access_indexed_variable(struct indexed_variable_t *iv, char *record_name, struct expression_data_t *expr)
{
  struct variable_access_t *va = new_variable_access();
    va->type = VARIABLE_ACCESS_T_INDEXED_VARIABLE;
    va->data.iv = iv;
    va->recordname = record_name;
    va->expr = expr;

    return va;
}

struct variable_access_t *set_variable_access_attribute_designator(struct attribute_designator_t *ad)
{
    struct variable_access_t *va = new_variable_access();
    va->type = VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR;
    va->data.ad = ad;

    return va;  
}

struct variable_access_t *set_variable_access_method_designator(struct method_designator_t *md, char *recordname, struct expression_data_t *expr)
{
    struct variable_access_t *va = new_variable_access();
    va->type = VARIABLE_ACCESS_T_METHOD_DESIGNATOR;
    va->data.md = md;
    va->recordname = recordname;
    va->expr = expr;

    return va;
}

////////////////////////////////////////////////////////////////////////////////////////

struct variable_declaration_t *set_variable_declaration(struct identifier_list_t *il, struct type_denoter_t *tden, int line_number)
{
    struct variable_declaration_t *vd = new_variable_declaration();
    vd->il = il;
    vd->tden = tden;
    vd->line_number = line_number;
    vd->var_list = create_var_list(il, tden);

    return vd;
}

struct variable_declaration_list_t *set_variable_declaration_list(struct variable_declaration_t *vd, struct variable_declaration_list_t *next)
{
    struct variable_declaration_list_t *vdl = new_variable_declaration_list();
    vdl->vd = vd;
    vdl->next = next;
    vdl->vd->var_list = concat_var_lists(vd->var_list, next);

    return vdl;
}

struct while_statement_t *set_while_statement(struct expression_t *e, struct statement_t *s)
{
    
    struct while_statement_t *ws = new_while_statement();
    ws->e = e;
    ws->s = s;

    return ws;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/*            Helper method implemenatations              */
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void create_class_hash_table(struct class_list_t *class_list)
{
    while(class_list != NULL)
    {
       if(!check_against_reserved_words(class_list->ci->id, class_list->ci->line_number, ENTITY_CLASS))
        {
            check_for_same_name_vars(class_list->ci->id, class_list->cb->attribute_hash_table);
            add_class_to_cht(class_list);
        }
        class_list = class_list->next;
    }
}

void add_class_to_cht(struct class_list_t *class_list)
{
    struct class_table_t *class_t = create_class_node(class_list);
    add_class_to_hash_table(class_t);
}

void create_statement_hash_table(struct func_declaration_list_t *func_dec_list)
{
    while(func_dec_list != NULL)
    {
        add_statement_list_to_sht(func_dec_list->fd);

        func_dec_list = func_dec_list->next;
    }

}

void add_statement_list_to_sht(struct function_declaration_t *func_dec)
{
    struct statement_sequence_t *stat_seq = func_dec->fb->ss;
    while(stat_seq != NULL)
    {
        struct statement_table_t *statement = create_statement_node(stat_seq->s, func_dec, stat_seq->line_number);
        add_statement_to_hash_table(statement);

        stat_seq = stat_seq->next;
    }
}

int* create_attribute_hash_table(struct func_declaration_list_t *func_dec_list, struct variable_declaration_list_t *var_dec_list)
{
    /* dummy function dec to use in the key of NFV nodes */
    struct function_declaration_t *NFV_dummy_func_dec = (struct function_declaration_t*)(malloc(sizeof(NFV_dummy_func_dec)+1));
    int *arr = (int*)malloc(2*sizeof(int)+1);
    arr[0] = add_class_attrs_to_aht(var_dec_list, SCOPE_NFV, NFV_dummy_func_dec);
    arr[1] = add_class_funcs_to_aht(func_dec_list, NFV_dummy_func_dec, 0);
    return arr;
}

int add_class_attrs_to_aht(struct variable_declaration_list_t *var_dec_list, int type, struct function_declaration_t *dummy_func_dec)
{
    int order = 0;
    while(var_dec_list != NULL)
    {
        order = parse_var_dec(var_dec_list->vd, SCOPE_NFV, dummy_func_dec, order);
        var_dec_list = var_dec_list->next;
    }
    return order;
}

void parse_param_section(struct formal_parameter_section_t *param_section, int scope, struct function_declaration_t *func)
{
    struct identifier_list_t *id_list = param_section->il;
    while(id_list != NULL)
    {
        if(strcmp(id_list->id, func->fh->id) != 0)
        {
            struct type_denoter_t *type = generate_type_denoter(param_section->id);
            struct attribute_table_t *var = create_attribute_node(id_list->id,
                                                                  type,
                                                                  func->line_number,
                                                                  scope,
                                                                  FALSE,
                                                                  NULL,
                                                                  func,
                                                                  -1);

            add_attribute_to_hash_table(var, ENTITY_VARIABLE);
        }
        else
        {
            error_variable_name_invalid(func->line_number, id_list->id);
        }

        id_list = id_list->next;
    }
}

int parse_var_dec(struct variable_declaration_t *var_dec, int scope, struct function_declaration_t *func, int order)
{
    struct identifier_list_t *id_list = var_dec->il;
    while(id_list != NULL)
    {
        char *func_id = "";
        if(func->fh != NULL)
        {
            func_id = func->fh->id;
        }
        if(strcmp(id_list->id, func_id) != 0)
        {
            struct attribute_table_t *var = create_attribute_node(id_list->id,
                                                                  var_dec->tden,
                                                                  var_dec->line_number,
                                                                  scope,
                                                                  FALSE,
                                                                  NULL,
                                                                  func,
                                                                  order);
            add_attribute_to_hash_table(var, ENTITY_VARIABLE);
            order += 1;
        }
        else
        {
            error_variable_name_invalid(var_dec->line_number, id_list->id);
        }
        id_list = id_list->next;
    }
    return order;
}

int add_class_funcs_to_aht(struct func_declaration_list_t *func_dec_list, struct function_declaration_t *dummy_func_dec, int order)
{
    while(func_dec_list != NULL)
    {
        struct type_denoter_t *type = generate_type_denoter(func_dec_list->fd->fh->res);
        struct attribute_table_t *func = create_attribute_node(func_dec_list->fd->fh->id,
                                                               type,
                                                               func_dec_list->fd->line_number,
                                                               SCOPE_NFV,
                                                               TRUE,
                                                               func_dec_list->fd->fh->fpsl,
                                                               dummy_func_dec,
                                                               order);
        
        add_attribute_to_hash_table(func, ENTITY_FUNCTION);
        order += 1;

        add_func_var_to_aht(func_dec_list->fd->fb->vdl, SCOPE_FV, func_dec_list->fd);
        
        add_func_params_to_aht(func_dec_list->fd->fh->fpsl, SCOPE_FV, func_dec_list->fd);

        func_dec_list = func_dec_list->next;
    }
    return order;
}

void add_func_params_to_aht(struct formal_parameter_section_list_t *param_list, int scope, struct function_declaration_t *func)
{
    while(param_list != NULL)
    {
        parse_param_section(param_list->fps, scope, func);
        
        param_list = param_list->next;
    }
}


void add_func_var_to_aht(struct variable_declaration_list_t *var_dec_list, int scope, struct function_declaration_t *func)
{
    while(var_dec_list != NULL)
    {
        parse_var_dec(var_dec_list->vd, scope, func, -1);
        var_dec_list = var_dec_list->next;
    }
}

struct type_denoter_t* generate_type_denoter(char* return_type)
{
    struct type_denoter_t *type = (struct type_denoter_t*)malloc(sizeof(struct type_denoter_t)+1);
    if( (is_integer(return_type) || is_real(return_type) || is_boolean(return_type)) )
    {
        type->type = TYPE_DENOTER_T_IDENTIFIER;
    }
    else
    {
        type->type = -1;
    }

    type->name = (char*)malloc(strlen(return_type)*sizeof(char)+1);
    strcpy(type->name, return_type);
    return type;
}

void add_class_to_hash_table(struct class_table_t *class_t)
{
    // struct class_table_t *item_ptr;
    // HASH_FIND_STR(class_hash_table, class_t->id, item_ptr);
    // if(item_ptr == NULL)
    // {
    //     HASH_ADD_STR(class_hash_table, id, class_t);
    // }
    // else
    // {
    //     error_class_already_declared(class_t->line_number, class_t->id, item_ptr->line_number);
    // }
}

void add_statement_to_hash_table(struct statement_table_t *statement)
{
    // struct statement_table_t *item_ptr;
    // HASH_FIND_STR(stat_hash_table, statement->string_key, item_ptr);
    // if(item_ptr == NULL)
    // {
    //     HASH_ADD_STR(stat_hash_table, string_key, statement);
    // }   
}

void add_attribute_to_hash_table(struct attribute_table_t *attr, int entity_type)
{
    // if(check_against_reserved_words(attr->id, attr->line_number, entity_type))
    // {
    //     return; /*this was a reserved word*/
    // }

    // struct attribute_table_t *item_ptr = NULL;
    
    // HASH_FIND_STR(attr_hash_table, attr->string_key, item_ptr);
    // if(item_ptr == NULL)
    // {
    //     HASH_ADD_STR(attr_hash_table, string_key, attr);
    // }
    // else
    // {
    //     attribute_hash_table_error(item_ptr, attr);
    // }
}

int check_against_reserved_words(char* id, int line_number, int entity_type)
{
    if( !(strcmp(id, "this.") && strcmp(id, "integer") && strcmp(id, "real") && strcmp(id, "boolean") && !is_true(id) && !is_false(id)) )
    {
        switch(entity_type)
        {
            case ENTITY_CLASS:
                error_class_name_invalid(line_number, id);
                break;
            case ENTITY_FUNCTION:
                error_function_name_invalid(line_number, id);
                break;
            case ENTITY_VARIABLE:
                error_variable_name_invalid(line_number, id);
                break;
        }
        return TRUE;
    }
    return FALSE;
}

void attribute_hash_table_error(struct attribute_table_t *item_ptr, struct attribute_table_t *failed_attr)
{
    if(item_ptr->is_func)
    {
        error_function_already_declared(failed_attr->line_number, failed_attr->id, item_ptr->line_number);
    }
    else
    {
        error_variable_already_declared(failed_attr->line_number, failed_attr->id, item_ptr->line_number);   
    }
}

struct class_table_t* create_class_node(struct class_list_t *class_list)
{
    struct class_table_t *class_t = (struct class_table_t*)malloc(sizeof(struct class_table_t)+1);
    
    class_t->attribute_hash_table = class_list->cb->attribute_hash_table;
    class_t->statement_hash_table = class_list->cb->statement_hash_table;
    if(class_list->ci->extend != NULL)
    {
        class_t->extend = (struct class_table_t*)malloc(sizeof(struct class_table_t)+1);
        class_t->extend->id = (char*)malloc(sizeof(char)*strlen(class_list->ci->extend->id)+1);
        strcpy(class_t->extend->id, class_list->ci->extend->id);
    }

    class_t->line_number = class_list->ci->line_number;
    class_t->class_list = class_list;
    class_t->class_var_num = class_list->cb->class_var_num;
    class_t->class_func_num = class_list->cb->class_func_num;
    
    class_t->id = (char*)malloc(strlen(class_list->ci->id)*sizeof(char)+1);
    strcpy(class_t->id, class_list->ci->id);

    return class_t;
}

struct statement_table_t* create_statement_node(struct statement_t *stat, struct function_declaration_t *function, int line_number)
{
    struct statement_table_t *statement = (struct statement_table_t*)malloc(sizeof(struct statement_table_t)+1);
    
    statement->type = stat->type;
    statement->line_number = line_number;
    statement->function = function;
    statement->statement_data = (union statement_union*)malloc(sizeof(stat->data)+1);
    statement->statement_data = &stat->data;

    char *key = (char*)malloc(strlen(statement->function->fh->id) + strlen("1") + sizeof(char *)+1);
    strcpy(key, statement->function->fh->id);

    switch(statement->type)
    {
        case STATEMENT_T_ASSIGNMENT:
            strcat(key, "1");
            break;
        case STATEMENT_T_SEQUENCE:
            strcat(key, "2");
            break;
        case STATEMENT_T_IF:
            strcat(key, "3");
            break;
        case STATEMENT_T_WHILE:
            strcat(key, "4");
            break;
        case STATEMENT_T_PRINT:
            strcat(key, "5");
            break;
    }

    strcat(key, (char *)statement->statement_data);

    strcpy(statement->string_key, key);

    return statement;
}

struct attribute_table_t* create_attribute_node(char* id,
                                                struct type_denoter_t *type,
                                                int line_number, 
                                                int scope, 
                                                int is_func, 
                                                struct formal_parameter_section_list_t *params, 
                                                struct function_declaration_t *function,
                                                int order)
{
        struct attribute_table_t *func = (struct attribute_table_t*)malloc(sizeof(struct attribute_table_t)+1);
        strncpy(func->id, id, strlen(id));
        

        func->id_length = strlen(id);
        func->type = type;
        func->line_number = line_number;
        func->scope = scope;
        func->is_func = is_func;
        func->params = params;
        func->function = function;
        func->order = order;

        int func_id_length  = 0;
        if(function->fh != NULL)
        {
            func_id_length = strlen(function->fh->id);
        }
        char *key = (char *)malloc(strlen(id) + strlen("1") + func_id_length+1);
        strcpy(key, id);
        
        if(scope)
        {
            strcat(key, "1");
        }
        else
        {
            strcat(key, "0");
        }
        
        if(func_id_length)
        {
            strcat(key, function->fh->id);    
        }
        
        strcpy(func->string_key, key);

        return func;
}

char* format_attr_id(char id[], int id_length)
{
    int i;
    char *real_id = (char*)malloc(id_length+1);
    for(i=0;i<id_length;i++)
    {
        real_id[i] = id[i];
    }

    return real_id;
}

void print_statement_hash_table(struct statement_table_t *stat)
{
    // struct statement_table_t *t;
    // printf("/////////////////STATEMENT HAST TABLE/////////////////////\n");

    // for(t=stat; t!=NULL; t= t->hh.next)
    // {
    //     printf("STATEMENT TYPE: %d LINE_NUMBER: %d FUNCTION: %s DATA PTR: %p\n", 
    //             t->type, t->line_number, t->function->fh->id, t->statement_data);

    // }
    // printf("///////////////////////////////////////////////////////////\n");
}

void find_undefined_extends(struct class_list_t *class_list)
{
    // while(class_list != NULL)
    // {
    //     if(class_list->ci->extend != NULL)
    //     {
    //         if(class_list->ci->extend->extend_class == NULL)
    //         {
    //             error_type_not_defined(class_list->ci->line_number, class_list->ci->extend->id);
    //         }
    //         else if(class_list->ci->extend->extend_class->ci->extend != NULL && 
    //                 !strcmp(class_list->ci->id, class_list->ci->extend->extend_class->ci->extend->id))
    //         {
    //             error_circular_extends(class_list->ci->line_number, class_list->ci->id, class_list->ci->extend->id);
    //         }
    //         else
    //         {
    //             check_extend_attributes(class_list, class_list->ci->extend->extend_class);
    //         }
    //     }
    //     class_list = class_list->next;
    // }
}

void check_extend_attributes(struct class_list_t *base_class, struct class_list_t *parent_class)
{
    // struct variable_declaration_list_t *var_dec_list = base_class->cb->vdl;
    // struct identifier_list_t *id_list;

    // while(var_dec_list != NULL)
    // {
    //     id_list = var_dec_list->vd->il;
    //     while(id_list != NULL)
    //     {
    //         check_id_against_var_dec_list(id_list->id, parent_class, var_dec_list->vd->line_number);
    //         id_list = id_list->next;
    //     }
    //     var_dec_list = var_dec_list->next;
    // }
}

void check_id_against_var_dec_list(char *id, struct class_list_t *parent_class, int line_number)
{
    // struct variable_declaration_list_t *var_dec_list = parent_class->cb->vdl;
    // struct identifier_list_t *id_list;
    // while(var_dec_list != NULL)
    // {
    //     id_list = var_dec_list->vd->il;
    //     while(id_list != NULL)
    //     {
    //         if(!strcmp(id, id_list->id))
    //         {
    //             error_variable_name_invalid(line_number, id);
    //         }
    //         id_list = id_list->next;
    //     }
    //     var_dec_list = var_dec_list->next;
    // }
}

struct class_table_t* find_hash_object(char *class_list)
{
    // struct class_table_t *item_ptr = NULL;
    // if(class_list != NULL)
    // {
    //     HASH_FIND_STR(class_hash_table, class_list, item_ptr);
    // }
    // return item_ptr;
    return NULL;
}

void check_class_constructors(struct class_table_t *class_hash_table)
{
    // struct class_table_t *c;
    // struct attribute_table_t *attr_ptr;
    // for(c=class_hash_table; c != NULL; c=c->hh.next)
    // {

    //     HASH_FIND_STR(c->attribute_hash_table, create_attribute_key(c->id, SCOPE_NFV, NULL), attr_ptr);
    //     if(attr_ptr != NULL)
    //     {
    //         if(strcmp(attr_ptr->type->name, "VOID"))
    //         {
    //             error_constructor_cannot_have_return_type(attr_ptr->line_number, c->id);
    //         }
    //         attr_ptr->type->name = (char*)malloc(sizeof(char)*strlen(c->id)+1);
    //         strcpy(attr_ptr->type->name, c->id);
    //         attr_ptr->type->type = TYPE_DENOTER_T_CLASS_TYPE;
    //         attr_ptr->type->data = *((union type_data_union*)malloc(sizeof(struct class_list_t*))+1);
    //         attr_ptr->type->data.cl = c->class_list;
    //     }
    // }
}

char* create_attribute_key(char* id, int scope, char *function_id)
{
    int func_len = 0;
    if(function_id != NULL)
    {
        func_len = strlen(function_id);
    }

    char *key = (char *)malloc(strlen(id) + strlen("1") + func_len+1);
    strcpy(key, id);
        
    if(scope)
    {
        strcat(key, "1");
    }
    else
    {
        strcat(key, "0");
    }
    
    if(func_len)
    {
        strcat(key, function_id);    
    }
    
    return key;


}

struct expression_data_t* check_real_or_integer(struct expression_data_t *expr_1, struct expression_data_t *expr_2, int line_number)
{
    if( (is_integer(expr_1->type) || is_real(expr_1->type)) && (is_integer(expr_2->type) || is_real(expr_2->type)))
    {
        if(is_real(expr_1->type) || is_real(expr_2->type))
        {
            return set_expression_data(EXPRESSION_DATA_REAL, "real");
        }
        else
        {
            return set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
        }
    }
    else
    {
        if(!is_integer(expr_1->type) && !is_real(expr_1->type))
        {
            error_datatype_is_not(line_number, expr_1->type, "integer or real");
        }
        if(!is_integer(expr_2->type) && !is_real(expr_2->type))
        {
            error_datatype_is_not(line_number, expr_2->type, "integer or real");
        }
    }
    return set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
}

struct expression_data_t* check_boolean(struct expression_data_t *expr_1, struct expression_data_t *expr_2, int line_number)
{
    if(!is_boolean(expr_1->type))
    {
        error_datatype_is_not(line_number, expr_1->type, "boolean");
    }
    if(!is_boolean(expr_2->type))
    {
        error_datatype_is_not(line_number, expr_2->type, "boolean");
    }
    return set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");

}

void check_for_same_name_vars(char *class_id, struct attribute_table_t *attr_hash_table)
{
    // struct attribute_table_t *attr_ptr = NULL;
    // HASH_FIND_STR(attr_hash_table, create_attribute_key(class_id, SCOPE_NFV, NULL), attr_ptr);
    // if(attr_ptr != NULL && !attr_ptr->is_func)
    // {
    //     error_variable_name_invalid(attr_ptr->line_number, class_id);
    //     exit_on_errors();
    // }
}

void validate_class_attribute_types(struct class_table_t *class_hash_table)
{
    // struct class_table_t *c;
    // struct attribute_table_t *a;
    // for(c=class_hash_table; c != NULL; c=c->hh.next)
    // {
    //     if(c->extend != NULL)
    //     {  
    //         c->extend = find_hash_object(c->extend->id);   
    //     }
    //     for(a=c->attribute_hash_table; a != NULL; a=a->hh.next)
    //     {
    //         validate_type_denoter(a->type, class_hash_table, a->line_number);
    //     }
    // }
}

void validate_type_denoter(struct type_denoter_t *td, struct class_table_t *class_hash_table, int line_number)
{
    // switch(td->type)
    // {
    //     case TYPE_DENOTER_T_IDENTIFIER:
    //         break;
    //     case TYPE_DENOTER_T_ARRAY_TYPE:
    //         validate_type_denoter(td->data.at->td, class_hash_table, line_number);
    //         break;
    //     case TYPE_DENOTER_T_CLASS_TYPE:
    //         check_for_class_existence(td->name, class_hash_table, line_number);
    //         break;
    // }
}

void check_for_class_existence(char *id, struct class_table_t *class_hash_table, int line_number)
{
    // struct class_table_t *class_ptr = NULL;
    // HASH_FIND_STR(class_hash_table, id, class_ptr);
    // if(class_ptr == NULL)
    // {
    //     error_type_not_defined(line_number, id);
    //     exit_on_errors();
    // }
}

void print_hash_table(struct attribute_table_t* attr)
{
    // struct attribute_table_t *t;
    // printf("//////////////ATTRIBUTE HASH TABLE//////////////\n");
                   
    // for(t=attr; t != NULL; t=t->hh.next) 
    // {
    //     printf("TYPE: %s LINE NUMBER %d SCOPE: %d IS FUNCTION: %d ID: %s ", t->type->name, t->line_number, t->scope, t->is_func, format_attr_id(t->id, t->id_length));
    //     if(t->function->fh != NULL)
    //     {
    //         printf("FUNCTION NAME: %s ", t->function->fh->id);
    //     }
    //     if(t->params != NULL)
    //     {
    //         printf(" PARAMETERS: ");
    //         struct formal_parameter_section_list_t *p;
    //         for(p=t->params; p != NULL; p=p->next)
    //         {
    //             struct identifier_list_t *il;
    //             for(il=p->fps->il; il != NULL; il=il->next)
    //             {
    //                 printf("%s, ", il->id);
    //             }
    //             printf(" TYPE: %s ", p->fps->id);
    //         }
            
    //     }
    //     printf("\n");
    // }
    // printf("/////////////////////////////////\n\n");
}

void print_class_hash_table(struct class_table_t* class_table)
{
    // printf("******************* CLASS TABLE **********************\n");
    // struct class_table_t* cl;
    // for(cl=class_table; cl != NULL; cl=cl->hh.next)
    // {
    //     printf("ID: %s", cl->id);
    //     if(cl->extend != NULL)
    //     {
    //         printf(" EXTEND: %s", cl->extend->id);
    //     }
    //     printf("\n\n");
    // }

}

int is_boolean(char *id)
{
    return !(strcmp(id, "boolean"));
}

int is_integer(char *id)
{
    if(!(strcmp(id, "integer")))
    {
        return TRUE;
    }
    if(!(strcmp(id, "VAR")))
    {
        return TRUE;
    }
    return FALSE;
}

int is_real(char *id)
{
    return !(strcmp(id, "real"));
}

int is_array(char *id)
{
    return !(strcmp(id, "array"));
}

int is_true(char *id)
{
    if(!(strcmp(id, "true")))
    {
        return TRUE;
    }
    if(!(strcmp(id, "True")))
    {
        return TRUE;
    }
    if(!(strcmp(id, "TRUE")))
    {
        return TRUE;
    }
    return FALSE;
}

int is_false(char *id)
{
    if(!(strcmp(id, "false")))
    {
        return TRUE;
    }
    if(!(strcmp(id, "False")))
    {
        return TRUE;
    }
    if(!(strcmp(id, "FALSE")))
    {
        return TRUE;
    }
    return FALSE;
}


struct expression_data_t* identify_primitive_data(char *id)
{
    if(is_integer(id))
    {
        return set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
    }
    if(is_real(id))
    {
        return set_expression_data(EXPRESSION_DATA_REAL, "real");
    }
    if(is_boolean(id))
    {
        return set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
    }
    error_unknown(-1);
    exit_on_errors();
    return NULL;
}


/* -----------------------------------------------------------------------
 * Project 3
 * ----------------------------------------------------------------------- 
 */

 int calc_array_size(struct array_type_t *at)
 {
    return (at->r->max->ui - at->r->min->ui +1)*calc_var_size(at->td);
 }

int calc_var_size(struct type_denoter_t *tden)
{
    int var_size = 1;
    switch(tden->type)
    {
        case TYPE_DENOTER_T_IDENTIFIER:
        case TYPE_DENOTER_T_CLASS_TYPE:
            var_size = 1;
            break;
        case TYPE_DENOTER_T_ARRAY_TYPE:
            var_size = calc_array_size(tden->data.at);
            break;
    }

    return var_size;
}


char* get_var_type(struct type_denoter_t *tden)
{
    char *type;
    switch(tden->type)
    {
        case TYPE_DENOTER_T_IDENTIFIER:
        case TYPE_DENOTER_T_CLASS_TYPE:
            type = create_id_char(tden->name);
            break;
        case TYPE_DENOTER_T_ARRAY_TYPE:
            type = get_var_type(tden->data.at->td);
            break;
    }

    return type;
}



struct VarNode_c* create_var_list(struct identifier_list_t *il, struct type_denoter_t *tden)
{
    if (il != NULL)
    {
        struct VarNode_c *head = (struct VarNode_c*)malloc(sizeof(struct VarNode_c));
        head->name = create_id_char(il->id);
        head->type = get_var_type(tden);
        head->size = calc_var_size(tden);
        if(tden->type == TYPE_DENOTER_T_ARRAY_TYPE)
        {
            head->array = tden->data.at;
        }
        il = il->next;
        struct VarNode_c *current = head;

        while(il != NULL)
        {
            current->next = (struct VarNode_c*)malloc(sizeof(struct VarNode_c));
            current->next->name = create_id_char(il->id);
            current->next->type = get_var_type(tden);
            current->next->size = calc_var_size(tden);
            if(tden->type == TYPE_DENOTER_T_ARRAY_TYPE)
            {
                current->next->array = tden->data.at;
            }
            il = il->next;
            current = current->next;
        }

        return head;
    }
    else
    {
        return NULL;
    }
}

struct VarNode_c* concat_var_lists(struct VarNode_c *base, struct variable_declaration_list_t *vdl)
{
    if(vdl == NULL)
    {
        return base;   
    }

    struct VarNode_c *head = base;
    while(base->next != NULL)
    {
        base = base->next;
    }
    
    base->next = vdl->vd->var_list;

    return head;
}

struct ClassNode_c* create_and_concat(struct class_identification_t *ci, struct class_block_t *cb, struct class_list_t *next)
{
    struct ClassNode_c *class_to_add = create_ClassNode_c(ci, cb, next);
    
    if(next != NULL)
    {
        struct ClassNode_c *start = next->class_node_list;
        while(next->class_node_list->next != NULL)
        {
            next->class_node_list = next->class_node_list->next;
        }

        next->class_node_list->next = class_to_add;

        return start;
    }
    else
    {
        return class_to_add;
    }
}

struct ClassNode_c* create_ClassNode_c(struct class_identification_t *ci, struct class_block_t *cb, struct class_list_t *next)
{
    struct ClassNode_c *cnode = (struct ClassNode_c*)malloc(sizeof(struct ClassNode_c));
    cnode->name = create_id_char(ci->id);
    if(ci->extend != NULL)
    {
        cnode->parent = look_up_class(ci->extend->id, next);
    }
    cnode->attributes = cb->vdl->vd->var_list;
    cnode->size = calc_class_size(cnode->attributes, next);
    return cnode;
}

/* takes in the name of the parent class to find in the list of all classes and returns  */
/* a pointer to the class node */
struct ClassNode_c* look_up_class(char *id, struct class_list_t *cl)
{
    if(id == NULL || cl == NULL)
    {
        return NULL;
    } 
    struct ClassNode_c *cnode_l = cl->class_node_list;
    
    while(cnode_l != NULL)
    {
        if(!strcmp(id, cnode_l->name))
        {
            return cnode_l;
        }
        cnode_l = cnode_l->next;
    }

    printf("INSIDE OF look_up_class in shared.c\n");
    return NULL;
}

int calc_class_size(struct VarNode_c *attr_list, struct class_list_t *class_list)
{
    int size = 0;
    while (attr_list != NULL)
    {
        size += attr_list->size;
        attr_list = attr_list->next;
    }
    return size;
}

int is_primitive(char* id)
{
    if(is_integer(id) || is_boolean(id))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

char* create_id_char(char* id)
{
    char *new_id = (char*)malloc(sizeof(char)*strlen(id) + 1);
    strncpy(new_id, id, strlen(id) +1);
    return new_id;
}

/* -----------------------------------------------------------------------
 * Printout on error message and exit
 * ----------------------------------------------------------------------- 
 */

void exit_on_errors()
{
  if (error_flag == 1) {
    printf("Errors detected. Exiting.\n");
    exit(-1);
  }
}

void print_class_table(struct ClassNode_c *classes)
{
    while(classes != NULL)
    {
        printf("------------------------------\n");
        printf("CLASS: %s\n", classes->name);
        if(classes->parent != NULL)
            printf("PARENT: %s\n", classes->parent->name);
        printf("SIZE: %d\n", classes->size);
        printf("ATTRIBUTES:\n");
        struct VarNode_c *tmp = classes->attributes;
        while(tmp != NULL)
        {
            printf("\t%s %s\n", tmp->type, tmp->name);
            tmp = tmp->next; 
        }
        printf("\n\n");
        classes = classes->next;
    }
}



