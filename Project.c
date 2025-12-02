#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

struct Employee {
    char empId[20];     // ma nhan vien (duy nhat)
    char name[50];      // ten nhan vien
    char position[15];  // chuc vu
    double baseSalary;  // luong co ban
    int workDays;       // so ngay cong
};

struct TimeSheet{
    char logId[20];     // ma nhat ki cham cong
    char empId[20];     // ma nhan vien
    char date[20];      // ngay thang "DD/MM/YYYY"
    char status[10];    // trang thai "Di lam" / "Nghi"
};

struct Employee employees[100];
int empCount = 0;

struct TimeSheet timesheets[500];
int logCount = 0;

//================== NGUYEN MAU HAM =====================
bool isInteger(const char *str);    
bool hasDigit(const char *str);     
int findEmployeeIndex(char empId[]); 
bool isEmptyString(char str[]);      
void removeNewline(char str[]);      
void normalizeDate(char *dest, const char *src); 
bool isLeapYear(int year);
bool isValidDate(const char *date);  
int printMenu();                     
void addEmployee();                  
void updateEmployee();               
void deleteEmployee();               
void displayEmployees();          
void searchEmployeeByName();         
void sortEmployees();                
void timeKeeping();                  
void viewTimeSheet();                

//---------------- HAM HO TRO -----------------------
bool isInteger(const char *str) {
    if (str[0] == '\0') return false;
    for (int i = 0; str[i]; i++) {
        if (str[i] < '0' || str[i] > '9')
            return false; 
    }
    return true;
};


bool hasDigit(const char *str) {
	if (str[0] == '\0') return false;
    for (int i = 0; str[i]; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            return true; 
        }
    }
    return false; 
};

void removeNewline(char str[]) {
    str[strcspn(str, "\n")] = 0;
};

bool isEmptyString(char str[]) {
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            return false;
    }
    return true;
};

int findEmployeeIndex(char empId[]) {
    for (int i = 0; i < empCount; i++) {
        if (strcmp(employees[i].empId, empId) == 0)
            return i;
    }
    return -1;
};

void normalizeDate(char *dest, const char *src) {
    int d, m, y;
    if (sscanf(src, "%d/%d/%d", &d, &m, &y) == 3) {
        sprintf(dest, "%02d/%02d/%04d", d, m, y);
    } else {
        strcpy(dest, src); 
    }
};

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
};

bool isValidDate(const char *date) {
    int d, m, y;
    if (sscanf(date, "%d/%d/%d", &d, &m, &y) != 3)
        return false;

    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;

    int maxDay;
    switch(m) {
        case 2:
            maxDay = isLeapYear(y) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            maxDay = 30;
            break;
        default:
            maxDay = 31;
    }
    return d >= 1 && d <= maxDay;
};

