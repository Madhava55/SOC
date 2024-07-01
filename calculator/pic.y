%{
		// Declarations (C++ code)

    #include "pic.cc"
    extern "C" void yyerror(const char *s);
    extern int yylex(void);
	extern map<string, TreeNode*> table;
	#include <queue>
	#include <vector>
	void bfs(TreeNode* root) {
    if (root == NULL) return;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int levelSize = q.size(); 
        vector<TreeNode*> levelNodes;   

        for (int i = 0; i < levelSize; i++) {
            TreeNode* current = q.front();
            q.pop();
            levelNodes.push_back(current);
            if (current->left != NULL) q.push(current->left);
            if (current->right != NULL) q.push(current->right);
        }
        for (int i = 0; i < levelNodes.size(); i++) {
            if (i != 0) cout << " ";
            levelNodes[i]->print();
        }
        cout << endl;  
    }
	}
	vector<TreeNode*> ast;
%}

%union{
	string *name;	
	TreeNode* exp;
    vector<TreeNode*>* vector_TreeNodes;
}

%token INT_CONST FLT_CONST NAME LET 				// Token declarations (lexer uses these declarations to return tokens)

%left '+' '-' 														// Left associative operators '+' and '-'
%left '*' '/' 														// Left associative operators '*' and '/'
%right Uminus 														// Right associative unary minus operator

%type <name> INT_CONST FLT_CONST NAME LET '(' ')'	// Declare token types to be of type *string
%type <exp> expression 	stmt								// Declare the expression non-terminal to be of type *TreeNode
%type <vector_TreeNodes> expressions

%start program 														// Starting rule for the grammar
%%

/* GRAMMAR */

program
	:	stmts													{ for(int i=0;i<ast.size();i++){bfs(ast[i]); cout<<"printing done";cout<<endl;} for(auto i:table){cout<<i.first<<endl;}}		// A program consists of lsit of statements
;

stmts
	: stmts stmt      											{ ast.push_back($2); } 
	| stmt														{ ast.push_back($1); }

stmt
	: LET NAME '=' expression ';'								{ TreeNode* head = $4; table[*($2)]=head; $$=head;}		// Assignment statement
; 

expressions
    : expressions ',' expression                        { $1->push_back($3); $$ = $1; }
    | expression                                        { vector<TreeNode*>* vec = new vector<TreeNode*>(); vec->push_back($1); $$ = vec;}
;

expression
	: '(' expression ')'				                { $$ = $2; }
    | '[' expressions ']'                               { auto ret = new TreeNode(($2)); $$ = ret; }
	| expression '/' expression							{ auto ret = new TreeNode("/"); ret->left = $1; ret->right = $3; $$ = ret;}
	| expression '*' expression							{ auto ret = new TreeNode("*"); ret->left = $1; ret->right = $3; $$ = ret;}
	| expression '+' expression							{ auto ret = new TreeNode("+"); ret->left = $1; ret->right = $3; $$ = ret;}
	| expression '-' expression					 		{ auto ret = new TreeNode("-"); ret->left = $1; ret->right = $3; $$ = ret;}
	| '-' expression %prec Uminus						{ auto a = new Expression(0.0); auto ret = new TreeNode("-"); ret->left = new TreeNode(*a); ret->right = $2; $$ = ret;}
	| INT_CONST											{ auto a = new Expression(atoi($1->c_str())); $$ = new TreeNode(*a);}
	| FLT_CONST											{ auto a = new Expression(atof($1->c_str()));$$ = new TreeNode(*a); }
	| NAME												{ $$ = table[*($1)]; }
;

%%

/* ADDITIONAL C CODE */

int main() { // here's our main function, where it all starts
  yyparse();
  return 0;
}