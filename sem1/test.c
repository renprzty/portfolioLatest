#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MAHASISWA 100
#define FILE_NAME "mahasiswa.txt"

/*
============================================================
    SISTEM DATA MAHASISWA
    Final Project Algoritma & Pemrograman - Bahasa C
============================================================

Materi yang digunakan:
1. Variabel dan tipe data
2. Operator
3. Input / Output
4. Percabangan if / else
5. Switch-case
6. Perulangan for, while, do-while
7. Array
8. String
9. Function
10. Pointer
11. Struct
12. Searching
13. Sorting
14. File Handling
15. Validasi input
============================================================
*/


/* =========================================================
   STRUCT
   ========================================================= */

typedef struct {
    char nim[20];
    char nama[100];
    char jurusan[100];
    int semester;
    float ipk;
} Mahasiswa;


/* =========================================================
   FUNCTION PROTOTYPE
   ========================================================= */

/* Menu */
void tampilkanMenu();

/* Input helper */
void bersihkanInput();
void hapusNewline(char *teks);
void pauseProgram();

/* File */
void muatData(Mahasiswa data[], int *jumlah);
void simpanData(Mahasiswa data[], int jumlah);

/* CRUD */
void tambahMahasiswa(Mahasiswa data[], int *jumlah);
void tampilkanSemuaMahasiswa(Mahasiswa data[], int jumlah);
void editMahasiswa(Mahasiswa data[], int jumlah);
void hapusMahasiswa(Mahasiswa data[], int *jumlah);

/* Searching */
void cariMahasiswa(Mahasiswa data[], int jumlah);
int cariIndexNIM(Mahasiswa data[], int jumlah, char nim[]);

/* Sorting */
void menuSorting(Mahasiswa data[], int jumlah);
void urutkanNamaAscending(Mahasiswa data[], int jumlah);
void urutkanIPKTertinggi(Mahasiswa data[], int jumlah);
void urutkanNIMAscending(Mahasiswa data[], int jumlah);

/* Statistik */
void tampilkanStatistik(Mahasiswa data[], int jumlah);

/* Utility */
void tampilkanSatuMahasiswa(Mahasiswa mhs, int nomor);
int nimSudahAda(Mahasiswa data[], int jumlah, char nim[]);


/* =========================================================
   MAIN PROGRAM
   ========================================================= */

int main() {

    Mahasiswa data[MAX_MAHASISWA];

    int jumlah = 0;
    int pilihan;

    /*
        Pointer:
        &jumlah mengirim alamat variabel jumlah
        ke function muatData().
    */
    muatData(data, &jumlah);

    do {

        system("cls"); /* Untuk Windows */

        tampilkanMenu();

        printf("Masukkan pilihan: ");

        if (scanf("%d", &pilihan) != 1) {

            printf("\nInput harus berupa angka!\n");

            bersihkanInput();
            pauseProgram();

            continue;
        }

        bersihkanInput();

        switch (pilihan) {

            case 1:
                tambahMahasiswa(data, &jumlah);
                break;

            case 2:
                tampilkanSemuaMahasiswa(data, jumlah);
                break;

            case 3:
                cariMahasiswa(data, jumlah);
                break;

            case 4:
                editMahasiswa(data, jumlah);
                break;

            case 5:
                hapusMahasiswa(data, &jumlah);
                break;

            case 6:
                menuSorting(data, jumlah);
                break;

            case 7:
                tampilkanStatistik(data, jumlah);
                break;

            case 0:

                /*
                    Data kembali disimpan sebelum program
                    benar-benar ditutup.
                */
                simpanData(data, jumlah);

                printf("\n====================================\n");
                printf(" Data berhasil disimpan.\n");
                printf(" Terima kasih!\n");
                printf("====================================\n");

                break;

            default:

                printf("\nPilihan tidak tersedia!\n");
                pauseProgram();
        }

    } while (pilihan != 0);

    return 0;
}


/* =========================================================
   MENU
   ========================================================= */

