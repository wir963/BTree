//
//  BTreeNode.h
//  BTree
//
//  Created by Welles Robinson on 11/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BTree_BTreeNode_h
#define BTree_BTreeNode_h

#include <math.h>

template<typename T, int M>
class BTreeNode {
public:
    BTreeNode();
private:
    T key[M];
    BTreeNode* ptr[M+1];
    int numKeys;
    bool isLeaf;
    bool isFull();
    bool isTooEmpty();
    template<typename Treetype, int M1>
    friend class BTree;
};

template <typename T, int M>
BTreeNode<T, M>::BTreeNode()
{
    for (int i = 0; i <= M; i++) {
        ptr[i] = NULL;
    }
}

/* Returns true if the node's capacity has been exceded
 * Returns false otherwise
 */
template <typename T, int M>
bool BTreeNode<T, M>::isFull()
{
    return (numKeys >= M);
}

/* Returns true if the node has less than floor(M/2)
 * Returns false if the node contains at least floor(M/2) values
 */
template <typename T, int M>
bool BTreeNode<T, M>::isTooEmpty()
{
    if (numKeys >= (((M+1)/2)-1)) {// equivalent to floor function
        return false;
    }
    else
        return true;
}



#endif
