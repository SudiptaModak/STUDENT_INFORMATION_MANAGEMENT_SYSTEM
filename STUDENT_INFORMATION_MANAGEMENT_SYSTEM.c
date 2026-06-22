#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long id1 = 0;
struct Student {
    int roll;
    char name[50];
    int age;
    int completedSem;
    long id;
    float sgpa[8];
    float currentCGPA;
    float cgpa;
};

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

// Helper comparison function to sort students by ID for Binary Search
int compareStudentsByID(const void *a, const void *b) {
    long idA = ((struct Student *)a)->id;
    long idB = ((struct Student *)b)->id;
    if (idA < idB) return -1;
    if (idA > idB) return 1;
    return 0;
}

int main() {
    int choice;

    while (1) {
        printf("\n====== STUDENT INFORMATION MANAGEMENT SYSTEM ======\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Student by ID\n");
        printf("4. Update Student Record\n");
        printf("5. Delete Student Record\n");
        printf("6. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6: {
                FILE *fp1;
                fp1 = fopen("hifhestID.dat", "w+"); 
                if (fp1 != NULL) {
                    fprintf(fp1, "%ld", id1);
                    rewind(fp1);
                    long id2 = 0;
                    if (fscanf(fp1, "%ld", &id2) == 1) {
                        printf("HIGHEST ID: %ld\n", id2);
                    }
                    fclose(fp1);
                }
                printf("Exiting Program...\n");
                exit(0);
            }
            default:
                printf("Invalid Choice!\n");
        }
    }
    return 0;
}

void addStudent() {
    FILE *fp;
    struct Student s, temp;
    int exists = 0;

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);

    fp = fopen("students.dat", "rb");

    if (fp != NULL) {
        while (fread(&temp, sizeof(temp), 1, fp) == 1) {
            if (temp.roll == s.roll) {
                exists = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (exists) {
        printf("Error! Roll Number %d already exists.\n", s.roll);
        return;
    }

    fp = fopen("students.dat", "ab");

    if (fp == NULL) {
        printf("File cannot be opened!\n");
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);

    printf("Enter Age: ");
    scanf("%d", &s.age);

    printf("How many semesters completed (1-8): ");
    scanf("%d", &s.completedSem);

    if (s.completedSem < 1 || s.completedSem > 8) {
        printf("Invalid semester count!\n");
        fclose(fp);
        return;
    }

    s.cgpa = -1;

    for (int i = 0; i < 8; i++) {
        s.sgpa[i] = -1;
    }

    for (int i = 0; i < s.completedSem; i++) {
        printf("Enter SGPA of Semester %d: ", i + 1);
        scanf("%f", &s.sgpa[i]);

        if (s.sgpa[i] < 0 || s.sgpa[i] > 10) {
            printf("Invalid SGPA! Must be between 0 and 10.\n");
            fclose(fp);
            return;
        }
    }
    float sum = 0;
    for (int i = 0; i < s.completedSem; i++) {
        sum += s.sgpa[i];
    }
    s.currentCGPA = sum / s.completedSem;
    if (s.completedSem == 8) {
        float total = 0;
        for (int i = 0; i < 8; i++) {
            total += s.sgpa[i];
        }
        s.cgpa = total / 8.0;
    }
    id1++;
    s.id = id1;
    printf("ID OF %s : %ld\n", s.name, s.id);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Student Record Added Successfully!\n");
}

void displayStudents() {
    FILE *fp;
    struct Student s;

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("\n====== STUDENT RECORDS ======\n");

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("\nRoll Number : %d\n", s.roll);
        printf("Name        : %s\n", s.name);
        printf("Age         : %d\n", s.age);
        printf("ID          : %ld\n", s.id);

        printf("\nSemester SGPA:\n");

        for (int i = 0; i < s.completedSem; i++) {
            printf("Semester %d : %.2f\n", i + 1, s.sgpa[i]);
        }

        if (s.cgpa == -1) {
            printf("\nCURRENT CGPA : %.2f\n", s.currentCGPA);
            printf("\nFINAL CGPA cannot be generated.\n");
            printf("Student has completed only %d semester(s).\n", s.completedSem);
        } else {
            printf("\nCGPA : %.2f\n", s.cgpa);
        }

        printf("\n----------------------------------\n");
    }

    fclose(fp);
}

void searchStudent() {
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("File not found!\n");
        return;
    }

  
    fseek(fp, 0, SEEK_END);
    long totalBytes = ftell(fp);
    int count = totalBytes / sizeof(struct Student);
    rewind(fp);

    if (count == 0) {
        printf("No records found!\n");
        fclose(fp);
        return;
    }

    struct Student *arr = (struct Student *)malloc(count * sizeof(struct Student));
    fread(arr, sizeof(struct Student), count, fp);
    fclose(fp);

   
    qsort(arr, count, sizeof(struct Student), compareStudentsByID);

    long searchId;
    printf("Enter Student ID to Search: ");
    scanf("%ld", &searchId);

   
    int low = 0, high = count - 1, foundIdx = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].id == searchId) {
            foundIdx = mid;
            break;
        } else if (arr[mid].id < searchId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (foundIdx != -1) {
        struct Student s = arr[foundIdx];
        printf("\nStudent Found!\n");
        printf("Roll Number : %d\n", s.roll);
        printf("Name        : %s\n", s.name);
        printf("Age         : %d\n", s.age);
        printf("ID          : %ld\n", s.id);

        printf("\nSemester SGPA:\n");
        for (int i = 0; i < s.completedSem; i++) {
            printf("Semester %d : %.2f\n", i + 1, s.sgpa[i]);
        }

        if (s.cgpa == -1) {
            printf("\nCURRENT CGPA : %.2f\n", s.currentCGPA);
            printf("\nFINAL CGPA cannot be generated.\n");
            printf("Student has completed only %d semester(s).\n", s.completedSem);
        } else {
            printf("\nCGPA : %.2f\n", s.cgpa);
        }
    } else {
        printf("Student Record Not Found!\n");
    }

    free(arr);
}