void tampilkanMenu() {

    printf("=============================================\n");
    printf("          SISTEM DATA MAHASISWA\n");
    printf("=============================================\n");
    printf("1. Tambah Mahasiswa\n");
    printf("2. Lihat Semua Mahasiswa\n");
    printf("3. Cari Mahasiswa\n");
    printf("4. Edit Mahasiswa\n");
    printf("5. Hapus Mahasiswa\n");
    printf("6. Urutkan Data\n");
    printf("7. Statistik Mahasiswa\n");
    printf("0. Keluar\n");
    printf("=============================================\n");
}


/* =========================================================
   INPUT HELPER
   ========================================================= */

void bersihkanInput() {

    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        /* Menghabiskan karakter yang tersisa */
    }
}


void hapusNewline(char *teks) {

    /*
        strcspn mencari posisi karakter '\n'.
        Kemudian karakter tersebut diganti '\0'.
    */

    teks[strcspn(teks, "\n")] = '\0';
}


void pauseProgram() {

    printf("\nTekan ENTER untuk melanjutkan...");
    getchar();
}


/* =========================================================
   FILE HANDLING - LOAD DATA
   ========================================================= */

void muatData(Mahasiswa data[], int *jumlah) {

    FILE *file;

    file = fopen(FILE_NAME, "r");

    /*
        Jika file belum tersedia, berarti program
        kemungkinan dijalankan pertama kali.
    */
    if (file == NULL) {
        return;
    }

    /*
        Membaca data dengan format:

        NIM|Nama|Jurusan|Semester|IPK
    */

    while (*jumlah < MAX_MAHASISWA &&
           fscanf(
               file,
               "%19[^|]|%99[^|]|%99[^|]|%d|%f\n",
               data[*jumlah].nim,
               data[*jumlah].nama,
               data[*jumlah].jurusan,
               &data[*jumlah].semester,
               &data[*jumlah].ipk
           ) == 5) {

        (*jumlah)++;
    }

    fclose(file);
}


/* =========================================================
   FILE HANDLING - SAVE DATA
   ========================================================= */

void simpanData(Mahasiswa data[], int jumlah) {

    FILE *file;

    file = fopen(FILE_NAME, "w");

    if (file == NULL) {

        printf("\nERROR: File tidak dapat dibuka!\n");

        return;
    }

    for (int i = 0; i < jumlah; i++) {

        fprintf(
            file,
            "%s|%s|%s|%d|%.2f\n",
            data[i].nim,
            data[i].nama,
            data[i].jurusan,
            data[i].semester,
            data[i].ipk
        );
    }

    fclose(file);
}


/* =========================================================
   TAMBAH MAHASISWA
   ========================================================= */

