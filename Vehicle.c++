#include <iostream>
#include <fstream>
#include <string>
using namespace std;


// Base class
class Vehicle {
protected:
    int id;
    string make;
    string model;
    int year;
    double pricePerDay;
    bool available;

public:
    Vehicle(int id, string make, string model, int year, double price, bool available) {
        this->id = id;
        this->make = make;
        this->model = model;
        this->year = year;
        this->pricePerDay = price;
        this->available = available;
    }
    virtual ~Vehicle() {}

    // getters
    int getId() { return id; }
    double getPrice() { return pricePerDay; }
    bool isAvailable() { return available; }
    string getName() { return make + " " + model; }

    // setters
    void setAvailable(bool a) { available = a; }
    void setPrice(double p) { pricePerDay = p; }

    // virtual functions (polymorphism)
    virtual string getType() = 0;
    virtual void display() {
        cout << id << "\t" << getType() << "\t" << make << "\t" << model << "\t"
             << year << "\t" << pricePerDay << "\t"
             << (available ? "Available" : "Rented") << "\t";
    }
    virtual void save(ofstream &f) {
        f << getType() << " " << id << " " << make << " " << model << " "
          << year << " " << pricePerDay << " " << available << " ";
    }
};

// Derived class 1
class Car : public Vehicle {
    int seats;

public:
    Car(int id, string make, string model, int year, double price, bool avail, int seats)
        : Vehicle(id, make, model, year, price, avail) {
        this->seats = seats;
    }
    string getType() { return "Car"; }
    void display() {
        Vehicle::display();
        cout << seats << " seats" << endl;
    }
    void save(ofstream &f) {
        Vehicle::save(f);
        f << seats << endl;
    }
};

// Derived class 2
class Bike : public Vehicle {
    int cc;

public:
    Bike(int id, string make, string model, int year, double price, bool avail, int cc)
        : Vehicle(id, make, model, year, price, avail) {
        this->cc = cc;
    }
    string getType() { return "Bike"; }
    void display() {
        Vehicle::display();
        cout << cc << " cc" << endl;
    }
    void save(ofstream &f) {
        Vehicle::save(f);
        f << cc << endl;
    }
};

class Customer {
public:
    string username, password, name, phone, email;
};

class Rental {
public:
    int rentalId;
    string username;
    int vehicleId;
    int days;
    double total;
    bool returned;
};



const int MAX_VEHICLES = 100;
const int MAX_CUSTOMERS = 100;
const int MAX_RENTALS = 500;

Vehicle *vehicles[MAX_VEHICLES]; // array of Vehicle pointers (needed for Car/Bike)
int vehicleCount = 0;

Customer customers[MAX_CUSTOMERS];
int customerCount = 0;

Rental rentals[MAX_RENTALS];
int rentalCount = 0;

const string ADMIN_USER = "admin";
const string ADMIN_PASS = "admin123";


// reads an integer safely (keeps asking until valid)
int getInt(string message) {
    int x;
    cout << message;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! " << message;
    }
    return x;
}

double getDouble(string message) {
    double x;
    cout << message;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! " << message;
    }
    return x;
}

Vehicle *findVehicle(int id) {
    for (int i = 0; i < vehicleCount; i++)
        if (vehicles[i]->getId() == id) return vehicles[i];
    return NULL;
}

void printVehicleHeader() {
    cout << "\nID\tType\tMake\tModel\tYear\tPrice/Day\tStatus\t\tExtra\n";
    cout << "--------------------------------------------------------------------------\n";
}



void saveVehicles() {
    ofstream f("vehicles.txt");
    for (int i = 0; i < vehicleCount; i++) vehicles[i]->save(f);
    f.close();
}

void loadVehicles() {
    ifstream f("vehicles.txt");
    string type, make, model;
    int id, year, extra;
    double price;
    bool avail;
    while (vehicleCount < MAX_VEHICLES &&
           f >> type >> id >> make >> model >> year >> price >> avail >> extra) {
        if (type == "Car")
            vehicles[vehicleCount] = new Car(id, make, model, year, price, avail, extra);
        else
            vehicles[vehicleCount] = new Bike(id, make, model, year, price, avail, extra);
        vehicleCount++;
    }
    f.close();
}

void saveCustomers() {
    ofstream f("customers.txt");
    for (int i = 0; i < customerCount; i++) {
        f << customers[i].username << " " << customers[i].password << " "
          << customers[i].name << " " << customers[i].phone << " "
          << customers[i].email << endl;
    }
    f.close();
}

void loadCustomers() {
    ifstream f("customers.txt");
    Customer c;
    while (customerCount < MAX_CUSTOMERS &&
           f >> c.username >> c.password >> c.name >> c.phone >> c.email) {
        customers[customerCount] = c;
        customerCount++;
    }
    f.close();
}

void saveRentals() {
    ofstream f("rentals.txt");
    for (int i = 0; i < rentalCount; i++) {
        f << rentals[i].rentalId << " " << rentals[i].username << " "
          << rentals[i].vehicleId << " " << rentals[i].days << " "
          << rentals[i].total << " " << rentals[i].returned << endl;
    }
    f.close();
}

