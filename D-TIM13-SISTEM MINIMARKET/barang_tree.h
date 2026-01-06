#ifndef BARANG_TREE_H
#define BARANG_TREE_H

#include <iostream>
#include <string>
using namespace std;

/*
  PERTEMUAN 10
  Materi : AVL Tree
  Fitur  : Manajemen barang + hapus root (predecessor)
*/

struct NodeBarang {
    string kodeBarang;
    string namaBarang;
    long hargaBarang;
    int height;
    NodeBarang* left;
    NodeBarang* right;
};

/* ====== ROOT GLOBAL (DECLARATION ONLY) ====== */
extern NodeBarang* rootUtama;

/* ====== PROTOTYPE FUNGSI ====== */
int tinggi(NodeBarang* n);
int max(int a, int b);
int balanceFactor(NodeBarang* n);

NodeBarang* buatNode(string kode, string nama);
NodeBarang* rightRotate(NodeBarang* y);
NodeBarang* leftRotate(NodeBarang* x);

NodeBarang* insertAVL(NodeBarang* node, string kode, string nama);
NodeBarang* cariPredecessor(NodeBarang* node);
NodeBarang* hapusAVL(NodeBarang* root, string kode);

void inorder(NodeBarang* root);

/* ====== IMPLEMENTASI ====== */

int tinggi(NodeBarang* n) {
    return (n == NULL) ? 0 : n->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

NodeBarang* buatNode(string kode, string nama) {
    NodeBarang* baru = new NodeBarang();
    baru->kodeBarang = kode;
    baru->namaBarang = nama;
    baru->height = 1;
    baru->left = baru->right = NULL;
    return baru;
}

int balanceFactor(NodeBarang* n) {
    return (n == NULL) ? 0 : tinggi(n->left) - tinggi(n->right);
}

/* ====== ROTASI ====== */

NodeBarang* rightRotate(NodeBarang* y) {
    NodeBarang* x = y->left;
    NodeBarang* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(tinggi(y->left), tinggi(y->right)) + 1;
    x->height = max(tinggi(x->left), tinggi(x->right)) + 1;

    return x;
}

NodeBarang* leftRotate(NodeBarang* x) {
    NodeBarang* y = x->right;
    NodeBarang* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(tinggi(x->left), tinggi(x->right)) + 1;
    y->height = max(tinggi(y->left), tinggi(y->right)) + 1;

    return y;
}

/* ====== INSERT AVL ====== */

NodeBarang* insertAVL(NodeBarang* node, string kode, string nama) {
    if (node == NULL)
        return buatNode(kode, nama);

    if (kode < node->kodeBarang)
        node->left = insertAVL(node->left, kode, nama);
    else if (kode > node->kodeBarang)
        node->right = insertAVL(node->right, kode, nama);
    else
        return node;

    node->height = 1 + max(tinggi(node->left), tinggi(node->right));
    int bf = balanceFactor(node);

    // LL
    if (bf > 1 && kode < node->left->kodeBarang)
        return rightRotate(node);

    // RR
    if (bf < -1 && kode > node->right->kodeBarang)
        return leftRotate(node);

    // LR
    if (bf > 1 && kode > node->left->kodeBarang) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL
    if (bf < -1 && kode < node->right->kodeBarang) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

/* ====== PREDECESSOR ====== */

NodeBarang* cariPredecessor(NodeBarang* node) {
    NodeBarang* cur = node;
    while (cur->right != NULL)
        cur = cur->right;
    return cur;
}

/* ====== DELETE AVL ====== */

NodeBarang* hapusAVL(NodeBarang* root, string kode) {
    if (root == NULL) return root;

    if (kode < root->kodeBarang)
        root->left = hapusAVL(root->left, kode);
    else if (kode > root->kodeBarang)
        root->right = hapusAVL(root->right, kode);
    else {
        if (root->left == NULL || root->right == NULL) {
            NodeBarang* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            delete temp;
        } else {
            NodeBarang* temp = cariPredecessor(root->left);
            root->kodeBarang = temp->kodeBarang;
            root->namaBarang = temp->namaBarang;
            root->left = hapusAVL(root->left, temp->kodeBarang);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(tinggi(root->left), tinggi(root->right));
    int bf = balanceFactor(root);

    if (bf > 1 && balanceFactor(root->left) >= 0)
        return rightRotate(root);

    if (bf > 1 && balanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (bf < -1 && balanceFactor(root->right) <= 0)
        return leftRotate(root);

    if (bf < -1 && balanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* ====== TAMPIL ====== */

void inorder(NodeBarang* root) {
    if (root == NULL) return;
    inorder(root->left);
    cout << "[" << root->kodeBarang << "] " << root->namaBarang << endl;
    inorder(root->right);
}

/* ====== VISUALISASI POHON (PENGGANTI INORDER) ====== */
void visualisasiPohon(NodeBarang* root, int indent = 0) {
    if (root == NULL) return;

    // Jarak horizontal antar level pohon
    int spasi = 10;
    indent += spasi;

    // 1. Rekursif ke kanan dulu (agar tampil di bagian atas terminal)
    visualisasiPohon(root->right, indent);

    // 2. Cetak node dengan spasi sesuai tingkatannya
    cout << endl;
    for (int i = spasi; i < indent; i++) cout << " ";
    cout << "|--[" << root->kodeBarang << "]" << endl;

    // 3. Rekursif ke kiri (tampil di bagian bawah terminal)
    visualisasiPohon(root->left, indent);
}

/* ====== SEARCH AVL ====== */
NodeBarang* cariBarang(NodeBarang* root, string kode) {
    // Jika pohon kosong atau kode ditemukan di root
    if (root == NULL || root->kodeBarang == kode)
        return root;

    // Jika kode lebih besar dari kode root, cari di kanan
    if (kode > root->kodeBarang)
        return cariBarang(root->right, kode);

    // Jika kode lebih kecil, cari di kiri
    return cariBarang(root->left, kode);
}
#endif
/*
====================================================
PENJELASAN DELETE ROOT AVL TREE

Penghapusan root pada AVL Tree dilakukan dengan mencari
node predecessor, yaitu node dengan nilai terbesar pada
subtree kiri. Nilai predecessor menggantikan nilai root
yang dihapus. Setelah penghapusan, dilakukan proses
pengecekan keseimbangan dari node bekas penghapusan hingga
ke root. Jika ditemukan ketidakseimbangan, maka dilakukan
rotasi AVL (LL, RR, LR, atau RL) agar pohon tetap seimbang.
====================================================
*/
