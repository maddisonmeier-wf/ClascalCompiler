#ifndef CONTROL_FLOW_H
#define CONTROL_FLOW_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include "shared.h"

#define STAT_PLUS 0
#define STAT_MINUS 1
#define STAT_STAR 2
#define STAT_SLASH 3
#define STAT_MOD 4
#define STAT_EQUAL 5
#define STAT_NOTEQUAL 6
#define STAT_LT 7
#define STAT_GT 8
#define STAT_LE 9
#define STAT_GE 10
#define STAT_AND 11
#define STAT_OR 12
#define STAT_NONE -1

#define STAT_SIGN_POSITIVE 1
#define STAT_SIGN_NEGATIVE -1

#define TERM_TYPE_CONST 0
#define TERM_TYPE_VAR 1

using namespace std;


class Term {
	public:
		int type = -1;
		int sign =0; /* ALSO USED TO SIGNAL A NOT */
		union
		{
			int constant;
			variable_access_t *var;
		}data;

		~Term()
		{
			delete[] data.var;
			cout <<"\n\n TERM DESTRUCTOR CALLED \n\n";
		}
};


class RHS {
	public:
		Term *t1;
		int op = STAT_NONE;
		Term *t2;
		bool is_new = false; /*the assignment was a new Class name will be held in class_name*/
		string class_name;

		~RHS()
		{
			delete t1;
			delete t2;
			cout <<"\n\n RHS DESTRUCTOR CALLED \n\n";
		}
};

class Val_obj {
	public:
		int val_num;
		bool is_const;
	~Val_obj(){
		cout <<"\n\n Val_obj DESTRUCTOR CALLED \n\n";
	}
};

class BasicBlock;
class Statement {
	public:
		variable_access_t *lhs;
		RHS *rhs;
		bool is_goto = false; /*if is goto the if val will be in lhs*/
		BasicBlock* goto_ptr;
		int goto_index = -1;
		bool is_print = false; /* is a print stat if true, the va will be held in lhs*/
		bool is_function_call = false; /* will be true if this is solely a function call statement 
											the rhs will be populated 
											NOTE if it is a static function call
											then the va of the md will be '#'*/
		variable_access_t* va; /*will be populated with the acutal call so you can
									find out return type*/
		bool is_return_assign = false; /* will be true if this is the statement 
										that assigns the return register to the temp var
										lhs will be the temp var expecting the value and
										rhs will be NULL */
										/* OR signals a goto statement is the return stat
											from a function call, nothing will be populated*/



	~Statement()
	{
		delete[] lhs;
		delete rhs;
		cout <<"\n\n Statement DESTRUCTOR CALLED \n\n";
	}
};

class BasicBlock {
	public:
		std::vector<int> parents;
		std::vector<int> children;
		std::vector<BasicBlock*> parents_ptrs;
		std::vector<BasicBlock*> children_ptrs;
		std::vector<Statement*> statements;
		int extended_bb; //ranged from 1-n 
		string label;
		bool is_processed = false; /*needed for while blocks who should process their condition node parents */

	~BasicBlock()
	{
		cout <<"\n\n BasicBlock DESTRUCTOR CALLED \n\n";
		statements.clear();
	}
};

void give_bbs_label_names(int offset);
statement_sequence_t * reverse_ss(statement_sequence_t* ss);
void print_statement_list(statement_sequence_t* ss);
std::vector<BasicBlock*> create_CFG(statement_sequence_t *ss, program_t *program);
void populate_goto_ptrs();
void populate_par_child_ptrs();
void remove_dummy_nodes();
void add_statements_to_cfg(statement_sequence_t *ss);
void process_statement(statement_t *s);
void add_while_statement_to_cfg(while_statement_t *ws);
void add_statements_to_cfg(statement_sequence_t *ss);
void add_print_statement_to_cfg(print_statement_t *ps);
void add_assignment_to_cfg(assignment_statement_t *as);
void add_if_statement_to_cfg(if_statement_t *ifs);
void add_condition_to_bb(expression_t *expr);
RHS* get_rhs_from_expr(expression_t *expr);
void add_next_bb(std::vector<int> parents);
void add_goto_statement(int current_index, int goto_index, variable_access_t *if_var);
int add_if_body_to_cfg(statement_t *st, int parent);
variable_access_t* create_temp_id();
variable_access_t* create_id(char* id);
bool is_3_address_code(expression_t *expr);
Term* gen_term_from_expr(expression_t *expr);
Term* gen_term_from_se(simple_expression_t *se);
int relop_to_statop(int relop);
variable_access_t* create_and_insert_stat(RHS *rhs);
Term* create_temp_term(variable_access_t* id);
RHS* IN3ADD_gen_rhs_from_3_add_expr(expression_t *expr);
std::vector<Term*> IN3ADD_get_terms_from_expr(expression_t *expr);
std::vector<Term*> IN3ADD_get_terms_from_se(simple_expression_t *se);
std::vector<Term*> IN3ADD_get_terms_from_term(term_t *t);
std::vector<Term*> IN3ADD_get_terms_from_factor(factor_t *f);
std::vector<Term*> IN3ADD_get_terms_from_primary(primary_t *p);
Term* gen_term_from_term(term_t *t);
Term* gen_term_from_factor(factor_t *f);
Term* create_negative_factor_term(factor_t *f);
RHS* IN3ADD_gen_rhs_from_3_add_expr(expression_t *expr);
std::vector<Term*> IN3ADD_get_terms_from_expr(expression_t *expr);
std::vector<Term*> IN3ADD_get_terms_from_se(simple_expression_t *se);
std::vector<Term*> IN3ADD_get_terms_from_term(term_t *t);
std::vector<Term*> IN3ADD_get_terms_from_factor(factor_t *f);
std::vector<Term*> IN3ADD_get_terms_from_primary(primary_t *p);
int mulop_to_statop(int mulop);
int addop_to_statop(int addop);
char * print_var_access(variable_access_t* va);
void print_CFG(std::vector<BasicBlock*> cfg);
int term_term_count(term_t *t);
int factor_term_count(factor_t *f);
int primary_term_count(primary_t *p);
int expr_term_count(expression_t *expr);
int se_term_count(simple_expression_t *se);
Term* gen_term_from_primary(primary_t *p);
bool expr_not_constant(expression_t *e);
expression_t* replace_expr_with_term_id(variable_access_t *va);
expression_t *gen_expr_for_index_va(indexed_variable_t *iv);
char * print_expr(expression_t* e);
std::vector<BasicBlock*> create_FuncCFG(statement_sequence_t* ss, int current_ff_num, string return_type);


#endif /* CONTROL_FLOW_H */
