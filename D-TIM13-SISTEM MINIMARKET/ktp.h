#ifndef KTP_H
#define KTP_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Tanggal {
    int hari, bulan, tahun;
};

class KTP {
public:
    string nik, nama, tempat_lahir, alamat, kel_desa, kecamatan, pekerjaan, berlaku_hingga;
    Tanggal tgl_lahir;
    bool jenis_kelamin, status_perkawinan, kewarganegaraan;
    char gol_darah;
    int rt, rw, agama;
    KTP* next;

    KTP() { next = NULL; }

    // --- FUNGSI INPUT (Dibutuhkan oleh admin.h) ---
    void inputKTP() {
        cout << "\n+==========================================+" << endl;
        cout << "|      FORM REGISTRASI MEMBER (KTP)        |" << endl;
        cout << "+==========================================+" << endl;
        
        cout << "  NIK               : "; cin >> nik;
        cin.ignore();
        cout << "  Nama Lengkap      : "; getline(cin, nama);
        cout << "  Tempat Lahir      : "; getline(cin, tempat_lahir);
        cout << "  Tgl Lahir(DD MM YYYY): "; cin >> tgl_lahir.hari >> tgl_lahir.bulan >> tgl_lahir.tahun;

        int jk;
        cout << "\n  --- Jenis Kelamin ---" << endl;
        cout << "  1. Laki-laki" << endl;
        cout << "  2. Perempuan" << endl;
        cout << "  Pilih (1/2): "; cin >> jk;
        jenis_kelamin = (jk == 1);

        cout << "  Gol. Darah (A/B/O/AB): "; cin >> gol_darah;
        cin.ignore();
        cout << "  Alamat            : "; getline(cin, alamat);
        cout << "  RT/RW (misal 01 05): "; cin >> rt >> rw;
        cin.ignore();
        cout << "  Kel/Desa          : "; getline(cin, kel_desa);
        cout << "  Kecamatan         : "; getline(cin, kecamatan);

        cout << "\n  --- Daftar Agama ---" << endl;
        cout << "  1. Islam" << endl;
        cout << "  2. Kristen Protestan" << endl;
        cout << "  3. Katolik" << endl;
        cout << "  4. Hindu" << endl;
        cout << "  5. Budha" << endl;
        cout << "  6. Khonghucu" << endl;
        cout << "  7. Lainnya" << endl;
        cout << "  Pilih (1-7): "; cin >> agama;

        int sp;
        cout << "\n  --- Status Perkawinan ---" << endl;
        cout << "  1. Kawin" << endl;
        cout << "  2. Belum Kawin" << endl;
        cout << "  Pilih (1/2): "; cin >> sp;
        status_perkawinan = (sp == 1);

        cin.ignore();
        cout << "  Pekerjaan         : "; getline(cin, pekerjaan);

        int kw;
        cout << "\n  --- Kewarganegaraan ---" << endl;
        cout << "  1. WNI (Indonesia)" << endl;
        cout << "  2. WNA (Asing)" << endl;
        cout << "  Pilih (1/2): "; cin >> kw;
        kewarganegaraan = (kw == 1);

        int bh;
        cout << "\n  --- Masa Berlaku ---" << endl;
        cout << "  1. Seumur Hidup" << endl;
        cout << "  2. Hingga 5 Tahun" << endl;
        cout << "  Pilih (1/2): "; cin >> bh;
        berlaku_hingga = (bh == 1) ? "SEUMUR HIDUP" : "5 TAHUN";

        // Simpan ke database file teks secara permanen
        simpanDataKeFile();
        cout << "\n  [Sistem] Member '" << nama << "' Berhasil Terdaftar!" << endl;
    }

    // Fungsi simpan data ke file membership.txt (format: NIK|Nama|Status)
    void simpanDataKeFile() {
        ofstream file("membership.txt", ios::app);
        if (file.is_open()) {
            file << nik << "|" << nama << "|MEMBER PRIORITAS" << endl;
            file.close();
        }
    }
};

// --- VARIABEL & FUNGSI GLOBAL ---
extern KTP* headMember; // Dideklarasikan di main.cpp

// Fungsi memuat data dari file ke Linked List saat program dibuka
inline void muatKTPDariFile() {
    ifstream file("membership.txt");
    if (!file) return;

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '=') continue; 

        stringstream ss(line);
        string vNik, vNama, vStatus;
        
        // Parsing data menggunakan delimiter '|'
        if (getline(ss, vNik, '|') && getline(ss, vNama, '|') && getline(ss, vStatus, '|')) {
            KTP* baru = new KTP;
            baru->nik = vNik;
            baru->nama = vNama;
            
            // Masukkan ke Linked List (Memory)
            baru->next = headMember;
            headMember = baru;
        }
    }
    file.close();
}

// Fungsi inisialisasi data member awal jika database masih kosong
inline void inisialisasiMemberDefault() {
    ifstream check("membership.txt");
    if (!check) {
        // Data 4 orang member default (mahasiswa/owner)
        string data[4][2] = {
            {"2400018185", "Resty Amandha"},
            {"2400018187", "Linda Amelia Sari"},
            {"2400018170", "Devina Agnesia Pratami"},
            {"2400018169", "Alifia Hartisya"}
        };

        for(int i=0; i<4; i++) {
            KTP* m = new KTP;
            m->nik = data[i][0];
            m->nama = data[i][1];
            m->simpanDataKeFile(); // Tulis ke file
            
            // Tautkan ke Linked List memori
            m->next = headMember;
            headMember = m;
        }
        cout << "[Sistem] Database Member Default Berhasil Dibuat.\n";
    } else {
        check.close();
        muatKTPDariFile(); // Jika file sudah ada, baca isinya ke memori
    }
}

// Fungsi pencarian NIK untuk divalidasi oleh sistem Kasir
inline bool cekNIKTerdaftar(string nikCari) {
    KTP* temp = headMember;
    while (temp != NULL) {
        if (temp->nik == nikCari) return true;
        temp = temp->next;
    }
    return false;
}

#endif