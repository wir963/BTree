//
//  BTree.h
//  BTree
//
//  Created by Welles Robinson on 11/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BTree_BTree_h
#define BTree_BTree_h

#include "BTreeNode.h"
#include <math.h>
#include <iostream>
#include <iomanip>

enum ReturnSuccess {
    success, unsuccess, duplicate, DNE
};

using namespace std;

template<typename Treetype, int M1>
class BTree {
    public:
    BTree();
    void insert(Treetype val);
    void remove(Treetype val);
    bool find(Treetype val);
    void printByLevel();
    ~BTree();
private:
    void findPredecessor(BTreeNode<Treetype, M1> *_t, Treetype &data);
    ReturnSuccess _insert(Treetype val, BTreeNode<Treetype, M1> *_rt, BTreeNode<Treetype, M1> *&_t, Treetype &t_val);
    ReturnSuccess _remove(Treetype val, BTreeNode<Treetype, M1> *_rt);
    ReturnSuccess _handleUnsuccessfulDelete(BTreeNode<Treetype, M1> *&_rt, int i);
    bool _find(Treetype val, BTreeNode<Treetype, M1> *_rt);
    void _printByLevel(BTreeNode<Treetype, M1> *_rt, int numspaces);
    void _delete_all(BTreeNode<Treetype, M1> *_rt);
    BTreeNode<Treetype, M1> *_root, *_temp;
};

template<typename Treetype, int M1>
BTree<Treetype, M1>::BTree()
{
    _root = new BTreeNode<Treetype, M1>;
    _root->isLeaf = true;
}

template<typename Treetype, int M1>
void BTree<Treetype, M1>::insert(Treetype val)
{
    Treetype val4;
    _temp = NULL;
    ReturnSuccess _rs = _insert(val, _root, _temp, val4);
    if (_rs == unsuccess) {
        
        /* this is the case when the root is full 
         * it is special because the root node is already full
         * so now have one value and root points to the ptr[0] and _temp points to the ptr[1] 
        */
        //cout << "inserting " << val4 << " as the only value into the new root node" << endl;
        BTreeNode<Treetype, M1> *_new_root;
        _new_root = new BTreeNode<Treetype, M1>;
        _new_root->key[0] = val4;
        _new_root->ptr[0] = _root;
        _new_root->ptr[1] = _temp;
        _new_root->isLeaf = false;
        _new_root->numKeys = 1;
        _root = _new_root;
        // insert into the root
    }
}

template<typename Treetype, int M1>
ReturnSuccess BTree<Treetype, M1>::_insert(Treetype val, BTreeNode<Treetype, M1> *_rt, BTreeNode<Treetype, M1> *&_t, Treetype &t_val)
{
    
    if (_rt == NULL) {
        t_val = val;
        return unsuccess;
    }
    Treetype temp_val;
    int i;
    for (i = 0; i < _rt->numKeys; i++) {//loop thru the node
        if (_rt->key[i] >= val)
            break;
    }// end of for loop
    // check for duplicates in the current node
    if (_rt->key[i] == val){
        cout << "value already exists, duplicates are not allowed" << endl;
        return duplicate;
    }

    _temp = NULL;
    // insert into the appropriate child node
    ReturnSuccess _rs = _insert(val, _rt->ptr[i], _temp, temp_val);
    if (_rs == unsuccess) {
        /* either the insert didn't work because the node is a leaf node or it did work but it made the node too full */
        /* loop through the node */
        for (i = 0; i < _rt->numKeys; i++) {
            if (_rt->key[i] >= val)
                break; // when the value to be inserted is less than the value of the current node
        }
        if (_rt->key[i] == val) {
            cout << "value already exists, duplicates are not allowed" << endl;
            return duplicate;
        }
        /* shift the keys and the pointers right one to make room */
        for (int j = _rt->numKeys; j > i; j--) {// init j is equal to the first empy index, i = the index to insert into
            _rt->key[j] = _rt->key[j-1];// shift the keys one spot to the right
            _rt->ptr[j+1] = _rt->ptr[j];// shift the pointer one spot to the right
        }// j is the redundant index (whose value was just copied over to j+1)
        // break when the redundant index is equal to the index to be inserted (when j = i)
        _rt->key[i] = temp_val;//insert the new value into its rightful position
        _rt->ptr[i+1] = _temp;//insert the new pointer into its right position
        _rt->numKeys++;
            
        if (_rt->isFull() == true) {
            /* the node is full */
            
            _t = new BTreeNode<Treetype, M1>; // create a new node and point _t to it
            _t->isLeaf = _rt->isLeaf;
            int start_val = floor(M1/2);// store the middle index of the full array
            t_val = _rt->key[start_val];// store the value of the middle index
            _rt->numKeys--;// decrement by one because removing the middle index
            int new_i = 0;
            int old_i = start_val +1;
            while( old_i < M1) {
                _t->key[new_i] = _rt->key[old_i];//copy the value over
                _t->ptr[new_i] = _rt->ptr[old_i];//copy the pointer over
                _rt->numKeys--; _t->numKeys++;
                old_i++; new_i++;
            }
            _t->ptr[new_i] = _rt->ptr[old_i]; //copy the last pointer over
            return unsuccess;
        }
    }
    return success;
}

