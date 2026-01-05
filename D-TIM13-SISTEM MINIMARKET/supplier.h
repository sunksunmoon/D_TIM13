#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
  PERTEMUAN 5 & 6
  Materi : Doubly Linked List & File Handling
  Fitur  : Manajemen Supplier (CRUD Lengkap + Sinkronisasi Gudang)
*/

struct Supplier {
    string idSupplier;
    string namaPT;
    long long int kontak;
    string alamat;
    string kategoriProduk;
    Supplier* next;
    Supplier* prev; 
};

// Pointer Global untuk Doubly Linked List
Supplier* headSupplier = NULL;
Supplier* tailSupplier = NULL;

// ================= FUNGSI HELPER (SINKRONISASI FILE) =================
void simpanKeFileSupplier() {
    ofstream file("daftar_supplier.txt", ios::trunc); 
    if (file.is_open()) {
        Supplier* temp = headSupplier;
        while (temp != NULL) {
            file << "ID: " << temp->idSupplier << " | PT: " << temp->namaPT 
                 << " | CP: " << temp->kontak << " | Bidang: " << temp->kategoriProduk << endl;
            temp = temp->next;
        }
        file.close();
    }
}

// ================= 1. FUNGSI TAMBAH (CREATE) =================
// Fungsi Otomatis: Dipanggil saat input barang di Gudang
void tambahSupplierOtomatis(string id, string nama) {
    // Validasi Duplikasi: Cek apakah nama supplier sudah ada di list
    Supplier* cek = headSupplier;
    while (cek != NULL) {
        if (cek->namaPT == nama || cek->idSupplier == id) return; 
        cek = cek->next;
    }

    Supplier* baru = new Supplier;
    baru->idSupplier = id;
    baru->namaPT = nama;
    baru->kontak = 0; 
    baru->alamat = "-";
    baru->kategoriProduk = "-";
    baru->next = NULL;
    baru->prev = NULL;

    if (headSupplier == NULL) {
        headSupplier = baru;
        tailSupplier = baru;
    } else {
        tailSupplier->next = baru;
        baru->prev = tailSupplier;
        tailSupplier = baru;
    }
    simpanKeFileSupplier();
}

// Fungsi Manual: Input lengkap dari Menu Admin/Supplier
void tambahSupplier() {
    Supplier* baru = new Supplier;
    cout << "\n==== REGISTRASI SUPPLIER BARU ====\n";
    cout << "ID Supplier      : "; cin >> baru->idSupplier;
    cin.ignore();
    cout << "Nama Perusahaan  : "; getline(cin, baru->namaPT);
    cout << "Kontak (WA/Telp) : "; cin >> baru->kontak;
    cin.ignore();
    cout << "Alamat Kantor    : "; getline(cin, baru->alamat);
    cout << "Kategori Produk  : "; getline(cin, baru->kategoriProduk);
    
    baru->next = NULL;
    baru->prev = NULL;

    if (headSupplier == NULL) {
        headSupplier = baru;
        tailSupplier = baru;
    } else {
        tailSupplier->next = baru;
        baru->prev = tailSupplier;
        tailSupplier = baru;
    }
    simpanKeFileSupplier();
    cout << "[Sistem] Data Supplier berhasil disimpan!\n";
}

// ================= 2. FUNGSI TAMPIL (READ) =================
void tampilSupplier() {
    if (headSupplier == NULL) {
        cout << "\n[Pesan] Belum ada data supplier terdaftar.\n";
        return;
    }

    cout << "\n============ DAFTAR SUPPLIER (DOUBLY LINKED LIST) ============\n";
    cout << "ID\tNama PT\t\tKontak\t\tKategori\n";
    cout << "--------------------------------------------------------------\n";
    
    Supplier* temp = headSupplier;
    while (temp != NULL) {
        cout << temp->idSupplier << "\t" 
             << temp->namaPT << "\t\t" 
             << temp->kontak << "\t\t" 
             << temp->kategoriProduk << endl;
        temp = temp->next;
    }
    cout << "--------------------------------------------------------------\n";
}

// ================= 3. FUNGSI UPDATE (EDIT) =================
void updateSupplier() {
    if (headSupplier == NULL) {
        cout << "\n[Pesan] Daftar supplier masih kosong.\n";
        return;
    }

    string idCari;
    cout << "\n==== LENGKAPI DATA SUPPLIER ====\n";
    cout << "Masukkan ID/Nama Supplier: ";
    cin >> idCari;

    Supplier* temp = headSupplier;
    bool found = false;

    while (temp != NULL) {
        if (temp->idSupplier == idCari || temp->namaPT == idCari) {
            cout << "Data ditemukan: " << temp->namaPT << endl;
            cout << "--------------------------------\n";
            cout << "Kontak Baru (WA) : "; cin >> temp->kontak;
            cin.ignore();
            cout << "Alamat Baru      : "; getline(cin, temp->alamat);
            cout << "Kategori Produk  : "; getline(cin, temp->kategoriProduk);
            
            found = true;
            simpanKeFileSupplier();
            cout << "\n[Sistem] Data berhasil diperbarui & sinkron ke file!\n";
            break;
        }
        temp = temp->next;
    }
    if (!found) cout << "[Gagal] Supplier dengan identitas tersebut tidak ditemukan.\n";
}

// ================= 4. FUNGSI HAPUS (DELETE) =================
void hapusSupplier() {
    if (headSupplier == NULL) {
        cout << "\n[Pesan] Tidak ada data untuk dihapus.\n";
        return;
    }

    string idCari;
    cout << "\n==== HAPUS DATA SUPPLIER ====\n";
    cout << "Masukkan ID/Nama yang akan dihapus: ";
    cin >> idCari;

    Supplier* temp = headSupplier;
    bool found = false;

    while (temp != NULL) {
        if (temp->idSupplier == idCari || temp->namaPT == idCari) {
            found = true;
            
            // Logika penghapusan Doubly Linked List
            if (temp == headSupplier) { // Kasus 1: Hapus di depan
                headSupplier = temp->next;
                if (headSupplier != NULL) headSupplier->prev = NULL;
                else tailSupplier = NULL;
            } 
            else if (temp == tailSupplier) { // Kasus 2: Hapus di belakang
                tailSupplier = temp->prev;
                tailSupplier->next = NULL;
            } 
            else { // Kasus 3: Hapus di tengah
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
            }

            delete temp;
            simpanKeFileSupplier();
            cout << "[Sistem] Supplier '" << idCari << "' telah dihapus selamanya.\n";
            break;
        }
        temp = temp->next;
    }
    if (!found) cout << "[Gagal] Supplier tidak ditemukan.\n";
}

#endif
