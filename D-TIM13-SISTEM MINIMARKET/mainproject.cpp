#include <iostream>
using namespace std;

#include "ktp.h"
#include "barang_statis.h"
#include "stok_gudang.h"
#include "transaksi_stack.h"
#include "supplier.h" 
#include "kasir_queue.h" 
#include "prioritas_queue.h" 
#include "kategori_tree.h" 
#include "barang_tree.h" 

NodeBarang* rootUtama = NULL;
KTP member;

// ================= FUNGSI BANTU KASIR =================

// Kurangi stok barang di gudang (otomatis)
bool kurangiStokGudang(string namaBarang, long jumlahBeli) {
    BarangGudang* temp = headGudang;
    while (temp != NULL) {
        if (temp->nama == namaBarang) {
            if (temp->jumlah >= jumlahBeli) {
                temp->jumlah -= jumlahBeli;
                return true;
            } else {
                cout << "[WARN] Stok tidak cukup!\n";
                return false;
            }
        }
        temp = temp->next;
    }
    cout << "[WARN] Barang tidak ditemukan!\n";
    return false;
}

// Kembalikan stok jika transaksi dibatalkan
void kembalikanStok(Transaksi* t) {
    BarangGudang* temp = headGudang;
    while (temp != NULL) {
        if (temp->nama == t->namaBarang) {
            temp->jumlah += t->jumlah; // langsung tambahkan
            break;
        }
        temp = temp->next;
    }
}

// ================= LOGIN ROLE =================
bool loginAdmin() {
    int pass;
    cout << "\n==============================================";
    cout << "\n                LOGIN ADMIN                   ";
    cout << "\n==============================================";
    cout << "\nMasukkan password admin: ";
    cin >> pass;

    if (pass == 1111) {
        cout << ">>> Login Berhasil!\n";
        return true;
    } else {
        cout << ">>> Password Salah!\n";
        return false;
    }
}

bool loginKasir() {
    int pass;
    cout << "\n==============================================";
    cout << "\n                LOGIN KASIR                   ";
    cout << "\n==============================================";
    cout << "\nMasukkan password kasir: ";
    cin >> pass;

    if (pass != 2222) {
        cout << ">>> Password Salah!\n";
        return false;
    }

    // Pilihan Nama Kasir
    int namaPilihan;
    cout << "\nPilih Nama Kasir:\n";
    cout << "1. Elizabteh\n";
    cout << "2. Victoria\n";
    cout << "3. William\n";
    cout << "4. Phillips\n";
    cout << "Pilih nomor: ";
    cin >> namaPilihan;

    string kasirDipilih;
    switch(namaPilihan) {
        case 1: kasirDipilih = "Elizabteh"; break;
        case 2: kasirDipilih = "Victoria"; break;
        case 3: kasirDipilih = "William"; break;
        case 4: kasirDipilih = "Phillips"; break;
        default: 
            cout << "[WARNING] Pilihan salah, pakai default: Elizabteh\n";
            kasirDipilih = "Elizabteh"; 
            break;
    }

    // Pilihan Shift
    int shiftPilihan;
    cout << "\nPilih Shift Kasir:\n";
    cout << "1. Pagi (07:00-15:00)\n";
    cout << "2. Sore (15:00-23:00)\n";
    cout << "3. Malam (23:00-07:00)\n";
    cout << "Pilih nomor shift: ";
    cin >> shiftPilihan;

    string shiftDipilih;
    switch(shiftPilihan) {
        case 1: shiftDipilih = "07:00-15:00"; break;
        case 2: shiftDipilih = "15:00-23:00"; break;
        case 3: shiftDipilih = "23:00-07:00"; break;
        default: 
            cout << "[WARNING] Pilihan salah, pakai default: 07:00-15:00\n";
            shiftDipilih = "07:00-15:00"; 
            break;
    }

    kasirAktif = kasirDipilih;
    cout << ">>> Login Berhasil!\n";
    cout << "Kasir Aktif : " << kasirAktif << endl;
    cout << "Shift       : " << shiftDipilih << endl;

    return true;
}

bool loginGudang() {
    int pass;
    cout << "\n==============================================";
    cout << "\n                LOGIN GUDANG                  ";
    cout << "\n==============================================";
    cout << "\nMasukkan password gudang: ";
    cin >> pass;

    if (pass == 3333) {
        cout << ">>> Login Berhasil!\n";
        return true;
    } else {
        cout << ">>> Password Salah!\n";
        return false;
    }
}

// ================= MENU PER ROLE =================

