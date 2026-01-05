#ifndef KTP_H
#define KTP_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Tanggal {
    int hari;
    int bulan;
    int tahun;
};

class KTP;
extern KTP* headMember;

class KTP {
public:
    string nik;
    string nama;
    string tempat_lahir;
    Tanggal tgl_lahir;
    bool jenis_kelamin;     
    char gol_darah;        
    string alamat;
    int rt;
    int rw;
    string kel_desa;
    string kecamatan;
    int agama;             
    bool status_perkawinan; 
    string pekerjaan;
    bool kewarganegaraan;   
    string berlaku_hingga;

 KTP* next = NULL;

string pseudoHuffmanEncode(string data) {
        string hasil = "";
        for (size_t i = 0; i < data.length(); i++) {
            char c = data[i];
            if (c % 2 == 0)
                hasil += "10";
            else
                hasil += "01";
        }
        return hasil;
    }

void inputKTP() {
    cout << "\n==== FORM INPUT KTP (REGISTRASI MEMBER) ====\n";
    cout << "NIK               : "; cin >> nik;
    cin.ignore();
    cout << "Nama Lengkap      : "; getline(cin, nama);
    cout << "Tempat Lahir      : "; getline(cin, tempat_lahir);
    cout << "Tgl Lahir (DD MM YYYY): "; cin >> tgl_lahir.hari >> tgl_lahir.bulan >> tgl_lahir.tahun;

    int jk;
    cout << "Jenis Kelamin:\n";
    cout << " 1. Laki-laki\n";
    cout << " 2. Perempuan\n";
    cout << " Pilih (1/2): "; cin >> jk;
    jenis_kelamin = (jk == 1);

    cout << "Gol. Darah (A/B/O/AB): "; cin >> gol_darah;
    cin.ignore();
    cout << "Alamat            : "; getline(cin, alamat);
    cout << "RT                : "; cin >> rt;
	cout << "RW                : "; cin >> rw;
	cin.ignore();
    cout << "Kel/Desa          : "; getline(cin, kel_desa);
    cout << "Kecamatan         : "; getline(cin, kecamatan);

    cout << "Agama:\n";
    cout << " 1. Islam\n";
    cout << " 2. Kristen\n";
    cout << " 3. Katolik\n";
    cout << " 4. Hindu\n";
    cout << " 5. Budha\n";
    cout << " 6. Konghucu\n";
    cout << " 7. Lainnya\n";
    cout << " Pilih (1-7): "; cin >> agama;

    int sp;
    cout << "Status Perkawinan:\n";
    cout << " 1. Kawin\n";
    cout << " 2. Belum Kawin\n";
    cout << " Pilih (1/2): "; cin >> sp;
    status_perkawinan = (sp == 1);

    cin.ignore();
    cout << "Pekerjaan         : "; getline(cin, pekerjaan);

    int kw;
    cout << "Kewarganegaraan:\n";
    cout << " 1. WNI\n";
    cout << " 2. WNA\n";
    cout << " Pilih (1/2): "; cin >> kw;
    kewarganegaraan = (kw == 1);

    int bh;
    cout << "Berlaku Hingga:\n";
    cout << " 1. Seumur Hidup\n";
    cout << " 2. 5 Tahun\n";
    cout << " Pilih (1/2): "; cin >> bh;
    if (bh == 1) {
        berlaku_hingga = "SEUMUR HIDUP";
    } else {
        berlaku_hingga = "5 TAHUN";
    }

    simpanKeFile();
     this->next = NULL;
    if(!headMember) {
        headMember = this;
    } else {
        KTP* temp = headMember;
        while(temp->next) temp = temp->next;
        temp->next = this;
    }

    cout << "\n[Sistem] Data member '" << nama << "' berhasil ditambahkan ke daftar linked list!\n";
}

    void simpanKeFile() {
	    ofstream file("database_member.txt", ios::app);
	    if (file.is_open()) {
	        file << "==========================================\n";
	        file << "NIK             : " << pseudoHuffmanEncode(nik) << endl;
	        file << "Nama            : " << nama << endl;
	        file << "Tempat/Tgl Lahir: " << tempat_lahir << ", " << tgl_lahir.hari << "-" << tgl_lahir.bulan << "-" << tgl_lahir.tahun << endl;
	        file << "Jenis Kelamin   : " << (jenis_kelamin ? "Laki-laki" : "Perempuan") << endl;
	        file << "Alamat          : " << alamat << endl;
	        file << "RT/RW           : " << rt << "/" << rw << endl;
	        file << "Kel/Desa        : " << kel_desa << endl;
	        file << "Kecamatan       : " << kecamatan << endl;
	        file << "Status Kawin    : " << (status_perkawinan ? "Kawin" : "Belum Kawin") << endl;
	        file << "Pekerjaan       : " << pekerjaan << endl;
	        file << "Berlaku Hingga  : " << berlaku_hingga << endl;
	        file << "==========================================\n\n"; 
	        
	        file.close();
	        cout << "\n[Sistem] Data KTP '" << nama << "' Berhasil Disimpan ke Database!\n";
	    } else {
	        cout << "\n[Error] Gagal membuka file database!\n";
	    }
	}
    
    void tampilkanDatabaseMember() {
        ifstream file("database_member.txt");
        string baris;
        
        cout << "\n==========================================================\n";
        cout << "             DATABASE MEMBER MINIMARKET ABADI             \n";
        cout << "==========================================================\n";

        if (!file.is_open()) {
            cout << " Belum ada data member yang terdaftar.\n";
        } else {
            int no = 1;
            bool adaData = false;
            while (getline(file, baris)) {
                if(baris.empty()) continue; 
                cout << no << ". " << baris << endl;
                no++;
                adaData = true;
            }
            if(!adaData) cout << " Database kosong.\n";
        }
        
        file.close();
        cout << "==========================================================\n";
        cout << "Tekan Enter untuk kembali...";
        cin.ignore();
        cin.get();
    	}
    
	bool cekNIKTerdaftar(string nikCari) {
	    ifstream file("database_member.txt");
	    string baris;
	    string nikEncode = pseudoHuffmanEncode(nikCari);
	
	    while (getline(file, baris)) {
	        if (baris.find("NIK") != string::npos &&
	            baris.find(nikEncode) != string::npos) {
	            file.close();
	            return true;
	        }
	    }
	    file.close();
	    return false;
	}
	
	void tampilMember() {
	    if(!headMember) {
	        cout << "[INFO] Belum ada member terdaftar.\n";
	        return;
	    }
	
	    cout << "\n==== DAFTAR MEMBER TERDAFTAR ====\n";
	    cout << "No\tNIK\t\tNama\n";
	    int no = 1;
	    KTP* temp = headMember;
	    while(temp != NULL) {
	        cout << no++ << "\t" << temp->nik << "\t" << temp->nama << endl;
	        temp = temp->next;
	    }
	    cout << "=================================\n";
	}
};
KTP* headMember = NULL;
#endif

