#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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