void tambahMahasiswa(Mahasiswa data[], int *jumlah) {

    char nimBaru[20];

    system("cls");

    printf("=============================================\n");
    printf("            TAMBAH MAHASISWA\n");
    printf("=============================================\n");

    if (*jumlah >= MAX_MAHASISWA) {

        printf("\nData mahasiswa sudah penuh!\n");

        pauseProgram();

        return;
    }

    /*
        INPUT NIM
    */

    do {

        printf("NIM       : ");

        fgets(nimBaru, sizeof(nimBaru), stdin);

        hapusNewline(nimBaru);

        if (strlen(nimBaru) == 0) {

            printf("NIM tidak boleh kosong!\n");

        } else if (nimSudahAda(data, *jumlah, nimBaru)) {

            printf("NIM tersebut sudah terdaftar!\n");
        }

    } while (
        strlen(nimBaru) == 0 ||
        nimSudahAda(data, *jumlah, nimBaru)
    );

    strcpy(data[*jumlah].nim, nimBaru);


    /*
        INPUT NAMA
    */

    do {

        printf("Nama      : ");

        fgets(
            data[*jumlah].nama,
            sizeof(data[*jumlah].nama),
            stdin
        );

        hapusNewline(data[*jumlah].nama);

        if (strlen(data[*jumlah].nama) == 0) {
            printf("Nama tidak boleh kosong!\n");
        }

    } while (strlen(data[*jumlah].nama) == 0);


    /*
        INPUT JURUSAN
    */

    do {

        printf("Jurusan   : ");

        fgets(
            data[*jumlah].jurusan,
            sizeof(data[*jumlah].jurusan),
            stdin
        );

        hapusNewline(data[*jumlah].jurusan);

        if (strlen(data[*jumlah].jurusan) == 0) {
            printf("Jurusan tidak boleh kosong!\n");
        }

    } while (strlen(data[*jumlah].jurusan) == 0);


    /*
        INPUT SEMESTER
    */

    do {

        printf("Semester  : ");

        if (scanf("%d", &data[*jumlah].semester) != 1) {

            printf("Semester harus berupa angka!\n");

            bersihkanInput();

            data[*jumlah].semester = 0;

            continue;
        }

        bersihkanInput();

        if (
            data[*jumlah].semester < 1 ||
            data[*jumlah].semester > 14
        ) {

            printf("Semester harus antara 1 - 14!\n");
        }

    } while (
        data[*jumlah].semester < 1 ||
        data[*jumlah].semester > 14
    );


    /*
        INPUT IPK
    */

    do {

        printf("IPK       : ");

        if (scanf("%f", &data[*jumlah].ipk) != 1) {

            printf("IPK harus berupa angka!\n");

            bersihkanInput();

            data[*jumlah].ipk = -1;

            continue;
        }

        bersihkanInput();

        if (
            data[*jumlah].ipk < 0 ||
            data[*jumlah].ipk > 4.0
        ) {

            printf("IPK harus antara 0.00 - 4.00!\n");
        }

    } while (
        data[*jumlah].ipk < 0 ||
        data[*jumlah].ipk > 4.0
    );


    /*
        Jumlah mahasiswa bertambah.
    */

    (*jumlah)++;


    /*
        Langsung simpan perubahan ke file.
    */

    simpanData(data, *jumlah);

    printf("\n=============================================\n");
    printf("Mahasiswa berhasil ditambahkan!\n");
    printf("=============================================\n");

    pauseProgram();
}


/* =========================================================
   TAMPILKAN SEMUA MAHASISWA
   ========================================================= */

void tampilkanSemuaMahasiswa(Mahasiswa data[], int jumlah) {

    system("cls");

    printf("================================================================================================================\n");
    printf("                                           DATA MAHASISWA\n");
    printf("================================================================================================================\n");

    if (jumlah == 0) {

        printf("\nBelum ada data mahasiswa.\n");

        pauseProgram();

        return;
    }

    printf(
        "%-4s %-15s %-30s %-30s %-10s %-6s\n",
        "No",
        "NIM",
        "Nama",
        "Jurusan",
        "Semester",
        "IPK"
    );

    printf("----------------------------------------------------------------------------------------------------------------\n");

    /*
        Traversal array menggunakan for loop.
    */

    for (int i = 0; i < jumlah; i++) {

        printf(
            "%-4d %-15s %-30s %-30s %-10d %.2f\n",
            i + 1,
            data[i].nim,
            data[i].nama,
            data[i].jurusan,
            data[i].semester,
            data[i].ipk
        );
    }

    printf("================================================================================================================\n");

    printf("\nTotal mahasiswa: %d\n", jumlah);

    pauseProgram();
}


/* =========================================================
   TAMPILKAN SATU MAHASISWA
   ========================================================= */

void tampilkanSatuMahasiswa(Mahasiswa mhs, int nomor) {

    printf("\n=============================================\n");

    if (nomor > 0) {
        printf("Data ke   : %d\n", nomor);
    }

    printf("NIM       : %s\n", mhs.nim);
    printf("Nama      : %s\n", mhs.nama);
    printf("Jurusan   : %s\n", mhs.jurusan);
    printf("Semester  : %d\n", mhs.semester);
    printf("IPK       : %.2f\n", mhs.ipk);

    printf("=============================================\n");
}


/* =========================================================
   LINEAR SEARCH BERDASARKAN NIM
   ========================================================= */

