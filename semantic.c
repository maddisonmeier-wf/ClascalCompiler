/*
* Maddison Meier and Derrick Rahbar Compilers fall 2014
*
*
*/


/*
 * semantic.c
 *
 * Implements all functions that participate in semantic analysis.
 */


#include "shared.h"
#include "semantic.h"
#include "rulefuncs.h"
#include "symtab.h"

struct class_table_t *g_class_table_head = NULL;

/* ----------------------------------------------------------------------- 
 * Carries out semantic analysis on a program
 * ----------------------------------------------------------------------- 
 */
void semantic_analysis(struct program_t *p)
{
    struct class_table_t *ct;
    struct statement_table_t *st;

    g_class_table_head = p->class_hash_table;
    /*analyze all classes*/               
    for(ct= p->class_hash_table; ct != NULL; ct=ct->hh.next) 
    {
        /*validate all statements written in the class */
        for(st=ct->statement_hash_table; st!=NULL; st=st->hh.next)
        {
            switch(st->type)
            {
                case STATEMENT_T_ASSIGNMENT:
                    validate_assignment_statement(st, ct->attribute_hash_table, st->function);
                    break;
                case STATEMENT_T_SEQUENCE:
                    validate_statement_sequence(st, ct->attribute_hash_table, st->function);
                    break;
                case STATEMENT_T_IF:
                    validate_if_statement(st, ct->attribute_hash_table, st->function);
                    break;
                case STATEMENT_T_WHILE:
                    validate_while_statement(st, ct->attribute_hash_table, st->function);
                    break;
                case STATEMENT_T_PRINT:
                    validate_print_statement(st, ct->attribute_hash_table, st->function);
                    break;
            }
        }
    }
}

void validate_assignment_statement(struct statement_table_t* statement, 
                                   struct attribute_table_t* attr_hash_table, 
                                   struct function_declaration_t *statement_func)
{
    struct assignment_statement_t *a_stat = statement->statement_data->as;

    if(a_stat->va->type == VARIABLE_ACCESS_T_IDENTIFIER)
    {
        if(is_true(a_stat->va->data.id) || is_false(a_stat->va->data.id))
        {
            error_datatype_is_not(statement->line_number, "boolean", "variable");
        }
        if(!strcmp(a_stat->va->data.id, statement_func->fh->id))
        {
            if(a_stat->e != NULL)
            {
                struct expression_data_t *expr_type = get_expr_expr_data(a_stat->e, attr_hash_table, statement_func, statement->line_number);
                if(structurally_equivalent(set_expression_data(-1, statement_func->fh->res), expr_type))   
                {
                    return;
                }
                else
                {
                    error_datatype_is_not(statement->line_number, expr_type->type, statement_func->fh->res);
                }
            }
            else
            {
                struct expression_data_t *obj_type = get_obj_inst_expr_data(a_stat->oe, attr_hash_table, statement_func, statement->line_number);
                if(structurally_equivalent(set_expression_data(-1, statement_func->fh->res), obj_type))
                {
                    return;
                }
                else
                {
                    error_datatype_is_not(statement->line_number, obj_type->type, statement_func->fh->res);
                } 
            }
            return;
        }
    }

    struct expression_data_t *va_type = get_va_expr_data(a_stat->va, attr_hash_table, statement_func, statement->line_number);

    if(a_stat->e != NULL)
    {    
        struct expression_data_t *expr_type = get_expr_expr_data(a_stat->e, attr_hash_table, statement_func, statement->line_number);

        if(!structurally_equivalent(va_type, expr_type)) /*types are different*/
        {
            struct class_table_t *class_check = NULL;
            HASH_FIND_STR(g_class_table_head, va_type->type, class_check);
            if(class_check != NULL)
            {
                if(class_check->extend != NULL && !strcmp(class_check->extend->id, expr_type->type))
                {
                    return;
                }
            }
            error_type_mismatch(statement->line_number, va_type->type, expr_type->type);
        }
    }
    else /*has an object_instantiation*/
    {
        struct expression_data_t *obj_inst_type = get_obj_inst_expr_data(a_stat->oe, attr_hash_table, statement_func, statement->line_number);

        if(!structurally_equivalent(va_type, obj_inst_type)) /*types are different*/
        {
           struct class_table_t *class_check = NULL;
            HASH_FIND_STR(g_class_table_head, va_type->type, class_check);
            if(class_check != NULL)
            {
                if(class_check->extend != NULL && !strcmp(class_check->extend->id, obj_inst_type->type))
                {
                    return;
                }
            }
            error_type_mismatch(statement->line_number, va_type->type, obj_inst_type->type);
        }

    }
}

