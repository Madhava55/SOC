#include "pic.hh"

vector<unordered_map<string, express*>> table;

/* void bfs(TreeNode* root) {
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
            if(levelNodes[i]->stmtType==STMT_EXPRESSION) {
                levelNodes[i]->print();
            }
        }
        cout << endl;  
    }
} */

express* variable(string* a) {
    for(auto j:table.back())
    {
        if(j.first==*(a))
        {
            return j.second;
        }
    }
	return new express(a);
}

express* print_var(string *s) {
	for(int i=(int)table.size();i>=0;i--)
	{
		for(auto j:table[i])
		{
			if(j.first==*(s))
			{
				return j.second;
			}
		}
	}
	cout<<"variable "<<*s<<" not found. Exiting..."<<endl;
	exit(1);
}

value operator<(const Expression& lhs, const Expression& rhs) {
    value comparison;
    if(lhs.etype!=rhs.etype)comparison.boolean=false;
    else {
        int type = lhs.etype;
        if(type==0) comparison.boolean = (lhs.v.a < rhs.v.a);
        else if(type==1 || type ==3) comparison.boolean = (lhs.v.boolean < rhs.v.boolean);
        else if(type==2) comparison.boolean = (lhs.v.c < rhs.v.c );
    }
    return comparison;
}
value operator>(const Expression& lhs, const Expression& rhs) {
    value comparison;
    if(lhs.etype!=rhs.etype)comparison.boolean=false;
    else {
        int type = lhs.etype;
        if(type==0) comparison.boolean = (lhs.v.a > rhs.v.a);
        else if(type==1 || type ==3) comparison.boolean = (lhs.v.boolean > rhs.v.boolean);
        else if(type==2) comparison.boolean = (lhs.v.c > rhs.v.c );
    }
    return comparison;
}
value operator==(const Expression& lhs, const Expression& rhs) {
    value comparison;
    if(lhs.etype!=rhs.etype)comparison.boolean=false;
    else {
        int type = lhs.etype;
        if(type==0) comparison.boolean = (lhs.v.a == rhs.v.a);
        else if(type==1 || type ==3) comparison.boolean = (lhs.v.boolean == rhs.v.boolean);
        else if(type==2) comparison.boolean = (lhs.v.c == rhs.v.c );
    }
    return comparison;
}
value operator!=(const Expression& lhs, const Expression& rhs) {
    value comparison;
    if(lhs.etype!=rhs.etype)comparison.boolean=false;
    else {
        int type = lhs.etype;
        if(type==0) comparison.boolean = (lhs.v.a != rhs.v.a);
        else if(type==1 || type ==3) comparison.boolean = (lhs.v.boolean != rhs.v.boolean);
        else if(type==2) comparison.boolean = (lhs.v.c != rhs.v.c );
    }
    return comparison;
}
value operator!(const Expression& lhs) {
    value comparison;
        int type = lhs.etype;
        if(type==0) comparison.boolean = !(lhs.v.a);
        else if(type==1 || type ==3) comparison.boolean = !(lhs.v.boolean);
        else if(type==2) comparison.boolean = true;
    return comparison;
}
object operator/(const object& lhs, const object& rhs) {
    if (rhs.exp.v.a == 0.0) {
        throw runtime_error("Division by zero");
    }
    return object(*new Expression(lhs.exp.v.a / rhs.exp.v.a));
}

object operator-(const object& lhs, const object& rhs) {
    return object(*new Expression(lhs.exp.v.a - rhs.exp.v.a));
}
object operator*(const object& lhs, const object& rhs) {
    return object(*new Expression(lhs.exp.v.a * rhs.exp.v.a));
}
object operator+(const object& lhs, const object& rhs) {
    return object(*new Expression(lhs.exp.v.a + rhs.exp.v.a));
}
object operator<(const object& lhs, const object& rhs) {
    return object(*new Expression(lhs.exp < rhs.exp));
}
object operator>(const object& lhs, const object& rhs) {
    return object(*new Expression(lhs.exp > rhs.exp));
}
object operator==(const object& lhs, const object& rhs) {
    return object(*new Expression(lhs.exp == rhs.exp));
}
object operator!=(const object& lhs, const object& rhs) {
    return object(*new Expression(lhs.exp != rhs.exp));
}
object operator!(const object& rhs) {
    return object(*new Expression(!rhs.exp));
}

express::express(block* blk){
	type = 4;
	obj.node=blk;
	stmtType = STMT_EXPRESSION;
} 