void loadRentals() {
    ifstream f("rentals.txt");
    Rental r;
    while (rentalCount < MAX_RENTALS &&
           f >> r.rentalId >> r.username >> r.vehicleId >> r.days >> r.total >> r.returned) {
        rentals[rentalCount] = r;
        rentalCount++;
    }
    f.close();
}

// ======================= ADMIN FUNCTIONS =======================

void addVehicle() {
    cout << "\n--- Add Vehicle ---\n";
    if (vehicleCount >= MAX_VEHICLES) {
        cout << "Vehicle list is full!\n";
        return;
    }
    int id = getInt("Vehicle ID: ");
    if (findVehicle(id) != NULL) {
        cout << "A vehicle with this ID already exists!\n";
        return;
    }
    int type = getInt("Type (1 = Car, 2 = Bike): ");
    string make, model;
    cout << "Make: ";
    cin >> make;
    cout << "Model: ";
    cin >> model;
    int year = getInt("Year: ");
    double price = getDouble("Price per day: ");

    if (type == 1) {
        int seats = getInt("Number of seats: ");
        vehicles[vehicleCount] = new Car(id, make, model, year, price, true, seats);
    } else {
        int cc = getInt("Engine cc: ");
        vehicles[vehicleCount] = new Bike(id, make, model, year, price, true, cc);
    }
    vehicleCount++;
    saveVehicles();
    cout << "Vehicle added successfully!\n";
}

void viewAllVehicles() {
    if (vehicleCount == 0) {
        cout << "\nNo vehicles in the system.\n";
        return;
    }
    printVehicleHeader();
    for (int i = 0; i < vehicleCount; i++) vehicles[i]->display();
}

void updateVehicle() {
    int id = getInt("\nEnter ID of vehicle to update: ");
    Vehicle *v = findVehicle(id);
    if (v == NULL) {
        cout << "Vehicle not found!\n";
        return;
    }
    double price = getDouble("Enter new price per day: ");
    v->setPrice(price);
    saveVehicles();
    cout << "Vehicle updated successfully!\n";
}

void deleteVehicle() {
    int id = getInt("\nEnter ID of vehicle to delete: ");
    for (int i = 0; i < vehicleCount; i++) {
        if (vehicles[i]->getId() == id) {
            if (!vehicles[i]->isAvailable()) {
                cout << "Cannot delete: vehicle is currently rented!\n";
                return;
            }
            delete vehicles[i];
            // shift the remaining vehicles one place to the left
            for (int j = i; j < vehicleCount - 1; j++)
                vehicles[j] = vehicles[j + 1];
            vehicleCount--;
            saveVehicles();
            cout << "Vehicle deleted successfully!\n";
            return;
        }
    }
    cout << "Vehicle not found!\n";
}

void viewAllRentals() {
    if (rentalCount == 0) {
        cout << "\nNo rental records.\n";
        return;
    }
    cout << "\nRentalID\tCustomer\tVehicleID\tDays\tTotal\tStatus\n";
    cout << "------------------------------------------------------------\n";
    for (int i = 0; i < rentalCount; i++) {
        cout << rentals[i].rentalId << "\t\t" << rentals[i].username << "\t\t"
             << rentals[i].vehicleId << "\t\t" << rentals[i].days << "\t"
             << rentals[i].total << "\t"
             << (rentals[i].returned ? "Returned" : "Active") << endl;
    }
}