void validate_statement_sequence(struct statement_table_t* statement,
                                 struct attribute_table_t* attr_hash_table, 
                                 struct function_declaration_t *statement_func)
{
    struct statement_sequence_t *stat_seq = statement->statement_data->ss;

    while(stat_seq != NULL)
    {
        eval_statement(stat_seq->s, attr_hash_table, statement_func); 
        stat_seq = stat_seq->next;
    }
}

void validate_if_statement(struct statement_table_t* statement, 
                           struct attribute_table_t* attr_hash_table, 
                           struct function_declaration_t *statement_func)
{
    struct if_statement_t *if_stat = statement->statement_data->is;

    struct expression_data_t *expr_type = get_expr_expr_data(if_stat->e, attr_hash_table, statement_func, statement->line_number);

    if(strcmp("boolean", expr_type->type)) /*expression is not boolean*/
    {
        error_datatype_is_not(statement->line_number, expr_type->type, "boolean");
    }
    
    /*validate both statements */
    eval_statement(if_stat->s1, attr_hash_table, statement_func);
    eval_statement(if_stat->s2, attr_hash_table, statement_func);
}

void validate_while_statement(struct statement_table_t* statement,
                              struct attribute_table_t* attr_hash_table, 
                              struct function_declaration_t *statement_func)
{
    struct while_statement_t *while_stat = statement->statement_data->ws;

    char *expr_type = get_expr_expr_data(while_stat->e, attr_hash_table, statement_func, statement->line_number)->type;

    if(strcmp("boolean", expr_type)) /*expression is not boolean*/
    {
        error_datatype_is_not(statement->line_number, expr_type, "boolean");
    }

    /*validate statement*/
    eval_statement(while_stat->s, attr_hash_table, statement_func);
}

void validate_print_statement(struct statement_table_t* statement, 
                              struct attribute_table_t* attr_hash_table, 
                              struct function_declaration_t *statement_func)
{
    struct print_statement_t *pr_stat = statement->statement_data->ps;

    get_va_expr_data(pr_stat->va, attr_hash_table, statement_func, statement->line_number); /*validating will happen within here*/

}

struct statement_table_t *create_statement_table(struct statement_t *statement)
{
    struct statement_table_t* statement_table = (struct statement_table_t*)malloc(sizeof(struct statement_table_t));

    statement_table->line_number = statement->line_number;
    statement_table->statement_data = (union statement_union *)malloc(sizeof(*statement));

    switch(statement->type)
    {
        case STATEMENT_T_ASSIGNMENT:
            statement_table->statement_data->as = statement->data.as;
            break;
        case STATEMENT_T_SEQUENCE:
            statement_table->statement_data->ss = statement->data.ss;
            break;
        case STATEMENT_T_IF:
            statement_table->statement_data->is = statement->data.is;
            break;
        case STATEMENT_T_WHILE:
            statement_table->statement_data->ws = statement->data.ws;
            break;
        case STATEMENT_T_PRINT:
            statement_table->statement_data->ps = statement->data.ps;
            break;
    }
    
    return statement_table;
}

void eval_statement(struct statement_t *statement,
                    struct attribute_table_t* attr_hash_table, 
                    struct function_declaration_t *statement_func)
{
    switch(statement->type) /*must wrap these in statement_table_t structs*/
    {
        case STATEMENT_T_ASSIGNMENT:
            validate_assignment_statement(create_statement_table(statement), attr_hash_table, statement_func);
            break;
        case STATEMENT_T_SEQUENCE:
            validate_statement_sequence(create_statement_table(statement), attr_hash_table, statement_func);
            break;
        case STATEMENT_T_IF:
            validate_if_statement(create_statement_table(statement), attr_hash_table, statement_func);
            break;
        case STATEMENT_T_WHILE:
            validate_while_statement(create_statement_table(statement), attr_hash_table, statement_func);
            break;
        case STATEMENT_T_PRINT:
            validate_print_statement(create_statement_table(statement), attr_hash_table, statement_func);
            break;
    }
}


