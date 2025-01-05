#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures
typedef struct {
    int bus_id;
    char bus_name[50];
    char route[100];
    int total_seats;
    int available_seats;
    char departure_time[10];
    char arrival_time[10];
} Bus;

typedef struct {
    int user_id;
    char name[50];
    char email[50];
    char password[20];
} Passenger;

typedef struct {
    int reservation_id;
    int bus_id;
    int user_id;
    int seat_number;
    char reservation_date[20];
} Reservation;

// Function Prototypes
void adminMenu();
void passengerMenu(int user_id);
void addBus();
void viewBuses();
void bookSeat(int user_id);
void viewReservations(int user_id);
int authenticateUser(char *email, char *password);
void registerUser();
void saveBus(Bus bus);
void saveReservation(Reservation reservation);
int generateReservationID();
int generateUserID();
int isBusAvailable(int bus_id);
void updateBusAvailability(int bus_id, int seat_number, int increment);

// Main Function
int main() {
    int choice, user_id;
    char email[50], password[20];

    while (1) {
        printf("\n--- Bus Reservation System ---\n");
        printf("1. Admin Login\n");
        printf("2. Passenger Login\n");
        printf("3. Register as Passenger\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminMenu();
                break;
            case 2:
                printf("Enter Email: ");
                scanf("%s", email);
                printf("Enter Password: ");
                scanf("%s", password);
                user_id = authenticateUser(email, password);
                if (user_id != -1) {
                    passengerMenu(user_id);
                } else {
                    printf("Invalid credentials. Try again.\n");
                }
                break;
            case 3:
                registerUser();
                break;
            case 4:
                printf("Thank you for using the system.\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

// Admin Menu
void adminMenu() {
    int choice;

    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Bus\n");
        printf("2. View Buses\n");
        printf("3. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBus();
                break;
            case 2:
                viewBuses();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// Passenger Menu
void passengerMenu(int user_id) {
    int choice;

    while (1) {
        printf("\n--- Passenger Menu ---\n");
        printf("1. View Buses\n");
        printf("2. Book a Seat\n");
        printf("3. View My Reservations\n");
        printf("4. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBuses();
                break;
            case 2:
                bookSeat(user_id);
                break;
            case 3:
                viewReservations(user_id);
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// Add Bus
void addBus() {
    Bus bus;

    printf("Enter Bus ID: ");
    scanf("%d", &bus.bus_id);
    printf("Enter Bus Name: ");
    scanf("%s", bus.bus_name);
    printf("Enter Route: ");
    scanf("%s", bus.route);
    printf("Enter Total Seats: ");
    scanf("%d", &bus.total_seats);
    bus.available_seats = bus.total_seats;
    printf("Enter Departure Time: ");
    scanf("%s", bus.departure_time);
    printf("Enter Arrival Time: ");
    scanf("%s", bus.arrival_time);

    saveBus(bus);
    printf("Bus added successfully!\n");
}

// Save Bus to File
void saveBus(Bus bus) {
    FILE *file = fopen("buses.dat", "ab");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }
    fwrite(&bus, sizeof(Bus), 1, file);
    fclose(file);
}

// View Buses
void viewBuses() {
    FILE *file = fopen("buses.dat", "rb");
    if (!file) {
        printf("No buses available.\n");
        return;
    }

    Bus bus;
    printf("\n--- Available Buses ---\n");
    while (fread(&bus, sizeof(Bus), 1, file)) {
        printf("Bus ID: %d | Name: %s | Route: %s | Seats: %d/%d | Departure: %s | Arrival: %s\n",
               bus.bus_id, bus.bus_name, bus.route, bus.available_seats, bus.total_seats, bus.departure_time, bus.arrival_time);
    }
    fclose(file);
}

// Book Seat
void bookSeat(int user_id) {
    int bus_id, seat_number;
    char date[20];

    printf("Enter Bus ID: ");
    scanf("%d", &bus_id);
    if (!isBusAvailable(bus_id)) {
        printf("Invalid Bus ID or no available seats.\n");
        return;
    }
    printf("Enter Seat Number: ");
    scanf("%d", &seat_number);
    printf("Enter Reservation Date (DD-MM-YYYY): ");
    scanf("%s", date);

    Reservation reservation = {generateReservationID(), bus_id, user_id, seat_number, ""};
    strcpy(reservation.reservation_date, date);

    saveReservation(reservation);
    updateBusAvailability(bus_id, seat_number, -1);
    printf("Reservation successful! Reservation ID: %d\n", reservation.reservation_id);
}

// Save Reservation to File
void saveReservation(Reservation reservation) {
    FILE *file = fopen("reservations.dat", "ab");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }
    fwrite(&reservation, sizeof(Reservation), 1, file);
    fclose(file);
}

// View Reservations
void viewReservations(int user_id) {
    FILE *file = fopen("reservations.dat", "rb");
    if (!file) {
        printf("No reservations found.\n");
        return;
    }

    Reservation reservation;
    printf("\n--- My Reservations ---\n");
    while (fread(&reservation, sizeof(Reservation), 1, file)) {
        if (reservation.user_id == user_id) {
            printf("Reservation ID: %d | Bus ID: %d | Seat: %d | Date: %s\n",
                   reservation.reservation_id, reservation.bus_id, reservation.seat_number, reservation.reservation_date);
        }
    }
    fclose(file);
}

// Authenticate User
int authenticateUser(char *email, char *password) {
    FILE *file = fopen("users.dat", "rb");
    if (!file) {
        printf("No users found.\n");
        return -1;
    }

    Passenger user;
    while (fread(&user, sizeof(Passenger), 1, file)) {
        if (strcmp(user.email, email) == 0 && strcmp(user.password, password) == 0) {
            fclose(file);
            return user.user_id;
        }
    }
    fclose(file);
    return -1;
}

// Register User
void registerUser() {
    Passenger user;
    user.user_id = generateUserID();

    printf("Enter Name: ");
    scanf("%s", user.name);
    printf("Enter Email: ");
    scanf("%s", user.email);
    printf("Enter Password: ");
    scanf("%s", user.password);

    FILE *file = fopen("users.dat", "ab");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }
    fwrite(&user, sizeof(Passenger), 1, file);
    fclose(file);
    printf("Registration successful! Your User ID is %d\n", user.user_id);
}

// Generate Unique IDs
int generateReservationID() { return rand() % 10000; }
int generateUserID() { return rand() % 10000; }

// Check Bus Availability
int isBusAvailable(int bus_id) {
    FILE *file = fopen("buses.dat", "rb");
    if (!file) return 0;

    Bus bus;
    while (fread(&bus, sizeof(Bus), 1, file)) {
        if (bus.bus_id == bus_id && bus.available_seats > 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Update Bus Availability
void updateBusAvailability(int bus_id, int seat_number, int increment) {
    FILE *file = fopen("buses.dat", "rb+");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Bus bus;
    while (fread(&bus, sizeof(Bus), 1, file)) {
        if (bus.bus_id == bus_id) {
            bus.available_seats += increment;
            fseek(file, -sizeof(Bus), SEEK_CUR);
            fwrite(&bus, sizeof(Bus), 1, file);
            break;
        }
    }
    fclose(file);
}
