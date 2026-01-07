# 🛒 Minimarket Abadi - Sistem Kasir & Manajemen Stok

Sistem Minimarket berbasis **C++** yang dirancang untuk menangani transaksi kasir, antrean pelanggan, dan manajemen gudang secara efisien menggunakan konsep **Struktur Data** tingkat lanjut.

---

### 👥 Anggota Tim (TIM 13 - Kelas D)
| Nama | NIM |
| :--- | :--- |
| **Resty Amandha** | 2400018185 |
| **Linda Amelia Sari** | 2400018187 |

---

### 🚀 Fitur Utama
* **💳 Sistem Kasir:** * **Priority Queue:** Member diprioritaskan dalam antrean.
    * **Promo Khusus:** Diskon Seasonal (seperti *Jumat Berkah*).
    * **Struk Otomatis:** Pencetakan nota belanja setelah transaksi.
* **📦 Manajemen Gudang:**
    * Manajemen stok menggunakan integrasi **AVL Tree** dan **Linked List**.
    * Filtering barang berdasarkan Kode atau Nama.
* **🔐 Panel Admin:**
    * Laporan pemasukan harian secara *real-time*.
    * **Audit Trail:** Riwayat transaksi menggunakan sistem Stack.

---

### 🛠️ Struktur Data yang Digunakan
Proyek ini mengimplementasikan berbagai struktur data untuk mengoptimalkan performa:
1.  **Linked List**: Digunakan untuk penyimpanan dinamis data stok barang dan database member.
2.  **Priority Queue**: Mengatur urutan pelanggan di kasir (Member didahulukan daripada Non-Member).
3.  **Stack**: Menyimpan riwayat nota transaksi terakhir (LIFO) untuk keperluan audit admin.
4.  **AVL Tree**: Mengelompokkan kategori barang agar proses pencarian dan filtering menjadi lebih cepat dan seimbang.

---

### 🔑 Akses Login (Default)
| Role | Password | Kode Enkripsi |
| :--- | :--- | :--- |
| **Admin** | `1111` | 4444 |
| **Kasir** | `2222` | 5555 |
| **Gudang** | `3333` | 6666 |

---

### 📝 Catatan Jalankan Program
Agar program berjalan lancar saat menggunakan database eksternal:
1. Pastikan file `database_stok.txt` dan `membershp.txt` berada di folder yang sama dengan file `.exe`.

---
*Proyek ini dibuat untuk memenuhi Tugas Besar Mata Kuliah Struktur Data.*