// 1. MENU ADMIN 
void menuAdmin() {
    int pilih;
    do {
        cout << "\n==============================================";
        cout << "\n                MENU ADMIN                    ";
        cout << "\n==============================================";
        cout << "\n1. Manajemen Master Barang (Statis + Dinamis)";
        cout << "\n2. Manajemen Stok Gudang (Single LL)";
        cout << "\n3. Manajemen Data Supplier (Doubly LL)";
        cout << "\n4. Manajemen Pohon Barang (AVL Tree)";
        cout << "\n0. Kembali";
        cout << "\n----------------------------------------------";
        cout << "\nPilih: ";
        cin >> pilih;
        
        switch (pilih) {
            case 1: 
                menuBarangStatis(); 
                break;

            case 2: {
                int pGudang;
                do {
                    cout << "\n--- SUB-MENU STOK GUDANG ---";
                    cout << "\n1. Tambah Stok (Auto-Sync Supplier)";
                    cout << "\n2. Tampilkan Laporan Stok";
                    cout << "\n0. Kembali";
                    cout << "\nPilih: "; cin >> pGudang;
                    if(pGudang == 1) tambahStok();
                    else if(pGudang == 2) tampilStok();
                } while(pGudang != 0);
                break;
            }

            case 3: {
                int pSupp;
                do {
                    cout << "\n--- SUB-MENU SUPPLIER ---";
                    cout << "\n1. Tambah Supplier Manual";
                    cout << "\n2. Tampilkan Daftar Supplier";
                    cout << "\n3. Lengkapi/Update Data Supplier"; 
                    cout << "\n4. Hapus Supplier (Doubly LL Logic)"; 
                    cout << "\n0. Kembali";
                    cout << "\nPilih: "; cin >> pSupp;
                    if(pSupp == 1) tambahSupplier();
                    else if(pSupp == 2) tampilSupplier();
                    else if(pSupp == 3) updateSupplier(); 
                    else if(pSupp == 4) hapusSupplier();
                } while(pSupp != 0);
                break;
            }

            case 4: {
                int pTree;
                do {
                    cout << "\n--- SUB-MENU DATA TERURUT (AVL) ---";
                    cout << "\n1. Tambah Kode Barang";
                    cout << "\n2. Tampilkan Struktur Visual Pohon";
                    cout << "\n0. Kembali";
                    cout << "\nPilih: "; cin >> pTree;
                    if (pTree == 1) {
                        string kodeBaru, namaB;
                        cout << "Masukkan kode: "; cin >> kodeBaru;
                        cin.ignore();
                        cout << "Masukkan nama: "; getline(cin, namaB);
                        rootUtama = insertAVL(rootUtama, kodeBaru, namaB);
                        cout << "Berhasil ditambahkan ke Tree.\n";
                    }
                    else if (pTree == 2) {
			            if (rootUtama == NULL) {
			                cout << "Tree masih kosong!\n";
			            } else {
			                cout << "\nVisualisasi Struktur AVL Tree (Akar di Paling Kiri):\n";
			                cout << "--------------------------------------------------\n";
			                visualisasiPohon(rootUtama, 0);
			                cout << "--------------------------------------------------\n";
			            }
			        }
                } while (pTree != 0);
                break;
            }
        }
    } while (pilih != 0);
}

