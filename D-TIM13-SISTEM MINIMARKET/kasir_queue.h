#ifndef KASIR_QUEUE_H
#define KASIR_QUEUE_H
#include <iostream>
#include "transaksi_stack.h" // untuk pushTransaksi, kurangiStokBarang
#include "stok_gudang.h"     // untuk update stok
using namespace std;

/*
  PERTEMUAN 7
  Materi : Queue (Circular Queue / FIFO)
  Fitur  : Manajemen antrian kasir minimarket + integrasi transaksi & stok
*/

#define MAX_Q 5
string queueKasir[MAX_Q];
long frontIdx = -1;
long rearIdx = -1;

// ================= ENQUEUE =================
void enqueueKasir() {
    if ((frontIdx == 0 && rearIdx == MAX_Q - 1) || (rearIdx + 1 == frontIdx)) { 
        cout << "Antrian penuh! Mohon tunggu pelanggan lain selesai.\n"; 
        return;
    }

    string nama;
    cout << "Masukkan Nama Pelanggan: "; 
    cin.ignore();
    getline(cin, nama);

    if (frontIdx == -1) frontIdx = rearIdx = 0;
    else rearIdx = (rearIdx + 1) % MAX_Q;

    queueKasir[rearIdx] = nama;
    cout << "[Sistem] Pelanggan '" << nama << "' berhasil masuk antrian.\n";
}

// ================= DEQUEUE =================
void dequeueKasir() {
    if (frontIdx == -1) { 
        cout << "Antrian kosong, kasir siap melayani.\n"; 
        return;
    }

    string nama = queueKasir[frontIdx];
    cout << ">>> Sedang Melayani Pelanggan: " << nama << " <<<\n";

    // --- Input Transaksi ---
    cout << "[INFO] Masukkan transaksi pelanggan " << nama << ":\n";
    pushTransaksi();

    // --- Kurangi Stok Gudang otomatis ---
    cout << "[INFO] Update stok gudang untuk barang yang dibeli...\n";
    // Di pushTransaksi() nanti harus panggil kurangiStokBarang(namaBarang, jumlah)

    // --- Update antrian ---
    if (frontIdx == rearIdx) frontIdx = rearIdx = -1;
    else frontIdx = (frontIdx + 1) % MAX_Q;
}

// ================= TAMPIL ANTRIAN =================
void tampilKasir() {
    if (frontIdx == -1) { 
        cout << "Tidak ada antrian di kasir.\n"; 
        return;
    }

    long i = frontIdx;
    cout << "\n==== DAFTAR ANTRIAN SAAT INI (CIRCULAR) ====\n";
    while (true) {
        cout << "- Pelanggan: " << queueKasir[i] << endl;
        if (i == rearIdx) break;
        i = (i + 1) % MAX_Q;
    }
    cout << "------------------------------------------\n";
}

#endif

