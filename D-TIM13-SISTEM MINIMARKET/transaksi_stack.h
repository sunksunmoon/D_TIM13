#ifndef TRANSAKSI_STACK_H
#define TRANSAKSI_STACK_H
#include <ctime>
#include "ktp.h"
#include "barang_statis.h"
#include "stok_gudang.h"
#include <iostream>
#include <string>
#include <iomanip>  // untuk setw
using namespace std;

// ================= VARIABEL SHIFT =================
string kasirAktif = "";
string shiftAktif = "";
int jumlahPelangganShiftIni = 0;
long long totalDuitShiftIni = 0;

// ================= STRUCT STACK ===================
struct Transaksi {
    string kode;
    string namaBarang;
    long harga;        // harga satuan
    long jumlah;       // jumlah beli
    long total;        // total bayar
    string jam;
    string tgl;
    string metodeBayar;
    string infoDiskon;
    Transaksi* next;
};

Transaksi* topTransaksi = NULL;

// ================= HITUNG TOTAL ===================
long hitungTotal(long harga, long jumlah) {
    return harga * jumlah;
}

// ================= PUSH TRANSAKSI =================
void pushTransaksi() {
    Transaksi* baru = new Transaksi;

    // ================= TANGGAL & JAM OTOMATIS =================
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char tglBuff[9];
    sprintf(tglBuff, "%02d/%02d/%02d", ltm->tm_mday, ltm->tm_mon + 1, (ltm->tm_year + 1900) % 100);
    baru->tgl = tglBuff;

    char jamBuff[6];
    sprintf(jamBuff, "%02d:%02d", ltm->tm_hour, ltm->tm_min);
    baru->jam = jamBuff;

    cout << "\n========== INPUT TRANSAKSI BARU ==========\n";
    cout << "Apakah ingin input kode barang langsung?\n";
	cout << "1. Ya\n";
	cout << "2. Pilih kategori\n";
	cout << "Pilih (1/2): ";
    int mode; cin >> mode;

    Item barang; // struct Item dari barang_statis.h
    bool ketemu = false;

    if(mode == 1) {
        string kode;
        cout << "Masukkan kode barang: "; cin >> kode;
        for(int i=0;i<50;i++){
            if(daftarBarang[i].id == kode){
                barang = daftarBarang[i];
                ketemu = true;
                break;
            }
        }
        if(!ketemu){
            cout << "[WARNING] Kode barang tidak ditemukan!\n";
            delete baru;
            return;
        }
    } else {
        // Daftar kategori unik
        string kategori[5] = {"Elektronik","Makanan","Minuman","Furniture","Alat Mandi"};
        cout << "\nDaftar kategori:\n";
        for(int i=0;i<5;i++) cout << i+1 << ". " << kategori[i] << endl;

        int pilihKat;
        cout << "Pilih kategori: "; cin >> pilihKat;
        if(pilihKat < 1 || pilihKat > 5){
            cout << "[WARNING] Pilihan kategori tidak valid!\n";
            delete baru;
            return;
        }
        string katDipilih = kategori[pilihKat-1];

        cout << "\nBarang kategori " << katDipilih << ":\n";
        int indexBarang[50], jumlahBarangKat = 0;
        for(int i=0;i<50;i++){
            if(daftarBarang[i].kategori == katDipilih){
                cout << jumlahBarangKat+1 << ". " << daftarBarang[i].nama
                     << " (Kode: " << daftarBarang[i].id 
                     << " | Rp" << daftarBarang[i].harga << ")\n";
                indexBarang[jumlahBarangKat++] = i;
            }
        }

        int pilihBarang;
        cout << "Pilih barang: "; cin >> pilihBarang;
        if(pilihBarang < 1 || pilihBarang > jumlahBarangKat){
            cout << "[WARNING] Pilihan barang tidak valid!\n";
            delete baru;
            return;
        }
        barang = daftarBarang[indexBarang[pilihBarang-1]];
    }

    // ================= MASUKKAN JUMLAH BELI =================
    long jumlahBeli;
    cout << "Jumlah beli: "; cin >> jumlahBeli;

    // ================= CEK STOK DI GUDANG SESUAI JUMLAH =================
    if(!kurangiStokBarang(barang.nama, jumlahBeli)) {
        cout << "[WARNING] Stok barang tidak cukup!\n";
        delete baru;
        return;
    }

    // ================= MASUKKAN DATA BARANG KE TRANSAKSI =================
    baru->kode = barang.id;
    baru->namaBarang = barang.nama;
    baru->harga = barang.harga; 
    baru->jumlah = jumlahBeli;

    // ================= STATUS MEMBER =================
    int status;
    cout << "\nStatus Pelanggan: ";
	cout << "\n1. Member";
	cout << "\n2. Non-Member";
	cout << "\n3. Daftar Member";
	cout << "\nPilih: ";
    cin >> status;

    bool isMember = false;
    KTP ktp;

    if(status == 1) {
    	ktp.tampilMember();
        string nik;
        cout << "Masukkan NIK Member: "; cin >> nik;
        if(ktp.cekNIKTerdaftar(nik)) {
            isMember = true;
            cout << "[INFO] Diskon 15% diterapkan\n";
        } else cout << "[WARNING] NIK tidak terdaftar\n";
    } else if(status == 3) {
        ktp.inputKTP();
        isMember = true;
        cout << "[INFO] Member baru dibuat\n";
    }

    // ================= HITUNG TOTAL =================
    long totalAsli = hitungTotal(barang.harga, jumlahBeli);
    long totalAkhir = totalAsli;
    if(isMember){
        totalAkhir -= totalAsli*15/100;
        baru->infoDiskon = "Diskon 15% (Member)";
    } else baru->infoDiskon = "Non-Member";

    // ================= METODE BAYAR =================
    cout << "\nMetode Bayar: ";
	cout << "\n1. Tunai";
	cout << "\n2. QRIS";
	cout << "\nPilih: ";
    int bayarPilihan; cin >> bayarPilihan;
    baru->metodeBayar = (bayarPilihan==1) ? "Tunai" : "QRIS";

    long uangBayar = 0;
    if(bayarPilihan == 1){
        cout << "Masukkan uang tunai: Rp "; cin >> uangBayar;
        if(uangBayar < totalAkhir){
            cout << "[WARNING] Uang tidak cukup!\n";
            
            // PERBAIKAN DISINI: Harus kirim 4 parameter sesuai definisi baru
            tambahStokBarang(barang.id, barang.nama, barang.kategori, jumlahBeli);
            
            delete baru;
            return;
        }
    }

    // ================= UPDATE SHIFT =================
    baru->total = totalAkhir;
    jumlahPelangganShiftIni++;
    totalDuitShiftIni += totalAkhir;

    // ================= PUSH STACK =================
    baru->next = topTransaksi;
    topTransaksi = baru;

    // ================= CETAK STRUK =================
    cout << "\n============================================\n";
    cout << "           MINIMARKET ABADI MART            \n";
    cout << "       Jl. Merdeka No. 123 - Yogyakarta    \n";
    cout << "============================================\n";
    cout << left << setw(15) << "ID Barang:" << baru->kode << endl;
    cout << left << setw(15) << "Tanggal:" << baru->tgl << endl;
    cout << left << setw(15) << "Jam:" << baru->jam << endl;
    cout << left << setw(15) << "Kasir:" << kasirAktif << endl;
    cout << left << setw(15) << "Shift:" << shiftAktif << endl;
    cout << "--------------------------------------------\n";

    cout << left << setw(20) << "Item"
         << right << setw(10) << "Harga"
         << right << setw(10) << "Jumlah"
         << right << setw(15) << "Subtotal" << endl;
    cout << "--------------------------------------------\n";

    cout << left << setw(20) << baru->namaBarang
         << right << setw(10) << baru->harga
         << right << setw(10) << baru->jumlah
         << right << setw(15) << totalAsli << endl;

    cout << "--------------------------------------------\n";
    cout << left << setw(20) << "Diskon:" << baru->infoDiskon << endl;
    cout << left << setw(20) << "Total Bayar:" << totalAkhir << endl;
    cout << left << setw(20) << "Metode Bayar:" << baru->metodeBayar << endl;

    if(bayarPilihan == 1){
        cout << left << setw(20) << "Uang Bayar:" << uangBayar << endl;
        cout << left << setw(20) << "Kembalian:" << uangBayar - totalAkhir << endl;
    }

    cout << "------------------------------------------------------\n";
    cout << "         									           \n";
    cout << "           Terima Kasih Telah Berbelanja :)           \n";
    cout << "  Barang yang sudah dibeli tidak dapat di kembalikan  \n"; 
    cout << "         									           \n";
    cout << "======================================================\n";
}

