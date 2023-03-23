#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    // BinarySearchTree<int,int> bt;
    // bt.insert(std::make_pair(1,1));
    // bt.insert(std::make_pair('c',2));
    // bt.insert(std::make_pair('d',3));
    // bt.insert(std::make_pair('e',4));
    // bt.insert(std::make_pair('a',5));
    // bt.insert(std::make_pair('g',6));

    // bt.remove(1);

    // BinarySearchTree<char,int>::iterator it = bt.begin();
    // cout << it->first << " " << it->second << endl;   
    // ++it;
    // cout << it->first << " " << it->second << endl;   


    // cout << "Binary Search Tree contents:" << endl;
    // for(BinarySearchTree<int,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
    //     cout << it->first << " " << it->second << endl;
    // }

    // bt.print();
    
    // if(bt.find('b') != bt.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;
    // bt.remove('b');

    // cout << "BALANCED? " << bt.isBalanced() << endl;

    // AVL Tree Tests
    AVLTree<int,int> at;
    at.insert(std::make_pair(3,1));
    at.insert(std::make_pair(4,2));
    at.insert(std::make_pair(6,2));
    at.insert(std::make_pair(7,2));
    at.insert(std::make_pair(9,2));
    at.insert(std::make_pair(10,2));

    // at.insert(std::make_pair('d',2));
    // at.insert(std::make_pair('e',2));
    // at.insert(std::make_pair('f',2));


    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<int,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    // if(at.find('b') != at.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;

    at.print();

    return 0;
}
