#ifndef KASIR_H
#define KASIR_H

#include <iostream>
#include <sstream> 
#include <cstdio> 
#include <string>
#include <ctime> 
#include "struktur_data.h"
#include "ktp.h"
#include "gudang.h"

// Forward declaration fungsi admin agar bisa dipanggil di sini
void simpanLaporanKeFile(string detail, long total);

using namespace std;

// Inisialisasi variabel antrean (Queue)
Pelanggan *frontQ = NULL;
Pelanggan *rearQ = NULL;
int nomorStruk = 101; 

// --- FUNGSI DETEKSI HARI JUMAT ---
bool apakahHariJumat() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    // tm_wday: 0 = Minggu, 5 = Jumat
    return (ltm->tm_wday == 5);
}

// --- FUNGSI ANTREAN (Priority Queue) ---
void tambahAntrian() {
    string nikInput;
    Pelanggan* baru = new Pelanggan;
    
    cout << "\n+==========================================+" << endl;
    cout << "|                AMBIL ANTRIAN               |" << endl;
    cout << "+==========================================+" << endl;
    cout << "  Masukkan NIK (Kosongkan jika umum): ";
    cin.ignore();
    getline(cin, nikInput);
    
    cout << "  Nama Pelanggan: ";
    getline(cin, baru->nama);
    baru->next = NULL;

    bool isMember = false;
    if (nikInput != "" && cekNIKTerdaftar(nikInput)) {
        isMember = true;
        baru->nama = "[MEMBER] " + baru->nama;
        cout << "  >>> Member Terdeteksi (Antrean Prioritas)\n";
    } else {
        cout << "  >>> Pelanggan Umum.\n";
    }

    if (frontQ == NULL) {
        frontQ = rearQ = baru;
    } else if (isMember) {
        // Logika Priority Queue: Member disisipkan ke depan antrean umum
        if (frontQ->nama.find("[MEMBER]") == string::npos) {
            baru->next = frontQ;
            frontQ = baru;
        } else {
            Pelanggan* bantu = frontQ;
            while (bantu->next != NULL && bantu->next->nama.find("[MEMBER]") != string::npos) {
                bantu = bantu->next;
            }
            baru->next = bantu->next;
            bantu->next = baru;
            if (baru->next == NULL) rearQ = baru;
        }
    } else {
        rearQ->next = baru;
        rearQ = baru;
    }
    cout << "  >>> Berhasil masuk antrean.\n";
}