template<typename Treetype, int M1>
void BTree<Treetype, M1>::remove(Treetype val)
{
    if (_remove(val, _root) == unsuccess) {
        // this means a value was deleted from the node
        if (_root->numKeys == 0) {
            _temp = _root;
            _root = _root->ptr[0];
            delete _temp;
            
        }
    }
}

template<typename Treetype, int M1>
ReturnSuccess BTree<Treetype, M1>::_remove(Treetype val, BTreeNode<Treetype, M1> *_rt)
{
    if (_rt == NULL) {
        cout << val << ", the value to be deleted couldn't be found" << endl;
        return DNE;
    }
    int i;
    for (i = 0; i < _rt->numKeys; i++) {//loop thru the node
        if (_rt->key[i] >= val)
            break;
    }// end of for loop
    if (val == _rt->key[i]) {
        // the value was found
        if (_rt->isLeaf == true) {
            for (int j = i; j < _rt->numKeys; j++) {
                _rt->key[j] = _rt->key[j+1];
            }
            _rt->numKeys--;
            if (_rt->isTooEmpty()) {
                return unsuccess;
            }
            else
                return success;
        }
        else { // the node containing the key to be deleted isn't a leaf node
            Treetype data;
            _temp = _rt;
            findPredecessor(_rt->ptr[i], data);
            _temp->key[i] = data;
            if (_remove(data, _rt->ptr[i]) == unsuccess) {
                return _handleUnsuccessfulDelete(_rt, i);
            }
            else {
                return success;
            }
        }
    }
    
    else {// the value wasn't found in the current node
        
        if (_remove(val, _rt->ptr[i]) == unsuccess){
            return _handleUnsuccessfulDelete(_rt, i);
        }
        else
            return success;
    }
}