express::express(expr_coll* expr){
	type = 5;
	obj.node=expr;
	stmtType = STMT_EXPRESSION;
} 

express* expr_evaluate(TreeNode* node)
{
    express* node_ret = new express();
    if((node->obj.ch)=="/")
    {
        node_ret->obj = (expr_evaluate(node->left)->obj)/(expr_evaluate(node->right)->obj);
        return node_ret;
    }
    else if((node->obj.ch)=="*")
    {
        node_ret->obj = (expr_evaluate(node->left)->obj)*(expr_evaluate(node->right)->obj);
        return node_ret;
    }
    else if((node->obj.ch)=="+")
    {
        node_ret->obj = (expr_evaluate(node->left)->obj)+(expr_evaluate(node->right)->obj);
        return node_ret;
    }
    else if((node->obj.ch)=="-")
    {
        node_ret->obj = (expr_evaluate(node->left)->obj)-(expr_evaluate(node->right)->obj);
        return node_ret;
    }
    else if((node->obj.ch)=="==")
    {
        node_ret->obj = (expr_evaluate(node->left)->obj)==(expr_evaluate(node->right)->obj);
        return node_ret;
    }
    else if((node->obj.ch)==">")
    {
        node_ret->obj = (expr_evaluate(node->left)->obj)>(expr_evaluate(node->right)->obj);
        return node_ret;
    }
    else if((node->obj.ch)=="<")
    {
        node_ret->obj = (expr_evaluate(node->left)->obj)<(expr_evaluate(node->right)->obj);
        return node_ret;
    }
    else if((node->obj.ch)=="!=")
    {
        node_ret->obj = (expr_evaluate(node->left)->obj)!=(expr_evaluate(node->right)->obj);
        return node_ret;
    }
    else if((node->obj.ch)=="!")
    {
        node_ret->obj = (!(expr_evaluate(node->obj.node)->obj));
        return node_ret;
    }
    else if(node->left==NULL || node->right==NULL)
    {
        express* node_ret = new express();
        if(node->type==1){
            node_ret = print_var(node->obj.b);
        }
        else if(node->type==0){
            node_ret->type = node->type;
            node_ret->obj = node->obj;
        }
        return node_ret;
    }
    else {
        cout<<"wierd assignment resolution"<<endl;
        exit(1);
    }
}

void evaluate(TreeNode* node)
{
    auto check = node->stmtType;
    if(check == STMT_ASSIGN){
        express* temp = expr_evaluate(node->right);
        int done=0;
        if(node->obj.exp.v.a==0) {
            /*for(auto j:table.back())
            {
                if(done==1)break;
                if(j.first==*(node->obj.b))
                {
                    j.second = temp;done=1;
                }
            }*/
            if(done==0)
            {
                table.back()[*(node->obj.b)]=temp;
            }
        }
        else{
            for(int i=(int)table.size();i>=0;i--)
            {
                if(done==1)break;
                for(auto j:table[i])
                {
                    if(j.first==*(node->obj.b))
                    {
                        table[i][j.first] = temp;done=1;break;
                    }
                }
            }
            if(done==0){
            cout<<"variable "<<*(node->obj.b)<<" not found. Exiting..."<<endl;
            exit(1);
            }
        }
    }
    else if(check == STMT_BLOCK){
        for(int i=0;i<node->obj.vec->size();i++){
            vector<TreeNode*> temporary=*(node->obj.vec);
            evaluate(temporary[i]);
        }
    }
    else if(check == STMT_PRINT){
        print_var(node->obj.b)->print();
    }
    else if(check == STMT_SCOPE){
        table.push_back(unordered_map<string, express*>());
        evaluate(node->obj.node);
        table.pop_back();
    }
    else if(check == STMT_WHILE){
        express* check = expr_evaluate(node->left);
        while(check->obj.exp.v.boolean==1){
            evaluate(node->right);
            check = expr_evaluate(node->left);
        }
    }
    else if(check == STMT_IF_ELSE){
        express* check = expr_evaluate(node->obj.node);
        if(check->obj.exp.v.boolean==1){
            evaluate(node->left);
        }
        else evaluate(node->right);
    }
    else if(check == STMT_FOR){
        vector<TreeNode*> statements = *(node->obj.vec);
        evaluate(statements[0]);
        (statements[3]->obj.node->obj.vec)->push_back(statements[2]);
        While *for_to_while = new While(statements[1],statements[3]);
        evaluate(for_to_while);
    }
}

