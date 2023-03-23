#ifndef RECCHECK
#include <algorithm>

#endif

#include "equal-paths.h"
using namespace std;


int getHeight(Node* root) {
    if (root == nullptr) {
        return 0;
    }
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    return 1 + max(leftHeight, rightHeight);
}

bool equalPaths(Node* root)
{
    if (root == nullptr) {
        return true;
    }

    if (root->left != nullptr && root->right != nullptr) {
        int leftHeight = getHeight(root->left);
        int rightHeight = getHeight(root->right);

        if (abs(leftHeight - rightHeight) > 0) {
            return false;
        }
    }
    
    return equalPaths(root->left) && equalPaths(root->right);
}