// ====================== MENU =========================
int printMenu() {
    char buffer[100];
    int choice = -1;

    printf("\n+----------- \x1b[34mQUAN LY NHAN VIEN & CHAM CONG\x1b[0m ------------+\n");
    printf("|%-54s|\n","1. Them nhan vien moi.");
    printf("|%-54s|\n","2. Cap nhat ho so nhan vien.");
    printf("|%-54s|\n","3. Sa thai nhan vien.");
    printf("|%-54s|\n","4. Hien thi danh sach.");
    printf("|%-54s|\n","5. Tra cuu theo ten.");
    printf("|%-54s|\n","6. Sap xep danh sach theo luong.");
    printf("|%-54s|\n","7. Cham cong.");
    printf("|%-54s|\n","8. Xem bang cong ca nhan.");
    printf("|%-54s|\n","0. Thoat.");
    printf("+------------------------------------------------------+\n");
    printf("Nhap lua chon cua ban: ");

    fgets(buffer, sizeof(buffer), stdin);
    removeNewline(buffer);

	if (isInteger(buffer)) {
	    choice = atoi(buffer);
	} else {
	    choice = -1;
	}
    return choice;
};
// -------------------------------- F01: THEM NHAN VIEN MOI ------------------------------------
void addEmployee() {
    if (empCount >= 100) {
        printf("\x1b[31m>> Danh sach NV da day. Khong the them nua!\x1b[0m\n");
        return;
    }
    struct Employee e;
    char buffer[50];
    do {
        printf("Nhap ma NV: ");
        fgets(e.empId, sizeof(e.empId), stdin);
        removeNewline(e.empId);
        if (strlen(e.empId) == 0) {
            printf("\x1b[31m>> Ma nhan vien khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
        } else if (findEmployeeIndex(e.empId) != -1) {
            printf("\x1b[31m>> Ma NV da bi trung! Vui long nhap lai.\x1b[0m\n");
            strcpy(e.empId, "");
        }
    } while (strlen(e.empId) == 0);
    
    do {
        printf("Nhap ten NV: ");
        fgets(e.name, sizeof(e.name), stdin);
        removeNewline(e.name);
        if (strlen(e.name) == 0) {
            printf("\x1b[31m>> Ten khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
        } else if (hasDigit(e.name)) {
            printf("\x1b[31m>> Ten khong duoc chua SO! Vui long nhap lai.\x1b[0m\n");
            strcpy(e.name, "");
        }
    } while (strlen(e.name) == 0);
    
    do {
        printf("Nhap chuc vu: ");
        fgets(e.position, sizeof(e.position), stdin);
        removeNewline(e.position);

        if (strlen(e.position) == 0) {
            printf("\x1b[31m>> Chuc vu khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
        } else if (hasDigit(e.position)) {
            printf("\x1b[31m>> Chuc vu khong duoc nhap chu so! Vui long nhap lai.\x1b[0m\n");
            strcpy(e.position, "");
        }
    } while (strlen(e.position) == 0);
    
   while (1) {
	    printf("Nhap luong co ban: ");
	    if (fgets(buffer, sizeof(buffer), stdin) == NULL){
	        continue;
	    }       
	    if (isEmptyString(buffer)){
	        printf(">> Vui long nhap lai khong duoc de trong!\n");
	        continue;	
		} 
	    if (sscanf(buffer, "%lf", &e.baseSalary) != 1) {
	        printf(">> Loi: Vui long nhap so hop le!\n");
	        continue;
	    }
	    if (e.baseSalary <= 0) {
	        printf(">> Luong co ban phai lon hon 0!\n");
	        continue;
	    }
	    break; 
    }
    e.workDays = 0;

    employees[empCount++] = e;
    printf("\x1b[32m>> Them nhan vien thanh cong!\x1b[0m\n");
};

// ---------------------------------- F02: CAP NHAT HO SO NHAN VIEN ------------------------------------
void updateEmployee() {
    char id[20];
    int idx;
    char buffer[100];
    do {
        printf("Nhap ma NV can cap nhat: ");
        fgets(id, sizeof(id), stdin);
        removeNewline(id);
        if (strlen(id) == 0) {
            printf("\x1b[31m>> Ma nhan vien khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
            break; 
        }
        idx = findEmployeeIndex(id);
        if (idx == -1) {
            printf("\x1b[31m>> Khong tim thay nhan vien co ma [%s]! Vui long nhap lai.\x1b[0m\n", id);
            continue; 
        }
    } while (strlen(id) == 0 || idx == -1);

    char newPos[20];
    do {
        printf("Nhap chuc vu: ");
        fgets(newPos, sizeof(newPos), stdin);                                        
        removeNewline(newPos);

        if (strlen(newPos) == 0) {
            printf("\x1b[31m>> Chuc vu khong duoc de trong! Vui long nhap lai.\x1b[0m\n"); 
        } else if (hasDigit(newPos)) {
            printf("\x1b[31m>> Chuc vu khong duoc chua SO! Vui long nhap lai.\x1b[0m\n");
            strcpy(newPos, "");
        } 
    } while (strlen(newPos) == 0);

    double newSalary;
    while (1) {
	    printf("Nhap luong co ban: ");
	    if (fgets(buffer, sizeof(buffer), stdin) == NULL){
	        continue;
	    }       
	    if (isEmptyString(buffer)){
	        printf(">> Vui long nhap lai khong duoc de trong!\n");
	        continue;	
		} 
	    if (sscanf(buffer, "%lf", &newSalary) != 1) {
	        printf(">> Loi: Vui long nhap so hop le!\n");
	        continue;
	    }
	    if (newSalary  <= 0) {
	        printf(">> Luong co ban phai lon hon 0!\n");
	        continue;
	    }
	    break; 
    }
    strcpy(employees[idx].position, newPos);
    employees[idx].baseSalary = newSalary;
    printf("\x1b[32m>> Cap nhat thanh cong!!!\x1b[0m\n");
}

// -------------------------------------- F03: XOA NHAN VIEN  --------------------------------------------
void deleteEmployee() {
    char id[20];
    int idx;
    char choice;
    char buffer[20];
    
    while (1) {
        printf("Nhap ma nhan vien can xoa: ");
        fgets(id, sizeof(id), stdin);
        removeNewline(id);
        if (strlen(id) == 0) {
            printf("\x1b[31m>> Ma nhan vien khong duoc de trong!!! Nhap lai.\x1b[0m\n");
            continue;
        }

        idx = findEmployeeIndex(id);
        if (idx == -1) {
            printf("\x1b[31m>> Khong tim thay nhan vien co ma [%s].\x1b[0m\n", id);
            return;
        }

        while (1) {
            printf("Ban co chac chan muon xoa nhan vien [%s]? (Y/N): ", id);
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);
            if (strlen(buffer) == 0) {
                printf("\x1b[31m>> Lua chon khong duoc de trong! Hay nhap Y hoac N.\x1b[0m\n");
                continue;
            }
            if (strlen(buffer) != 1) {
                printf("\x1b[31m>>Loi: lua chon khong hop le! Hay nhap Y hoac N.\x1b[0m\n");
                continue;
            }
            choice = buffer[0];
            if (choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n') {
                break;
            } else {
            	printf("\x1b[31m>> Lua chon khong hop le! Hay nhap Y hoac N.\x1b[0m\n");
			} 
        }
        if (choice == 'N' || choice == 'n') {
            printf("\x1b[35m}}} Huy thao tac xoa nhan vien!\x1b[0m\n");
            return;
        }
        for (int i = idx; i < empCount - 1; i++){
        	employees[i] = employees[i + 1];
		}
        empCount--;
        printf("\x1b[32m>> Xoa nhan vien [%s] thanh cong.\x1b[0m\n", id);
        return;
    }
};

//----------------------------------------- F04: BANG NHAN VIEN --------------------------------------------- 
void displayEmployees() {
    if (empCount == 0) {
        printf("\x1b[31m>> Danh sach nhan vien hien dang rong.\x1b[0m\n");
        return;
    }
    int pageSize = 4; 
    int totalPages = empCount / pageSize;
    if (empCount % pageSize != 0) totalPages++;

    char input[20];
    while (1) {
        printf("\x1b[36mDanh sach co %d trang. Nhap so trang muon xem (1-%d) hoac 0 de thoat:\x1b[0m ", totalPages, totalPages);
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        removeNewline(input);
        if (strlen(input) == 0) {
            printf("\x1b[31m>> Khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
            continue;
        }
        
        int page;
        char extra;
        if (sscanf(input, "%d %c", &page, &extra) != 1) {
            printf("\x1b[31m>> Vui long nhap mot so nguyen hop le!\x1b[0m\n");
            continue;
        }

        if (page == 0) break; 
        if (page < 1 || page > totalPages) {
            printf("\x1b[31m>> So trang nhap khong hop le! Vui long nhap lai.\x1b[0m\n");
            continue;
        }

        int start = (page - 1) * pageSize;
        int end = start + pageSize;
        if (end > empCount) end = empCount;

        printf("\nDanh sach trang %d/%d:\n", page, totalPages);
        printf("+----------+--------------------+--------------------+--------------------+--------------------+\n");
        printf("|%-10s|%-20s|%-20s|%-20s|%-20s|\n", "Ma NV", "Ten NV", "Chuc vu", "Luong co ban", "So ngay cong");
        printf("+----------+--------------------+--------------------+--------------------+--------------------+\n");
        for (int i = start; i < end; i++) {
    	    printf("|%-10s|%-20s|%-20s|%-20.0lf|%-20d|\n", employees[i].empId, employees[i].name, employees[i].position, employees[i].baseSalary, employees[i].workDays);
		}
        printf("+----------+--------------------+--------------------+--------------------+--------------------+\n");
        printf("\x1b[36m>> Tong so nhan vien hien tai: %d\x1b[0m\n\n", empCount);
    }
};

//---------------------------------F05: TRA CUU THEO TEN NHAN VIEN ---------------------------------- 
void searchEmployeeByName() {
    if (empCount == 0) {
        printf("\x1b[31m>> Danh sach nhan vien hien dang rong.\x1b[0m\n");
        return;
    }

    char keyword[50];
    while (1) {
        printf("Nhap ten hoac mot phan cua ten nhan vien (khong dau): ");
        fgets(keyword, sizeof(keyword), stdin);
        removeNewline(keyword);

        if (strlen(keyword) == 0) {
            printf("\x1b[31m>> Khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
            continue;
        }
        
        bool hasAccent = false;
        for (int i = 0; keyword[i]; i++) {
            char c = keyword[i];
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ')) {
                hasAccent = true;
                break;
            }
        }
        if (hasAccent) {
            printf("\x1b[31m>> Loi: Khong duoc nhap chu co dau! Vui long nhap lai.\x1b[0m\n");
            continue;
        }
        break;
    }
   
    for (int i = 0; keyword[i]; i++) {
        if (keyword[i] >= 'A' && keyword[i] <= 'Z') {
            keyword[i] += 32;
        }
    }
    printf("+----------+--------------------+--------------------+--------------------+--------------------+\n");
    printf("|%-10s|%-20s|%-20s|%-20s|%-20s|\n", "Ma NV", "Ten NV", "Chuc vu", "Luong co ban", "So ngay cong");
    printf("+----------+--------------------+--------------------+--------------------+--------------------+\n");

    int found = 0;
    for (int i = 0; i < empCount; i++) {
        char lowerName[50];
        strcpy(lowerName, employees[i].name);

        for (int j = 0; lowerName[j]; j++) {
            if (lowerName[j] >= 'A' && lowerName[j] <= 'Z') {
                lowerName[j] += 32;
            }
        }
        if (strstr(lowerName, keyword) != NULL) {
            printf("|%-10s|%-20s|%-20s|%-20.0lf|%-20d|\n",
                employees[i].empId, employees[i].name, employees[i].position,
                employees[i].baseSalary, employees[i].workDays);
            found++;
        }
    }
    printf("+----------+--------------------+--------------------+--------------------+--------------------+\n");

    if (found == 0) {
        printf("\x1b[31m>> Khong tim thay nhan vien nao!\x1b[0m\n");
    } else {
        printf("\x1b[32m>> Tim thay %d nhan vien.\x1b[0m\n", found);
    }
};

// ---------------------- F06: SAP XEP DANH SACH NHAN VIEN THEO LUONG CO BAN -------------------------
void sortEmployees() {
    if (empCount == 0) {
        printf("\x1b[31m>> Danh sach nhan vien hien dang trong.\x1b[0m\n");
        return;
    }
    int order;
    char buffer[20];
    long num;
    char extra; 
    while (1) {
        printf("Chon thu tu sap xep theo Luong co ban:\n");
        printf("1. Tang dan\n");
        printf("2. Giam dan\n");
        printf("Nhap lua chon (1-2): ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        removeNewline(buffer);

        if (strlen(buffer) == 0) {
            printf("\x1b[31m>> Lua chon khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
            continue;
        }
        
        if (sscanf(buffer, "%ld %c", &num, &extra) != 1) {
            printf("\x1b[31m>> Lua chon khong hop le! Vui long nhap 1 hoac 2.\x1b[0m\n");
            continue;
        }

        if (num != 1 && num != 2) {
            printf("\x1b[31m>> Lua chon khong hop le! Vui long nhap 1 hoac 2.\x1b[0m\n");
            continue;
        }
        order = (int)num;
        break;
    }

    for (int i = 0; i < empCount - 1; i++) {
	    for (int j = 0; j < empCount - i - 1; j++) {
	        int needSwap = (order == 1) ? (employees[j].baseSalary > employees[j + 1].baseSalary) : (employees[j].baseSalary < employees[j + 1].baseSalary);   
	        if (needSwap) {
	            struct Employee temp = employees[j];
	            employees[j] = employees[j + 1];
	            employees[j + 1] = temp;
	        }
	    }
	}
	printf("\x1b[32m>> Sap xep danh sach nhan vien theo luong thanh cong!\x1b[0m\n");
    printf("\n+----------+--------------------+--------------------+--------------------+--------------------+\n");
    printf("|%-10s|%-20s|%-20s|%-20s|%-20s|\n", "Ma NV", "Ten NV", "Chuc vu", "Luong co ban", "So ngay cong");
    printf("+----------+--------------------+--------------------+--------------------+--------------------+\n");
    for (int i = 0; i < empCount; i++) {
        printf("|%-10s|%-20s|%-20s|%-20.0lf|%-20d|\n", employees[i].empId, employees[i].name, employees[i].position, employees[i].baseSalary, employees[i].workDays);
    }
    printf("+----------+--------------------+--------------------+--------------------+--------------------+\n");
};
 
// -------------------------------------- F07: CHAM NGAY CONG CUA NHAN VIEN -------------------------------------
void timeKeeping() {
    char empId[20];
    char date[20];
    char normDate[20];

    int idx;
    while (1) {
        printf("Nhap ma nhan vien can cham cong: ");
        fgets(empId, sizeof(empId), stdin);
        removeNewline(empId);

        if (strlen(empId) == 0) {
            printf("\x1b[31m>> Ma NV khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
            continue;
        }

        idx = findEmployeeIndex(empId);
        if (idx == -1) {
            printf("\x1b[31m>> Khong ton tai nhan vien co ma [%s].\x1b[0m\n", empId);
            return;
        }
        break;
    }

    while (1) {
        printf("Nhap ngay (DD/MM/YYYY): ");
        fgets(date, sizeof(date), stdin);
        removeNewline(date);

        if (strlen(date) == 0) {
            printf("\x1b[31m>> Ngay khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
            continue;
        }

        if (!isValidDate(date)) {
            int d, m, y;
            sscanf(date, "%d/%d/%d", &d, &m, &y);
            if (d == 29 && m == 2 && !isLeapYear(y)) {
                printf("\x1b[31m>> Nam %d khong phai nam nhuan. 29/02 khong hop le!\x1b[0m\n", y);
            } else {
                printf("\x1b[36m>> Ngay khong hop le! Dinh dang dung: DD/MM/YYYY. Vi du: 05/12/2025\x1b[0m\n");
            }
            continue;
        }

        normalizeDate(normDate, date);
        bool isDuplicate = false;
        for (int i = 0; i < logCount; i++) {
            if (strcmp(timesheets[i].empId, empId) == 0) {
                char existingNorm[20];
                normalizeDate(existingNorm, timesheets[i].date);
                if (strcmp(existingNorm, normDate) == 0) {
                    isDuplicate = true;
                    break;
                }
            }
        }

        if (isDuplicate) {
            printf("\x1b[31m>> Da cham cong cho nhan vien [%s] vao ngay %s! Vui long chon ngay khac.\x1b[0m\n", empId, normDate);
            continue;
        }
        break;
    }

    struct TimeSheet t;
    int logNum = 1;
    for (int i = 0; i < logCount; i++) {
        if (strcmp(timesheets[i].empId, empId) == 0) logNum++;
    }
    sprintf(t.logId, "%03d", logNum);

    strcpy(t.empId, empId);
    strcpy(t.date, normDate);
    strcpy(t.status, "Di lam");

    timesheets[logCount++] = t;
    employees[idx].workDays++;

    printf(GREEN ">> Cham cong thanh cong: [%s] - Ngay %s (Di lam)!\n" RESET, empId, normDate);
}

//--------------------------------- F08: XEM BANG CONG CA NHAN ------------------------------------
void viewTimeSheet() {
    if (logCount == 0) {
        printf("\x1b[31m>> Danh sach cham cong hien dang trong.\x1b[0m\n");
        return;
    }

    char empId[20];
    while (1) {
        printf("Nhap ma nhan vien can xem bang cong: ");
        fgets(empId, sizeof(empId), stdin);
        removeNewline(empId);

        if (strlen(empId) == 0) {
            printf("\x1b[31m>> Ma nhan vien khong duoc de trong! Vui long nhap lai.\x1b[0m\n");
            continue;
        }

        int empIndex = findEmployeeIndex(empId);
        if (empIndex == -1) {
            printf("\x1b[31m>> Khong ton tai nhan vien co ma [%s].\x1b[0m\n", empId);
            return;
        }
        break;
    }

    int month, year;
    char buffer[50];

    while (1) {
        printf("Nhap thang muon xem (1-12): ");
        fgets(buffer, sizeof(buffer), stdin);
        removeNewline(buffer);

        if (!isInteger(buffer)) {
            printf("\x1b[31m>> Thang khong hop le! Vui long nhap so nguyen 1-12.\x1b[0m\n");
            continue;
        }

        month = atoi(buffer);
        if (month < 1 || month > 12) {
            printf("\x1b[31m>> Thang khong hop le! Vui long nhap 1-12.\x1b[0m\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Nhap nam muon xem (>=1900): ");
        fgets(buffer, sizeof(buffer), stdin);
        removeNewline(buffer);

        if (!isInteger(buffer)) {
            printf("\x1b[31m>> Nam khong hop le! Vui long nhap so nguyen >=1900.\x1b[0m\n");
            continue;
        }

        year = atoi(buffer);
        if (year < 1900) {
            printf("\x1b[31m>> Nam khong hop le! Vui long nhap so nguyen >=1900.\x1b[0m\n");
            continue;
        }
        break;
    }
    struct TimeSheet temp[200];
    int count = 0;
    int d, m, y;

    for (int i = 0; i < logCount; i++) {
        if (strcmp(timesheets[i].empId, empId) == 0) {
            sscanf(timesheets[i].date, "%d/%d/%d", &d, &m, &y);
            if (m == month && y == year) {
                temp[count++] = timesheets[i];
            }
        }
    }
    if (count == 0) {
        printf("\x1b[33m>> Khong co du lieu cham cong cua nhan vien [%s] trong thang %02d/%d.\x1b[0m\n", empId, month, year);
        return;
    }

    long a, b;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            sscanf(temp[i].date, "%d/%d/%d", &d, &m, &y);
            a = (long)y * 10000 + m * 100 + d;

            sscanf(temp[j].date, "%d/%d/%d", &d, &m, &y);
            b = (long)y * 10000 + m * 100 + d;

            if (a > b) {
                struct TimeSheet t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }
    int empIndex = findEmployeeIndex(empId);

    printf("\n------\x1b[36m Thong tin nhan vien \x1b[0m------\n");
    printf("Ma NV       : %s\n", employees[empIndex].empId);
    printf("Ten NV      : %s\n", employees[empIndex].name);
    printf("Chuc vu     : %s\n", employees[empIndex].position);
    printf("Luong co ban: %.0f VND\n", employees[empIndex].baseSalary);
    printf("So ngay lam : %d\n", employees[empIndex].workDays);

    printf("\n\x1b[36mBANG CHAM CONG THANG %02d/%d CUA NV [%s]\x1b[0m\n", month, year, empId);
    printf("+---------+----------------+---------------+\n");
    printf("| ID Log  | Ngay/Thang/Nam | Trang thai    |\n");
    printf("+---------+----------------+---------------+\n");
    for (int i = 0; i < count; i++) {
        sscanf(temp[i].date, "%d/%d/%d", &d, &m, &y);
        printf("|%-9s| %02d/%02d/%04d     | %-14s|\n", temp[i].logId, d, m, y, temp[i].status);
    }
    printf("+---------+----------------+---------------+\n");
};

//-------------------------------- MAIN ---------------------------------------
int main() {
    struct Employee sampleEmployees[15] = {
        {"NV001", "Nguyen Van A 11", "Nhan vien", 5000000, 0},
        {"NV002", "Tran Thi B", "Ke toan", 6000000, 0},
        {"NV003", "Le Van C", "Truong phong", 8000000, 0},
        {"NV004", "Pham Thi D", "Nhan vien", 4500000, 0},
        {"NV005", "Hoang Van E", "Giam doc", 15000000, 0},
        {"NV006", "Dang Thi F", "Nhan vien", 4800000, 0},
        {"NV007", "Vu Van G", "Nhan vien", 4700000, 0},
        {"NV008", "Nguyen Thi H", "Ke toan", 6200000, 0},
        {"NV009", "Tran Van I", "Truong phong", 8500000, 0},
        {"NV010", "Le Thi J", "Nhan vien", 4600000, 0},
    };
    for (int i = 0; i < 10; i++) {
        employees[empCount++] = sampleEmployees[i];
    }
    int choice;
    do {
        choice = printMenu();
        switch (choice) {
            case 1: 
			    addEmployee(); 
				break;
            case 2: 
			    updateEmployee(); 
				break;
            case 3: 
			    deleteEmployee(); 
				break;
            case 4: 
			    displayEmployees(); 
				break;
			case 5:	
			    searchEmployeeByName();
			    break; 
			case 6:
				sortEmployees();
			    break;
			case 7:
				timeKeeping();
			    break;
			case 8:
				viewTimeSheet();
				break;	    
            case 0: 
			    printf("Thoat chuong trinh.\n"); 
				break;
            default: 
			    printf("\x1b[33m>> Lua chon khong hop le!\x1b[0m\n");
        }
    } while(choice != 0);

    return 0;
};
