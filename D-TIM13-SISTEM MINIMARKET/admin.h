#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <cstdio> 
#include <fstream>
#include <vector>
#include "gudang.h"
#include "kasir.h"
#include "ktp.h"

using namespace std;

// --- 1. FUNGSI LOG PENJUALAN PERMANEN (FILE I/O) ---

void simpanLaporanKeFile(string detail, long total) {
    ofstream file("laporan_penjualan.txt", ios::app);
    if (file.is_open()) {
        file << "KASIR:" << (kasirAktif == "" ? "Unknown" : kasirAktif) << "|" 
             << "SHIFT:" << (shiftAktif == "" ? "Unknown" : shiftAktif) << "|" 
             << "TOTAL:" << total << "|" 
             << "INFO:" << detail << endl;
        file.close();
    }
}

void lihatLaporanPenjualanLengkap() {
    ifstream file("laporan_penjualan.txt");
    cout << "\n+===========================================================================+" << endl;
    cout << "|                       LAPORAN PENJUALAN KESELURUHAN                       |" << endl;
    cout << "+===========================================================================+" << endl;
    
    if (!file) {
        cout << "|                [!] Belum ada riwayat penjualan tersimpan.                 |" << endl;
        cout << "+===========================================================================+" << endl;
        return;
    }

    string line;
    long grandTotal = 0;
    int count = 1;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string k, s, t, d;
        
        getline(ss, k, '|'); // Bagian Kasir
        getline(ss, s, '|'); // Bagian Shift
        getline(ss, t, '|'); // Bagian Total
        getline(ss, d, '|'); // Bagian Info
        
        // Parsing angka total (Menghapus prefix "TOTAL:")
        long valTotal = 0;
        if (t.find("TOTAL:") != string::npos) {
            valTotal = atol(t.substr(6).c_str());
        }
        grandTotal += valTotal;

        cout << " " << count << ". [" << k << "] [" << s << "]" << endl;
        cout << "    Ringkasan : " << (d.length() > 5 ? d.substr(5, 50) + "..." : "-") << endl;
        cout << "    Subtotal  : Rp " << valTotal << endl;
        cout << "    ---------------------------------------------------------------------" << endl;
        count++;
    }
    
    cout << "+===========================================================================+" << endl;
    cout << "  TOTAL PENDAPATAN TERAKUMULASI : Rp " << grandTotal << endl;
    cout << "+===========================================================================+" << endl;
    file.close();
}

// --- 2. MANAJEMEN MEMBER ---

void registrasiMember() {
    KTP* baru = new KTP;
    baru->inputKTP(); 
    
    if (headMember == NULL) {
        headMember = baru;
    } else {
        baru->next = headMember;
        headMember = baru;
    }
    cout << "\n  [OK] Data member berhasil ditambahkan ke Linked List.\n";
}

void lihatDaftarMember() {
    cout << "\n+=======================================================================+" << endl;
    cout << "|                         DAFTAR MEMBER MINIMARKET                      |" << endl;
    cout << "+-----+-----------------+---------------------------+-------------------+" << endl;
    cout << "| No  |      NIK        |       Nama Member         |      Status       |" << endl;
    cout << "+-----+-----------------+---------------------------+-------------------+" << endl;
    
    KTP* temp = headMember;
    if (temp == NULL) {
        cout << "|                [!] Belum ada member yang terdaftar.                   |" << endl;
    } else {
        int no = 1;
        while (temp != NULL) {
            printf("| %-3d | %-15s | %-25s | %-17s |\n", 
                   no, temp->nik.c_str(), temp->nama.c_str(), "MEMBER PRIORITAS");
            temp = temp->next;
            no++;
        }
    }
    cout << "+-----+-----------------+---------------------------+-------------------+" << endl;
}

// --- 3. MONITORING STOK ---

void pantauStokKritis() {
    cout << "\n+=======================================================+" << endl;
    cout << "|            LAPORAN STOK KRITIS (STOK <= 5)            |" << endl;
    cout << "+-----+---------------+---------------------------+--------+" << endl;
    cout << "| No  | Kode Barang   | Nama Produk               | Sisa   |" << endl;
    cout << "+-----+---------------+---------------------------+--------+" << endl;

    Barang* b = headGudang;
    bool adaKritis = false;
    int no = 1;

    while(b) {
        if (b->stok <= 5) {
            printf("| %-3d | %-13s | %-25s | %-6d |\n", 
                   no, b->kode.c_str(), b->nama.c_str(), b->stok);
            adaKritis = true;
            no++;
        }
        b = b->next;
    }

    if (!adaKritis) {
        cout << "|             [OK] Tidak ada stok kritis saat ini.       |" << endl;
    }
    cout << "+-----+---------------+---------------------------+--------+" << endl;
}

// --- 4. MENU UTAMA ADMIN ---

void menuAdmin() {
    int pil;
    do {
        cout << "\n+==========================================+" << endl;
        cout << "|             MENU UTAMA ADMIN             |" << endl;
        cout << "+==========================================+" << endl;
        cout << "| 1. Registrasi Member Baru                |" << endl;
        cout << "| 2. Lihat Daftar Member Terdaftar         |" << endl;
        cout << "| 3. Riwayat Transaksi (Session Stack)     |" << endl;
        cout << "| 4. Pantau Laporan Shift & Pemasukan      |" << endl;
        cout << "| 5. Cek Stok Barang Kritis                |" << endl;
        cout << "| 6. Lihat Seluruh Stok & Kategori         |" << endl;
        cout << "| 7. Log Penjualan Permanen (Database)     |" << endl;
        cout << "| 0. Kembali ke Menu Login                 |" << endl;
        cout << "+------------------------------------------+" << endl;
        cout << "  Pilih: "; pil = inputAngka(); 

        switch(pil) {
            case 1: registrasiMember(); break;
            case 2: lihatDaftarMember(); break;
            case 3:
                cout << "\n--- RIWAYAT NOTA (STACK) ---" << endl;
                {
                    Nota* t = topNota;
                    if (!t) cout << "  [!] Belum ada transaksi di sesi ini.\n";
                    while(t) {
                        cout << " > " << t->detail << " | Total: Rp " << t->subtotal << endl;
                        t = t->next;
                    }
                }
                break;
            case 4:
                cout << "\n+------- LAPORAN OPERASIONAL SHIFT -------+" << endl;
                cout << "  Kasir Aktif : " << (kasirAktif == "" ? "N/A" : kasirAktif) << endl;
                cout << "  Shift       : " << (shiftAktif == "" ? "N/A" : shiftAktif) << endl;
                cout << "  Total Pelanggan : " << pelangganDilayani << " orang" << endl;
                cout << "  Total Pemasukan : Rp " << totalPemasukanShift << endl;
                cout << "+-----------------------------------------+" << endl;
                cout << "  Reset data shift sekarang? (y/n): ";
                char r; cin >> r;
                if(r == 'y' || r == 'Y') {
                    totalPemasukanShift = 0;
                    pelangganDilayani = 0;
                    cout << "  [!] Data shift telah di-nolkan.\n";
                }
                break;
            case 5: pantauStokKritis(); break;
            case 6:
                cout << "\n--- STRUKTUR KATEGORI (AVL TREE) ---" << endl;
                tampilKategori(rootKategori);
                tampilkanTabelGudang(); 
                break;
            case 7: lihatLaporanPenjualanLengkap(); break;
        }
    } while (pil != 0);
}

#endif
