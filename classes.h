#ifndef CLASSES_H
#define CLASSES_H

#include <vector>
#include <iostream>
#include "shared.h"
#include "control_flow.h"

using namespace std;

class VarNode {
public:
	string name;
	bool is_global = false; /* needed for part 2*/
	bool is_primitive = false;
	int offset = -1;
	int size = -1;
    array_type_t *array = NULL;
	string type; /* the class its from */
    bool is_var = false; /*for param vars only shows is pass by reference*/
    bool is_class_attr = false; /* signals if a var is a class attr */
    bool is_param = false; /*for knowing whether the offset is above or 
                              below mem[FP] for function calls, 
                              true --> var_location = mem[FP] - offset */
    bool is_used = false; /* signals tmp var hasnt been used yet */
};

class FuncNode {
public:
    string return_type; /*note _VOID is set if the method has no return type (in pascal.y)*/
    string name;
    string label;
    std::vector<BasicBlock*> cfg;
    std::vector<VarNode*> params;
    std::vector<VarNode*> vars;
    bool is_processed = false;
};

class ClassNode {
public:
	string name;
	ClassNode *parent;
	std::vector<VarNode*> attributes;
    std::vector<FuncNode*> functions;
	int size;
	bool is_primitive = false;
};


 class ListNode  {
    public:
    int val;
    ListNode * next;
 
};
#endif /* CLASSES_H */
