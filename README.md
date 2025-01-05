# -Bus-Reservation-System
he Bus Reservation System is a console-based application written in C, designed to manage bus schedules, reservations, and passenger details. It provides functionality for both administrators and passengers, ensuring an organized system for managing bus travel bookings.


-----------The Bus Reservation System is a console-based program designed to manage:---------
* Bus Operations: Admins can add and view buses.
* Passenger Services: Passengers can book seats, view buses, and manage their reservations.
* Data Management: Data is stored in files (buses.dat, users.dat, reservations.dat) for persistence.

-------------Advantages--------------------
* Persistence: All data is stored in files, allowing it to be retained after the program exits.
* Role-Based Menus: Admin and passenger roles have separate menus and functionality.
* Scalability: New features, like cancellations or advanced searches, can be added easily.

---------Limitations-------------
* Concurrency: No mechanism for handling simultaneous access to files.
* Security: Passwords are stored in plain text (encryption should be implemented).
* Error Handling: Limited checks for invalid inputs or file read/write errors.
* Seat Selection: No visual representation of bus seats.