int cariIndexNIM(Mahasiswa data[], int jumlah, char nim[]) {

    /*
        Linear Search

        Memeriksa data dari index 0 sampai index terakhir.
    */

    for (int i = 0; i < jumlah; i++) {

        if (strcmp(data[i].nim, nim) == 0) {

            return i;
        }
    }

    /*
        -1 berarti data tidak ditemukan.
    */

    return -1;
}


/* =========================================================
   CEK NIM DUPLIKAT
   ========================================================= */

int nimSudahAda(Mahasiswa data[], int jumlah, char nim[]) {

    if (cariIndexNIM(data, jumlah, nim) != -1) {
        return 1;
    }

    return 0;
}


/* =========================================================
   CARI MAHASISWA
   ========================================================= */

void cariMahasiswa(Mahasiswa data[], int jumlah) {

    char nim[20];

    int index;

    system("cls");

    printf("=============================================\n");
    printf("             CARI MAHASISWA\n");
    printf("=============================================\n");

    if (jumlah == 0) {

        printf("\nBelum ada data mahasiswa.\n");

        pauseProgram();

        return;
    }

    printf("Masukkan NIM: ");

    fgets(nim, sizeof(nim), stdin);

    hapusNewline(nim);


    /*
        Panggil algoritma Linear Search.
    */

    index = cariIndexNIM(data, jumlah, nim);


    if (index == -1) {

        printf("\nMahasiswa dengan NIM %s tidak ditemukan.\n", nim);

    } else {

        printf("\nMahasiswa ditemukan!\n");

        tampilkanSatuMahasiswa(data[index], index + 1);
    }

    pauseProgram();
}


/* =========================================================
   EDIT MAHASISWA
   ========================================================= */

void editMahasiswa(Mahasiswa data[], int jumlah) {

    char nim[20];

    int index;

    system("cls");

    printf("=============================================\n");
    printf("              EDIT MAHASISWA\n");
    printf("=============================================\n");

    if (jumlah == 0) {

        printf("\nBelum ada data mahasiswa.\n");

        pauseProgram();

        return;
    }

    printf("Masukkan NIM mahasiswa: ");

    fgets(nim, sizeof(nim), stdin);

    hapusNewline(nim);


    index = cariIndexNIM(data, jumlah, nim);


    if (index == -1) {

        printf("\nMahasiswa tidak ditemukan!\n");

        pauseProgram();

        return;
    }


    printf("\nData lama:");

    tampilkanSatuMahasiswa(data[index], index + 1);


    printf("\nMasukkan data baru\n");
    printf("---------------------------------------------\n");


    /*
        NIM tidak diubah supaya tetap menjadi
        identifier mahasiswa.
    */


    do {

        printf("Nama      : ");

        fgets(
            data[index].nama,
            sizeof(data[index].nama),
            stdin
        );

        hapusNewline(data[index].nama);

    } while (strlen(data[index].nama) == 0);


    do {

        printf("Jurusan   : ");

        fgets(
            data[index].jurusan,
            sizeof(data[index].jurusan),
            stdin
        );

        hapusNewline(data[index].jurusan);

    } while (strlen(data[index].jurusan) == 0);


    do {

        printf("Semester  : ");

        if (scanf("%d", &data[index].semester) != 1) {

            bersihkanInput();

            data[index].semester = 0;

            printf("Input tidak valid!\n");

            continue;
        }

        bersihkanInput();

        if (
            data[index].semester < 1 ||
            data[index].semester > 14
        ) {

            printf("Semester harus 1 - 14!\n");
        }

    } while (
        data[index].semester < 1 ||
        data[index].semester > 14
    );


    do {

        printf("IPK       : ");

        if (scanf("%f", &data[index].ipk) != 1) {

            bersihkanInput();

            data[index].ipk = -1;

            printf("Input tidak valid!\n");

            continue;
        }

        bersihkanInput();

        if (
            data[index].ipk < 0 ||
            data[index].ipk > 4
        ) {

            printf("IPK harus 0.00 - 4.00!\n");
        }

    } while (
        data[index].ipk < 0 ||
        data[index].ipk > 4
    );


    simpanData(data, jumlah);

    printf("\nData mahasiswa berhasil diperbarui!\n");

    pauseProgram();
}