void adminMenu() {
    int choice;
    do {
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1. Add Vehicle\n2. Remove Vehicle\n3. Update Vehicle\n";
        cout << "4. View All Vehicles\n5. View Rentals\n6. Logout\n";
        choice = getInt("Enter choice: ");
        switch (choice) {
        case 1: addVehicle(); break;
        case 2: deleteVehicle(); break;
        case 3: updateVehicle(); break;
        case 4: viewAllVehicles(); break;
        case 5: viewAllRentals(); break;
        case 6: cout << "Logged out.\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);
}


void viewAvailableVehicles() {
    bool found = false;
    printVehicleHeader();
    for (int i = 0; i < vehicleCount; i++) {
        if (vehicles[i]->isAvailable()) {
            vehicles[i]->display();
            found = true;
        }
    }
    if (!found) cout << "No vehicles available right now.\n";
}

void rentVehicle(string username) {
    if (rentalCount >= MAX_RENTALS) {
        cout << "Rental records are full!\n";
        return;
    }
    viewAvailableVehicles();
    int id = getInt("\nEnter ID of vehicle to rent: ");
    Vehicle *v = findVehicle(id);
    if (v == NULL) {
        cout << "Vehicle not found!\n";
        return;
    }
    if (!v->isAvailable()) {
        cout << "Sorry, this vehicle is not available.\n";
        return;
    }
    int days = getInt("Number of days: ");
    if (days <= 0) {
        cout << "Days must be at least 1!\n";
        return;
    }

    Rental r;
    r.rentalId = rentalCount + 1;
    r.username = username;
    r.vehicleId = id;
    r.days = days;
    r.total = days * v->getPrice(); // fee calculation
    r.returned = false;
    rentals[rentalCount] = r;
    rentalCount++;

    v->setAvailable(false);
    saveVehicles();
    saveRentals();
    cout << "\nBooking successful!\n";
    cout << "Rental ID: " << r.rentalId << "  |  Estimated charge: " << r.total << endl;
}

void returnVehicle(string username) {
    bool hasActive = false;
    cout << "\nYour active rentals:\n";
    for (int i = 0; i < rentalCount; i++) {
        if (rentals[i].username == username && !rentals[i].returned) {
            cout << "Rental ID: " << rentals[i].rentalId
                 << "  Vehicle ID: " << rentals[i].vehicleId
                 << "  Days: " << rentals[i].days << endl;
            hasActive = true;
        }
    }
    if (!hasActive) {
        cout << "You have no active rentals.\n";
        return;
    }

    int rid = getInt("Enter Rental ID to return: ");
    for (int i = 0; i < rentalCount; i++) {
        if (rentals[i].rentalId == rid && rentals[i].username == username && !rentals[i].returned) {
            rentals[i].returned = true;
            Vehicle *v = findVehicle(rentals[i].vehicleId);
            if (v != NULL) v->setAvailable(true);
            saveVehicles();
            saveRentals();

            // invoice
            cout << "\n========== INVOICE ==========\n";
            cout << "Rental ID : " << rentals[i].rentalId << endl;
            cout << "Customer  : " << username << endl;
            if (v != NULL) cout << "Vehicle   : " << v->getName() << endl;
            cout << "Days      : " << rentals[i].days << endl;
            cout << "Total Fee : " << rentals[i].total << endl;
            cout << "=============================\n";
            cout << "Vehicle returned successfully!\n";
            return;
        }
    }
    cout << "Invalid Rental ID!\n";
}

void viewMyRentals(string username) {
    bool found = false;
    cout << "\nRentalID\tVehicleID\tDays\tTotal\tStatus\n";
    cout << "------------------------------------------------\n";
    for (int i = 0; i < rentalCount; i++) {
        if (rentals[i].username == username) {
            cout << rentals[i].rentalId << "\t\t" << rentals[i].vehicleId << "\t\t"
                 << rentals[i].days << "\t" << rentals[i].total << "\t"
                 << (rentals[i].returned ? "Returned" : "Active") << endl;
            found = true;
        }
    }
    if (!found) cout << "You have no rental history.\n";
}

void customerMenu(string username) {
    int choice;
    do {
        cout << "\n===== CUSTOMER MENU =====\n";
        cout << "1. View Available Vehicles\n2. Rent Vehicle\n3. Return Vehicle\n";
        cout << "4. View My Rentals\n5. Logout\n";
        choice = getInt("Enter choice: ");
        switch (choice) {
        case 1: viewAvailableVehicles(); break;
        case 2: rentVehicle(username); break;
        case 3: returnVehicle(username); break;
        case 4: viewMyRentals(username); break;
        case 5: cout << "Logged out.\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}


void registerCustomer() {
    if (customerCount >= MAX_CUSTOMERS) {
        cout << "Customer list is full!\n";
        return;
    }
    Customer c;
    cout << "\n--- Customer Registration ---\n";
    cout << "Username: ";
    cin >> c.username;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].username == c.username) {
            cout << "Username already taken!\n";
            return;
        }
    }
    cout << "Password: ";
    cin >> c.password;
    cout << "Name: ";
    cin >> c.name;
    cout << "Phone: ";
    cin >> c.phone;
    cout << "Email: ";
    cin >> c.email;
    customers[customerCount] = c;
    customerCount++;
    saveCustomers();
    cout << "Registration successful!\n";
}

void adminLogin() {
    string u, p;
    cout << "\nAdmin username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;
    if (u == ADMIN_USER && p == ADMIN_PASS)
        adminMenu();
    else
        cout << "Invalid admin credentials!\n";
}

void customerLogin() {
    string u, p;
    cout << "\nUsername: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].username == u && customers[i].password == p) {
            cout << "Welcome, " << customers[i].name << "!\n";
            customerMenu(u);
            return;
        }
    }
    cout << "Invalid username or password!\n";
}


int main() {
    loadVehicles();
    loadCustomers();
    loadRentals();

    int choice;
    do {
        cout << "\n===== VEHICLE RENTAL MANAGEMENT SYSTEM =====\n";
        cout << "1. Admin Login\n2. Customer Login\n3. Register Customer\n4. Exit\n";
        choice = getInt("Enter choice: ");
        switch (choice) {
        case 1: adminLogin(); break;
        case 2: customerLogin(); break;
        case 3: registerCustomer(); break;
        case 4: cout << "Thank you for using VRMS. Goodbye!\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    // free memory
    for (int i = 0; i < vehicleCount; i++) delete vehicles[i];
    return 0;
}