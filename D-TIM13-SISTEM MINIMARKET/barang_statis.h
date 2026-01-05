#ifndef BARANG_STATIS_H
#define BARANG_STATIS_H

#include <iostream>
#include <fstream>
#include <string>
#include "barang_dinamis.h"

using namespace std;

/*
  PERTEMUAN 2 & 3
  Array Statis
  Master Data Barang Minimarket
*/

struct Item {
    string id;
    string nama;
    long harga;
    string kategori;
};

/* ================= DATA BARANG STATIS ================= */
Item daftarBarang[50] = {
    // ELEKTRONIK (BAR01A - BAR10A)
	{"BAR01A", "Setrika", 150000, "Elektronik"},
	{"BAR02A", "Kipas Angin", 200000, "Elektronik"},
	{"BAR03A", "Rice Cooker", 250000, "Elektronik"},
	{"BAR04A", "Blender", 180000, "Elektronik"},
	{"BAR05A", "Dispenser", 300000, "Elektronik"},
	{"BAR06A", "Televisi", 1500000, "Elektronik"},
	{"BAR07A", "Speaker", 500000, "Elektronik"},
	{"BAR08A", "Charger HP", 50000, "Elektronik"},
	{"BAR09A", "Lampu LED", 35000, "Elektronik"},
	{"BAR10A", "Stop Kontak", 25000, "Elektronik"},
	
	// MAKANAN (BAR01B - BAR10B)
	{"BAR01B", "Indomie Rebus", 3000, "Makanan"},
	{"BAR02B", "Indomie Goreng", 3000, "Makanan"},
	{"BAR03B", "Mie Sedaap", 3000, "Makanan"},
	{"BAR04B", "Beras Ramos", 75000, "Makanan"},
	{"BAR05B", "Gula Pasir", 15000, "Makanan"},
	{"BAR06B", "Minyak Goreng", 18000, "Makanan"},
	{"BAR07B", "Tepung Terigu", 12000, "Makanan"},
	{"BAR08B", "Biskuit Kaleng", 10000, "Makanan"},
	{"BAR09B", "Sarden Kaleng", 15000, "Makanan"},
	{"BAR10B", "Kornet Sapi", 25000, "Makanan"},
	
	// MINUMAN (BAR01C - BAR10C)
	{"BAR01C", "Aqua Botol", 4000, "Minuman"},
	{"BAR02C", "Teh Botol Sosro", 5000, "Minuman"},
	{"BAR03C", "Kopi Kapal Api", 5000, "Minuman"},
	{"BAR04C", "Susu UHT 250ml", 10000, "Minuman"},
	{"BAR05C", "Jus Jeruk", 12000, "Minuman"},
	{"BAR06C", "Coca Cola", 8000, "Minuman"},
	{"BAR07C", "Mizone", 7000, "Minuman"},
	{"BAR08C", "Yogurt Cimory", 15000, "Minuman"},
	{"BAR09C", "Sirup ABC", 20000, "Minuman"},
	{"BAR10C", "Teh Kotak", 6000, "Minuman"},
	
	// FURNITURE (BAR01D - BAR10D)
	{"BAR01D", "Kursi Plastik", 50000, "Furniture"},
	{"BAR02D", "Meja Belajar", 150000, "Furniture"},
	{"BAR03D", "Lemari Kayu", 400000, "Furniture"},
	{"BAR04D", "Jemuran Besi", 100000, "Furniture"},
	{"BAR05D", "Rak Sepatu", 75000, "Furniture"},
	{"BAR06D", "Cermin Besar", 60000, "Furniture"},
	{"BAR07D", "Bantal Tidur", 35000, "Furniture"},
	{"BAR08D", "Kasur Lipat", 500000, "Furniture"},
	{"BAR09D", "Karpet Lantai", 120000, "Furniture"},
	{"BAR10D", "Gantungan Baju", 15000, "Furniture"},
	
	// ALAT MANDI (BAR01E - BAR10E)
	{"BAR01E", "Sabun Batang", 5000, "Alat Mandi"},
	{"BAR02E", "Shampoo Cair", 15000, "Alat Mandi"},
	{"BAR03E", "Pasta Gigi", 10000, "Alat Mandi"},
	{"BAR04E", "Sikat Gigi", 8000, "Alat Mandi"},
	{"BAR05E", "Handuk Mandi", 50000, "Alat Mandi"},
	{"BAR06E", "Gayung Mandi", 12000, "Alat Mandi"},
	{"BAR07E", "Ember Plastik", 25000, "Alat Mandi"},
	{"BAR08E", "Sikat Lantai", 15000, "Alat Mandi"},
	{"BAR09E", "Detergen Bubuk", 20000, "Alat Mandi"},
	{"BAR10E", "Pewangi Pakaian", 10000, "Alat Mandi"},
};

/* ================= TAMPILKAN BARANG ================= */
void tampilBarangStatis() {
    cout << "\n============================================================\n";
    cout << "ID\tNama Barang\t\tHarga\t\tKategori\n";
    cout << "------------------------------------------------------------\n";
    for (int i = 0; i < 50; i++) {
        cout << daftarBarang[i].id << "\t"
             << daftarBarang[i].nama << "\t\tRp "
             << daftarBarang[i].harga << "\t"
             << daftarBarang[i].kategori << endl;
    }
    cout << "============================================================\n";
}

/* ================= MENU BARANG STATIS ================= */
void menuBarangStatis() {
    int pilih;
    do {
        cout << "\n============================================";
        cout << "\n             MANAJEMEN BARANG              ";
        cout << "\n============================================";
        cout << "\n1. Lihat Semua Barang";
        cout << "\n2. Tambah Barang Dinamis";
        cout << "\n0. Kembali";
        cout << "\n--------------------------------------------";
        cout << "\nPilih: ";
        cin >> pilih;

        if (pilih == 1) tampilBarangStatis();
        else if (pilih == 2) menuBarangDinamis();

    } while (pilih != 0);
}

#endif

