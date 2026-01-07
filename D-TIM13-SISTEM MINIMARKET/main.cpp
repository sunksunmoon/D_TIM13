#include <iostream>
#include <fstream>
#include <string>
#include "struktur_data.h" 
#include "ktp.h"
#include "gudang.h"
#include "kasir.h"
#include "admin.h"

using namespace std;

// --- DEFINISI VARIABEL GLOBAL ---
string kasirAktif = "";
string shiftAktif = "";
int pelangganDilayani = 0;
long totalPemasukanShift = 0;

KTP* headMember = NULL; 
Barang* headGudang = NULL;      
NodeKategori* rootKategori = NULL; 
Nota* topNota = NULL;

// --- FUNGSI KEAMANAN ---

// Enkripsi Caesar Cipher (Geser 3 karakter)
string enkripsi(string data) {
    for(int i = 0; i < (int)data.length(); i++) {
        data[i] = data[i] + 3; 
    }
    return data;
}

// Validasi Password Langsung di Kode
bool validasiLogin(string role, string inputPass) {
    string encryptedInput = enkripsi(inputPass); 

    // ADMIN: "1111" -> "4444"
    // KASIR: "2222" -> "5555"
    // GUDANG: "3333" -> "6666"
    if (role == "ADMIN") return (encryptedInput == "4444");
    if (role == "KASIR") return (encryptedInput == "5555");
    if (role == "GUDANG") return (encryptedInput == "6666");

    return false;
}

// --- FUNGSI LOGIN PER ROLE ---

bool prosesLoginAdmin() {
    string pass;
    cout << "\n+==========================================+" << endl;
    cout << "|                LOGIN ADMIN               |" << endl;
    cout << "+==========================================+" << endl;
    cout << "  Masukkan password admin: "; cin >> pass;
    return validasiLogin("ADMIN", pass);
}

bool prosesLoginGudang() {
    string pass;
    cout << "\n+==========================================+" << endl;
    cout << "|                LOGIN GUDANG              |" << endl;
    cout << "+==========================================+" << endl;
    cout << "  Masukkan password gudang: "; cin >> pass;
    return validasiLogin("GUDANG", pass);
}

bool prosesLoginKasir() {
    string pass;
    int n, s;
    cout << "\n+==========================================+" << endl;
    cout << "|                LOGIN KASIR               |" << endl;
    cout << "+==========================================+" << endl;
    cout << "  Masukkan password kasir: "; cin >> pass;
    
    if (!validasiLogin("KASIR", pass)) return false;

    // --- VALIDASI PEMILIHAN NAMA KASIR (Loop sampai benar) ---
    while (true) {
        cout << "\n+------------------------+" << endl;
        cout << "|     Pilih Nama Kasir   |" << endl;
        cout << "+------------------------+" << endl;
        cout << "| 1. Elizabeth           |" << endl;
        cout << "| 2. Victoria            |" << endl;
        cout << "| 3. William             |" << endl;
        cout << "| 4. Phillips            |" << endl;
        cout << "+------------------------+" << endl;
        cout << "  Pilih (1-4): "; n = inputAngka();

        if (n == 1) { kasirAktif = "Elizabeth"; break; }
        else if (n == 2) { kasirAktif = "Victoria"; break; }
        else if (n == 3) { kasirAktif = "William"; break; }
        else if (n == 4) { kasirAktif = "Phillips"; break; }
        else {
            cout << "  [!] Input Salah! Silakan pilih angka 1 sampai 4.\n";
        }
    }

    // --- VALIDASI PEMILIHAN SHIFT (Loop sampai benar) ---
    while (true) {
        cout << "\n+------------------------+" << endl;
        cout << "|     Pilih Jam Shift    |" << endl;
        cout << "+------------------------+" << endl;
        cout << "| 1. Pagi (07:00-15:00)  |" << endl;
        cout << "| 2. Sore (15:00-23:00)  |" << endl;
        cout << "| 3. Malam (23:00-07:00) |" << endl;
        cout << "+------------------------+" << endl;
        cout << "  Pilih (1-3): "; s = inputAngka();

        if (s == 1) { shiftAktif = "07:00-15:00"; break; }
        else if (s == 2) { shiftAktif = "15:00-23:00"; break; }
        else if (s == 3) { shiftAktif = "23:00-07:00"; break; }
        else {
            cout << "  [!] Input Salah! Silakan pilih angka 1 sampai 3.\n";
        }
    }
    
    return true;
}

// --- MAIN PROGRAM ---
int main() {
    // 1. Mencoba memuat data dari file database_stok.txt
    muatStokDariFile();             
    
    // 2. LOGIKA PERBAIKAN: Hanya muat data default JIKA file kosong atau tidak ada
    if (headGudang == NULL) {
        cout << "  [Sistem] Memuat data stok awal..." << endl;
        inisialisasiDefault();          
        simpanStokKeFile(); // Langsung simpan agar file database tercipta
    }

    // 3. Muat data member dari file membership.txt (Jika ada)
    // Dan gunakan default jika file belum ada
    inisialisasiMemberDefault();    
    
    int pilih;
    do {
        cout << "\n+==================================================+" << endl;
        cout << "|                  SELAMAT DATANG                  |" << endl;
        cout << "|            DI SISTEM MINIMARKET ABADI            |" << endl;
        cout << "+==================================================+" << endl;
        cout << "| 1. Login Admin (Member & Laporan)                |" << endl;
        cout << "| 2. Login Kasir (Transaksi & Antrian)             |" << endl;
        cout << "| 3. Login Gudang (Stok Barang)                    |" << endl;
        cout << "| 0. Logout & Keluar                               |" << endl;
        cout << "+--------------------------------------------------+" << endl;
        cout << "  Pilih : "; pilih = inputAngka();

        switch(pilih) {
            case 1: 
                if (prosesLoginAdmin()) menuAdmin(); 
                else cout << "  [X] Password Salah!\n"; 
                break;
            case 2: 
                if (prosesLoginKasir()) menuKasir(); 
                else cout << "  [X] Password Salah!\n"; 
                break;
            case 3: 
                if (prosesLoginGudang()) menuGudang(); 
                else cout << "  [X] Password Salah!\n"; 
                break;
            case 0: 
                // Simpan perubahan terakhir sebelum benar-benar keluar
                simpanStokKeFile(); 
                cout << "\n  [!] Data tersimpan. Keluar sistem. Terima Kasih!\n"; 
                break;
            default:
                cout << "  [!] Pilihan tidak tersedia.\n";
        }
    } while (pilih != 0);

    return 0;
}