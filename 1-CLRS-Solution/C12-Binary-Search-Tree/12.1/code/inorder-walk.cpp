
class Stack_Solution {
        /*T = n S = n*/
    public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        stack<TreeNode *> Root;
        while(!Root.empty() || root){
            if(root){
                Root.push(root);
                root = root->left;   
            }else{
                root = Root.top() -> right;
                res.push_back(Root.top()->val);
                Root.pop();
            }
        }
        return res;
    }
};

class Recursize_Solution {
        /*T = n S = n*/
public:
    void inorder(TreeNode* root, vector<int>& nodes) {
        if (!root) return;
        inorder(root -> left, nodes);
        nodes.push_back(root -> val);
        inorder(root -> right, nodes);
    }
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> nodes;
        inorder(root, nodes);
        return nodes;
    } 
};

class Merge_Solution {
        /*T = nlgn S = n*/
public:
    vector<int> inorderTraversal(TreeNode* root){
        if(root){
            vector<int> left = inorderTraversal(root -> left);
            vector<int> right = inorderTraversal(root -> right);
            vector<int> res;
            for(int i = 0; i != left.size(); ++i)
                res.push_back(left[i]);
            res.push_back(root->val);
            for(int i = 0; i != right.size(); ++i)
                res.push_back(right[i]);
            return res;
        }else{
            vector<int> res;
            return res;
        }
    } 
};

class Stack_Solution {
        /*T = n S = n*/
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> ans;
        stack<TreeNode*> s;
        TreeNode* p = root;
        while(p || !s.empty()){
            while(p){
                s.push(p);
                p = p->left;
            }
            p = s.top();
            s.pop();
            ans.push_back(p->val);
            p=p->right;
        }
        return ans;
    }
};

class Half_Stack_Solution {
        /*T = n S = n*/
private:
    vector<int> v;
    void Half_stack_Inorder_Walk(TreeNode* x){
        stack<TreeNode*> s;
        while(x){
            s.push(x);
            x = x->left;
        }
        while(!s.empty()){
            x = s.top();
            s.pop();
            v.push_back(x->val);
            if(x->right)
                Half_stack_Inorder_Walk(x->right);
        }
    }
public:
    vector<int> inorderTraversal(TreeNode* root) {
        Half_stack_Inorder_Walk(root);
        return v;
    }
};

class Morris_Solution {
    /*T = n S = 1*/
public:
    vector<int> inorderTraversal(TreeNode* root) {
        TreeNode* p = root;
        TreeNode* decesser = NULL;
        vector<int> ans;
        int i = 0;
        while(p){
            if(p->left){
                decesser = p->left;
                while(decesser->right && decesser->right != p){
                    decesser = decesser->right;
                }
                if(decesser->right == NULL){
                    decesser->right = p;
                    p = p->left;
                }else{
                    decesser->right = NULL;
                    ans.push_back(p->val);
                    p = p ->right;
                }
            }else{
                ans.push_back(p->val);
                p = p -> right;
            }
        }
        return ans;
    }
};