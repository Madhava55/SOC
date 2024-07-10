%{
    #include "pic.cc"
    extern "C" void yyerror(const char *s);
    extern int yylex(void);
	extern map<string, express*> table;
%}

%union{
	string *name;	
	express* exp;
	TreeNode* tree;
	expr_coll* wrap_exp;
	block* stmt_wrap;
	vector<TreeNode*>* vector_TreeNodes;
}

%token INT_CONST FLT_CONST NAME LET FOR WHILE IF ELSE FUNC NOT 				// Token declarations (lexer uses these declarations to return tokens)

%left '+' '-' 											// Left associative operators '+' and '-'
%left '*' '/' 											// Left associative operators '*' and '/'
%right Uminus NOT										// Right associative unary minus operator
%nonassoc '<' '>' "==" NOT_EQUAL


%type <name> INT_CONST FLT_CONST NAME LET '(' ')' '{' '}' '[' ']' 		// Declare token types to be of type *string
%type <exp> expression 								// Declare the expression non-terminal to be of type *TreeNode
%type <tree> stmt 
%type <stmt_wrap> stmts
%type <wrap_exp> expressions

%start program 											// Starting rule for the grammar
%%

/* GRAMMAR */

program
	: stmts	{ 
			programm *top = new programm($1); 
			for(int i=0;i<(*top->obj.node->obj.vec).size();i++){bfs((*top->obj.node->obj.vec)[i]);cout<<endl;}
			}
;

stmts
	: stmts stmt { 
			$1->push($2); 
			$$ = $1;
			} 
	| stmt	{ 
			vector<TreeNode*>* vec = new vector<TreeNode*>();
			vec->push_back($1);
			$$ = new block(vec); 	
			}

stmt
	: LET NAME '=' expression ';'						{ auto ret = new express(($2)); assign* node = new assign(ret,$4);table[*($2)]=$4; $$ = node;}
	| NAME ';'     	 									{ cout<<table[*($1)]->comp<<endl; }
	| FOR '(' expression ')' expression					{ For* loop = new For($3,$5); $$ = loop; }
	| WHILE '(' expression ')' expression				{ While* loop = new While($3,$5); $$ = loop; }
	| IF '(' expression ')' expression ELSE expression	{ if_else* loop = new if_else($3,$5,$7); $$ = loop; }
; 

expressions
    : expressions ',' expression                        { $1->push($3); $$ = $1; }
    | expression                                        { vector<TreeNode*>* vec = new vector<TreeNode*>(); vec->push_back($1); $$ = new expr_coll(vec); }
;

expression
	: '(' expression ')'				                { $$ = $2; }
	| '{' stmts '}'    				           			{ auto ret = new express(($2)); $$ = ret; }
    | '[' expressions ']'                               { auto ret = new express(($2)); $$ = ret; }
	| expression '/' expression							{ auto ret = new express("/"); ret->left = $1; ret->right = $3; ret->comp = ($1->comp)/($3->comp); $$ = ret;  }
	| expression '*' expression							{ auto ret = new express("*"); ret->left = $1; ret->right = $3; ret->comp = ($1->comp)*($3->comp); $$ = ret;  }
	| expression '+' expression							{ auto ret = new express("+"); ret->left = $1; ret->right = $3; ret->comp = ($1->comp)+($3->comp); $$ = ret;  }
	| expression '-' expression					 		{ auto ret = new express("-"); ret->left = $1; ret->right = $3; ret->comp = ($1->comp)-($3->comp); $$ = ret;  }
	| expression '<' expression							{ auto ret = new express("<"); ret->left = $1; ret->right = $3; $$ = ret;  }
	| expression '>' expression							{ auto ret = new express(">"); ret->left = $1; ret->right = $3; $$ = ret;  }
	| expression "==" expression						{ auto ret = new express("=="); ret->left = $1; ret->right = $3; $$ = ret; }
	| expression NOT_EQUAL expression					{ auto ret = new express("!="); ret->left = $1; ret->right = $3; $$ = ret; }
	| NOT expression 									{ auto a = new Expression(0.0); auto ret = new express("!"); ret->left = new express(*a); ret->right = $2; $$ = ret; }
	| '-' expression %prec Uminus						{ auto a = new Expression(0.0); auto ret = new express("-"); ret->left = new express(*a); ret->right = $2;  ret->comp = -1*($2->comp); $$ = ret; }
	| INT_CONST											{ auto a = new Expression(atoi($1->c_str())); auto temp = new express(*a); temp->comp = atof($1->c_str()); $$ = temp;}
	| FLT_CONST											{ auto a = new Expression(atof($1->c_str())); auto temp = new express(*a); temp->comp = atof($1->c_str()); $$ = temp;}
	| NAME												{ auto ret = new express(($1)); $$ = ret; }
;

%%

/* ADDITIONAL C CODE */

int main() {
	yyparse();
    return 0;
}