void updateStudent() {
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("File not found!\n");
        return;
    }

  
    fseek(fp, 0, SEEK_END);
    long totalBytes = ftell(fp);
    int count = totalBytes / sizeof(struct Student);
    rewind(fp);

    if (count == 0) {
        printf("No records found!\n");
        fclose(fp);
        return;
    }

    struct Student *arr = (struct Student *)malloc(count * sizeof(struct Student));
    fread(arr, sizeof(struct Student), count, fp);
    fclose(fp);

    qsort(arr, count, sizeof(struct Student), compareStudentsByID);

    long searchId;
    printf("Enter Student ID to Update: ");
    scanf("%ld", &searchId);

    // Binary Search
    int low = 0, high = count - 1, foundIdx = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].id == searchId) {
            foundIdx = mid;
            break;
        } else if (arr[mid].id < searchId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (foundIdx != -1) {
        int sem = arr[foundIdx].completedSem;
        float prevSgpa[8];
        for (int i = 0; i < sem; i++) {
            prevSgpa[i] = arr[foundIdx].sgpa[i];
        }

        printf("\nEnter New Name: ");
        scanf(" %[^\n]", arr[foundIdx].name);

        printf("Enter New Age: ");
        scanf("%d", &arr[foundIdx].age);

        printf("How many semesters completed (1-8): ");
        scanf("%d", &arr[foundIdx].completedSem);

        for (int i = 0; i < sem; i++) {
            arr[foundIdx].sgpa[i] = prevSgpa[i];
        }
        for (int i = sem; i < 8; i++) {
            arr[foundIdx].sgpa[i] = -1;
        }

        for (int i = sem; i < arr[foundIdx].completedSem; i++) {
            printf("Enter SGPA of Semester %d: ", i + 1);
            scanf("%f", &arr[foundIdx].sgpa[i]);
        }

        float sum = 0;
        for (int i = 0; i < arr[foundIdx].completedSem; i++) {
            sum += arr[foundIdx].sgpa[i];
        }
        arr[foundIdx].currentCGPA = sum / arr[foundIdx].completedSem;

        if (arr[foundIdx].completedSem == 8) {
            float total = 0;
            for (int i = 0; i < 8; i++) {
                total += arr[foundIdx].sgpa[i];
            }
            arr[foundIdx].cgpa = total / 8.0;
        } else {
            arr[foundIdx].cgpa = -1;
        }

        // Rewrite all records back sorted or modified to save changes
        fp = fopen("students.dat", "wb");
        fwrite(arr, sizeof(struct Student), count, fp);
        fclose(fp);

        printf("Record Updated Successfully!\n");
    } else {
        printf("Student Record Not Found!\n");
    }

    free(arr);
}

void deleteStudent() {
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("File not found!\n");
        return;
    }

    // Load data into an array to prepare for Binary Search
    fseek(fp, 0, SEEK_END);
    long totalBytes = ftell(fp);
    int count = totalBytes / sizeof(struct Student);
    rewind(fp);

    if (count == 0) {
        printf("No records found!\n");
        fclose(fp);
        return;
    }

    struct Student *arr = (struct Student *)malloc(count * sizeof(struct Student));
    fread(arr, sizeof(struct Student), count, fp);
    fclose(fp);
    qsort(arr, count, sizeof(struct Student), compareStudentsByID);

    long searchId;
    printf("Enter Student ID to Delete: ");
    scanf("%ld", &searchId);

   
    int low = 0, high = count - 1, foundIdx = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].id == searchId) {
            foundIdx = mid;
            break;
        } else if (arr[mid].id < searchId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (foundIdx != -1) {
     
        fp = fopen("students.dat", "wb");
        for (int i = 0; i < count; i++) {
            if (i != foundIdx) {
                fwrite(&arr[i], sizeof(struct Student), 1, fp);
            }
        }
        fclose(fp);
        printf("Record Deleted Successfully!\n");
    } else {
        printf("Student Record Not Found!\n");
    }

    free(arr);
}