//AI được sử dụng trong bài để:
//+ cải thiện UI
//+ gợi ý sử dụng file .dat
//+ kiểm soát các lỗ hổng lỗi, cấu trúc
//+ gợi ý các tên hàm, cấu trúc phù hợp dự án

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STUDENT_ID_LENGTH 8
#define FILENAME "students.dat"
#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 12

typedef struct {
    char fullName[MAX_NAME_LENGTH];
    char rollNumber[STUDENT_ID_LENGTH + 1];
    char birthday[MAX_DATE_LENGTH];
    float prfMark;
    float maeMark;
    float csiMark;
    float ceaMark;
    float gpa;
} Student;

Student *studentList = NULL;
int studentCount = 0;

// Function prototypes
void add_student();
void update_student();
void delete_student();
void display_all_students();
void calculate_gpa();
void search_student();
void sort_students();
void save_to_file();
void load_from_file();
void clear_input_buffer();
void free_memory();
int validate_roll_number(const char *roll);
int validate_date(const char *date);
int validate_mark(float mark);
int is_duplicate_roll(const char *roll);
int get_valid_choice(int min, int max);
float get_valid_mark(const char *subject);
void trim_string(char *str);

int main() {
    int choice;

    load_from_file();

    do {
        printf("\n\n========================================\n");
        printf("   CHUONG TRINH QUAN LY SINH VIEN\n");
        printf("========================================\n");
        printf("1. Them sinh vien moi\n");
        printf("2. Sua thong tin sinh vien\n");
        printf("3. Xoa sinh vien\n");
        printf("4. Xem ca lop\n");
        printf("5. Tinh GPA cho ca lop\n");
        printf("6. Tim kiem sinh vien\n");
        printf("7. Sap xep danh sach\n");
        printf("8. Luu du lieu\n");
        printf("0. Thoat\n");
        printf("========================================\n");
        
        choice = get_valid_choice(0, 8);

        switch (choice) {
            case 1: add_student(); break;
            case 2: update_student(); break;
            case 3: delete_student(); break;
            case 4: display_all_students(); break;
            case 5: calculate_gpa(); break;
            case 6: search_student(); break;
            case 7: sort_students(); break;
            case 8: save_to_file(); break;
            case 0:
                printf("\n>> Dang luu du lieu...\n");
                save_to_file();
                printf(">> Cam on ban da su dung chuong trinh! Bye bye!\n");
                break;
        }
    } while (choice != 0);

    free_memory();
    return 0;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void trim_string(char *str) {
    char *start = str;
    char *end;
    
    // Trim leading spaces
    while(isspace((unsigned char)*start)) start++;
    
    if(*start == 0) {
        *str = 0;
        return;
    }
    
    // Trim trailing spaces
    end = start + strlen(start) - 1;
    while(end > start && isspace((unsigned char)*end)) end--;
    
    *(end + 1) = 0;
    
    if(start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

void free_memory() {
    if (studentList != NULL) {
        free(studentList);
        studentList = NULL;
    }
    studentCount = 0;
}

int get_valid_choice(int min, int max) {
    int choice;
    char input[100];
    
    while (1) {
        printf(">> Lua chon cua ban: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("[LOI] Khong doc duoc du lieu. Thu lai.\n");
            continue;
        }
        
        trim_string(input);
        
        if (strlen(input) == 0) {
            printf("[LOI] Khong duoc de trong! Thu lai.\n");
            continue;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("[LOI] Vui long nhap so tu %d den %d!\n", min, max);
            continue;
        }
        
        if (choice < min || choice > max) {
            printf("[LOI] Lua chon phai tu %d den %d. Thu lai.\n", min, max);
            continue;
        }
        
        return choice;
    }
}

float get_valid_mark(const char *subject) {
    float mark;
    char input[100];
    
    while (1) {
        printf("Nhap diem %s (0-10): ", subject);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("[LOI] Khong doc duoc du lieu. Thu lai.\n");
            continue;
        }
        
        trim_string(input);
        
        if (strlen(input) == 0) {
            printf("[LOI] Khong duoc de trong! Thu lai.\n");
            continue;
        }
        
        if (sscanf(input, "%f", &mark) != 1) {
            printf("[LOI] Diem phai la so! Thu lai.\n");
            continue;
        }
        
        if (!validate_mark(mark)) {
            printf("[LOI] Diem phai tu 0 den 10! Thu lai.\n");
            continue;
        }
        
        return mark;
    }
}

int validate_mark(float mark) {
    return (mark >= 0.0 && mark <= 10.0);
}

int validate_roll_number(const char *roll) {
    if (strlen(roll) != STUDENT_ID_LENGTH) {
        return 0;
    }
    
    // Check format: 2 letters + 6 digits (e.g., SE123456)
    if (!isalpha(roll[0]) || !isalpha(roll[1])) {
        return 0;
    }
    
    for (int i = 2; i < STUDENT_ID_LENGTH; i++) {
        if (!isdigit(roll[i])) {
            return 0;
        }
    }
    
    return 1;
}

int validate_date(const char *date) {
    // Format: dd/mm/yyyy
    if (strlen(date) != 10) {
        return 0;
    }
    
    if (date[2] != '/' || date[5] != '/') {
        return 0;
    }
    
    int day, month, year;
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) {
        return 0;
    }
    
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2025) {
        return 0;
    }
    
    // Check days in month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Leap year check
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    
    if (day > daysInMonth[month - 1]) {
        return 0;
    }
    
    return 1;
}

