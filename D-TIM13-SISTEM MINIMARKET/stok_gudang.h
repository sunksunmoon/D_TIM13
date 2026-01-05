#ifndef STOK_GUDANG_H
#define STOK_GUDANG_H

#include <iostream>
#include <fstream>

using namespace std;

// DEKLARASI EXTERN
// Menghubungkan ke fungsi di supplier.h tanpa tabrakan include
extern void tambahSupplierOtomatis(string id, string nama);

struct BarangGudang {
    string kode;
    string nama;
    string kategori;
    long jumlah;
    string supplier;
    BarangGudang* next;
};

BarangGudang* headGudang = NULL;

// --- FUNGSI SIMPAN LAPORAN GUDANG ---
void simpanKeFileGudang(BarangGudang* baru) {
    ofstream file("laporan_gudang.txt", ios::app); // 'app' agar data lama tidak hilang (append)
    if (file.is_open()) {
        file << "KODE:" << baru->kode << " | BARANG:" << baru->nama 
             << " | JML:" << baru->jumlah << " | SUPP:" << baru->supplier << endl;
        file.close();
    }
}

void tambahStok() {
    BarangGudang* baru = new BarangGudang;
    
    cout << "\n==== INPUT BARANG MASUK GUDANG ====\n";
    cout << "Kode Barang    : "; cin >> baru->kode;
    cin.ignore();
    cout << "Nama Barang    : "; getline(cin, baru->nama);
    cout << "Kategori       : "; cin >> baru->kategori;
    cout << "Jumlah Masuk   : "; cin >> baru->jumlah;
    cin.ignore();
    cout << "Nama Supplier  : "; getline(cin, baru->supplier);
    
    baru->next = NULL;

    // 1. Integrasi Otomatis ke Supplier
    // Nama supplier dari gudang langsung didaftarkan ke Doubly Linked List Supplier
    tambahSupplierOtomatis(baru->supplier, baru->supplier); 

    // 2. Masukkan ke Linked List Gudang (Insert Last)
    if (headGudang == NULL) {
        headGudang = baru;
    } else {
        BarangGudang* temp = headGudang;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = baru;
    }
    
    // 3. Simpan ke File Laporan
    simpanKeFileGudang(baru);

    cout << "[Gudang] Barang berhasil dicatat & Supplier otomatis terdaftar!\n";
}

void tampilStok() {
    if (headGudang == NULL) {
        cout << "Gudang kosong, belum ada barang masuk.\n";
        return;
    }

    cout << "\n============ LAPORAN STOK GUDANG ============\n";
    cout << "Kode\tNama Barang\tKat\tStok\tSupplier\n";
    cout << "--------------------------------------------------\n";
    
    BarangGudang* temp = headGudang;
    while (temp != NULL) {
        cout << temp->kode << "\t" 
             << temp->nama << "\t" 
             << temp->kategori << "\t" 
             << temp->jumlah << "\t" 
             << temp->supplier << endl;
        temp = temp->next;
    }
    cout << "--------------------------------------------------\n";
}

// ================= TAMBAH STOK SECARA PROGRAM =================
void tambahStokBarang(string namaBarang, long jumlah) {
    BarangGudang* temp = headGudang;
    while (temp != NULL) {
        if (temp->nama == namaBarang) {
            temp->jumlah += jumlah;
            return;
        }
        temp = temp->next;
    }
    // Tambahkan barang baru jika tidak ada
    BarangGudang* baru = new BarangGudang;
    baru->kode = "UNKNOWN";
    baru->nama = namaBarang;
    baru->kategori = "UNKNOWN";
    baru->jumlah = jumlah;
    baru->supplier = "UNKNOWN";
    baru->next = NULL;

    if (headGudang == NULL) headGudang = baru;
    else {
        BarangGudang* temp2 = headGudang;
        while (temp2->next != NULL) temp2 = temp2->next;
        temp2->next = baru;
    }
}

// Kurangi stok barang di gudang
bool kurangiStokBarang(string namaBarang, long jumlah) {
    BarangGudang* temp = headGudang;
    while (temp != NULL) {
        if (temp->nama == namaBarang) {
            if (temp->jumlah >= jumlah) {
                temp->jumlah -= jumlah;
                return true;
            } else {
                cout << "[WARNING] Stok tidak cukup untuk " << namaBarang << endl;
                return false;
            }
        }
        temp = temp->next;
    }
    cout << "[WARNING] Barang " << namaBarang << " tidak ditemukan di gudang\n";
    return false;
}


#endif