/* =========================================================
   HAPUS MAHASISWA
   ========================================================= */

void hapusMahasiswa(Mahasiswa data[], int *jumlah) {

    char nim[20];

    char konfirmasi;

    int index;

    system("cls");

    printf("=============================================\n");
    printf("             HAPUS MAHASISWA\n");
    printf("=============================================\n");

    if (*jumlah == 0) {

        printf("\nBelum ada data mahasiswa.\n");

        pauseProgram();

        return;
    }

    printf("Masukkan NIM: ");

    fgets(nim, sizeof(nim), stdin);

    hapusNewline(nim);


    index = cariIndexNIM(data, *jumlah, nim);


    if (index == -1) {

        printf("\nMahasiswa tidak ditemukan!\n");

        pauseProgram();

        return;
    }


    printf("\nData yang akan dihapus:");

    tampilkanSatuMahasiswa(data[index], index + 1);


    printf("\nYakin ingin menghapus data? (y/n): ");

    scanf("%c", &konfirmasi);

    bersihkanInput();


    if (
        konfirmasi != 'y' &&
        konfirmasi != 'Y'
    ) {

        printf("\nPenghapusan dibatalkan.\n");

        pauseProgram();

        return;
    }


    /*
        Menggeser array ke kiri.

        Contoh:

        A B C D E
            ↑ hapus C

        menjadi:

        A B D E
    */

    for (int i = index; i < *jumlah - 1; i++) {

        data[i] = data[i + 1];
    }


    (*jumlah)--;


    simpanData(data, *jumlah);


    printf("\nMahasiswa berhasil dihapus!\n");

    pauseProgram();
}


/* =========================================================
   MENU SORTING
   ========================================================= */

void menuSorting(Mahasiswa data[], int jumlah) {

    int pilihan;

    system("cls");

    printf("=============================================\n");
    printf("              URUTKAN DATA\n");
    printf("=============================================\n");
    printf("1. Nama A - Z\n");
    printf("2. IPK Tertinggi - Terendah\n");
    printf("3. NIM Terkecil - Terbesar\n");
    printf("0. Kembali\n");
    printf("=============================================\n");

    printf("Pilihan: ");

    if (scanf("%d", &pilihan) != 1) {

        bersihkanInput();

        printf("\nInput tidak valid!\n");

        pauseProgram();

        return;
    }

    bersihkanInput();


    switch (pilihan) {

        case 1:

            urutkanNamaAscending(data, jumlah);

            printf("\nData berhasil diurutkan berdasarkan nama.\n");

            break;


        case 2:

            urutkanIPKTertinggi(data, jumlah);

            printf("\nData berhasil diurutkan berdasarkan IPK.\n");

            break;


        case 3:

            urutkanNIMAscending(data, jumlah);

            printf("\nData berhasil diurutkan berdasarkan NIM.\n");

            break;


        case 0:
            return;


        default:

            printf("\nPilihan tidak tersedia!\n");

            pauseProgram();

            return;
    }


    /*
        Simpan urutan baru ke file.
    */

    simpanData(data, jumlah);

    pauseProgram();
}


/* =========================================================
   BUBBLE SORT - NAMA A-Z
   ========================================================= */

void urutkanNamaAscending(Mahasiswa data[], int jumlah) {

    Mahasiswa temp;

    /*
        Bubble Sort

        strcmp > 0 berarti string sebelah kiri
        secara alfabet lebih besar.
    */

    for (int i = 0; i < jumlah - 1; i++) {

        for (int j = 0; j < jumlah - i - 1; j++) {

            if (
                strcmp(
                    data[j].nama,
                    data[j + 1].nama
                ) > 0
            ) {

                temp = data[j];

                data[j] = data[j + 1];

                data[j + 1] = temp;
            }
        }
    }
}


/* =========================================================
   BUBBLE SORT - IPK DESCENDING
   ========================================================= */

