#ifndef KATEGORI_TREE_H
#define KATEGORI_TREE_H

#include <iostream>

using namespace std;

/*
  PERTEMUAN 9
  Materi : Pohon (Binary Tree)
  Fitur  : Struktur kategori barang minimarket
*/

struct NodeKat {
    char data;
    NodeKat* left;
    NodeKat* right;
};

NodeKat* buatNode(char nilai) {
    NodeKat* baru=new NodeKat();
    baru->data=nilai;
    baru->left=baru->right=NULL;
    return baru;
}

void preorder(NodeKat* node){
    if(node==NULL) return;
    cout<<node->data<<" ";
    preorder(node->left);
    preorder(node->right);
}

void inorder(NodeKat* node){
    if(node==NULL) return;
    inorder(node->left);
    cout<<node->data<<" ";
    inorder(node->right);
}

void postorder(NodeKat* node){
    if(node==NULL) return;
    postorder(node->left);
    postorder(node->right);
    cout<<node->data<<" ";
}

#endif

