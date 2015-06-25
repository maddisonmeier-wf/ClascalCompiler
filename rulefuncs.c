/*
* Maddison Meier and Derrick Rahbar Compilers fall 2014
*
*
*/


/*
 * rulefuncs.c
 *
 * Implements all functions that assist in developing the yacc rules
 */

#include "rulefuncs.h"
#include "shared.h"





/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new identifier_list
 * ----------------------------------------------------------------------- 
 */
struct identifier_list_t *new_identifier_list()
{
  struct identifier_list_t *il;

  il = (struct identifier_list_t *) malloc(sizeof(struct identifier_list_t));
  CHECK_MEM_ERROR(il)
  il->id = NULL;
  il->next = NULL;

  return il;
}



/* ----------------------------------------------------------------------- 
 * Adds an identifier to the end of the identifier_list
 * ----------------------------------------------------------------------- 
 */
void add_to_identifier_list(struct identifier_list_t **il, char *id)
{
  if (*il == NULL) {
    *il = new_identifier_list();
    (*il)->id = id;
  }
  else {
    struct identifier_list_t *temp;
    
    temp = *il;

    GOTO_END_OF_LIST(temp)
    temp->next = new_identifier_list();
    temp->next->id = id;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns the number of identifiers in an identifier list
 * ----------------------------------------------------------------------- 
 */
long identifier_list_count(struct identifier_list_t *il)
{
  struct identifier_list_t *temp_il;
  long count = 0;

  temp_il = il;
  while (temp_il != NULL){
    count++;
    temp_il = temp_il->next;
  }

  return count;
}


/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new identifier
 * ----------------------------------------------------------------------- 
 */
char *new_identifier(char *text)
{
 char *id;

 id = (char *) malloc(strlen(text) + 1); /* +1 for '\0' */
 CHECK_MEM_ERROR(id)
 strcpy(id, text);

 return id;
}



/* ----------------------------------------------------------------------- 
 * Adds a class_identification and block to the end of the class_list
 * ----------------------------------------------------------------------- 
 */
void add_to_class_list(struct class_list_t **cl, struct class_identification_t *ci, struct class_block_t *cb)
{
  if (*cl == NULL) {
    *cl = new_class_list();
    (*cl)->ci = ci;
    (*cl)->cb = cb;
  }
  else {
    struct class_list_t *temp;
    
    temp = *cl;

    GOTO_END_OF_LIST(temp)
    temp->next = new_class_list();
    temp->next->ci = ci;
    temp->next->cb = cb;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new identifier
 * ----------------------------------------------------------------------- 
 */
struct class_list_t *new_class_list()
{
  struct class_list_t *cl;

  cl = (struct class_list_t *) malloc(sizeof(struct class_list_t));
  CHECK_MEM_ERROR(cl)
  cl->ci = NULL;
  cl->cb = NULL;
  cl->next = NULL;

  return cl;
}


/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new class_identification
 * ----------------------------------------------------------------------- 
 */
struct class_identification_t *new_class_identification()
{
  struct class_identification_t *ci;

  ci = (struct class_identification_t*) malloc(sizeof(struct class_identification_t));
  CHECK_MEM_ERROR(ci)
  ci->id = NULL;
  ci->extend = NULL;

  return ci;
}

struct class_extend_t *new_class_extend()
{
  struct class_extend_t *ce;
  ce = (struct class_extend_t *)malloc(sizeof(struct class_extend_t));
  CHECK_MEM_ERROR(ce);
  ce->id = NULL;
  ce->extend_class = NULL;

  return ce;
}

/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new actual_parameter_list
 * ----------------------------------------------------------------------- 
 */
struct actual_parameter_list_t *new_actual_parameter_list()
{
  struct actual_parameter_list_t *apl;

  apl = (struct actual_parameter_list_t *)
	 malloc(sizeof(struct actual_parameter_list_t));
  CHECK_MEM_ERROR(apl)
  apl->ap = NULL;
  apl->next = NULL;

  return apl;
}


/* ----------------------------------------------------------------------- 
 * Adds an actual_parameter to the end of the actual_parameter_list
 * ----------------------------------------------------------------------- 
 */
void add_to_actual_parameter_list(struct actual_parameter_list_t **apl,
                                  struct actual_parameter_t *ap)
{
  if (*apl == NULL) {
    *apl = new_actual_parameter_list();
    (*apl)->ap = ap;
  }
  else {
    struct actual_parameter_list_t *temp;

    temp = *apl;

    GOTO_END_OF_LIST(temp)
    temp->next = new_actual_parameter_list();
    temp->next->ap = ap;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new type_denoter
 * ----------------------------------------------------------------------- 
 */
struct type_denoter_t *new_type_denoter()
{
  struct type_denoter_t *td;

  td = (struct type_denoter_t *)
    malloc(sizeof(struct type_denoter_t));
  CHECK_MEM_ERROR(td)
  td->type = -1;
  td->name = NULL;

  return td;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new variable_declaration_list
 * ----------------------------------------------------------------------- 
 */
struct variable_declaration_list_t *new_variable_declaration_list()
{
  struct variable_declaration_list_t *vdl;

  vdl = (struct variable_declaration_list_t *)
    malloc(sizeof(struct variable_declaration_list_t));
  CHECK_MEM_ERROR(vdl)
  vdl->vd = NULL;
  vdl->next = NULL;

  return vdl;
}



/* ----------------------------------------------------------------------- 
 * Adds a variable_declaration to the end of the variable_declaration_list
 * ----------------------------------------------------------------------- 
 */
void add_to_variable_declaration_list(struct variable_declaration_list_t **vdl,
				      struct variable_declaration_t *vd)
{
 if (*vdl == NULL) {
    *vdl = new_variable_declaration_list();
    (*vdl)->vd = vd;
  }
  else {
    struct variable_declaration_list_t *temp;

    temp = *vdl;

    GOTO_END_OF_LIST(temp)

    temp->next = new_variable_declaration_list();
    temp->next->vd = vd;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new range_t
 * ----------------------------------------------------------------------- 
 */
struct range_t *new_range()
{
  struct range_t *r;

  r = (struct range_t *) malloc(sizeof(struct range_t));
  CHECK_MEM_ERROR(r)
  r->min = NULL;
  r->max = NULL;

  return r;
}


/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new function_designator_t
 * ----------------------------------------------------------------------- 
 */
struct function_designator_t *new_function_designator()
{
  struct function_designator_t *fd;

  fd = (struct function_designator_t *) malloc(sizeof(struct function_designator_t));
  CHECK_MEM_ERROR(fd)
  fd->id = NULL;
  fd->apl = NULL;

  return fd;
}


/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new formal_parameter_section_list
 * ----------------------------------------------------------------------- 
 */
struct formal_parameter_section_list_t *new_formal_parameter_section_list()
{
  struct formal_parameter_section_list_t *fpsl;

  fpsl = (struct formal_parameter_section_list_t *)
    malloc(sizeof(struct formal_parameter_section_list_t));
  CHECK_MEM_ERROR(fpsl)
  fpsl->fps = NULL;
  fpsl->next = NULL;

  return fpsl;
}



/* ----------------------------------------------------------------------- 
 * Adds a formal_parameter_section to the end of the formal_parameter_section_list
 * ----------------------------------------------------------------------- 
 */
void add_to_formal_parameter_section_list(struct formal_parameter_section_list_t **fpsl,
					  struct formal_parameter_section_t *fps)
{
 if (*fpsl == NULL) {
    *fpsl = new_formal_parameter_section_list();
    (*fpsl)->fps = fps;
  }
  else {
    struct formal_parameter_section_list_t *temp;

    temp = *fpsl;

    GOTO_END_OF_LIST(temp)

    temp->next = new_formal_parameter_section_list();
    temp->next->fps = fps;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new variable_access
 * ----------------------------------------------------------------------- 
 */
struct variable_access_t *new_variable_access()
{
  struct variable_access_t *va;

  va = (struct variable_access_t *)
    malloc(sizeof(struct variable_access_t));
  CHECK_MEM_ERROR(va)
  va->type = -1;
  va->recordname = NULL;

  return va;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new object_instantiation
 * ----------------------------------------------------------------------- 
 */
struct object_instantiation_t *new_object_instantiation()
{
  struct object_instantiation_t *os;

  os = (struct object_instantiation_t *)
    malloc(sizeof(struct object_instantiation_t));
  CHECK_MEM_ERROR(os)
  os->id = NULL;
  os->apl = NULL;

  return os;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new assignment_statement
 * ----------------------------------------------------------------------- 
 */
struct assignment_statement_t *new_assignment_statement()
{
  struct assignment_statement_t *as;

  as = (struct assignment_statement_t *)
    malloc(sizeof(struct assignment_statement_t));
  CHECK_MEM_ERROR(as)
  as->va = NULL;
  as->e = NULL;

  return as;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new print_statement
 * ----------------------------------------------------------------------- 
 */
struct print_statement_t *new_print_statement()
{
  struct print_statement_t *ps;

  ps = (struct print_statement_t *) malloc(sizeof(struct print_statement_t));
  CHECK_MEM_ERROR(ps)
  ps->va = NULL;

  return ps;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new expression
 * ----------------------------------------------------------------------- 
 */
struct expression_t *new_expression()
{
  struct expression_t *e;

  e = (struct expression_t *) malloc(sizeof(struct expression_t));
  CHECK_MEM_ERROR(e)
  e->se1 = NULL;
  e->relop = -1;
  e->se2 = NULL;

  return e;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new statement
 * ----------------------------------------------------------------------- 
 */
struct statement_t *new_statement()
{
  struct statement_t *s;

  s = (struct statement_t*) malloc(sizeof(struct statement_t));
  CHECK_MEM_ERROR(s)
  s->type = -1;
  s->line_number = -1;

  return s;
}


/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new statement_sequence
 * ----------------------------------------------------------------------- 
 */

struct statement_sequence_t *new_statement_sequence()
{
  struct statement_sequence_t *ss;

  ss = (struct statement_sequence_t *)
    malloc(sizeof(struct statement_sequence_t));
  CHECK_MEM_ERROR(ss)
  ss->s = NULL;
  ss->next = NULL;

  return ss;
}



/* ----------------------------------------------------------------------- 
 * Adds a statement to the end of the statement_sequence
 * ----------------------------------------------------------------------- 
 */
void add_to_statement_sequence(struct statement_sequence_t **ss,
			       struct statement_t *s)
{
 if (*ss == NULL) {
    *ss = new_statement_sequence();
    (*ss)->s = s;
  }
  else {
    struct statement_sequence_t *temp;

    temp = *ss;

    GOTO_END_OF_LIST(temp)

    temp->next = new_statement_sequence();
    temp->next->s = s;
  }
}


struct function_block_t *new_function_block()
{
  struct function_block_t *fb;

  fb = (struct function_block_t *) malloc(sizeof(struct function_block_t));
  CHECK_MEM_ERROR(fb)
  fb->vdl = NULL;
  fb->ss = NULL;

  return fb;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new if_statement
 * ----------------------------------------------------------------------- 
 */
struct if_statement_t *new_if_statement()
{
  struct if_statement_t *is;

  is = (struct if_statement_t *) malloc(sizeof(struct if_statement_t));
  CHECK_MEM_ERROR(is)
  is->e = NULL;
  is->s1 = NULL;
  is->s2 = NULL;

  return is;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new while_statement
 * ----------------------------------------------------------------------- 
 */
struct while_statement_t *new_while_statement()
{
  struct while_statement_t *ws;

  ws = (struct while_statement_t *) malloc(sizeof(struct while_statement_t));
  CHECK_MEM_ERROR(ws)
  ws->e = NULL;
  ws->s = NULL;

  return ws;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new indexed_variable
 * ----------------------------------------------------------------------- 
 */
struct indexed_variable_t *new_indexed_variable()
{
  struct indexed_variable_t *iv;

  iv = (struct indexed_variable_t *) malloc(sizeof(struct indexed_variable_t));
  CHECK_MEM_ERROR(iv)
  iv->va = NULL;
  iv->iel = NULL;

  return iv;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new attribute_designator
 * ----------------------------------------------------------------------- 
 */
struct attribute_designator_t *new_attribute_designator()
{
  struct attribute_designator_t *fd;

  fd = (struct attribute_designator_t *) malloc(sizeof(struct attribute_designator_t));
  CHECK_MEM_ERROR(fd)
  fd->va = NULL;
  fd->id = NULL;

  return fd;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new method_designator
 * ----------------------------------------------------------------------- 
 */
struct method_designator_t *new_method_designator()
{
  struct method_designator_t *md;

  md = (struct method_designator_t *) malloc(sizeof(struct method_designator_t));
  CHECK_MEM_ERROR(md)
  md->va = NULL;
  md->fd = NULL;

  return md;
}


/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new index_expression_list
 * ----------------------------------------------------------------------- 
 */
struct index_expression_list_t *new_index_expression_list()
{
  struct index_expression_list_t *iel;

  iel = (struct index_expression_list_t *)
    malloc(sizeof(struct index_expression_list_t));
  CHECK_MEM_ERROR(iel)
  iel->e = NULL;
  iel->next = NULL;

  return iel;
}



/* ----------------------------------------------------------------------- 
 * Adds a index_expression to the end of the index_expression_list
 * ----------------------------------------------------------------------- 
 */
void add_to_index_expression_list(struct index_expression_list_t **iel,
				  struct expression_t *e)
{
 if (*iel == NULL) {
    *iel = new_index_expression_list();
    (*iel)->e = e;
  }
  else {
    struct index_expression_list_t *temp;

    temp = *iel;

    GOTO_END_OF_LIST(temp)

    temp->next = new_index_expression_list();
    temp->next->e = e;
  }
}


/* -----------------------------------------------------------------------
 * Returns the number of index_expression_list items that are nested
 * (pointing to each other)
 * -----------------------------------------------------------------------
 */
long index_expression_list_count(struct index_expression_list_t *iel)
{
  struct index_expression_list_t *temp_iel;
  long count = 0;

  temp_iel = iel;
  while (temp_iel != NULL){
    count++;
    temp_iel = temp_iel->next;
  }

  return count;

}

/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new actual_parameter
 * ----------------------------------------------------------------------- 
 */
struct actual_parameter_t *new_actual_parameter()
{
  struct actual_parameter_t *ap;

  ap = (struct actual_parameter_t *) malloc(sizeof(struct actual_parameter_t));
  CHECK_MEM_ERROR(ap)
  ap->e1 = NULL;
  ap->e2 = NULL;
  ap->e3 = NULL;

  return ap;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new simple_expression
 * ----------------------------------------------------------------------- 
 */
struct simple_expression_t *new_simple_expression()
{
  struct simple_expression_t *se;

  se = (struct simple_expression_t *)
    malloc(sizeof(struct simple_expression_t));
  CHECK_MEM_ERROR(se)
  se->t = NULL;
  se->addop = -1;
  se->next = NULL;

  return se;
}



/* ----------------------------------------------------------------------- 
 * Adds a term to the end of the simple_expression
 * ----------------------------------------------------------------------- 
 */
void add_to_simple_expression(struct simple_expression_t **se,
			      int addop,
			      struct term_t *t)
{
 if (*se == NULL) {
    *se = new_simple_expression();
    (*se)->t = t;
  }
  else {
    struct simple_expression_t *temp;

    temp = *se;

    GOTO_END_OF_LIST(temp)

    temp->next = new_simple_expression();
    temp->next->t = t;
    temp->addop = addop;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new term
 * ----------------------------------------------------------------------- 
 */
struct term_t *new_term()
{
  struct term_t *t;

  t = (struct term_t *)
    malloc(sizeof(struct term_t));
  CHECK_MEM_ERROR(t)
  t->f = NULL;
  t->mulop = -1;
  t->next = NULL;

  return t;
}



/* ----------------------------------------------------------------------- 
 * Adds a factor to the end of the term
 * ----------------------------------------------------------------------- 
 */
void add_to_term(struct term_t **t, int mulop, struct factor_t *f)
{
 if (*t == NULL) {
    *t = new_term();
    (*t)->f = f;
  }
  else {
    struct term_t *temp;

    temp = *t;

    GOTO_END_OF_LIST(temp)

    temp->next = new_term();
    temp->next->f = f;
    temp->mulop = mulop;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to the new factor
 * ----------------------------------------------------------------------- 
 */
struct factor_t *new_factor()
{
  struct factor_t *f;

  f = (struct factor_t *)
    malloc(sizeof(struct factor_t));
  CHECK_MEM_ERROR(f)
  f->type = -1;

  return f;
}

struct factor_data_t *new_factor_data()
{
  struct factor_data_t *fd;

  fd = (struct factor_data_t *)malloc(sizeof(struct factor_data_t));
  CHECK_MEM_ERROR(fd);
  fd->sign = -1;
  fd->next = NULL;

  return fd;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new sign
 * ----------------------------------------------------------------------- 
 */
int *new_sign()
{
 int *s;

 s = (int *) malloc(sizeof(int));
 CHECK_MEM_ERROR(s)

 return s;
}


/* ----------------------------------------------------------------------- 
 * Returns a pointer to the new primary
 * ----------------------------------------------------------------------- 
 */
struct primary_t *new_primary()
{
 struct primary_t *p;

 p = (struct primary_t *) malloc(sizeof(struct primary_t));
 CHECK_MEM_ERROR(p)
 p->type = -1;

 return p;
}


/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new unsigned_number
 * ----------------------------------------------------------------------- 
 */
struct unsigned_number_t *new_unsigned_number()
{
 struct unsigned_number_t *un;

 un = (struct unsigned_number_t *) malloc(sizeof(struct unsigned_number_t));
 CHECK_MEM_ERROR(un)

 return un;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new array_type
 * ----------------------------------------------------------------------- 
 */
struct array_type_t *new_array_type()
{
 struct array_type_t *at;

 at = (struct array_type_t *) malloc(sizeof(struct array_type_t));
 CHECK_MEM_ERROR(at)

 return at;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new class_block
 * ----------------------------------------------------------------------- 
 */
struct class_block_t *new_class_block()
{
 struct class_block_t *cb;

 cb = (struct class_block_t *) malloc(sizeof(struct class_block_t));
 CHECK_MEM_ERROR(cb)
 cb->vdl = NULL;
 cb->fdl = NULL;

 return cb;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new primitive_type
 * ----------------------------------------------------------------------- 
 */
char *new_primitive_type(char *type)
{
  char *t;
  t = (char *) malloc(strlen(type) + 1); /* +1 for '\0' */
  CHECK_MEM_ERROR(t)
  strcpy(t, type);

  return t;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new func_declaration_list
 * ----------------------------------------------------------------------- 
 */
struct func_declaration_list_t *new_func_declaration_list()
{
  struct func_declaration_list_t *fdl;

  fdl = (struct func_declaration_list_t *)
    malloc(sizeof(struct func_declaration_list_t));
  CHECK_MEM_ERROR(fdl)
  fdl->fd = NULL;
  fdl->next = NULL;

  return fdl;
}



/* ----------------------------------------------------------------------- 
 * Adds a function_declaration to the end of the func_declaration_list
 * ----------------------------------------------------------------------- 
 */
void add_to_func_declaration_list(struct func_declaration_list_t **fdl,
				  struct function_declaration_t *fd)
{
 if (*fdl == NULL) {
    *fdl = new_func_declaration_list();
    (*fdl)->fd = fd;
  }
  else {
    struct func_declaration_list_t *temp;

    temp = *fdl;

    GOTO_END_OF_LIST(temp)

    temp->next = new_func_declaration_list();
    temp->next->fd = fd;
  }
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new function_declaration
 * ----------------------------------------------------------------------- 
 */
struct function_declaration_t *new_function_declaration()
{
 struct function_declaration_t *fd;

 fd = (struct function_declaration_t *)
   malloc(sizeof(struct function_declaration_t));
 CHECK_MEM_ERROR(fd)
 fd->fh = NULL;
 fd->fb = NULL;
 fd->line_number = -1;

 return fd;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new function_heading
 * ----------------------------------------------------------------------- 
 */
struct function_heading_t *new_function_heading()
{
 struct function_heading_t *fh;

 fh = (struct function_heading_t *)
   malloc(sizeof(struct function_heading_t));
 CHECK_MEM_ERROR(fh)
 fh->id = NULL;
 fh->res = NULL;
 fh->fpsl = NULL;

 return fh;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new variable_declaration
 * ----------------------------------------------------------------------- 
 */
struct variable_declaration_t *new_variable_declaration()
{
 struct variable_declaration_t *vd;

 vd = (struct variable_declaration_t *)
   malloc(sizeof(struct variable_declaration_t));
 CHECK_MEM_ERROR(vd)
 vd->il = NULL; 
 vd->tden = NULL; 
 vd->line_number = -1;

 return vd;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new formal_parameter_section
 * ----------------------------------------------------------------------- 
 */
struct formal_parameter_section_t *new_formal_parameter_section()
{
 struct formal_parameter_section_t *fps;

 fps = (struct formal_parameter_section_t *)
   malloc(sizeof(struct formal_parameter_section_t));
 CHECK_MEM_ERROR(fps)
 fps->il = NULL; 
 fps->id = NULL; 
 fps->is_var = -1; 

 return fps;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new program
 * ----------------------------------------------------------------------- 
 */
struct program_t *new_program()
{
 struct program_t *p;

 p = (struct program_t *) malloc(sizeof(struct program_t));
 CHECK_MEM_ERROR(p)
 p->ph = NULL; 
 p->cl = NULL; 

 return p;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new program_heading
 * ----------------------------------------------------------------------- 
 */
struct program_heading_t *new_program_heading()
{
 struct program_heading_t *ph;

 ph = (struct program_heading_t *) malloc(sizeof(struct program_heading_t));
 CHECK_MEM_ERROR(ph)
 ph->id = NULL; 
 ph->il = NULL; 

 return ph;
}



/* ----------------------------------------------------------------------- 
 * Returns a pointer to a new expression_data
 * ----------------------------------------------------------------------- 
 */
struct expression_data_t *new_expression_data()
{
 struct expression_data_t *ed;

 ed = (struct expression_data_t *) malloc(sizeof(struct expression_data_t));
 CHECK_MEM_ERROR(ed)
 ed->val = -1; 
 ed->type = NULL; 

 return ed;
}

void assign_missing_extend(struct class_list_t *class_list)
{
    struct class_list_t *cl_head = class_list;
    while(class_list != NULL)
    {
        check_for_children(class_list, cl_head);
        class_list = class_list->next;
    }

}

void check_for_children(struct class_list_t *parent, struct class_list_t *cl_head)
{
    while(cl_head != NULL)
    {
        if(cl_head->ci->extend != NULL && !strcmp(cl_head->ci->extend->id, parent->ci->id))
        {
            cl_head->ci->extend->extend_class = parent;
        }
        cl_head = cl_head->next;
    }
}

/*
 * Ensures the name does not already exist as a class
*/
void check_class_duplicate(struct class_list_t *class_list, struct class_identification_t *class_name)
{
    while(class_list != NULL)
    {
      if(strcmp(class_list->ci->id, class_name->id) == 0)//current class_list name is equal
      {
        error_class_already_declared(class_name->line_number, class_name->id, class_list->ci->line_number);
        return;
      }

      class_list = class_list->next;
    }
}

void check_for_extend(struct class_list_t *class_list, struct class_identification_t *class_name)
{
    if(class_name->extend != NULL)
    {
      while(class_list != NULL)
      {
        if(strcmp(class_list->ci->id, class_name->extend->id) == 0)
        {
            //this is the class that our class is extending
            class_name->extend->extend_class = class_list;
            return;
        }

        class_list = class_list->next;
      }
    }
}



void check_id_duplicate(struct identifier_list_t *id_list, char *id, int line_number)
{
  while(id_list != NULL)
  {
    if(strcmp(id_list->id, id) == 0)
    {
      error_variable_already_declared(line_number, id, line_number);
      return;
    }

    id_list = id_list->next;
  }

}

int is_prim_data_type(char *id)
{
  if(strcmp("integer", id) ==0 || strcmp("boolean", id) == 0 || strcmp("real", id) == 0)
  {
    return 1;
  }

  return 0;
}
