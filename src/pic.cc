#include "pic.hh"

map<string, express*> table;

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
            if(levelNodes[i]->stmtType==STMT_EXPRESSION) {
                levelNodes[i]->print();
            }
        }
        cout << endl;  
    }
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