void urutkanIPKTertinggi(Mahasiswa data[], int jumlah) {

    Mahasiswa temp;

    for (int i = 0; i < jumlah - 1; i++) {

        for (int j = 0; j < jumlah - i - 1; j++) {

            /*
                Jika IPK sebelah kiri lebih kecil,
                tukar posisinya.
            */

            if (data[j].ipk < data[j + 1].ipk) {

                temp = data[j];

                data[j] = data[j + 1];

                data[j + 1] = temp;
            }
        }
    }
}


/* =========================================================
   BUBBLE SORT - NIM ASCENDING
   ========================================================= */

void urutkanNIMAscending(Mahasiswa data[], int jumlah) {

    Mahasiswa temp;

    for (int i = 0; i < jumlah - 1; i++) {

        for (int j = 0; j < jumlah - i - 1; j++) {

            if (
                strcmp(
                    data[j].nim,
                    data[j + 1].nim
                ) > 0
            ) {

                temp = data[j];

                data[j] = data[j + 1];

                data[j + 1] = temp;
            }
        }
    }
}


/* =========================================================
   STATISTIK MAHASISWA
   ========================================================= */

void tampilkanStatistik(Mahasiswa data[], int jumlah) {

    float totalIPK = 0;

    float rataRata;

    float ipkTertinggi;

    float ipkTerendah;

    int indexTertinggi = 0;

    int indexTerendah = 0;

    int cumlaude = 0;

    system("cls");

    printf("=============================================\n");
    printf("           STATISTIK MAHASISWA\n");
    printf("=============================================\n");


    if (jumlah == 0) {

        printf("\nBelum ada data mahasiswa.\n");

        pauseProgram();

        return;
    }


    /*
        Nilai awal diambil dari mahasiswa pertama.
    */

    ipkTertinggi = data[0].ipk;

    ipkTerendah = data[0].ipk;


    /*
        Traversal semua mahasiswa.
    */

    for (int i = 0; i < jumlah; i++) {

        totalIPK += data[i].ipk;


        /*
            Mencari IPK tertinggi.
        */

        if (data[i].ipk > ipkTertinggi) {

            ipkTertinggi = data[i].ipk;

            indexTertinggi = i;
        }


        /*
            Mencari IPK terendah.
        */

        if (data[i].ipk < ipkTerendah) {

            ipkTerendah = data[i].ipk;

            indexTerendah = i;
        }


        /*
            Hitung mahasiswa dengan IPK >= 3.50.
        */

        if (data[i].ipk >= 3.50) {

            cumlaude++;
        }
    }


    /*
        Casting tidak diperlukan karena totalIPK
        sudah bertipe float.
    */

    rataRata = totalIPK / jumlah;


    printf("Jumlah mahasiswa       : %d\n", jumlah);

    printf("Rata-rata IPK          : %.2f\n", rataRata);

    printf(
        "IPK tertinggi          : %.2f (%s)\n",
        ipkTertinggi,
        data[indexTertinggi].nama
    );

    printf(
        "IPK terendah           : %.2f (%s)\n",
        ipkTerendah,
        data[indexTerendah].nama
    );

    printf(
        "IPK >= 3.50            : %d mahasiswa\n",
        cumlaude
    );


    /*
        Menghitung persentase.

        Casting (float) digunakan agar pembagian
        menghasilkan angka desimal.
    */

    float persentaseCumlaude =
        ((float) cumlaude / jumlah) * 100;


    printf(
        "Persentase IPK >= 3.50 : %.2f%%\n",
        persentaseCumlaude
    );


    printf("=============================================\n");


    /*
        Contoh percabangan berdasarkan rata-rata IPK.
    */

    if (rataRata >= 3.50) {

        printf("Keterangan: Rata-rata IPK sangat baik.\n");

    } else if (rataRata >= 3.00) {

        printf("Keterangan: Rata-rata IPK baik.\n");

    } else if (rataRata >= 2.50) {

        printf("Keterangan: Rata-rata IPK cukup.\n");

    } else {

        printf("Keterangan: Rata-rata IPK perlu ditingkatkan.\n");
    }


    pauseProgram();
}