/*GRAMMAR GET TYPES*/
struct expression_data_t* get_expr_expr_data(struct expression_t *expr, 
                                             struct attribute_table_t* attr_hash_table, 
                                             struct function_declaration_t *statement_func, 
                                             int line_number)
{
    if(expr->expr != NULL && expr->expr->type != NULL)
    {
        return expr->expr; /*we already have type*/
    }
    else if(expr->se2 == NULL) /* if se2 is null then the expression type is the type of se1 */
    {
        return get_simple_expr_expr_data(expr->se1, attr_hash_table, statement_func, line_number);
    }
    else
    {
        struct expression_data_t *se1_data = get_simple_expr_expr_data(expr->se1, attr_hash_table, statement_func, line_number);
        struct expression_data_t *se2_data = get_simple_expr_expr_data(expr->se2, attr_hash_table, statement_func, line_number);
        switch(expr->relop)
        {
            case RELOP_EQUAL:
            case RELOP_NOTEQUAL:
                if(strcmp(se1_data->type, se2_data->type))
                {
                    error_type_mismatch(line_number, se1_data->type, se2_data->type);
                }
                break;
            case RELOP_LT:
            case RELOP_GT:
            case RELOP_LE:
            case RELOP_GE:
                if( !is_integer(se1_data->type) && !is_real(se1_data->type) )
                {
                    error_datatype_is_not(line_number, se1_data->type, "real or integer");
                }
                if( !is_integer(se2_data->type) && !is_real(se2_data->type) )
                {
                    error_datatype_is_not(line_number, se2_data->type, "real or integer");
                }
                break;
        }
        return set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
    }
}

struct expression_data_t* get_obj_inst_expr_data(struct object_instantiation_t *obj_inst, 
                                                 struct attribute_table_t* attr_hash_table, 
                                                 struct function_declaration_t *statement_func, 
                                                 int line_number)
{
    struct class_table_t *class = NULL;
    struct attribute_table_t *function = NULL;
    HASH_FIND_STR(g_class_table_head, obj_inst->id, class);

    if(class == NULL)
    {
        error_type_not_defined(line_number, obj_inst->id);
        exit_on_errors();
        return NULL;
    }
    else
    {
        /* If there are params (aka its not using default constructor) */
        if(obj_inst->apl != NULL)
        {
            char *lookup_key = create_attribute_key(obj_inst->id, SCOPE_NFV, NULL);
            HASH_FIND_STR(class->attribute_hash_table, lookup_key, function);
            if(function != NULL && function->is_func)
            {
                check_param_list_against_function(obj_inst->apl, function->params, obj_inst->id, attr_hash_table, statement_func, line_number);
            }
            else
            {
                error_function_not_declared(line_number, obj_inst->id);
            }
        }
        return set_expression_data(-1, obj_inst->id);
    }
}

struct expression_data_t* get_va_expr_data(struct variable_access_t* va, 
                                           struct attribute_table_t *attr_hash_table, 
                                           struct function_declaration_t *func, 
                                           int line_number)
{
    switch(va->type)
    {
        case VARIABLE_ACCESS_T_IDENTIFIER:
            return evaluate_va_identifier(va, attr_hash_table, func, line_number);
            break;
        case VARIABLE_ACCESS_T_INDEXED_VARIABLE:
            return evaluate_va_indexed_var(va, attr_hash_table, func, line_number);
            break;
        case VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR:
            return evaluate_va_attribute_designator(va, attr_hash_table, func, line_number);
            break;
        case VARIABLE_ACCESS_T_METHOD_DESIGNATOR:
            return evaluate_va_method_designator(va, attr_hash_table, func, line_number);
            break;
    }
    error_unknown(line_number);
    exit_on_errors();
    return NULL;
}

struct expression_data_t* get_simple_expr_expr_data(struct simple_expression_t *simple_expression, 
                                                    struct attribute_table_t* attr_hash_table, 
                                                    struct function_declaration_t *statement_func, 
                                                    int line_number)
{
    if(simple_expression->expr != NULL)
    {
        return simple_expression->expr;
    }

    struct expression_data_t *term = get_term_expr_data(simple_expression->t, attr_hash_table, statement_func, line_number);
    if(simple_expression->next == NULL)
    {
        return term;
    }
    else
    {
        struct expression_data_t *se = get_simple_expr_expr_data(simple_expression->next, attr_hash_table, statement_func, line_number);
        switch(simple_expression->addop)
        {
            /* both must be real or integer */
            case ADDOP_PLUS:
            case ADDOP_MINUS:
                return check_real_or_integer(se, term, line_number);
                break;
            /* both must be boolean */
            case ADDOP_OR:
                return check_boolean(se, term, line_number);
                break;
        }
        error_unknown(line_number);
        exit_on_errors();
        return NULL;
    }

}

