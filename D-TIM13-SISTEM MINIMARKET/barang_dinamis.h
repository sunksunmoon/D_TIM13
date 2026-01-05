#ifndef BARANG_DINAMIS_H
#define BARANG_DINAMIS_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*
  PERTEMUAN 3
  Materi : Array Dinamis 
  Fitur  : Input Data Barang (Ditulis ke File agar bisa dibaca Statis)
*/

struct ItemDinamis {
    string kode;
    string nama;
    string harga;
    string kategori;
};

void menuBarangDinamis() {
    size_t jumlahBarang; 
    
    cout << "\n==== INPUT DATA BARANG BARU (DINAMIS) ====\n";
    cout << "Masukkan jumlah barang yang ingin ditambah: "; 
    if (!(cin >> jumlahBarang)) {
        cout << "Input harus berupa angka!\n";
        cin.clear();
        string dummy; cin >> dummy;
        return;
    }

    // Alokasi Array Dinamis
    ItemDinamis* daftar = new ItemDinamis[jumlahBarang];

    for (size_t i = 0; i < jumlahBarang; i++) {
        cout << "\nData Barang ke-" << i + 1 << endl;
        cout << "Kode Barang : "; cin >> daftar[i].kode;
        cin.ignore();
        cout << "Nama Barang : "; getline(cin, daftar[i].nama);
        cout << "Harga       : "; cin >> daftar[i].harga;
        cout << "Kategori    : "; 
		cin.ignore(); 
		getline(cin, daftar[i].kategori);
    }

    ofstream file("barang_dinamis.txt", ios::app); 
	if (file.is_open()) {
	    for (size_t i = 0; i < jumlahBarang; i++) {
	        file << daftar[i].kode << "\t" 
             << daftar[i].nama << "\t\t" 
             << "Rp" << daftar[i].harga << "\t\t" 
             << daftar[i].kategori << endl;
	    }
	    file.close();
        cout << "\n[Sukses] " << jumlahBarang << " data berhasil ditambahkan ke database dinamis.\n";
    } else {
        cout << "\n[Error] Gagal membuka file database!\n";
    }

    // Dealokasi memori
    delete[] daftar;
}

#endif