int is_duplicate_roll(const char *roll) {
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(studentList[i].rollNumber, roll) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_student() {
    Student *temp = realloc(studentList, (studentCount + 1) * sizeof(Student));
    if (temp == NULL) {
        printf("\n[LOI NGHIEM TRONG] Het bo nho! Khong the them sinh vien.\n");
        return;
    }
    studentList = temp;

    Student *newStudent = &studentList[studentCount];
    char input[100];

    printf("\n========================================\n");
    printf("      THEM SINH VIEN MOI\n");
    printf("========================================\n");

    // Input and validate Roll Number
    while (1) {
        printf("Nhap MSSV (%d ky tu, VD: SE123456): ", STUDENT_ID_LENGTH);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("[LOI] Khong doc duoc du lieu. Thu lai.\n");
            continue;
        }
        
        trim_string(input);
        
        if (strlen(input) == 0) {
            printf("[LOI] MSSV khong duoc de trong!\n");
            continue;
        }
        
        if (!validate_roll_number(input)) {
            printf("[LOI] MSSV phai co dung %d ky tu (2 chu cai + 6 chu so).\n", STUDENT_ID_LENGTH);
            printf("      Vi du: SE123456, HE100234\n");
            continue;
        }
        
        if (is_duplicate_roll(input)) {
            printf("[LOI] MSSV nay da ton tai trong he thong!\n");
            continue;
        }
        
        strcpy(newStudent->rollNumber, input);
        break;
    }

    // Input and validate Full Name
    while (1) {
        printf("Nhap ho ten day du: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("[LOI] Khong doc duoc du lieu. Thu lai.\n");
            continue;
        }
        
        trim_string(input);
        
        if (strlen(input) == 0) {
            printf("[LOI] Ho ten khong duoc de trong!\n");
            continue;
        }
        
        if (strlen(input) >= MAX_NAME_LENGTH) {
            printf("[LOI] Ho ten qua dai (toi da %d ky tu)!\n", MAX_NAME_LENGTH - 1);
            continue;
        }
        
        strcpy(newStudent->fullName, input);
        break;
    }

    // Input and validate Birthday
    while (1) {
        printf("Nhap ngay sinh (dd/mm/yyyy): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("[LOI] Khong doc duoc du lieu. Thu lai.\n");
            continue;
        }
        
        trim_string(input);
        
        if (strlen(input) == 0) {
            printf("[LOI] Ngay sinh khong duoc de trong!\n");
            continue;
        }
        
        if (!validate_date(input)) {
            printf("[LOI] Ngay sinh khong hop le!\n");
            printf("      Dinh dang dung: dd/mm/yyyy (VD: 15/08/2005)\n");
            continue;
        }
        
        strcpy(newStudent->birthday, input);
        break;
    }

    // Input marks with validation
    newStudent->prfMark = get_valid_mark("PRF");
    newStudent->maeMark = get_valid_mark("MAE");
    newStudent->csiMark = get_valid_mark("CSI");
    newStudent->ceaMark = get_valid_mark("CEA");

    // Calculate GPA
    newStudent->gpa = (newStudent->prfMark + newStudent->maeMark + 
                       newStudent->csiMark + newStudent->ceaMark) / 4.0;
    
    studentCount++;
    
    printf("\n========================================\n");
    printf(">> THEM SINH VIEN THANH CONG! <<\n");
    printf("   MSSV: %s\n", newStudent->rollNumber);
    printf("   Ho ten: %s\n", newStudent->fullName);
    printf("   GPA: %.2f\n", newStudent->gpa);
    printf("========================================\n");
}