struct expression_data_t* get_term_expr_data(struct term_t *term, 
                                             struct attribute_table_t* attr_hash_table, 
                                             struct function_declaration_t *statement_func,
                                             int line_number)
{
    if(term->expr != NULL)
    {
        return term->expr;
    }
    struct expression_data_t *factor = get_factor_expr_data(term->f, attr_hash_table, statement_func, line_number);
    if(term->next == NULL)
    {
        return factor;
    }
    else
    {
        struct expression_data_t *sub_term = get_term_expr_data(term->next, attr_hash_table, statement_func, line_number);
        switch(term->mulop)
        {
            case MULOP_STAR:
            case MULOP_SLASH:
                return check_real_or_integer(sub_term, factor, line_number);
                break;
            case MULOP_MOD:
                if(!is_integer(sub_term->type))
                {
                    error_datatype_is_not(line_number, sub_term->type, "integer");
                }
                if(!is_integer(factor->type))
                {
                    error_datatype_is_not(line_number, sub_term->type, "integer");
                }
                return set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
                break;
            case MULOP_AND:
                return check_boolean(sub_term, factor, line_number);
                break;
        }
        error_unknown(line_number);
        exit_on_errors();
        return NULL;
    }
}

struct expression_data_t* get_factor_expr_data(struct factor_t *factor, 
                                               struct attribute_table_t* attr_hash_table, 
                                               struct function_declaration_t *statement_func,
                                               int line_number)
{
    if(factor->expr != NULL)
    {
        return factor->expr;
    }
    switch(factor->type)
    {
        /* if it has a sign then it must be an integer or real */
        case FACTOR_T_SIGNFACTOR:
            if(factor->data.f->next->type != FACTOR_T_PRIMARY)
            {
                error_too_many_signs(line_number);
            }
            struct expression_data_t *f_expr = get_factor_expr_data(factor->data.f->next, attr_hash_table, statement_func, line_number);
            return check_real_or_integer(f_expr, set_expression_data(EXPRESSION_DATA_INTEGER, "integer"), line_number);
            break;
        case FACTOR_T_PRIMARY:
            return get_primary_expr_data(factor->data.p, attr_hash_table, statement_func, line_number);
            break;
    }
    error_unknown(line_number);
    exit_on_errors();
    return NULL;
}

