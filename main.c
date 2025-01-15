#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h> 

#define MAX_USERS 100
#define MAX_BUSES 6
#define MAX_SEATS 36 

typedef struct {
    char username[50];
    char password[50];
} User;

typedef struct {
    char busName[50];
    char route[100];
    char morningTime[20];
    char nightTime[20];
    int availableSeats;
    bool seats[MAX_SEATS];  
} Bus;

User users[MAX_USERS];
Bus buses[MAX_BUSES];
int userCount = 0;
int busCount = 0;
int userTicketCount[MAX_USERS] = {0};  

void initializeSeats(Bus *bus) {
    bus->availableSeats = MAX_SEATS;
    for (int i = 0; i < MAX_SEATS; i++) {
        bus->seats[i] = false; 
    }
}

void loadBuses() {
    FILE *file = fopen("buses.dat", "r");
    if (file == NULL) {
        strcpy(buses[0].busName, "Hanif");
        strcpy(buses[0].route, "Mymensingh <-> Comilla");
        strcpy(buses[0].morningTime, "09:00 am");
        strcpy(buses[0].nightTime, "08:00 pm");
        initializeSeats(&buses[0]);

        strcpy(buses[1].busName, "Nabil");
        strcpy(buses[1].route, "Barisal <-> Dhaka <-> Rajshahi");
        strcpy(buses[1].morningTime, "10:00 am");
        strcpy(buses[1].nightTime, "11:00 pm");
        initializeSeats(&buses[1]);

        strcpy(buses[2].busName, "SR");
        strcpy(buses[2].route, "Dhaka <-> Dinajpur");
        strcpy(buses[2].morningTime, "10:00 am");
        strcpy(buses[2].nightTime, "08:30 pm");
        initializeSeats(&buses[2]);

        strcpy(buses[3].busName, "Saint Martin");
        strcpy(buses[3].route, "Dhaka <-> Cox's Bazar");
        strcpy(buses[3].morningTime, "10:00 am");
        strcpy(buses[3].nightTime, "08:00 pm");
        initializeSeats(&buses[3]);

        strcpy(buses[4].busName, "Green Line");
        strcpy(buses[4].route, "Teknaf <-> Dhaka <-> Tetulia");
        strcpy(buses[4].morningTime, "09:00 am");
        strcpy(buses[4].nightTime, "09:00 pm");
        initializeSeats(&buses[4]);

        strcpy(buses[5].busName, "Shamoli");
        strcpy(buses[5].route, "Sylhet <-> Dhaka <-> Khulna");
        strcpy(buses[5].morningTime, "07:30 am");
        strcpy(buses[5].nightTime, "09:00 pm");
        initializeSeats(&buses[5]);

        busCount = 6;  
        return;
    }

    while (fscanf(file, "%49s %99s %19s %19s %d\n", buses[busCount].busName, buses[busCount].route, buses[busCount].morningTime, buses[busCount].nightTime, &buses[busCount].availableSeats) != EOF) {
        for (int i = 0; i < MAX_SEATS; i++) {
            int seatStatus;
            fscanf(file, "%d", &seatStatus);
            buses[busCount].seats[i] = seatStatus == 1; 
        }
        busCount++;
    }
    fclose(file);
}