// --- FUNGSI TRANSAKSI PEMBAYARAN ---
void bayar() {
    if (frontQ == NULL) { 
        cout << "\n  [!] Antrean Kosong! Tidak ada pelanggan untuk dilayani.\n"; 
        return; 
    }
    
    bool isMember = (frontQ->nama.find("[MEMBER]") != string::npos);
    long grandTotal = 0;
    string listBelanja = "";
    string logBelanjaSimple = ""; 
    char lagi;

    cout << "\n+==========================================+" << endl;
    cout << "  MELAYANI : " << frontQ->nama << endl;
    cout << "+==========================================+" << endl;

    do {
        cout << "\nDaftar Kategori Produk (AVL Tree):" << endl;
        counterKategori = 1;
        tampilKategoriBerseri(rootKategori);
        cout << "Pilih Nomor Kategori (Hanya untuk referensi): ";
        int pilNomor = inputAngka();
        
        string katCari = "";
        int current = 1;
        cariKategoriByNomor(rootKategori, pilNomor, current, katCari);

        cout << "Cari Nama/Kode Barang: ";
        string keyword;
        cin.ignore();
        getline(cin, keyword);

        cout << "\nHasil Pencarian Global (Mencari di seluruh Gudang):" << endl;
        cout << "------------------------------------------------------------" << endl;
        printf("| %-10s | %-25s | %-15s |\n", "KODE", "NAMA PRODUK", "HARGA");
        cout << "------------------------------------------------------------" << endl;

        Barang* b = headGudang;
        bool found = false;
        
        // PERBAIKAN: Pencarian Global (Menghapus filter b->kategori == katCari)
        while(b) {
            if(b->nama.find(keyword) != string::npos || b->kode == keyword) {
                printf("| %-10s | %-25s | Rp%-13ld |\n", b->kode.c_str(), b->nama.c_str(), b->harga);
                found = true;
            }
            b = b->next;
        }

        if(!found) {
            cout << "|            [!] Barang tidak ditemukan.                    |" << endl;
            cout << "------------------------------------------------------------" << endl;
        } else {
            cout << "------------------------------------------------------------" << endl;
            cout << "Masukkan Kode Barang yang dibeli: ";
            string kodePilih; cin >> kodePilih;
            cout << "Jumlah Beli: ";
            int jml = inputAngka();

            b = headGudang;
            bool suksesItem = false;
            while(b) {
                if(b->kode == kodePilih) {
                    if(b->stok <= 0) {
                        cout << "  [X] Gagal! Stok barang '" << b->nama << "' sedang HABIS.\n";
                    } 
                    else if(b->stok >= jml) {
                        long sub = b->harga * jml;
                        grandTotal += sub;
                        b->stok -= jml;
                        
                        stringstream ss;
                        ss << b->nama << " (x" << jml << ")\n  ";
                        listBelanja += ss.str();
                        
                        stringstream ssLog;
                        ssLog << b->nama << " x" << jml << ", ";
                        logBelanjaSimple += ssLog.str();
                        
                        suksesItem = true;
                        cout << "  [OK] " << b->nama << " masuk keranjang.\n";
                    } else {
                        cout << "  [!] Stok tidak mencukupi! (Sisa: " << b->stok << ")\n";
                    }
                    break;
                }
                b = b->next;
            }
            if(!suksesItem && found) cout << "  [!] Kode salah atau transaksi gagal!\n";
        }

        cout << "\nTambah barang lain untuk pelanggan ini? (y/n): ";
        cin >> lagi;
    } while (lagi == 'y' || lagi == 'Y');

    if (grandTotal == 0) {
        cout << "  [!] Transaksi dibatalkan (tidak ada barang).\n";
        Pelanggan* temp = frontQ;
        frontQ = frontQ->next;
        delete temp;
        return;
    }

    // --- LOGIKA PROMO & DISKON ---
    long diskonMember = 0;
    long diskonVolume = 0;
    long diskonJumat = 0;

    if (isMember) diskonMember = grandTotal * 0.15;
    if (grandTotal > 200000) diskonVolume = grandTotal * 0.05;

    if (apakahHariJumat()) {
        diskonJumat = grandTotal * 0.02;
        cout << "  >>> PROMO JUMAT BERKAH AKTIF (Ekstra 2%)\n";
    }

    long totalDiskon = diskonMember + diskonVolume + diskonJumat;
    long totalSebelumPajak = grandTotal - totalDiskon;
    long ppn = totalSebelumPajak * 0.11; 
    long totalAkhir = totalSebelumPajak + ppn;

    // Tampilan Metode Pembayaran Menurun
    cout << "\nMetode Pembayaran:" << endl;
    cout << "1. Tunai" << endl;
    cout << "2. Cashless" << endl;
    cout << "Pilih metode (1/2): ";
    int pilMetode = inputAngka();
    long bayarTunai = 0;
    string metodeStr = (pilMetode == 2) ? "CASHLESS" : "TUNAI";

    if (pilMetode == 1) {
        do {
            cout << "  Total Tagihan : Rp " << totalAkhir << endl;
            cout << "  Bayar Tunai   : Rp ";
            bayarTunai = inputAngka();
            if (bayarTunai < totalAkhir) cout << "  [!] Uang tidak cukup!\n";
        } while (bayarTunai < totalAkhir);
    } else {
        bayarTunai = totalAkhir;
    }
    
    long kembalian = bayarTunai - totalAkhir;

    // --- SIMPAN DATA & CETAK STRUK ---
    Nota* notaBaru = new Nota;
    notaBaru->detail = frontQ->nama + " Belanja: " + logBelanjaSimple;
    notaBaru->subtotal = totalAkhir;
    notaBaru->next = topNota;
    topNota = notaBaru;

    simpanLaporanKeFile(logBelanjaSimple, totalAkhir);
    
    // Simpan perubahan stok ke file database gudang secara permanen
    simpanStokKeFile(); 

    totalPemasukanShift += totalAkhir;
    pelangganDilayani++;

    cout << "\n\n==========================================" << endl;
    cout << "              MINIMARKET ABADI              " << endl;
    cout << "==========================================" << endl;
    printf(" No. Struk : TR-%d\n", nomorStruk++);
    printf(" Pelanggan : %s\n", frontQ->nama.c_str());
    cout << "------------------------------------------" << endl;
    cout << " Item:\n  " << listBelanja << endl;
    cout << "------------------------------------------" << endl;
    printf("  SUBTOTAL           : Rp %10ld\n", grandTotal);
    if(diskonMember > 0) printf("  DISKON MEMBER (15%): Rp %10ld\n", diskonMember);
    if(diskonVolume > 0) printf("  DISKON VOL (>200K) : Rp %10ld\n", diskonVolume);
    if(diskonJumat > 0)  printf("  PROMO JUMAT (2%)   : Rp %10ld\n", diskonJumat);
    printf("  PPN (11%)          : Rp %10ld\n", ppn);
    cout << "------------------------------------------" << endl;
    printf("  GRAND TOTAL        : Rp %10ld\n", totalAkhir);
    printf("  METODE             : %s\n", metodeStr.c_str());
    printf("  KEMBALIAN          : Rp %10ld\n", kembalian);
    cout << "==========================================" << endl;

    // Hapus dari antrean setelah dilayani
    Pelanggan* temp = frontQ;
    frontQ = frontQ->next;
    if (frontQ == NULL) rearQ = NULL;
    delete temp;
    
    cout << "\n  [OK] Transaksi Selesai & Stok Diperbarui. Tekan Enter...";
    cin.ignore(); cin.get();
}

void menuKasir() {
    int pil;
    do {
        cout << "\n+==========================================+" << endl;
        cout << "|                MENU KASIR                |" << endl;
        cout << "+==========================================+" << endl;
        cout << "| 1. Ambil Antrian                         |" << endl;
        cout << "| 2. Proses Pembayaran (Antrean Terdepan)  |" << endl;
        cout << "| 0. Kembali                               |" << endl;
        cout << "+------------------------------------------+" << endl;
        cout << "  Pilih: "; pil = inputAngka();
        
        if (pil == 1) tambahAntrian();
        else if (pil == 2) bayar();
    } while (pil != 0);
}

#endif