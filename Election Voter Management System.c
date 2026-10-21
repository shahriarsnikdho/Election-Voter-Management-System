//Election Voter Management System by Snikdho

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADMIN_PASS 1234

struct Voter {
    int voterID;
    char name[50];
    int age;
    char area[30];
    int hasVoted;   // 0 = No, 1 = Yes
};

void registerVoter();
void viewVoters();
void castVote();
void areaWiseReport();
void adminLogin();

int main() {
    int choice;

    do {
        printf("\n Election Voter Management System \n");
        printf("1. Register Voter\n");
        printf("2. View Voter List\n");
        printf("3. Cast Vote\n");
        printf("4. Area-wise Vote Report (Admin)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: registerVoter(); break;
            case 2: viewVoters(); break;
            case 3: castVote(); break;
            case 4: adminLogin(); break;
            case 5: printf("System closed.\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 5);

    return 0;
}

// Register voter
void registerVoter() {
    struct Voter v;
    FILE *fp = fopen("voters.dat", "ab");

    if(fp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Voter ID: ");
    scanf("%d", &v.voterID);

    printf("Name: ");
    scanf(" %[^\n]", v.name);

    printf("Age: ");
    scanf("%d", &v.age);

    if(v.age < 18) {
        printf(" Voter must be 18+\n");
        fclose(fp);
        return;
    }

    printf("Area: ");
    scanf(" %[^\n]", v.area);

    v.hasVoted = 0;

    fwrite(&v, sizeof(v), 1, fp);
    fclose(fp);

    printf(" Voter registered successfully!\n");
}

// View voters
void viewVoters() {
    struct Voter v;
    FILE *fp = fopen("voters.dat", "rb");

    if(fp == NULL) {
        printf("No data found.\n");
        return;
    }

    printf("\n ID\tName\t\tArea\t\tStatus\ n");
    printf("---------------------------------------------\n");

    while(fread(&v, sizeof(v), 1, fp)) {
        printf("%d\t%s\t\t%s\t\t%s\n",
               v.voterID, v.name, v.area,
               v.hasVoted ? "Voted" : "Not Voted");
    }

    fclose(fp);
}

// Cast vote
void castVote() {
    struct Voter v;
    int id, found = 0;

    FILE *fp = fopen("voters.dat", "rb+");

    if(fp == NULL) {
        printf("No data found.\n");
        return;
    }

    printf("Enter Voter ID: ");
    scanf("%d", &id);

    while(fread(&v, sizeof(v), 1, fp)) {
        if(v.voterID == id) {
            found = 1;
            if(v.hasVoted) {
                printf(" Vote already cast!\n");
            } else {
                v.hasVoted = 1;
                fseek(fp, -sizeof(v), SEEK_CUR);
                fwrite(&v, sizeof(v), 1, fp);
                printf(" Vote successful!\n");
            }
            break;
        }
    }

    if(!found)
        printf(" Voter not found!\n");

    fclose(fp);
}

// Admin login
void adminLogin() {
    int pass;
    printf("Enter admin password: ");
    scanf("%d", &pass);

    if(pass == ADMIN_PASS) {
        areaWiseReport();
    } else {
        printf(" Wrong password!\n");
    }
}

// Area wise report
void areaWiseReport() {
    struct Voter v;
    FILE *fp = fopen("voters.dat", "rb");

    if(fp == NULL) {
        printf("No data found.\n");
        return;
    }

    int total = 0, voted = 0;

    while(fread(&v, sizeof(v), 1, fp)) {
        total++;
        if(v.hasVoted)
            voted++;
    }

    fclose(fp);

    printf("\n Election Summary \n");
    printf("Total Registered Voters: %d\n", total);
    printf("Total Votes Cast: %d\n", voted);
    printf("Voter Turnout: %.2f%%\n",
           total == 0 ? 0 : (voted * 100.0) / total);
}