// ================= POP TRANSAKSI =================
void popTransaksi() {
    if(!topTransaksi) return;

    Transaksi* hapus = topTransaksi;
    topTransaksi = topTransaksi->next;

    tambahStokBarang(hapus->kode, hapus->namaBarang, "Retur", hapus->jumlah);

    totalDuitShiftIni -= hapus->total;
    jumlahPelangganShiftIni--;

    delete hapus;
    cout << "[INFO] Transaksi terakhir dibatalkan & stok dikembalikan.\n";
}

// ================= LAPORAN SHIFT =================
void tampilLaporanShift() {
    cout << "\n===== LAPORAN SHIFT =====\n";
    cout << "Kasir     : " << kasirAktif << endl;
    cout << "Shift     : " << shiftAktif << endl;
    cout << "Pelanggan : " << jumlahPelangganShiftIni << endl;
    cout << "Omset     : Rp " << totalDuitShiftIni << endl;
}

// ================= TUTUP SHIFT ===================
void tutupShift() {
    tampilLaporanShift();
    kasirAktif = "";
    shiftAktif = "";
    jumlahPelangganShiftIni = 0;
    totalDuitShiftIni = 0;
    cout << "[INFO] Shift ditutup & data direset\n";
}

void tampilkanRiwayat() {
    if (!topTransaksi) {
        cout << "[INFO] Belum ada transaksi.\n";
        return;
    }

    cout << "\n========== RIWAYAT TRANSAKSI ==========\n";
    cout << left 
         << setw(8) << "ID" 
         << setw(12) << "Tanggal" 
         << setw(8) << "Jam" 
         << setw(20) << "Barang" 
         << setw(8) << "Jumlah" 
         << setw(10) << "Harga" 
         << setw(10) << "Total" 
         << setw(15) << "Metode Bayar" 
         << setw(20) << "Diskon" 
         << endl;

    cout << "------------------------------------------------------------------------------------------\n";

    Transaksi* temp = topTransaksi;
    while(temp != NULL) {
        cout << left
             << setw(8)  << temp->kode
             << setw(12) << temp->tgl
             << setw(8)  << temp->jam
             << setw(20) << temp->namaBarang
             << setw(8)  << temp->jumlah
             << setw(10) << temp->harga
             << setw(10) << temp->total
             << setw(15) << temp->metodeBayar
             << setw(20) << temp->infoDiskon
             << endl;
        temp = temp->next;
    }

    cout << "------------------------------------------------------------------------------------------\n";
}
#endif