void saveBuses() {
    FILE *file = fopen("buses.dat", "w");
    if (file == NULL) {
        printf("Error opening buses file for writing!\n");
        return;
    }

    for (int i = 0; i < busCount; i++) {
        fprintf(file, "%s %s %s %s %d\n", buses[i].busName, buses[i].route, buses[i].morningTime, buses[i].nightTime, buses[i].availableSeats);
        for (int j = 0; j < MAX_SEATS; j++) {
            fprintf(file, "%d ", buses[i].seats[j] ? 1 : 0); 
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void loadUsers() {
    FILE *file = fopen("users.dat", "r");
    if (file == NULL) {
        return; 
    }

    while (fscanf(file, "%49s %49s\n", users[userCount].username, users[userCount].password) != EOF) {
        userCount++;
    }
    fclose(file);
}

void saveUsers() {
    FILE *file = fopen("users.dat", "w");
    if (file == NULL) {
        printf("Error opening users file for writing!\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }

    fclose(file);
}

void readPassword(char *password) {
    char ch;
    int index = 0;
    while ((ch = _getch()) != '\r') { 
        if (ch == 8) { 
            if (index > 0) {
                printf("\b \b");
                index--;
            }
        } else {
            password[index++] = ch;
            printf("*");
        }
    }
    password[index] = '\0';
    printf("\n");
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }
    printf("Enter username: ");
    scanf("%s", users[userCount].username);
    printf("Enter password: ");
    readPassword(users[userCount].password); 
    userCount++;
    saveUsers();  
    printf("\nUser registered successfully!\n");
}

int loginUser() {
    char username[50], password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    readPassword(password); 

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i; 
        }
    }
    printf("\nInvalid credentials!\n");
    return -1;
}

void displaySeatMap(Bus *bus) {
    printf("\nSeat Map (X = Booked, O = Available):\n");
    char rows[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 4; j++) {
            int seatIndex = i * 4 + j;
            printf("%c%d [%c]  ", rows[j], i + 1, bus->seats[seatIndex] ? 'X' : 'O');
        }
        printf("\n");
    }
}

void searchBuses() {
    printf("----------------------------\n");
    printf("     AVAILABLE BUSES\n");
    printf("----------------------------\n");
    for (int i = 0; i < busCount; i++) {
        printf("\nBus Name: %s\n", buses[i].busName);
        printf("Route: %s\n", buses[i].route);
        printf("Morning Time: %s\n", buses[i].morningTime);
        printf("Night Time: %s\n", buses[i].nightTime);
        printf("Available Seats: %d\n", buses[i].availableSeats);
        printf("----------------------------\n");
    }
}

int seatToIndex(char row, int seatNumber) {
    int rowIndex = row - 'A';
    int colIndex = seatNumber - 1;
    return rowIndex * 9 + colIndex;
}

void bookTicket(int userIndex) {
    if (userTicketCount[userIndex] >= 1) {
        printf("\nYou have already booked a ticket. You can only book one ticket.\n");
        return;
    }

    char busName[50];
    printf("Enter bus name to book: ");
    scanf("%s", busName);

    for (int i = 0; i < busCount; i++) {
        if (strcmp(buses[i].busName, busName) == 0) {
                        displaySeatMap(&buses[i]);  

            int seatCount;
            printf("Enter the number of seats you want to book: ");
            scanf("%d", &seatCount);

            if (seatCount > buses[i].availableSeats) {
                printf("Invalid number of seats. Available seats: %d\n", buses[i].availableSeats);
                return;
            }

            char seat[3];
            int bookedSeats = 0;
            while (bookedSeats < seatCount) {
                printf("Enter seat %d (e.g., A1, B2): ", bookedSeats + 1);
                scanf("%2s", seat);

                if (strlen(seat) != 2 || seat[0] < 'A' || seat[0] > 'D' || seat[1] < '1' || seat[1] > '9') {
                    printf("Invalid seat input! Please enter a valid seat (e.g., A1, B2).\n");
                    continue;
                }

                char row = seat[0];
                int seatNumber = seat[1] - '0';

                int seatIndex = seatToIndex(row, seatNumber);

                if (buses[i].seats[seatIndex]) {
                    printf("Seat %s is already booked.\n", seat);
                } else {
                    buses[i].seats[seatIndex] = true;
                    buses[i].availableSeats--;
                    bookedSeats++;
                    printf("Seat %s booked successfully!\n", seat);
                }
            }

            userTicketCount[userIndex] += bookedSeats;
            printf("%d seats booked successfully!\n", bookedSeats);

            saveBuses();
            return;
        }
    }
    printf("Bus not found!\n");
}

void cancelTicket(int userIndex) {
    if (userTicketCount[userIndex] == 0) {
        printf("\nYou have no tickets to cancel.\n");
        return;
    }

    char busName[50];
    printf("Enter bus name for ticket cancellation: ");
    scanf("%s", busName);

    for (int i = 0; i < busCount; i++) {
        if (strcmp(buses[i].busName, busName) == 0) {
            displaySeatMap(&buses[i]);

            char seat[3];
            printf("Enter seat to cancel (e.g., A1, B2): ");
            scanf("%2s", seat);

            if (strlen(seat) != 2 || seat[0] < 'A' || seat[0] > 'D' || seat[1] < '1' || seat[1] > '9') {
                printf("Invalid seat input! Please enter a valid seat (e.g., A1, B2).\n");
                return;
            }

            char row = seat[0];
            int seatNumber = seat[1] - '0';

            int seatIndex = seatToIndex(row, seatNumber);

            if (!buses[i].seats[seatIndex]) {
                printf("Seat %s is not booked.\n", seat);
                return;
            }

            buses[i].seats[seatIndex] = false;
            buses[i].availableSeats++;
            userTicketCount[userIndex]--;
            printf("Seat %s canceled successfully!\n", seat);

            saveBuses();
            return;
        }
    }
    printf("Bus not found!\n");
}

void displayWelcomeMessage() {
    printf("==============================================\n");
    printf("   WELCOME TO THE BUS RESERVATION SYSTEM\n");
    printf("==============================================\n");
}

int main() {
    int choice, loggedInUser;

    displayWelcomeMessage();

    loadUsers();
    loadBuses();

    while (1) {
        printf("\n1. Register\n2. Login\n3. Search Buses\n4. Exit\nChoose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loggedInUser = loginUser();
                if (loggedInUser != -1) {
                    while (1) {
                        printf("\nUser Menu:\n1. Search Buses\n2. Book Ticket\n3. Cancel Ticket\n4. Logout\nChoose an option: ");
                        scanf("%d", &choice);

                        switch (choice) {
                            case 1:
                                searchBuses();
                                break;
                            case 2:
                                bookTicket(loggedInUser);
                                break;
                            case 3:
                                cancelTicket(loggedInUser);
                                break;
                            case 4:
                                loggedInUser = -1;
                                break;
                            default:
                                printf("Invalid option!\n");
                        }
                        if (loggedInUser == -1) break;
                    }
                }
                break;
            case 3:
                searchBuses();
                break;
            case 4:
                saveUsers();
                saveBuses();
                printf("Exiting... All data saved.\n");
                return 0;
            default:
                printf("Invalid option!\n");
        }
    }
    return 0;
}