// main.c

#include <stdio.h>
#include <stdlib.h>
#include "task.h"

void printMenu() {
    printf("\nMENU LIST TUGAS\n");
    printf("1. Tambahkan Tugas\n");
    printf("2. Lihat daftar Tugas\n");
    printf("3. Tandai Tugas sudah Selesai\n");
    printf("4. Hapus daftar Tugas\n");
    printf("5. Keluar\n");
    printf("Pilihan Anda: ");
}

int main() {
    TaskManager manager;
    const char* dataFilename = "daftarTugas.txt";

    // Inisialisasi manager
    initManager(&manager);

    // Kriteria 3: Muat data dari file saat program dimulai
    loadTasksFromFile(&manager, dataFilename);

    int choice;
    do {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // Bersihkan buffer input jika bukan angka
            choice = 0; // Set ke invalid
        }

        switch (choice) {
            case 1: addTask(&manager); break;
            case 2: displayTasks(manager); break;
            case 3: markTaskCompleted(&manager); break;// Pass by reference;
            case 4: deleteTask(&manager); break;// Pass by reference
            case 5: printf("Menyimpan data dan keluar...\n"); break;
            default: printf("Pilihan tidak ada. Silakan coba lagi.\n");
        }
    } while (choice != 5);

    // Simpan data ke file sebelum keluar
    saveTasksToFile(manager, dataFilename);

    // Bersihkan memori yang telah dialokasikan
    freeAllMemory(&manager);

    return 0;
}