void update_student() {
    char rollToUpdate[STUDENT_ID_LENGTH + 1];
    char input[100];
    int found = -1;

    if (studentCount == 0) {
        printf("\n[THONG BAO] Danh sach rong, khong co gi de sua.\n");
        return;
    }

    printf("\n========================================\n");
    printf("      CAP NHAT THONG TIN SINH VIEN\n");
    printf("========================================\n");
    
    while (1) {
        printf("Nhap MSSV can sua: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("[LOI] Khong doc duoc du lieu. Thu lai.\n");
            continue;
        }
        
        trim_string(input);
        
        if (strlen(input) == 0) {
            printf("[LOI] MSSV khong duoc de trong!\n");
            continue;
        }
        
        strcpy(rollToUpdate, input);
        break;
    }

    for (int i = 0; i < studentCount; i++) {
        if (strcmp(studentList[i].rollNumber, rollToUpdate) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\n[LOI] Khong tim thay sinh vien voi MSSV: %s\n", rollToUpdate);
        return;
    }

    printf("\n>> Tim thay sinh vien: %s\n", studentList[found].fullName);
    printf(">> Nhap thong tin moi (Enter de giu nguyen):\n\n");
    
    // Update Full Name
    printf("Ho ten hien tai: %s\n", studentList[found].fullName);
    printf("Ho ten moi: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        trim_string(input);
        if (strlen(input) > 0 && strlen(input) < MAX_NAME_LENGTH) {
            strcpy(studentList[found].fullName, input);
        }
    }

    // Update Birthday
    printf("\nNgay sinh hien tai: %s\n", studentList[found].birthday);
    while (1) {
        printf("Ngay sinh moi (dd/mm/yyyy): ");
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        
        trim_string(input);
        if (strlen(input) == 0) break;
        
        if (validate_date(input)) {
            strcpy(studentList[found].birthday, input);
            break;
        } else {
            printf("[LOI] Ngay sinh khong hop le! Thu lai hoac Enter de bo qua.\n");
        }
    }

    // Update Marks
    printf("\nDiem PRF hien tai: %.2f\n", studentList[found].prfMark);
    printf("Diem PRF moi (Enter de giu nguyen): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        trim_string(input);
        if (strlen(input) > 0) {
            float mark;
            if (sscanf(input, "%f", &mark) == 1 && validate_mark(mark)) {
                studentList[found].prfMark = mark;
            }
        }
    }

    printf("\nDiem MAE hien tai: %.2f\n", studentList[found].maeMark);
    printf("Diem MAE moi (Enter de giu nguyen): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        trim_string(input);
        if (strlen(input) > 0) {
            float mark;
            if (sscanf(input, "%f", &mark) == 1 && validate_mark(mark)) {
                studentList[found].maeMark = mark;
            }
        }
    }

    printf("\nDiem CSI hien tai: %.2f\n", studentList[found].csiMark);
    printf("Diem CSI moi (Enter de giu nguyen): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        trim_string(input);
        if (strlen(input) > 0) {
            float mark;
            if (sscanf(input, "%f", &mark) == 1 && validate_mark(mark)) {
                studentList[found].csiMark = mark;
            }
        }
    }

    printf("\nDiem CEA hien tai: %.2f\n", studentList[found].ceaMark);
    printf("Diem CEA moi (Enter de giu nguyen): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        trim_string(input);
        if (strlen(input) > 0) {
            float mark;
            if (sscanf(input, "%f", &mark) == 1 && validate_mark(mark)) {
                studentList[found].ceaMark = mark;
            }
        }
    }

    // Recalculate GPA
    studentList[found].gpa = (studentList[found].prfMark + studentList[found].maeMark + 
                              studentList[found].csiMark + studentList[found].ceaMark) / 4.0;
    
    printf("\n========================================\n");
    printf(">> CAP NHAT THANH CONG! <<\n");
    printf("   GPA moi: %.2f\n", studentList[found].gpa);
    printf("========================================\n");
}

void delete_student() {
    char rollToDelete[STUDENT_ID_LENGTH + 1];
    char input[100];
    char confirm;
    int found = -1;

    if (studentCount == 0) {
        printf("\n[THONG BAO] Danh sach rong, khong co gi de xoa.\n");
        return;
    }

    printf("\n========================================\n");
    printf("          XOA SINH VIEN\n");
    printf("========================================\n");
    
    while (1) {
        printf("Nhap MSSV can xoa: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("[LOI] Khong doc duoc du lieu. Thu lai.\n");
            continue;
        }
        
        trim_string(input);
        
        if (strlen(input) == 0) {
            printf("[LOI] MSSV khong duoc de trong!\n");
            continue;
        }
        
        strcpy(rollToDelete, input);
        break;
    }

    for (int i = 0; i < studentCount; i++) {
        if (strcmp(studentList[i].rollNumber, rollToDelete) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\n[LOI] Khong tim thay sinh vien voi MSSV: %s\n", rollToDelete);
        return;
    }

    printf("\n>> Tim thay sinh vien:\n");
    printf("   Ho ten: %s\n", studentList[found].fullName);
    printf("   MSSV: %s\n", studentList[found].rollNumber);
    printf("   GPA: %.2f\n\n", studentList[found].gpa);
    
    while (1) {
        printf("Ban co chac chan muon xoa? (Y/N): ");
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        
        trim_string(input);
        if (strlen(input) == 0) continue;
        
        confirm = toupper(input[0]);
        if (confirm == 'Y' || confirm == 'N') break;
        
        printf("[LOI] Vui long nhap Y hoac N!\n");
    }

    if (confirm != 'Y') {
        printf("\n>> Da huy thao tac xoa.\n");
        return;
    }

    // Delete student
    for (int i = found; i < studentCount - 1; i++) {
        studentList[i] = studentList[i + 1];
    }
    studentCount--;

    if (studentCount == 0) {
        free_memory();
    } else {
        Student* temp = realloc(studentList, studentCount * sizeof(Student));
        if (temp != NULL) {
            studentList = temp;
        }
    }
    
    printf("\n========================================\n");
    printf(">> DA XOA SINH VIEN THANH CONG! <<\n");
    printf("========================================\n");
}

void display_all_students() {
    printf("\n========================================\n");
    printf("        DANH SACH CA LOP\n");
    printf("========================================\n");
    
    if (studentCount == 0) {
        printf(">> Danh sach trong, chua co sinh vien nao.\n");
        return;
    }
    
    printf("\nTong so sinh vien: %d\n\n", studentCount);
    printf("%-25s | %-10s | %-12s | %-4s | %-4s | %-4s | %-4s | %-5s\n",
           "Ho Ten", "MSSV", "Ngay Sinh", "PRF", "MAE", "CSI", "CEA", "GPA");
    printf("---------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < studentCount; i++) {
        printf("%-25s | %-10s | %-12s | %4.2f | %4.2f | %4.2f | %4.2f | %5.2f\n",
               studentList[i].fullName, studentList[i].rollNumber, studentList[i].birthday,
               studentList[i].prfMark, studentList[i].maeMark, studentList[i].csiMark,
               studentList[i].ceaMark, studentList[i].gpa);
    }
    printf("========================================\n");
}

void calculate_gpa() {
    if (studentCount == 0) {
        printf("\n[THONG BAO] Chua co sinh vien nao de tinh GPA.\n");
        return;
    }
    
    for (int i = 0; i < studentCount; i++) {
        studentList[i].gpa = (studentList[i].prfMark + studentList[i].maeMark + 
                              studentList[i].csiMark + studentList[i].ceaMark) / 4.0;
    }
    
    printf("\n========================================\n");
    printf(">> DA TINH TOAN GPA CHO TAT CA! <<\n");
    printf("========================================\n");
    display_all_students();
}

void search_student() {
    char keyword[100];
    char input[100];
    int found = 0;

    if (studentCount == 0) {
        printf("\n[THONG BAO] Danh sach rong, khong the tim kiem.\n");
        return;
    }

    printf("\n========================================\n");
    printf("         TIM KIEM SINH VIEN\n");
    printf("========================================\n");
    
    while (1) {
        printf("Nhap MSSV hoac Ho Ten: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("[LOI] Khong doc duoc du lieu. Thu lai.\n");
            continue;
        }
        
        trim_string(input);
        
        if (strlen(input) == 0) {
            printf("[LOI] Tu khoa khong duoc de trong!\n");
            continue;
        }
        
        strcpy(keyword, input);
        break;
    }

    // Convert keyword to lowercase for case-insensitive search
    char lowerKeyword[100];
    strcpy(lowerKeyword, keyword);
    for (int j = 0; lowerKeyword[j]; j++) {
        lowerKeyword[j] = tolower(lowerKeyword[j]);
    }

    printf("\n>> Ket qua tim kiem cho '%s':\n\n", keyword);
    printf("%-25s | %-10s | %-5s\n", "Ho Ten", "MSSV", "GPA");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < studentCount; i++) {
        char lowerName[MAX_NAME_LENGTH];
        char lowerRoll[STUDENT_ID_LENGTH + 1];
        
        strcpy(lowerName, studentList[i].fullName);
        strcpy(lowerRoll, studentList[i].rollNumber);
        
        for (int j = 0; lowerName[j]; j++) lowerName[j] = tolower(lowerName[j]);
        for (int j = 0; lowerRoll[j]; j++) lowerRoll[j] = tolower(lowerRoll[j]);

        if (strstr(lowerRoll, lowerKeyword) != NULL || strstr(lowerName, lowerKeyword) != NULL) {
            printf("%-25s | %-10s | %5.2f\n",
                   studentList[i].fullName, studentList[i].rollNumber, studentList[i].gpa);
            found = 1;
        }
    }

    if (!found) {
        printf(">> Khong tim thay sinh vien nao phu hop.\n");
    }
    printf("========================================\n");
}

void sort_students() {
    if (studentCount == 0) {
        printf("\n[THONG BAO] Danh sach rong, khong co gi de sap xep.\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("         SAP XEP DANH SACH\n");
    printf("========================================\n");
    printf("1. Sap xep theo GPA (cao -> thap)\n");
    printf("2. Sap xep theo Ten (A-Z)\n");
    printf("========================================\n");
    
    int choice = get_valid_choice(1, 2);

    Student temp;

    if (choice == 1) {
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - i - 1; j++) {
                if (studentList[j].gpa < studentList[j + 1].gpa) {
                    temp = studentList[j];
                    studentList[j] = studentList[j + 1];
                    studentList[j + 1] = temp;
                }
            }
        }
        printf("\n>> Da sap xep theo GPA (giam dan). <<\n");
    } else {
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - i - 1; j++) {
                if (strcmp(studentList[j].fullName, studentList[j + 1].fullName) > 0) {
                    temp = studentList[j];
                    studentList[j] = studentList[j + 1];
                    studentList[j + 1] = temp;
                }
            }
        }
        printf("\n>> Da sap xep theo Ten (A-Z). <<\n");
    }

    display_all_students();
}

