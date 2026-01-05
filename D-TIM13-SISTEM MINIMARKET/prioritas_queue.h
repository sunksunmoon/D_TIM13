#ifndef PRIORITAS_QUEUE_H
#define PRIORITAS_QUEUE_H

#include <iostream>
#include <fstream>

using namespace std;

/*
  PERTEMUAN 8
  Materi : Priority Queue
  Fitur  : Antrian prioritas pelanggan
*/

struct Antrian {
    string nomor;       
    string prioritas;   
    Antrian* next;
    Antrian* prev;
};

Antrian* headPrioritas = NULL;
Antrian* tailPrioritas = NULL;

long manualStol(string s) {
    long hasil = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            hasil = hasil * 10 + (s[i] - '0');
        }
    }
    return hasil;
}

void simpanAntrianPrioritas() {
    ofstream file("antrian_prioritas.txt");
    Antrian* temp = headPrioritas;
    file << "=== LOG ANTRIAN PRIORITAS ===\n";
    while(temp != NULL){
        file << "ID: " << temp->nomor << " | Level: " 
             << (temp->prioritas == "1" ? "Member" : "Umum") << endl;
        temp = temp->next;
    }
    file.close();
}

void enqueuePrioritas() {
    Antrian* baru = new Antrian();
    cout << "\n==== INPUT ANTRIAN PRIORITAS ====\n";
    cout << "Nomor/Nama Pelanggan : "; cin >> baru->nomor;
    cout << "Prioritas (1:Member, 2:Umum): "; cin >> baru->prioritas;
    baru->next = NULL; 
    baru->prev = NULL;

    if(headPrioritas == NULL) {
        headPrioritas = tailPrioritas = baru;
    } else {
        Antrian* bantu = headPrioritas;
        while(bantu != NULL && manualStol(bantu->prioritas) <= manualStol(baru->prioritas)) {
            bantu = bantu->next;
        }

        if(bantu == headPrioritas) { // Sisip di depan
            baru->next = headPrioritas;
            headPrioritas->prev = baru;
            headPrioritas = baru;
        } else if(bantu == NULL) { // Sisip di belakang
            tailPrioritas->next = baru;
            baru->prev = tailPrioritas;
            tailPrioritas = baru;
        } else { // Sisip di tengah
            baru->next = bantu;
            baru->prev = bantu->prev;
            bantu->prev->next = baru;
            bantu->prev = baru;
        }
    }
    simpanAntrianPrioritas();
    cout << "[Sistem] Antrian berhasil ditambahkan.\n";
}

void dequeuePrioritas() {
    if(headPrioritas == NULL){ 
        cout << "Antrian prioritas sedang kosong.\n"; 
        return;
    }
    
    Antrian* hapus = headPrioritas;
    cout << "\n[Pelayanan] Memanggil: " << hapus->nomor 
         << " (" << (hapus->prioritas == "1" ? "Member" : "Umum") << ")\n";
    
    headPrioritas = headPrioritas->next;
    if(headPrioritas != NULL) {
        headPrioritas->prev = NULL;
    } else {
        tailPrioritas = NULL;
    }
    
    delete hapus;
    simpanAntrianPrioritas();
}

void tampilPrioritas() {
    if(headPrioritas == NULL) {
        cout << "Tidak ada antrian prioritas saat ini.\n";
        return;
    }
    
    cout << "\n========== DAFTAR ANTRIAN PRIORITAS ==========\n";
    Antrian* temp = headPrioritas;
    while(temp != NULL){
        cout << "Nomor: " << temp->nomor << " \t| Status: " 
             << (temp->prioritas == "1" ? "MEMBER (Prio)" : "UMUM") << endl;
        temp = temp->next;
    }
    cout << "----------------------------------------------\n";
}

#endif
