#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
	void rotateLeft(AVLNode<Key,Value>* n);
	void rotateRight(AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key,Value>* n, int diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    newNode->setBalance(0);
    
    if (this->root_ == NULL) {
        this->root_ = newNode;
        newNode->setBalance(0);
    } else {
        AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
        while (curr != NULL) {
            if (curr->getKey() == newNode->getKey()) {
                curr->setValue(newNode->getValue());
                delete newNode;
                break;
            } 
            
            if (curr->getKey() > newNode->getKey()) {
                if (curr->getLeft() == NULL) {
                    newNode->setParent(curr);
                    curr->setLeft(newNode);
                    newNode->setBalance(0);
                    if (curr->getBalance() == -1 || curr->getBalance() == 1) {
                        curr->setBalance(0);
                    } else {
                        curr->setBalance(-1);
                        insertFix(curr, curr->getLeft());
                    }
                    break;
                } else {
                    curr = curr->getLeft();
                }
            } else {
                if (curr->getRight() == NULL) {
                    newNode->setParent(curr);
                    curr->setRight(newNode);
                    newNode->setBalance(0);
                    if (curr->getBalance() == -1 || curr->getBalance() == 1) {
                        curr->setBalance(0);
                    } else {
                        curr->setBalance(1);
                        insertFix(curr, curr->getRight());
                    }
                    break;
                } else {
                    curr = curr->getRight();
                }
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix (AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    if (p == NULL || p->getParent() == NULL) {
        return;
    } 

    int sign = 0;

    AVLNode<Key,Value>* g = p->getParent();
    if (p == g->getLeft()) {
        sign = -1;
    } else {
        sign = 1;
    }

    void (AVLTree<Key, Value>::*rotate_typeR)(AVLNode<Key,Value>*);
    void (AVLTree<Key, Value>::*rotate_typeL)(AVLNode<Key,Value>*);

    if (sign == -1) {
        rotate_typeR = &AVLTree<Key, Value>::rotateRight;
        rotate_typeL = &AVLTree<Key, Value>::rotateLeft;
    } else {
        rotate_typeL = &AVLTree<Key, Value>::rotateRight;
        rotate_typeR = &AVLTree<Key, Value>::rotateLeft;
    }

    g->setBalance(g->getBalance () + (sign * 1));
    if (g->getBalance() == 0) {
        std::cout << "REACHED00" << std::endl;
        return;
    } else if (g->getBalance() == sign * 1) {
        std::cout << "REACHED0" << std::endl;
        insertFix(g, p); 
    } else if (g->getBalance() == sign * 2 ) {
        if (p->getBalance() == sign * 1 || p->getBalance() == 0) {  // zig-zig
            std::cout << "REACHED1" << std::endl;
            (this->*rotate_typeR)(g);
            p->setBalance(0);
            g->setBalance(0);
        } else if (p -> getBalance() == sign * -1) { //zig-zag
            std::cout << "REACHED2" << std::endl;
            (this->*rotate_typeL)(p);
            (this->*rotate_typeR)(g);

            if (n -> getBalance() == sign * 1) {
                p->setBalance(0);
                g->setBalance(sign * -1); 
                n->setBalance(0);

            } else if (n->getBalance() == 0) {
                p->setBalance(0);
                g->setBalance(0);
                n->setBalance(0);
            } else {
                p->setBalance(sign * 1);
                g->setBalance(0);
                n->setBalance(0);
            }
        }
        
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* A) {
    std::cout << "TRYING TO RL WITH " << A->getKey() << std::endl;
    AVLNode<Key, Value>* A_parent = A->getParent();
    AVLNode<Key, Value>* B = A->getRight();
    A->setRight(B->getLeft());

    if (B->getLeft()) {
        B->getLeft()->setParent(A);
    }
    B->setLeft(A);

    A->setParent(B);
    B->setParent(A_parent);

    if (A_parent != NULL) {
        if (A_parent->getRight() == A) {
            A_parent->setRight(B);
        } else {
            A_parent->setLeft(B);
        }
    } else {
        this->root_ = B;
        B->setParent(NULL);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* A) {
    std::cout << "TRYING TO RR WITH " << A->getKey() << std::endl;
    AVLNode<Key, Value>* A_parent = A->getParent();
    AVLNode<Key, Value>* B = A->getLeft();
    A->setLeft(B->getRight());
    if (B->getRight()) {
        B->getRight()->setParent(A);
    }
    B->setRight(A);

    A->setParent(B);
    B->setParent(A_parent);

    if (A_parent != NULL) {
        if (A_parent->getLeft() == A) {
            A_parent->setLeft(B);
        } else {
            A_parent->setRight(B);
        }
    } else {
        this->root_ = B;
        B->setParent(NULL);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff) {
    if (n == NULL) {
        return;
    }
    AVLNode<Key, Value>* parent = n->getParent();
    int ndiff = 0;
    if (parent)
	{
		if (n == parent->getLeft()) {
            ndiff = 1;
        }
		else {
            ndiff = -1;
        }
	}

    int sign = 0;
    if (diff == -1) {
        sign = -1;
    } else if (diff == 1) {
        sign = 1;
    } else {
        std::cout << "DIFF IS WACK" << std::endl;
    }

    void (AVLTree<Key, Value>::*rotate_typeR)(AVLNode<Key,Value>*);
    void (AVLTree<Key, Value>::*rotate_typeL)(AVLNode<Key,Value>*);

    if (sign == -1) {
        rotate_typeR = &AVLTree<Key, Value>::rotateRight;
        rotate_typeL = &AVLTree<Key, Value>::rotateLeft;
    } else {
        rotate_typeL = &AVLTree<Key, Value>::rotateRight;
        rotate_typeR = &AVLTree<Key, Value>::rotateLeft;
    }

    if (n->getBalance() + diff == sign * 2) {
        AVLNode<Key, Value>* c;
        if (sign == -1) {
            c = n -> getLeft();
        } else {
            c = n -> getRight();
        }

        if (c -> getBalance() == sign * 1)
        {
            (this->*rotate_typeR)(n);
            n ->setBalance(0);
            c ->setBalance(0);
            removeFix(parent, ndiff);
        }
        else if (c -> getBalance() == 0)
        {
            (this->*rotate_typeR)(n);
            n ->setBalance(sign * 1);
            c ->setBalance(sign * -1);
        } else if (c -> getBalance() == sign * -1){
            AVLNode<Key, Value>* g;
            if (diff == -1) {
                g = c -> getRight();
            } else {
                g = c -> getLeft();
            }

            (this->*rotate_typeL)(c);
            (this->*rotate_typeR)(n);

            if (g ->getBalance() == sign * -1)
            {
                n -> setBalance(0);
                c -> setBalance(sign * 1);
                g -> setBalance(0);
            }
            else if (g->getBalance() == 0)
            {
                n -> setBalance(0);
                c -> setBalance(0);
                g -> setBalance(0);
            }
            else
            {
                n -> setBalance(sign * -1);
                c -> setBalance(0);
                g -> setBalance(0);
            }
            removeFix(parent, ndiff);
        } 
    } else if (n->getBalance() + diff == sign * 1)
		{
			n->setBalance(sign * 1);
		}
	else if (n->getBalance() + diff == 0) {
        n->setBalance(0);
        removeFix(parent, ndiff);
	}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if (node == NULL) {
        return; 
    }
    
    if (node->getLeft() != NULL && node->getRight() != NULL) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        nodeSwap(node, pred);
    }

    int diff = 0;
    AVLNode<Key, Value>* parent = node->getParent();

    if (node->getLeft() == NULL && node->getRight() == NULL) {
        if (parent) {
            if (parent->getLeft() == node) {
                diff = 1;
                parent->setLeft(NULL);
            } else if (parent->getRight() == node) {
                diff = -1;
                parent->setRight(NULL);
            }
        } else {
            this->root_ = NULL;
        }

        delete node;
    } else if (node->getLeft() != NULL || node->getRight() != NULL) {

        AVLNode<Key, Value>* child;
        if (node->getLeft()) {
            child = node->getLeft();
        } else if (node->getRight()) {
            child = node->getRight();
        }

        if (parent) {
            if (parent->getLeft() == node) {
                diff = 1;
                parent->setLeft(child);
                child->setParent(parent);
            } else if (parent->getRight() == node) {
                diff = -1;
                parent->setRight(child);
                child->setParent(parent);
            }
        } 

        if (this->root_ == node || parent == NULL) {
            this->root_ = child;
            child->setParent(NULL);
        }   

        delete node;
    } 

    removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