void save_to_file() {
    FILE *f = fopen(FILENAME, "wb");
    if (f == NULL) {
        printf("\n[LOI] Khong mo duoc file de luu!\n");
        return;
    }
    
    if (studentCount > 0) {
        size_t written = fwrite(studentList, sizeof(Student), studentCount, f);
        if (written != studentCount) {
            printf("\n[LOI] Luu file khong hoan tat!\n");
            fclose(f);
            return;
        }
    }

    fclose(f);
    printf("\n>> Du lieu da duoc luu vao file '%s' <<\n", FILENAME);
}

void load_from_file() {
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) {
        printf("\n[THONG BAO] Khong tim thay file du lieu. Bat dau voi danh sach moi.\n");
        return;
    }

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    rewind(f);

    if (fileSize > 0 && fileSize % sizeof(Student) == 0) {
        studentCount = fileSize / sizeof(Student);
        
        studentList = (Student*) malloc(fileSize);
        if (studentList == NULL) {
            printf("\n[LOI NGHIEM TRONG] Het bo nho khi tai file!\n");
            studentCount = 0;
            fclose(f);
            return;
        }
        
        size_t read = fread(studentList, sizeof(Student), studentCount, f);
        if (read != studentCount) {
            printf("\n[LOI] Doc file khong hoan tat!\n");
            free_memory();
            fclose(f);
            return;
        }
        
        printf("\n>> Da tai thanh cong %d sinh vien tu file '%s' <<\n", studentCount, FILENAME);
    } else {
        printf("\n[LOI] File du lieu bi loi hoac trong!\n");
    }

    fclose(f);
}