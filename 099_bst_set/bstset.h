#include "set.h"
#include <stdexcept>

template<typename T>
class BstSet : public Set<T> {
private:
    struct TreeNode {
        T key;
        TreeNode *left;
        TreeNode *right;

        TreeNode(const T & key) : key(key), left(NULL), right(NULL) {}
    };

    TreeNode *root;

    TreeNode* insert(TreeNode *node, const T &key) {
        if (node == NULL) {
            return new TreeNode(key);
        } else if (key < node->key) {
            node->left = insert(node->left, key);
        } else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        return node;
    }

    bool contains(TreeNode *node, const T &key) const {
        if (node == NULL) {
            return false;
        }

        if (key < node->key) {
            return contains(node->left, key);
        } else if (key > node->key) {
            return contains(node->right, key);
        } else {
            return true;
        }
    }

    TreeNode* remove(TreeNode *node, const T &key) {
        if (node == NULL) {
            return NULL;
        }

        if (key < node->key) {
            node->left = remove(node->left, key);
        } else if (key > node->key) {
            node->right = remove(node->right, key);
        } else {
            if (node->left == NULL) {
                TreeNode *temp = node->right;
                delete node;
                return temp;
            } else if (node->right == NULL) {
                TreeNode *temp = node->left;
                delete node;
                return temp;
            }

            TreeNode *temp = node->right;
            TreeNode *parent = node;
            while (temp->left != NULL) {
                parent = temp;
                temp = temp->left;
            }
            node->key = temp->key;
            if (parent != node) {
                parent->left = temp->right;
            } else {
                node->right = temp->right;
            }
            delete temp;
        }

        return node;
    }

    void clear(TreeNode *node) {
        if (node == NULL) {
            return;
        }

        clear(node->left);
        clear(node->right);
        delete node;
    }

    TreeNode* copyTree(TreeNode* node) const {
        if (node == NULL) {
            return NULL;
        }
        TreeNode* newNode = new TreeNode(node->key);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

public:
    BstSet() : root(NULL) {}

    BstSet(const BstSet &rhs) {
        root = copyTree(rhs.root);
    }

    BstSet& operator=(const BstSet &rhs) {
        if (this != &rhs) {
            clear(root);
            root = copyTree(rhs.root);
        }
        return *this;
    }

    virtual ~BstSet() {
        clear(root);
    }

    virtual void add(const T &key) {
        root = insert(root, key);
    }

    virtual bool contains(const T& key) const {
        return contains(root, key);
    }

    virtual void remove(const T& key) {
        root = remove(root, key);
    }
};