template<typename Treetype, int M1>
ReturnSuccess BTree<Treetype, M1>::_handleUnsuccessfulDelete(BTreeNode<Treetype, M1> *&_rt, int i)
{
    // the key was deleted from the node but now the node is too empty
    // _rt->ptr[i] points to the too empty node
    // now check _rt->ptr[i-1] and _rt->ptr[i+1]
    /* this is assuming that we're only one level above the leaf nodes because unsuccess is only called by a _remove from a leaf node */
    
    Treetype temp_val;
    if (i > 0) { // make sure the key to the left exists
        if ((_rt->ptr[i-1]->numKeys-1) >= (((M1+1)/2)-1)) { // make sure the node[i-1] has keys to spare
            /* when you borrow from the left, you take it's key [numKeys-1] and it's ptr [numKeys]
             the temp_ptr becomes the ptr[0] of the lacking node
             */
            
            temp_val = _rt->ptr[i-1]->key[(_rt->ptr[i-1]->numKeys)-1];// the store the key to be borrowed
            _temp = _rt->ptr[i-1]->ptr[(_rt->ptr[i-1]->numKeys)-1];// the store the pointer to be borrowed
            _rt->ptr[i-1]->key[(_rt->ptr[i-1]->numKeys)-1] = NULL;
            for (int j = _rt->ptr[i]->numKeys; j > 0; j--) { // this makes room in the ith node for the new key-value
                _rt->ptr[i]->key[j] = _rt->ptr[i]->key[j-1];
                _rt->ptr[i]->ptr[j+1] = _rt->ptr[i]->ptr[j];
            }
            _rt->ptr[i]->ptr[1] = _rt->ptr[i]->ptr[0];// moves over the last ptr
            _rt->ptr[i]->key[0] = _rt->key[i-1];// add the value to the child node that was lacking keys
            _rt->ptr[i]->ptr[0] = _temp;// sets the new ptr
            _rt->ptr[i]->numKeys++;
            _rt->key[i-1] = temp_val;
            _rt->ptr[i-1]->numKeys--;
            return success;
        }
    }
    
    if (i + 1 < _rt->numKeys) {
        if ((_rt->ptr[i+1]->numKeys-1) >= (((M1+1)/2)-1)) {
            /* when you borrow from the right, you take it's key [0] and it's ptr [0]
             the temp_ptr becomes the ptr[0] of the lacking node
             */
            
            temp_val = _rt->ptr[i+1]->key[0];
            _temp = _rt->ptr[i+1]->ptr[0];
            _rt->ptr[i]->key[_rt->ptr[i]->numKeys] = _rt->key[i];
            _rt->ptr[i]->ptr[_rt->ptr[i]->numKeys+1] = _temp;
            _rt->ptr[i]->numKeys++;
            _rt->key[i] = temp_val;
            // moves the values in ptr[i+1] to the left one
            _rt->ptr[i+1]->numKeys--;
            for (int j = 0; j < _rt->ptr[i+1]->numKeys; j++) {
                _rt->ptr[i+1]->key[j] = _rt->ptr[i+1]->key[j+1];
                _rt->ptr[i+1]->ptr[j] = _rt->ptr[i+1]->ptr[j+1];
            }
            _rt->ptr[i+1]->ptr[_rt->ptr[i+1]->numKeys+1] = NULL;
            return success;
        }
    }
    
    
    /* no neighboring nodes can be borrowed from */
    if (i > 0) { // make sure the key to the left exists
        /* we copy from the child node i to the child node i-1 (both ptrs and keys)
         after we copy the i-1 key from the parent node into the child node i-1
         */
        
        _rt->ptr[i-1]->key[_rt->ptr[i-1]->numKeys] = _rt->key[i-1];
        _rt->ptr[i-1]->numKeys++;
        // copy the keys from _rt->ptr[i] into _rt->ptr[i-1] and increment accordingly
        int k = 0;
        int j = _rt->ptr[i-1]->numKeys;
        while (k < _rt->ptr[i]->numKeys) {
            _rt->ptr[i-1]->key[j] = _rt->ptr[i]->key[k];//copy the key over
            _rt->ptr[i-1]->ptr[j] = _rt->ptr[i]->ptr[k];//copy the ptr over
            k++; j++;
        }
        _rt->ptr[i-1]->ptr[j] = _rt->ptr[i]->ptr[k];//copy the last ptr over
        
        _rt->ptr[i-1]->numKeys += _rt->ptr[i]->numKeys;
        delete _rt->ptr[i];// delete the node - need to handle this in the destructor
        
        for (int l = i; l < _rt->numKeys; l++) {// this moves over the keys and the ptrs in the parent node
            _rt->key[l-1] = _rt->key[l];
            _rt->ptr[l] = _rt->ptr[l+1];
        }
        _rt->numKeys--;
        if (_rt->isTooEmpty()) {
            return unsuccess;
        }
        else
            return success;
    }
    else { // the key to the left does not exist so we borrow from the node to the right
        /* we copy from the i+1 child node to the ith child node
         after we copy the ith key from the parent node into the ith child node
         */
        
        _rt->ptr[i]->key[_rt->ptr[i]->numKeys] = _rt->key[i];
        _rt->ptr[i]->numKeys++;
        
        int k = 0;
        int j = _rt->ptr[i]->numKeys;
        while (k < _rt->ptr[i+1]->numKeys) {
            _rt->ptr[i]->key[j] = _rt->ptr[i+1]->key[k];// copy the key over
            _rt->ptr[i]->ptr[j] = _rt->ptr[i+1]->ptr[k];// copy the ptr over
            k++; j++;
        }
        _rt->ptr[i]->ptr[j] = _rt->ptr[i+1]->ptr[k];// copy the last ptr over
        
        _rt->ptr[i]->numKeys += _rt->ptr[i+1]->numKeys;
        delete _rt->ptr[i+1];
        
        for (int l = i+1; l < _rt->numKeys+1; l++) {// this moves over the keys and the ptrs in the parent node
            _rt->key[l-1] = _rt->key[l];
            _rt->ptr[l] = _rt->ptr[l+1];
        }
        _rt->numKeys--;
        if (_rt->isTooEmpty()) {
            return unsuccess;
        }
        else
            return success;
    }
}

