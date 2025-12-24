// task.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"

void initManager(TaskManager* manager) {
    manager->head = NULL;
    manager->count = 0;
}

void freeAllMemory(TaskManager* manager) {
    TaskNode* current = manager->head;
    TaskNode* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        // Bebaskan memori untuk deskripsi
        free(current->data.description);
        // Bebaskan memori untuk node
        free(current);
        current = nextNode;
    }
    manager->head = NULL;
    manager->count = 0;
}

void addTask(TaskManager* manager) {
    // Kriteria 2: Dynamic Memory Allocation untuk node baru
    TaskNode* newNode = (TaskNode*)malloc(sizeof(TaskNode));
    if (newNode == NULL) {
        printf("Gagal mengalokasikan memori untuk tugas baru!\n");
        return;
    }

    char buffer[256];
    printf("Masukkan deskripsi tugas: ");
    getchar(); // Mengkonsumsi newline karakter tersisa dari scanf sebelumnya
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Menghapus newline di akhir

    // Kriteria 2: Dynamic Memory Allocation untuk string deskripsi
    newNode->data.description = (char*)malloc(strlen(buffer) + 1);
    if (newNode->data.description == NULL) {
        printf("Gagal mengalokasikan memori untuk deskripsi!\n");
        free(newNode);
        return;
    }
    strcpy(newNode->data.description, buffer);

    int p;
    printf("Masukkan prioritas (0: rendah, 1: standar, 2: tinggi): ");
    if (scanf("%d", &p) != 1) {
        while(getchar() != '\n'); // Bersihkan buffer
        p = 0; // Default ke Rendah jika error
    }
    if (p == 0) newNode->data.priority = LOW;
    else if (p == 1) newNode->data.priority = MEDIUM;
    else newNode->data.priority = HIGH;

    newNode->data.isCompleted = 0;
    newNode->next = NULL;

    if (manager->head == NULL) {
        manager->head = newNode;
    } else {
        TaskNode* temp = manager->head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
    manager->count++;

    printf("Tugas berhasil ditambahkan!\n");
}

void displayTasks(TaskManager manager) {
    if (manager.count == 0) {
        printf("Daftar tugas kosong.\n");
        return;
    }

    printf("\nLIST DAFTAR TUGAS\n");
    TaskNode* current = manager.head;
    int i = 1;
    while (current != NULL) {
        const char* status = (current->data.isCompleted) ? "[Selesai]" : "[Belum]";
        const char* priorityStr;
        switch (current->data.priority) {
            case LOW: priorityStr = "Rendah"; break;
            case MEDIUM: priorityStr = "Sedang"; break;
            case HIGH: priorityStr = "Tinggi"; break;
        }
        printf("%d. %s\n", i, current->data.description);
        printf("   Status: %s | Prioritas: %s\n\n", status, priorityStr);
        current = current->next;
        i++;
    }
}

void markTaskCompleted(TaskManager* manager) {
    displayTasks(*manager); // Tampilkan daftar untuk memudahkan pemilihan
    if (manager->count == 0) return;

    int num;
    printf("Masukkan nomor tugas yang ingin di ubah ke selesai: ");
    if (scanf("%d", &num) != 1) {
        while(getchar() != '\n');
        num = -1;
    }

    if (num <= 0 || num > manager->count) {
        printf("Nomor tugas tidak valid.\n");
        return;
    }

    TaskNode* current = manager->head;
    for (int i = 1; i < num; i++) {
        current = current->next;
    }

    current->data.isCompleted = 1;
    printf("Tugas '%s' telah ditandai sebagai selesai.\n", current->data.description);
}

void deleteTask(TaskManager* manager) {
    displayTasks(*manager);
    if (manager->count == 0) return;

    int num;
    printf("Masukkan nomor tugas yang ingin dihapus: ");
    if (scanf("%d", &num) != 1) {
        while(getchar() != '\n');
        num = -1;
    }

    if (num <= 0 || num > manager->count) {
        printf("Nomor tugas tidak valid.\n");
        return;
    }

    TaskNode* toDelete = manager->head;
    TaskNode* prev = NULL;

    if (num == 1) {
        manager->head = toDelete->next;
    } else {
        for (int i = 1; i < num; i++) {
            prev = toDelete;
            toDelete = toDelete->next;
        }
        prev->next = toDelete->next;
    }
    
    printf("Tugas '%s' telah dihapus.\n", toDelete->data.description);
    free(toDelete->data.description); // Bebaskan memori deskripsi
    free(toDelete);                  // Bebaskan memori node
    manager->count--;
}

void saveTasksToFile(TaskManager manager, const char* filename) {
    // Ganti "wb" (write binary) jadi "w" (write text)
    FILE* file = fopen(filename, "w"); 
    if (file == NULL) {
        printf("Gagal membuka file untuk menulis.\n");
        return;
    }

    TaskNode* current = manager.head;
    while (current != NULL) {
        fprintf(file, "%d|%d|%s\n", 
            current->data.isCompleted, 
            current->data.priority, 
            current->data.description);
        
        current = current->next;
    }

    fclose(file);
    printf("Semua tugas berhasil disimpan ke %s (Format Text)\n", filename);
}

void loadTasksFromFile(TaskManager* manager, const char* filename) {
    // Ganti "rb" (read binary) jadi "r" (read text)
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Belum ada file data. Memulai dengan daftar kosong.\n");
        return;
    }

    char buffer[256]; // Wadah untuk menampung satu baris teks
    
    // fgets membaca file baris demi baris sampai habis
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        
        // 1. Bersihkan newline (\n) di ujung kalimat
        buffer[strcspn(buffer, "\n")] = 0;

        // 2. Siapkan Node Baru
        TaskNode* newNode = (TaskNode*)malloc(sizeof(TaskNode));
        
        // --- MEMECAH DATA (PARSING) ---
        
        // Ambil potongan pertama (Status) sebelum tanda '|'
        char* token = strtok(buffer, "|");
        if (token != NULL) {
            newNode->data.isCompleted = atoi(token); // Ubah string ke int
        }

        // Ambil potongan kedua (Prioritas)
        token = strtok(NULL, "|"); 
        if (token != NULL) {
            newNode->data.priority = (Priority)atoi(token);
        }

        // Ambil potongan ketiga (Deskripsi)
        token = strtok(NULL, "|");
        if (token != NULL) {
            // Malloc memori untuk deskripsi sesuai panjang teks
            newNode->data.description = (char*)malloc(strlen(token) + 1);
            strcpy(newNode->data.description, token);
        } else {
            // Jaga-jaga jika deskripsi kosong/rusak
            newNode->data.description = (char*)malloc(8);
            strcpy(newNode->data.description, "Unknown");
        }

        // --- MENYAMBUNGKAN NODE (Sama seperti logika addTask) ---
        newNode->next = NULL;
        
        if (manager->head == NULL) {
            manager->head = newNode;
        } else {
            // Cari ekor untuk disambung
            TaskNode* temp = manager->head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        manager->count++;
    }
    
    fclose(file);
    printf("Data berhasil dimuat dari %s\n", filename);
}