struct expression_data_t* get_primary_expr_data(struct primary_t* primary, 
                                                struct attribute_table_t* attr_hash_table, 
                                                struct function_declaration_t *statement_func,
                                                int line_number)
{
    if(primary->expr != NULL)
    {
        return primary->expr;
    }
    switch(primary->type)
    {
        case PRIMARY_T_VARIABLE_ACCESS:
            return get_va_expr_data(primary->data.va, attr_hash_table, statement_func, line_number);
            break;
        case PRIMARY_T_UNSIGNED_CONSTANT:
            return set_expression_data(EXPRESSION_DATA_INTEGER, "integer");
            break;
        case PRIMARY_T_FUNCTION_DESIGNATOR:
            return get_func_des_expr_data(primary->data.fd, attr_hash_table, statement_func, line_number);
            break;
        case PRIMARY_T_EXPRESSION:
            return get_expr_expr_data(primary->data.e, attr_hash_table, statement_func, line_number);           
            break;
        case PRIMARY_T_PRIMARY:
            make_call(primary, attr_hash_table, statement_func, line_number);
            struct expression_data_t *sub_primary = make_call(primary, attr_hash_table, statement_func, line_number);
            return check_boolean(sub_primary, set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean"), line_number);
            break;
    }
    error_unknown(line_number);
    exit_on_errors();
    return NULL;
}

struct expression_data_t* make_call(struct primary_t* primary, 
                                                struct attribute_table_t* attr_hash_table, 
                                                struct function_declaration_t *statement_func,
                                                int line_number)
{
    return get_primary_expr_data(primary->data.next, attr_hash_table, statement_func, line_number);
}

struct expression_data_t* get_func_des_expr_data(struct function_designator_t *func_des, 
                                                 struct attribute_table_t* attr_hash_table, 
                                                 struct function_declaration_t *statement_func, 
                                                 int line_number)
{
    struct attribute_table_t* func_ptr = NULL;
    HASH_FIND_STR(attr_hash_table, create_attribute_key(func_des->id, SCOPE_NFV, NULL), func_ptr);
    if(func_ptr != NULL && func_ptr->is_func)
    {
        check_param_list_against_function(func_des->apl, func_ptr->params, func_des->id, attr_hash_table, statement_func, line_number);
        return convert_td_to_expr_data(func_ptr->type, func_ptr->expr, line_number);
    }
    else
    {
        error_function_not_declared(line_number, func_des->id);
        exit_on_errors();
        return NULL;
    }
}

int structurally_equivalent(struct expression_data_t *expr_1, struct expression_data_t *expr_2)
{
    return check_for_equivalence(expr_1, expr_2);
}

int check_for_equivalence(struct expression_data_t *expr_1, struct expression_data_t *expr_2)
{
    if(is_primitive(expr_1->type) || is_primitive(expr_2->type))
    {
        return check_primitive_equivalence(expr_1->type, expr_2->type);
    }
    if(is_array(expr_1->type) || is_array(expr_2->type))
    {
        return check_array_equivalence(expr_1, expr_2);
    }
    return check_class_equivalence(expr_1, expr_2);
}

int check_class_equivalence(struct expression_data_t *expr_1, struct expression_data_t *expr_2)
{
    if(!strcmp(expr_1->type, expr_2->type))
    {
        return TRUE;
    }
    /* Line number is no longer relevant class retrieval */
    struct attribute_table_t *eat_1 = get_class_attr_table(expr_1->type, -1);
    struct attribute_table_t *eat_2 = get_class_attr_table(expr_2->type, -1);

    struct class_table_t *ect_1 = NULL;
    struct class_table_t *ect_2 = NULL;
    
    HASH_FIND_STR(g_class_table_head, expr_1->type, ect_1);
    HASH_FIND_STR(g_class_table_head, expr_2->type, ect_2);

    if(ect_1 == NULL || ect_2 == NULL)
    {
        error_unknown(-10);
        exit_on_errors();
    }
    if((ect_1->class_var_num != ect_2->class_var_num) || (ect_1->class_func_num != ect_2->class_func_num))
    {
        return FALSE;
    }

    struct type_denoter_t *expr_1_var_array[ect_1->class_var_num];
    struct function_declaration_t *expr_1_func_array[ect_1->class_func_num];
    struct type_denoter_t *expr_1_func_types[ect_1->class_func_num];
    
    struct attribute_table_t *a1;
    
    for(a1=eat_1; a1 != NULL; a1 = a1->hh.next)
    {
        if(a1->scope == SCOPE_NFV)
        {
            if(a1->is_func)
            {
                expr_1_func_array[a1->order] = a1->function;
                expr_1_func_types[a1->order] = a1->type;
            }
            else
            {
                expr_1_var_array[a1->order] = a1->type;
            }
        }
    }

    struct type_denoter_t *expr_2_var_array[ect_2->class_var_num];
    struct function_declaration_t *expr_2_func_array[ect_2->class_func_num];
    struct type_denoter_t *expr_2_func_types[ect_2->class_func_num];

    struct attribute_table_t *a2;
    for(a2=eat_2; a2 != NULL; a2 = a2->hh.next)
    {
        if(a2->scope == SCOPE_NFV)
        {
            if(a2->is_func)
            {
                expr_2_func_array[a2->order] = a2->function;
                expr_2_func_types[a2->order] = a2->type;
            }
            else
            {
                expr_2_var_array[a2->order] = a2->type;
            }
        }
    }

    int i;
    for(i=0; i<ect_2->class_var_num; i++)
    {
        struct type_denoter_t *t1 = expr_1_var_array[i];
        struct type_denoter_t *t2 = expr_2_var_array[i];

        if((t1->type == TYPE_DENOTER_T_CLASS_TYPE) && (t2->type == TYPE_DENOTER_T_CLASS_TYPE))
        {
            /* We arent supporting recursive equivalence so if one class has an attribute that is the type of the other class */
            /* We assume they are not equivalent */
            if(!strcmp(t1->name, expr_2->type))
            {
                return FALSE;
            }
            if(!strcmp(t2->name, expr_1->type))
            {
                return FALSE;
            }
        }
        
        struct expression_data_t *e_var_1 = set_expression_data(-1, t1->name);
        if(t1->type == TYPE_DENOTER_T_ARRAY_TYPE)
        {
            e_var_1->array = t1->data.at;
        }
       
        struct expression_data_t *e_var_2 = set_expression_data(-1, t2->name);
        if(t2->type == TYPE_DENOTER_T_ARRAY_TYPE)
        {
            e_var_2->array = t2->data.at;
        }
        if(!structurally_equivalent(e_var_1, e_var_2))
        {
            return FALSE;
        }
    }

    for(i=0; i<ect_2->class_func_num; i++)
    {
        struct function_declaration_t *t1 = expr_1_func_array[i];
        struct function_declaration_t *t2 = expr_2_func_array[i];

        if(strcmp(t1->fh->id, t2->fh->id))
        {
            return FALSE;
        }

        if(!formal_param_lists_equal(t1->fh->fpsl, t2->fh->fpsl))
        {
            return FALSE;
        }

        if((expr_1_func_types[i]->type == TYPE_DENOTER_T_CLASS_TYPE) && (expr_2_func_types[i]->type == TYPE_DENOTER_T_CLASS_TYPE))
        {
            if(!strcmp(t1->fh->id, expr_2->type) || !strcmp(t2->fh->id, expr_1->type))
            {
                return FALSE;
            }

            struct expression_data_t *func_expr_1 = set_expression_data(-1, t1->fh->id);
            struct expression_data_t *func_expr_2 = set_expression_data(-1, t2->fh->id);

            if(!structurally_equivalent(func_expr_1, func_expr_2))
            {
                return FALSE;
            }
        }

        /* At this point, we know that atleast one function return type is not a class */
        if(strcmp(t1->fh->res, t2->fh->res))
        {
            return FALSE;
        }

    }
    if(ect_1->extend != NULL && ect_2->extend != NULL)
    {
        return structurally_equivalent(set_expression_data(-1, ect_1->extend->id), set_expression_data(-1, ect_2->extend->id));
    }
    if(ect_1->extend != NULL || ect_2->extend != NULL)
    {
        return FALSE;
    }
    return TRUE;
}

int formal_param_lists_equal(struct formal_parameter_section_list_t *fpl_1, struct formal_parameter_section_list_t *fpl_2)
{
    while(fpl_1 != NULL && fpl_2 != NULL)
    {
        if(!formal_param_section_equal(fpl_1->fps, fpl_2->fps))
        {
            return FALSE;
        }
        fpl_1 = fpl_1->next;
        fpl_2 = fpl_2->next;
    }
    if(fpl_1 != NULL || fpl_2 != NULL)
    {
        return FALSE;
    }
    return TRUE;
}

int formal_param_section_equal(struct formal_parameter_section_t *fps_1, struct formal_parameter_section_t *fps_2)
{
    if(strcmp(fps_1->id, fps_2->id))
    {
        return FALSE;
    }
    if(fps_1->is_var != fps_2->is_var)
    {
        return FALSE;
    }
    /* id list names do not have to be the same (i.e x,y :int is the same as g, b :int) */
    while(fps_1->il != NULL && fps_2->il != NULL)
    {
        fps_1->il =fps_1->il->next;
        fps_2->il =fps_2->il->next;
    }
    if(fps_1->il != NULL || fps_2->il != NULL)
    {
        return FALSE;
    }
    return TRUE;
}

int check_array_equivalence(struct expression_data_t *expr_1, struct expression_data_t *expr_2)
{
    if(!is_array(expr_1->type) || !is_array(expr_2->type))
    {
        return FALSE;
    }
    
    struct array_type_t *a1 = expr_1->array; 
    struct array_type_t *a2 = expr_2->array;

    if( (a1->r->max->ui - a1->r->min->ui) !=  (a2->r->max->ui - a2->r->min->ui))
    {
        return FALSE;
    }
    else
    {
        struct expression_data_t *ae1 = set_expression_data(-1, a1->td->name);
        if(a1->td->type == TYPE_DENOTER_T_ARRAY_TYPE)
        {
            ae1->array = a1->td->data.at;
        }


        struct expression_data_t *ae2 = set_expression_data(-1, a2->td->name);
        if(a2->td->type == TYPE_DENOTER_T_ARRAY_TYPE)
        {
            ae2->array = a2->td->data.at;
        }

        return structurally_equivalent(ae1, ae2);
    }
}

int is_primitive(char *id)
{
    if(is_integer(id) || is_real(id) || is_boolean(id))
    {
        return TRUE;
    }
    return FALSE;
}

int check_primitive_equivalence(char *t1, char *t2)
{
    if(is_integer(t1) && is_integer(t2))
    {
        return TRUE;
    }
    return !strcmp(t1, t2);
}

void check_param_list_against_function(struct actual_parameter_list_t *apl, 
                                       struct formal_parameter_section_list_t *fpl, 
                                       char *id,
                                       struct attribute_table_t* attr_hash_table, 
                                       struct function_declaration_t *statement_func,
                                       int line_number)
{
    struct identifier_list_t *id_list;
    while(apl != NULL && fpl != NULL)
    {
        id_list = fpl->fps->il;
        while(id_list != NULL && apl != NULL)
        {
            if(!structurally_equivalent(set_expression_data(-1, fpl->fps->id), 
                get_expr_expr_data(apl->ap->e1, attr_hash_table, statement_func, line_number)))
            {
                error_function_not_declared(line_number, id);
                return;
            }
            apl = apl->next;
            id_list = id_list->next;
        }
        fpl = fpl->next;
    }
    if(apl != NULL || fpl != NULL || id_list != NULL)
    {
        error_function_not_declared(line_number, id);
    }
}


int has_this(char *id)
{
    char this[5];
    strncpy(this, id, 5);
    if(!strcmp(this, "this."))
    {
        return TRUE;
    }
    return FALSE;
}

char* get_class_id_from_this(char *id)
{
    char *class_id = (char*)malloc((strlen(id)-5)*sizeof(char));
    int i;
    for(i=5; i < strlen(id); i++)
    {
        class_id[i-5] = id[i];
    }
    return class_id;

}

struct expression_data_t* evaluate_va_identifier(struct variable_access_t *va, 
                                                 struct attribute_table_t *attr_hash_table,
                                                 struct function_declaration_t *func, 
                                                 int line_number)
{
    struct expression_data_t *va_expr;
    char *id = va->data.id;
    if(is_true(id) || is_false(id))
    {
        return set_expression_data(EXPRESSION_DATA_BOOLEAN, "boolean");
    }
    if(strlen(id) > 5 && has_this(id))
    {
        id = get_class_id_from_this(id);
        va_expr = evaluate_va_class_var(id, attr_hash_table, line_number);
        if(va_expr == NULL)
        {
            error_variable_not_declared(line_number, id);
            exit_on_errors();   
        }
        else
        {
            return va_expr;
        }
    }

    va_expr = evaluate_va_class_var(id, attr_hash_table, line_number);
    if(va_expr != NULL)
    {
        return va_expr;
    }

    va_expr = evaluate_va_function_var(id, attr_hash_table, func, line_number);
    if(va_expr != NULL)
    {
        return va_expr;
    }

    error_variable_not_declared(line_number, id);
    exit_on_errors();
    return NULL;
}

struct expression_data_t* evaluate_va_function_var(char *id, 
                                                   struct attribute_table_t* attr_hash_table, 
                                                   struct function_declaration_t *func, 
                                                   int line_number)
{
    struct attribute_table_t *attr_ptr = NULL;

    HASH_FIND_STR(attr_hash_table, create_attribute_key(id, SCOPE_FV, func->fh->id), attr_ptr);
    if(attr_ptr == NULL)
    {
        return NULL;
    }
    else
    {
        return convert_td_to_expr_data(attr_ptr->type, attr_ptr->expr, line_number);
    }
}

struct expression_data_t* evaluate_va_class_var(char *id, struct attribute_table_t *attr_hash_table, int line_number)
{
    struct attribute_table_t *attr_ptr = NULL;
    HASH_FIND_STR(attr_hash_table, create_attribute_key(id, SCOPE_NFV, NULL), attr_ptr);
    if(attr_ptr != NULL && !attr_ptr->is_func)
    {
        return convert_td_to_expr_data(attr_ptr->type, attr_ptr->expr, line_number);
    }
    else
    {
        return NULL;
    }
}

struct expression_data_t* convert_td_to_expr_data(struct type_denoter_t *type, struct expression_data_t *expr, int line_number)
{ 
    if(expr != NULL)
    {
        return expr;
    }
    switch(type->type)
    {
        case TYPE_DENOTER_T_ARRAY_TYPE:
            expr = set_expression_data(-1, "array");
            expr->array = type->data.at;
            return expr;
            break;
        case TYPE_DENOTER_T_CLASS_TYPE:
            check_for_class_existence(type->name, g_class_table_head, line_number);
            expr = set_expression_data(-1, type->name);
            return expr;
            break;
        case TYPE_DENOTER_T_IDENTIFIER:
            expr = identify_primitive_data(type->name);
            return expr;
            break;
    }
    error_unknown(line_number);
    exit_on_errors();
    return NULL;
}

struct expression_data_t *evaluate_va_indexed_var(struct variable_access_t *var, 
                                                  struct attribute_table_t *attr_hash_table,
                                                  struct function_declaration_t *func, 
                                                  int line_number)
{
    struct indexed_variable_t *index_var = var->data.iv;

    if(index_var->expr != NULL)
    {
        return index_var->expr;
    }

    struct expression_data_t *va = get_va_expr_data(index_var->va, attr_hash_table, func, line_number);

    if(!is_array(va->type))
    {
        error_datatype_is_not(line_number, va->type, "array");
        exit_on_errors();
    }

    struct expression_data_t *index_expr_type = get_iel_expr_data(index_var->iel, attr_hash_table, func, line_number);
    if(!is_integer(index_expr_type->type))
    {
        error_datatype_is_not(line_number, index_expr_type->type, "integer");
    }

    /* if index was a single literal, the value will be populated and we can check for bounds */
    if(!strcmp(index_expr_type->type, "VAR"))
    {
        if(!(va->array->r->min->ui <= index_expr_type->val && va->array->r->max->ui >= index_expr_type->val))
        {
            error_array_index_out_of_bounds(line_number, index_expr_type->val, va->array->r->min->ui, va->array->r->max->ui);
        }    
    }
    return convert_td_to_expr_data(va->array->td, NULL, line_number);
}

struct expression_data_t* get_iel_expr_data(struct index_expression_list_t *iel, 
                                            struct attribute_table_t *attr_hash_table,
                                            struct function_declaration_t *func, 
                                            int line_number)
{
    /* Based on assumptions.txt we are assuming iel->next will always be null */
    if(iel->expr != NULL)
    {
        return iel->expr;
    }
    return get_expr_expr_data(iel->e, attr_hash_table, func, line_number);
}

struct expression_data_t* evaluate_va_attribute_designator(struct variable_access_t *va, 
                                                           struct attribute_table_t *attr_hash_table,
                                                           struct function_declaration_t *func, 
                                                           int line_number)
{
    struct attribute_designator_t *attr_des = va->data.ad;
    struct expression_data_t *sub_va = get_va_expr_data(attr_des->va, attr_hash_table, func, line_number);
    struct attribute_table_t *classes_attr_table = get_class_attr_table(sub_va->type, line_number);

    struct attribute_table_t *va_attr_node = NULL;
    HASH_FIND_STR(classes_attr_table, create_attribute_key(attr_des->id, SCOPE_NFV, NULL), va_attr_node);

    if(va_attr_node == NULL || va_attr_node->is_func)
    {
        error_variable_not_declared(line_number, attr_des->id);
        exit_on_errors();
    }

    return convert_td_to_expr_data(va_attr_node->type, va_attr_node->expr, line_number);
}

struct expression_data_t *evaluate_va_method_designator(struct variable_access_t *va, 
                                                        struct attribute_table_t *attr_hash_table,
                                                        struct function_declaration_t *func, 
                                                        int line_number)
{
    struct method_designator_t *meth_des = va->data.md;
    struct expression_data_t *meth_va = get_va_expr_data(meth_des->va, attr_hash_table, func, line_number);

    struct expression_data_t *meth_fd = get_func_des_expr_data(meth_des->fd, get_class_attr_table(meth_va->type, line_number), func, line_number);

    return meth_fd;

}

struct attribute_table_t* get_class_attr_table(char *id, int line_number)
{
    struct class_table_t *class_ptr = NULL;
    HASH_FIND_STR(g_class_table_head, id, class_ptr);
    if(class_ptr == NULL)
    {
        error_type_not_defined(line_number, id);
        exit_on_errors();
    }
    return class_ptr->attribute_hash_table;
}








