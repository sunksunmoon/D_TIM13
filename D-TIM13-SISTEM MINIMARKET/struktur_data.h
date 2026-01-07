#ifndef STRUKTUR_DATA_H
#define STRUKTUR_DATA_H

#include <iostream>
#include <string>   
#include <fstream>  
#include <sstream>  
#include <cstdio>   
using namespace std;

// --- STRUKTUR DATA UTAMA ---

// Linked List untuk stok barang di gudang
struct Barang {
    string kode, nama, kategori;
    long harga;
    int stok;
    Barang* next;
};

// Stack untuk riwayat transaksi sesi ini
struct Nota {
    string detail;
    long subtotal;
    Nota* next;
};

// AVL Tree untuk kategori barang
struct NodeKategori {
    string namaKategori;
    int tinggi;
    NodeKategori *left, *right;
};

// Queue untuk antrean pelanggan di kasir
struct Pelanggan {
    string nama;
    bool isMember; // Tambahan: Untuk membedakan antrean prioritas
    Pelanggan* next;
};

// --- DEKLARASI VARIABEL GLOBAL (EXTERN) ---
// Variabel ini didefinisikan di main.cpp dan digunakan di seluruh file .h
extern string kasirAktif;
extern string shiftAktif;
extern int pelangganDilayani;
extern long totalPemasukanShift;

extern Barang* headGudang;
extern NodeKategori* rootKategori;
extern Nota* topNota;

// PENTING: Menambahkan headMember agar bisa diakses Admin, Kasir, dan KTP
struct KTP; // Forward declaration class KTP
extern KTP* headMember; 

// --- FORWARD DECLARATION (KOMUNIKASI LINTAS FILE) ---
void simpanStokKeFile();
void muatStokDariFile();
void inisialisasiDefault();
void inisialisasiMemberDefault();
void muatKTPDariFile();

// Fungsi tampilan yang dipanggil lintas file
void tampilKategori(NodeKategori* root);
void tampilKategoriBerseri(NodeKategori* root);
void tampilkanTabelGudang();

// Fungsi Menu Utama
void menuAdmin();
void menuKasir();
void menuGudang();

// --- FUNGSI VALIDASI INPUT GLOBAL ---
// Digunakan untuk mencegah program crash saat user memasukkan huruf pada pilihan menu
inline long inputAngka() {
    string masukan;
    long n;
    while (true) {
        cin >> masukan;
        stringstream ss(masukan);
        if (ss >> n && n >= 0) { 
            return n;
        } else {
            cout << "    [!] Error: Masukkan angka yang valid: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

#endif