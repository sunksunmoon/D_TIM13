#ifndef GUDANG_H
#define GUDANG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <cstdio> 
#include <algorithm> 
#include "struktur_data.h"

using namespace std;

// Inisialisasi variabel global pendukung
int counterKategori = 1;

// --- 1. FUNGSI AUXILIARY AVL (Pohon Kategori) ---

int hitungTinggi(NodeKategori* n) { 
    return (n == NULL) ? 0 : n->tinggi; 
}

NodeKategori* rotasiKanan(NodeKategori* y) {
    NodeKategori* x = y->left;
    NodeKategori* T2 = x->right;
    x->right = y; 
    y->left = T2;
    y->tinggi = (max(hitungTinggi(y->left), hitungTinggi(y->right))) + 1;
    x->tinggi = (max(hitungTinggi(x->left), hitungTinggi(x->right))) + 1;
    return x;
}

NodeKategori* rotasiKiri(NodeKategori* x) {
    NodeKategori* y = x->right;
    NodeKategori* T2 = y->left;
    y->left = x; 
    x->right = T2;
    x->tinggi = (max(hitungTinggi(x->left), hitungTinggi(x->right))) + 1;
    y->tinggi = (max(hitungTinggi(y->left), hitungTinggi(y->right))) + 1;
    return y;
}

NodeKategori* tambahKategori(NodeKategori* node, string kat) {
    if (node == NULL) {
        NodeKategori* baru = new NodeKategori;
        baru->namaKategori = kat;
        baru->tinggi = 1;
        baru->left = baru->right = NULL;
        return baru;
    }
    if (kat < node->namaKategori) node->left = tambahKategori(node->left, kat);
    else if (kat > node->namaKategori) node->right = tambahKategori(node->right, kat);
    else return node;

    node->tinggi = 1 + max(hitungTinggi(node->left), hitungTinggi(node->right));
    int balance = hitungTinggi(node->left) - hitungTinggi(node->right);

    if (balance > 1 && kat < node->left->namaKategori) return rotasiKanan(node);
    if (balance < -1 && kat > node->right->namaKategori) return rotasiKiri(node);
    return node;
}

// --- 2. FUNGSI MANAJEMEN STOK (CRUD & SORTING) ---

void simpanStokKeFile() {
    ofstream file("database_stok.txt");
    Barang* t = headGudang;
    while(t) {
        file << t->kode << "|" << t->nama << "|" << t->harga << "|" << t->kategori << "|" << t->stok << endl;
        t = t->next;
    }
    file.close();
}

void muatStokDariFile() {
    ifstream file("database_stok.txt");
    if (!file) return;

    // PENTING: Bersihkan list lama agar tidak duplikat saat dimuat ulang
    headGudang = NULL; 
    rootKategori = NULL;

    string line, k, n, kat, h_str, s_str;
    while (getline(file, line)) {
        if(line.empty()) continue;
        stringstream ss(line);
        getline(ss, k, '|');
        getline(ss, n, '|');
        getline(ss, h_str, '|');
        getline(ss, kat, '|');
        getline(ss, s_str, '|');

        Barang* baru = new Barang;
        baru->kode = k; baru->nama = n; baru->kategori = kat;
        baru->harga = atol(h_str.c_str()); 
        baru->stok = atoi(s_str.c_str());
        baru->next = NULL;

        // Gunakan Insert Back agar urutan tetap sesuai isi file
        if (headGudang == NULL) {
            headGudang = baru;
        } else {
            Barang* temp = headGudang;
            while(temp->next != NULL) temp = temp->next;
            temp->next = baru;
        }
        
        // Daftarkan kategori ke AVL Tree
        rootKategori = tambahKategori(rootKategori, kat);
    }
    file.close();
}

