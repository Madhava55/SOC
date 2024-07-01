#ifndef PIC_HH
#define PIC_HH
#include <map>
#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

class TreeNode;

union value {
	double a;
	string *b;
};


struct Expression {
	int etype;
	value v;
	Expression();
	~Expression();
	Expression(double x = 0) {
		etype = 0;
		v.a = x;
	}

	Expression(string *x) {
		etype = 1;
		v.b = x;
	}

	double get_value() {
		switch (etype)
		{
			case 0:
				return v.a;
			case 1:
				break;
			default:
				break;
		}

		printf("Error: get_value\n");
		exit(1);
	}
};
class TreeNode;

union object {
    Expression exp;
    string *b;
	vector<TreeNode*>* vec;
    object() {} 
    ~object() {} 
    object(Expression& e) : exp(e) {}
    object(string *s) : b(s) {}
	object(vector<TreeNode*>* v) : vec(v) {}
	object(const char* s) : b(new string(s)) {}
};

class TreeNode {
public:
    int type;
    object obj;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : type(0), obj(), left(nullptr), right(nullptr) {}

    ~TreeNode() {
        delete obj.b; 
    }

    TreeNode(Expression& e) : type(0), obj(e), left(nullptr), right(nullptr) {} 
    TreeNode(string *s) : type(1), obj(s), left(nullptr), right(nullptr) {} 
	TreeNode(const char* s): type(1), obj(s), left(nullptr), right(nullptr) {} 
	TreeNode(vector<TreeNode*>* v): type(2), obj(v), left(nullptr), right(nullptr) {} 

    void print() {
        if (type == 0) {
            cout<<obj.exp.get_value();
        } 
		else if(type == 1){
            cout<< *obj.b;
        }
		else{
			cout << "Array is ";
			for(int i=0;i<obj.vec->size();i++)
			{
				cout<<(*(obj.vec))[i]->obj.exp.get_value()<<" , ";
			}
			cout <<endl;
		}
    }
};
extern map<string,TreeNode*> table;

#endif // PIC_HH