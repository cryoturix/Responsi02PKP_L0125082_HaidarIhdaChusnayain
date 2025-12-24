// task.h

#ifndef TASK_H
#define TASK_H

//Enum untuk prioritas tugas
typedef enum {
    LOW,
    MEDIUM,
    HIGH
} Priority;

//Struct untuk data tugas tunggal
typedef struct {
    char* description; // Akan dialokasikan secara dinamis dengan malloc()
    Priority priority;
    unsigned int isCompleted : 1;   // unsigned= hanya nilai positif. Bitfield: Hanya menggunakan 1 bit
} Task;

typedef struct TaskNode { //Node untuk linked list
    Task data;
    struct TaskNode* next;
} TaskNode;

typedef struct { //Struct untuk mengelola seluruh daftar tugas (linked list)
    TaskNode* head;
    int count;
} TaskManager;

// daftar Fungsi
void initManager(TaskManager* manager); //Inisialisasi manager
void freeAllMemory(TaskManager* manager); //Bebaskan semua memori yang dialokasikan
void addTask(TaskManager* manager); //Kriteria 2: Fungsi yang menggunakan malloc untuk deskripsi
void displayTasks(TaskManager manager); //Menampilkan semua tugas
void markTaskCompleted(TaskManager* manager); //Menandai tugas sebagai selesai (menggunakan pass by reference)
void deleteTask(TaskManager* manager); //Menghapus sebuah tugas
void saveTasksToFile(TaskManager manager, const char* filename); //Fungsi untuk menyimpan dan memuat dari file
void loadTasksFromFile(TaskManager* manager, const char* filename);

#endif // TASK_H