// 2. MENU KASIR (ANTRIAN & TRANSAKSI)
void menuKasir() {
    int pilih;
    do {
        cout << "\n==============================================";
        cout << "\n                MENU KASIR                    ";
        cout << "\n==============================================\n";
        cout << "1. Menu Barang\n";
        cout << "2. Transaksi (Stack)\n";
        cout << "3. Antrian Reguler\n";
        cout << "4. Antrian Prioritas\n";
        cout << "5. Tampilkan Laporan Shift\n";
        cout << "6. Tutup Shift\n";
        cout << "0. Kembali\n";
        cout << "\n----------------------------------------------";
        cout << "\nPilih: "; cin >> pilih;

        switch(pilih) {
            case 1:
                menuBarangStatis(); // tampilkan daftar barang
                break;

            case 2: { // Sub-menu transaksi (stack)
                int pTrans;
                do {
                    cout << "\n--- SUB-MENU TRANSAKSI (STACK) ---";
                    cout << "\n1. Catat Transaksi Baru";
                    cout << "\n2. Batalkan Transaksi Terakhir";
                    cout << "\n3. Tampilkan Riwayat Transaksi";
                    cout << "\n0. Kembali";
                    cout << "\nPilih: "; cin >> pTrans;

                    if(pTrans == 1) pushTransaksi();
                    else if(pTrans == 2) popTransaksi();
                    else if(pTrans == 3) tampilkanRiwayat();
                    else if(pTrans != 0) cout << "[WARNING] Pilihan tidak valid!\n";
                } while(pTrans != 0);
                break;
            }

            case 3: { // Sub-menu antrian reguler
                int pReg;
                do {
                    cout << "\n--- SUB-MENU ANTRIAN REGULER ---";
                    cout << "\n1. Tambah Antrian";
                    cout << "\n2. Layani Pelanggan";
                    cout << "\n3. Tampilkan Antrian";
                    cout << "\n0. Kembali";
                    cout << "\nPilih: "; cin >> pReg;

                    if(pReg == 1) enqueueKasir();
                    else if(pReg == 2) dequeueKasir();
                    else if(pReg == 3) tampilKasir();
                    else if(pReg != 0) cout << "[WARNING] Pilihan tidak valid!\n";
                } while(pReg != 0);
                break;
            }

            case 4: { // Sub-menu antrian prioritas
                int pPrio;
                do {
                    cout << "\n--- SUB-MENU ANTRIAN PRIORITAS ---";
                    cout << "\n1. Tambah Member";
                    cout << "\n2. Layani Pelanggan";
                    cout << "\n3. Tampilkan Antrian";
                    cout << "\n0. Kembali";
                    cout << "\nPilih: "; cin >> pPrio;

                    if(pPrio == 1) enqueuePrioritas();
                    else if(pPrio == 2) dequeuePrioritas();
                    else if(pPrio == 3) tampilPrioritas();
                    else if(pPrio != 0) cout << "[WARNING] Pilihan tidak valid!\n";
                } while(pPrio != 0);
                break;
            }

            case 5:
                tampilLaporanShift();
                break;

            case 6:
                tutupShift();
                break;

            case 0:
                cout << "[INFO] Kembali ke menu utama.\n";
                break;

            default:
                cout << "[WARNING] Pilihan tidak valid!\n";
                break;
        }

    } while(pilih != 0);
}


// 3. MENU GUDANG (LOGISTIK)
void menuGudang() {
    int pilih;
    do {
        cout << "\n==============================================";
        cout << "\n                MENU GUDANG                   ";
        cout << "\n==============================================";
        cout << "\n1. Kelola Stok Gudang (Input Barang)";
        cout << "\n2. Kelola Data Supplier (Lengkapi Data)";
        cout << "\n0. Kembali";
        cout << "\n----------------------------------------------";
        cout << "\nPilih: "; cin >> pilih;

        switch(pilih) {
            case 1: {
                int pStok;
                do {
                    cout << "\n--- SUB-MENU STOK GUDANG ---";
                    cout << "\n1. Tambah Stok";
                    cout << "\n2. Tampilkan Stok";
                    cout << "\n0. Kembali";
                    cout << "\nPilih: "; cin >> pStok;
                    if(pStok == 1) tambahStok(); 
                    else if(pStok == 2) tampilStok();
                } while(pStok != 0);
                break;
            }
            case 2: {
                int pSupp;
                do {
                    cout << "\n--- SUB-MENU SUPPLIER ---";
                    cout << "\n1. Tambah Supplier Manual";
                    cout << "\n2. Tampilkan Semua Supplier";
                    cout << "\n3. Lengkapi/Update Detail Supplier";
                    cout << "\n0. Kembali";
                    cout << "\nPilih: "; cin >> pSupp;
                    if(pSupp == 1) tambahSupplier();
                    else if(pSupp == 2) tampilSupplier();
                    else if(pSupp == 3) updateSupplier(); 
                } while(pSupp != 0);
                break;
            }
        }
    } while (pilih != 0);
}

// ================= MAIN FUNCTION =================

int main() {
	for(int i = 0; i < 50; i++) {
        tambahStokBarang(daftarBarang[i].nama, 50); 
    }
    int pilih;
    do {
        cout << "\n==================================================";
        cout << "\n                SELAMAT DATANG                    ";
        cout << "\n          DI SISTEM MINIMARKET ABADI              ";
        cout << "\n==================================================";
        cout << "\n1. Login Admin";
        cout << "\n2. Login Kasir";
        cout << "\n3. Login Gudang";
        cout << "\n0. Logout";
        cout << "\n--------------------------------------------------";
        cout << "\nPilih : ";
        cin >> pilih;

        switch(pilih) {
            case 1: if (loginAdmin()) menuAdmin(); break;
            case 2: if (loginKasir()) menuKasir(); break;
            case 3: if (loginGudang()) menuGudang(); break;
            case 0: cout << "Terima kasih telah menggunakan sistem kami.\n"; break;
            default: cout << "Pilihan tidak valid!\n"; break;
        }

    } while (pilih != 0);

    return 0;
}