#include "map.h"

template<typename K, typename V>
class BstMap : public Map<K,V> {
private:
    struct TreeNode {
        K key;
        V value;
        TreeNode *left;
        TreeNode *right;

        TreeNode(const K & key, const V & value) : key(key), value(value), left(NULL), right(NULL) {}
    };

    TreeNode *root;

    TreeNode* insert(TreeNode *node, const K &key, const V &value) {
        if (node == NULL) {
            node = new TreeNode(key, value);
        } else if (key < node->key) {
            node->left = insert(node->left, key, value);
        } else if (key > node->key) {
            node->right = insert(node->right, key, value);
        } else {
            node->value = value;
        }
        return node;
    }

    const V & lookup(TreeNode *node, const K &key) const {
        if (node == NULL) {
            throw std::invalid_argument("Key not found");
        }

        if (key < node->key) {
            return lookup(node->left, key);
        } else if (key > node->key) {
            return lookup(node->right, key);
        } else {
            return node->value;
        }
    }

    TreeNode* remove(TreeNode *node, const K &key) {
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
            node->value = temp->value;
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
        TreeNode* newNode = new TreeNode(node->key, node->value);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

public:
    BstMap() : root(NULL) {}

    BstMap(const BstMap &rhs) {
        root = copyTree(rhs.root);
    }

    BstMap& operator=(const BstMap &rhs) {
        if (this != &rhs) {
            clear(root);
            root = copyTree(rhs.root);
        }
        return *this;
    }

    virtual ~BstMap() {
        clear(root);
    }

    virtual void add(const K &key, const V &value) {
        root = insert(root, key, value);
    }

    virtual const V & lookup(const K& key) const throw(std::invalid_argument) {
        return lookup(root, key);
    }

    virtual void remove(const K& key) {
        root = remove(root, key);
    }
};