void urutkanStokTerendah() {
    if (headGudang == NULL || headGudang->next == NULL) {
        cout << "  [!] Data tidak cukup untuk diurutkan.\n";
        return;
    }
    bool swapped;
    Barang* ptr1;
    Barang* lptr = NULL;
    do {
        swapped = false;
        ptr1 = headGudang;
        while (ptr1->next != lptr) {
            if (ptr1->stok > ptr1->next->stok) {
                swap(ptr1->kode, ptr1->next->kode);
                swap(ptr1->nama, ptr1->next->nama);
                swap(ptr1->kategori, ptr1->next->kategori);
                swap(ptr1->harga, ptr1->next->harga);
                swap(ptr1->stok, ptr1->next->stok);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    cout << "  [OK] Stok berhasil diurutkan!\n";
}

void hapusBarangGudang() {
    cout << "\n--- HAPUS PRODUK ---\n";
    cout << "Masukkan Kode Barang: ";
    string target; cin >> target;

    Barang *temp = headGudang, *prev = NULL;
    if (temp != NULL && temp->kode == target) {
        headGudang = temp->next;
        delete temp;
        cout << ">>> Produk " << target << " berhasil dihapus!\n";
        simpanStokKeFile();
        return;
    }
    while (temp != NULL && temp->kode != target) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        cout << ">>> [!] Kode barang tidak ditemukan.\n";
        return;
    }
    prev->next = temp->next;
    delete temp;
    cout << ">>> Produk berhasil dihapus!\n";
    simpanStokKeFile();
}

void restockBarang() {
    cout << "\n--- UPDATE STOK (RESTOCK) ---\n";
    cout << "Masukkan Kode Barang: ";
    string target; cin >> target;
    Barang* t = headGudang;
    while(t) {
        if(t->kode == target) {
            cout << "Barang: " << t->nama << " | Stok Sekarang: " << t->stok << endl;
            cout << "Jumlah Stok Masuk: ";
            int tambahan = inputAngka();
            t->stok += tambahan;
            cout << ">>> Stok berhasil diperbarui!\n";
            simpanStokKeFile();
            return;
        }
        t = t->next;
    }
    cout << ">>> [!] Barang tidak ditemukan.\n";
}

// --- 3. TAMPILAN & MENU ---

void tampilkanTabelGudang() {
    cout << "\n+=======================================================================+" << endl;
    cout << "|                        DAFTAR STOK GUDANG                             |" << endl;
    cout << "+-----+---------------+---------------------------+------------+--------+" << endl;
    cout << "| No  | Kode Barang   | Nama Produk               | Harga      | Stok   |" << endl;
    cout << "+-----+---------------+---------------------------+------------+--------+" << endl;

    Barang* t = headGudang;
    int no = 1;
    if (t == NULL) {
        cout << "|                  [!] Gudang Kosong / Belum ada data.                  |" << endl;
    } else {
        while (t != NULL) {
            string status = (t->stok <= 5) ? "!" : " ";
            printf("| %-3d | %-13s | %-25s | Rp%-8ld | %-6d%s|\n", 
                   no, t->kode.c_str(), t->nama.c_str(), t->harga, t->stok, status.c_str());
            t = t->next;
            no++;
        }
    }
    cout << "+-----+---------------+---------------------------+------------+--------+" << endl;
    cout << "  Keterangan: (!) Stok Kritis (<= 5)" << endl;
}

void tambahBarangGudang() {
    Barang* baru = new Barang;
    cout << "\n--- INPUT BARANG BARU ---\n";
    cout << "Masukkan Kode Baru  : "; cin >> baru->kode;
    cin.ignore();
    cout << "Masukkan Nama Barang: "; getline(cin, baru->nama);
    cout << "Masukkan Kategori   : "; getline(cin, baru->kategori);
    cout << "Masukkan Harga      : "; baru->harga = inputAngka();
    cout << "Masukkan Stok       : "; baru->stok = inputAngka();

    baru->next = headGudang;
    headGudang = baru;
    rootKategori = tambahKategori(rootKategori, baru->kategori);
    simpanStokKeFile();
    cout << ">>> Barang berhasil ditambahkan!\n";
}

void menuGudang() {
    int pil;
    do {
        cout << "\n+==========================================+" << endl;
        cout << "|          MENU MANAJEMEN GUDANG           |" << endl;
        cout << "+==========================================+" << endl;
        cout << "| 1. Input Barang Baru                     |" << endl;
        cout << "| 2. Restock Barang (Tambah Stok)          |" << endl;
        cout << "| 3. Hapus Barang                          |" << endl;
        cout << "| 4. Lihat Seluruh Stok (Tabel)            |" << endl;
        cout << "| 5. Urutkan Stok Terendah (Bubble Sort)   |" << endl;
        cout << "| 0. Kembali                               |" << endl;
        cout << "+------------------------------------------+" << endl;
        cout << "  Pilih: "; pil = inputAngka();
        
        switch(pil) {
            case 1: tambahBarangGudang(); break;
            case 2: restockBarang(); break;
            case 3: hapusBarangGudang(); break;
            case 4: tampilkanTabelGudang(); break;
            case 5: urutkanStokTerendah(); break;
        }
    } while (pil != 0);
}

// --- 4. FUNGSI PENDUKUNG KATEGORI (AVL TREE TRANSVERSAL) ---

void tampilKategori(NodeKategori* root) {
    if (root != NULL) {
        tampilKategori(root->left);
        cout << " - " << root->namaKategori << endl;
        tampilKategori(root->right);
    }
}

void tampilKategoriBerseri(NodeKategori* root) {
    if (root != NULL) {
        tampilKategoriBerseri(root->left);
        cout << " " << counterKategori++ << ". " << root->namaKategori << endl;
        tampilKategoriBerseri(root->right);
    }
}

void cariKategoriByNomor(NodeKategori* root, int target, int &current, string &hasil) {
    if (root == NULL) return;
    cariKategoriByNomor(root->left, target, current, hasil);
    if (current == target) hasil = root->namaKategori;
    current++;
    cariKategoriByNomor(root->right, target, current, hasil);
}

// Fungsi inisialisasi default yang digunakan di main.cpp
void inisialisasiDefault() {
    // Fungsi ini bisa diisi dengan daftar barang manual 
    // agar saat pertama kali dijalankan (file kosong), sistem punya data awal.
    // Contoh:
    Barang* b1 = new Barang;
    b1->kode = "B001"; b1->nama = "Beras 5kg"; b1->kategori = "Sembako";
    b1->harga = 65000; b1->stok = 20; b1->next = headGudang;
    headGudang = b1;
    rootKategori = tambahKategori(rootKategori, b1->kategori);
}

#endif