template<typename Treetype, int M1>
void BTree<Treetype, M1>::findPredecessor(BTreeNode<Treetype, M1> *_t, Treetype &data)
{
    while (_t->ptr[_t->numKeys] != NULL) {
        _t = _t->ptr[_t->numKeys];
    }
    data = _t->key[_t->numKeys-1];
}

template<typename Treetype, int M1>
bool BTree<Treetype, M1>::find(Treetype val)
{
    return _find(val, _root);
}

template<typename Treetype, int M1>
bool BTree<Treetype, M1>::_find(Treetype val, BTreeNode<Treetype, M1> *_rt)
{
    if (_rt == NULL) {
        return false;
    }
    else {
        int i;
        for (i = 0; i < _rt->numKeys; i++) {//loop thru the node
            if (_rt->key[i] >= val)
                break;
        }// end of for loop
        if (val == _rt->key[i])
            return true;
        else
            return _find(val, _rt->ptr[i]);
    }
    
}

template<typename Treetype, int M1>
void BTree<Treetype, M1>::printByLevel()
{
    _printByLevel(_root, 0);
}


template<typename Treetype, int M1>
void BTree<Treetype, M1>::_printByLevel(BTreeNode<Treetype, M1> *_rt, int numspaces)
{
    if (_rt == NULL) {
        return;
    }
    std::cout << setw(numspaces) << "";
    
    for (int i = 0; i < _rt->numKeys; i++){
        cout << setw(3) << _rt->key[i] << "";
    }
    /*if (_rt->isLeaf == true)
        cout << " leaf node" << "";
    else if (_rt->isLeaf == false)
        cout << " is not a leaf node" << "";*/
    cout << endl;
    for (int i = 0; i <= _rt->numKeys; i++){
        _printByLevel(_rt->ptr[i], numspaces+8);
    }
}

template<typename Treetype, int M1>
void BTree<Treetype, M1>::_delete_all(BTreeNode<Treetype, M1> *_rt)
{
    if (_rt == NULL) {
        return;
    }
    else {
        for (int i = 0; i <= _rt->numKeys; i++) {
            _delete_all(_rt->ptr[i]);
        }
        delete _rt;
    }
}


template<typename Treetype, int M1>
BTree<Treetype, M1>::~BTree()
{
    _delete_all(_root);
}



#endif
