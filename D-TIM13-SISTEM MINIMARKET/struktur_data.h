#ifndef STRUKTUR_DATA_H
#define STRUKTUR_DATA_H

#include <iostream>
#include <string>   
#include <fstream>  
#include <sstream>  
#include <cstdio>   
using namespace std;

// --- STRUKTUR DATA UTAMA ---
struct Barang {
    string kode, nama, kategori;
    long harga;
    int stok;
    Barang* next;
};

struct Nota {
    string detail;
    long subtotal;
    Nota* next;
};

struct NodeKategori {
    string namaKategori;
    int tinggi;
    NodeKategori *left, *right;
};

struct Pelanggan {
    string nama;
    Pelanggan* next;
};

// --- DEKLARASI VARIABEL GLOBAL (EXTERN) ---
extern string kasirAktif;
extern string shiftAktif;
extern int pelangganDilayani;
extern long totalPemasukanShift;

extern Barang* headGudang;
extern NodeKategori* rootKategori;
extern Nota* topNota;

// --- FORWARD DECLARATION (PENTING!) ---
// Tambahkan semua fungsi yang dipanggil lintas file di sini
void simpanStokKeFile();
void muatStokDariFile();
void inisialisasiDefault();
void inisialisasiMemberDefault();
void tampilKategoriBerseri(NodeKategori* root);
void tampilkanTabelGudang();
void menuAdmin();
void menuKasir();
void menuGudang();

// --- FUNGSI VALIDASI INPUT GLOBAL ---
inline long inputAngka() {
    string masukan;
    long n;
    while (true) {
        cin >> masukan;
        stringstream ss(masukan);
        if (ss >> n && n >= 0) { // Validasi harus angka dan tidak negatif
            return n;
        } else {
            cout << "   [!] Error: Masukkan angka yang valid: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}
#endif
