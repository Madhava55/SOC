#ifndef PIC_HH
#define PIC_HH
#include <map>
#include <cstdio>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
using namespace std;

class TreeNode;
class block;      
class expr_coll; 

union value {
	double a;
	string *b;
	char c;
	bool boolean;
};

struct Expression {
	int etype;
	value v;
	Expression();
	~Expression(){};
	Expression(double x = 0) {
		etype = 0;
		v.a = x;
	}
	Expression(int x = 0) {
		etype = 0;
		v.a = x;
	}
	Expression(string *x) {
		etype = 1;
		v.b = x;
	}

	Expression(char x) {
		etype = 2;
		v.c = x;
	}

	Expression(bool x) {
		etype = 3;
		v.boolean = x;
	}

	void get_value() {
		switch (etype) {
			case 0:
				cout<<v.a;
				break;
			case 1:
				cout<<v.b;
				break;
			case 2:
				cout<<v.c;
				break;
			case 3:
				cout<<v.boolean;
				break;
			default:
            cout << "Unknown type: " << etype << endl;
			cout<<"this etype is the problem "<<etype<<endl;
			printf("Error: get_value\n");
			exit(1);
		}
	}
};

union object {
    Expression exp;
    string *b;
	TreeNode* node;
	vector<TreeNode*>* vec;
	const char *ch; 
    object() {} 
    ~object() {} 
    object(Expression& e) : exp(e) {}
    object(string *s) : b(s) {}
	object(const char *s) : ch(s) {}
	object(vector<TreeNode*>* v) : vec(v) {}
	object(TreeNode* n) : node(n) {}
};

enum StatementType {
	START,   // start of program... instantiated only in the beginning and takes vector of
	STMT_BLOCK, // collection of statement/asts, takes vector of statements as input and just holds them
    STMT_FUNC,  // takes vector of arguments and execute as input
    STMT_FOR,   //	takes 2 nodes again...condition and execute
    STMT_WHILE,  //  takes 2 nodes again...condition and execute
    STMT_IF_ELSE,  //  takes condition and execute as input (2 tree nodes)..note that presently else has to be included always
	STMT_ASSIGN,	// let statmement
	EXPR_COLL,		// collection of expression separated by comma
    STMT_EXPRESSION,  // simple expression including integers, stings, sum , ...basically string of operations
};

class TreeNode {
public:
	double comp=0;
	int type=99;
    object obj; 
	TreeNode* left;
    TreeNode* right;
	StatementType stmtType; 
	TreeNode() : left(nullptr), right(nullptr) {}
    ~TreeNode() {}
	virtual void print(){}
};


class assign : public TreeNode{
public:
	assign(){stmtType = STMT_ASSIGN;}
	~assign(){stmtType = STMT_ASSIGN;}
	assign(TreeNode* name, TreeNode* expression){left=name; right=expression; stmtType = STMT_ASSIGN;}
	void print() override {cout<<right->comp<<endl;}
};

class express : public TreeNode{
public:
    express() {type = 6; stmtType = STMT_EXPRESSION;} 
    ~express() { delete obj.b; stmtType = STMT_EXPRESSION;}
    express(Expression e){
		type = 0;
		obj.exp = e;
		stmtType = STMT_EXPRESSION;
	} 
    express(string *s)
	{
		type=1;
		obj.b=s;
		stmtType = STMT_EXPRESSION;
	}  
	express(const char *s)
	{
		type=8;
		obj.ch=s;
		stmtType = STMT_EXPRESSION;
	} 
	express(vector<TreeNode*>* v){
		type = 2;
		obj.vec=v;
		stmtType = STMT_EXPRESSION;
	} 

	express(block* blk);
	express(expr_coll* expr);

	express(express* ex){
		type = 3;
		right=ex;
		stmtType = STMT_EXPRESSION;
	}

    void print() override {
        if (type == 0) {
            obj.exp.get_value();
        } else if (type == 1) {
            cout<< *obj.b;
        } else if(type==2){
			cout << "[ ";
			for (int i=0; i < obj.vec->size(); i++)
				{(*(obj.vec))[i]->obj.exp.get_value(); if(i<obj.vec->size()-1)cout<<",";}
			cout <<"]"<<endl;
		}
		else if(type == 8)
		{
			cout<<*obj.ch;
		}
		else 
		{
			obj.node->print();
		}
    }
};

class For : public TreeNode{
public:
	For(){stmtType = STMT_FOR;}
	~For(){stmtType = STMT_FOR;}
	For(TreeNode* condition, TreeNode* execute){left=condition; right=execute; stmtType = STMT_FOR;}
};


class While : public TreeNode{
public:
	While(){stmtType = STMT_WHILE;}
	~While(){stmtType = STMT_WHILE;}
	While(TreeNode* condition, TreeNode* execute){left=condition; right=execute; stmtType = STMT_WHILE;}
};

class if_else : public TreeNode{
public:
	
	if_else(){stmtType = STMT_IF_ELSE;}
	~if_else(){stmtType = STMT_IF_ELSE;}
	if_else(express* condition, TreeNode* this_exp, TreeNode* that_exp){obj.node=condition; left=this_exp; right=that_exp; stmtType = STMT_IF_ELSE;}
	void print() override{
		cout<<"condition is ";
		obj.node->print();
		cout<<endl;
		cout<<"if: ";
		left->print();
		cout<<endl;
		cout<<"else: ";
		right->print();
		cout<<endl;
	}
};

class functions : public TreeNode{
public:
	functions(){stmtType = STMT_FUNC;}
	~functions(){stmtType = STMT_FUNC;}
	functions(vector<TreeNode*>* args, TreeNode* execute){obj.vec=args; right=execute; stmtType = STMT_FUNC;}
};


class expr_coll : public TreeNode{
public:
	expr_coll(){stmtType = EXPR_COLL;}
	~expr_coll(){stmtType = EXPR_COLL;}
	expr_coll(vector<TreeNode*>* expr){obj.vec=expr; stmtType = EXPR_COLL;}
	void push (TreeNode* expr)
		{obj.vec->push_back(expr);
		stmtType = EXPR_COLL;}
	void print() override{
		cout << "[";
			for (int i=0; i < obj.vec->size(); i++)
				{(*(obj.vec))[i]->obj.exp.get_value(); if(i<obj.vec->size()-1)cout<<",";}
			cout <<"]"<<endl;
	}
};

class block : public TreeNode{
public:
	block(){stmtType = STMT_BLOCK;}
	~block(){stmtType = STMT_BLOCK;}
	block(vector<TreeNode*>* stmts){obj.vec=stmts; stmtType = STMT_BLOCK;}
	void push (TreeNode* stmt) {obj.vec->push_back(stmt);}
};


class programm : public TreeNode{
public:
	programm(){stmtType = START;}
	~programm(){stmtType = START;}
	programm(block* stmts) { left = NULL; obj.node = stmts; stmtType = START; }
};

// OTHER DECLARATIONS

void bfs(TreeNode* root);

#endif  //PIC_HH