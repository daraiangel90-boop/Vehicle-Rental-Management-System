//   Admin login    -> username: admin   password: admin123
//   Note: type names as ONE word (no spaces), e.g. Ram_Sharma
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
 
// Vehicle is the parent class. Car and Bike are made from it.
class Vehicle {
protected:
    int id;
    string make;        // the brand, e.g. Toyota
    string model;       // e.g. Hilux
    int year;
    double pricePerDay;
    bool available;     // true = free to rent, false = already rented
 
public:
    Vehicle(int vid, string vmake, string vmodel, int vyear, double vprice, bool vavailable) {
        id = vid;
        make = vmake;
        model = vmodel;
        year = vyear;
        pricePerDay = vprice;
        available = vavailable;
    }
 
    virtual ~Vehicle() {}
 
    // functions to get the values
    int getId() { return id; }
    double getPrice() { return pricePerDay; }
    bool isAvailable() { return available; }
    string getName() { return make + " " + model; }
 
    // functions to change the values
    void setAvailable(bool a) { available = a; }
    void setPrice(double p) { pricePerDay = p; }
 
    // Car and Bike each write their own version of these (polymorphism)
    virtual string getType() = 0;
 
    virtual void display() {
        string status;
        if (available) {
            status = "Available";
        } else {
            status = "Rented";
        }
        cout << id << "\t" << getType() << "\t" << make << "\t" << model << "\t"
             << year << "\t" << pricePerDay << "\t" << status << "\t";
    }
 
    virtual void save(ofstream &f) {
        f << getType() << " " << id << " " << make << " " << model << " "
          << year << " " << pricePerDay << " " << available << " ";
    }
};
 
 
// Car is a type of Vehicle, it also has seats
class Car : public Vehicle {
private:
    int seats;
 
public:
    Car(int vid, string vmake, string vmodel, int vyear, double vprice, bool vavailable, int numSeats)
        : Vehicle(vid, vmake, vmodel, vyear, vprice, vavailable) {
        seats = numSeats;
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
 
 
// Bike is a type of Vehicle, it also has engine cc
class Bike : public Vehicle {
private:
    int cc;
 
public:
    Bike(int vid, string vmake, string vmodel, int vyear, double vprice, bool vavailable, int engineCC)
        : Vehicle(vid, vmake, vmodel, vyear, vprice, vavailable) {
        cc = engineCC;
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
 
 
// stores the details of one customer
class Customer {
public:
    string username;
    string password;
    string name;
    string phone;
    string email;
};
 
 
// stores the details of one rental (one booking)
class Rental {
public:
    int rentalId;
    string username;    // who rented it
    int vehicleId;      // which vehicle
    int days;
    double total;       // total fee
    bool returned;      // false = still with customer, true = given back
};
 
//  DATA (arrays to store everything)
 
const int MAX_VEHICLES = 100;
const int MAX_CUSTOMERS = 100;
const int MAX_RENTALS = 500;
 
Vehicle *vehicles[MAX_VEHICLES];     // pointers, so one array can hold both Car and Bike
int totalVehicles = 0;
 
Customer customers[MAX_CUSTOMERS];
int totalCustomers = 0;
 
Rental rentals[MAX_RENTALS];
int totalRentals = 0;
 
const string ADMIN_USER = "admin";
const string ADMIN_PASS = "admin123";
 
 
// asks for a whole number and keeps asking until the user types a valid one
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
 
// same as above but for decimal numbers (like price)
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
 
// searches for a vehicle by its ID, gives NULL if not found
Vehicle *findVehicle(int id) {
    for (int i = 0; i < totalVehicles; i++) {
        if (vehicles[i]->getId() == id) {
            return vehicles[i];
        }
    }
    return NULL;
}  
 
// prints the heading line of the vehicle table
void printVehicleHeader() {
    cout << "\nID\tType\tMake\tModel\tYear\tPrice/Day\tStatus\t\tExtra\n";
    cout << "--------------------------------------------------------------------------\n";
}
 
//  FILE HANDLING (save and load data)

 void saveVehicles() {
    ofstream f("vehicles.txt");
    for (int i = 0; i < totalVehicles; i++) {
        vehicles[i]->save(f);
    }
    f.close();
}
 
void loadVehicles() {
    ifstream f("vehicles.txt");
    string type, make, model;
    int id, year, extra;
    double price;
    bool avail;
 
    while (totalVehicles < MAX_VEHICLES &&
           f >> type >> id >> make >> model >> year >> price >> avail >> extra) {
        if (type == "Car") {
            vehicles[totalVehicles] = new Car(id, make, model, year, price, avail, extra);
        } else {
            vehicles[totalVehicles] = new Bike(id, make, model, year, price, avail, extra);
        }
        totalVehicles++;
    }
    f.close();
}
 
void saveCustomers() {
    ofstream f("customers.txt");
    for (int i = 0; i < totalCustomers; i++) {
        f << customers[i].username << " " << customers[i].password << " "
          << customers[i].name << " " << customers[i].phone << " "
          << customers[i].email << endl;
    }
    f.close();
}
 
void loadCustomers() {
    ifstream f("customers.txt");
    Customer c;
 
    while (totalCustomers < MAX_CUSTOMERS &&
           f >> c.username >> c.password >> c.name >> c.phone >> c.email) {
        customers[totalCustomers] = c;
        totalCustomers++;
    }
    f.close();
}
 
void saveRentals() {
    ofstream f("rentals.txt");
    for (int i = 0; i < totalRentals; i++) {
        f << rentals[i].rentalId << " " << rentals[i].username << " "
          << rentals[i].vehicleId << " " << rentals[i].days << " "
          << rentals[i].total << " " << rentals[i].returned << endl;
    }
    f.close();
}
 
void loadRentals() {
    ifstream f("rentals.txt");
    Rental r;
 
    while (totalRentals < MAX_RENTALS &&
           f >> r.rentalId >> r.username >> r.vehicleId >> r.days >> r.total >> r.returned) {
        rentals[totalRentals] = r;
        totalRentals++;
    }
    f.close();
}
 
//  ADMIN FUNCTIONS
 
void addVehicle() {
    cout << "\n--- Add Vehicle ---\n";
 
    if (totalVehicles >= MAX_VEHICLES) {
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
        vehicles[totalVehicles] = new Car(id, make, model, year, price, true, seats);
    } else {
        int cc = getInt("Engine cc: ");
        vehicles[totalVehicles] = new Bike(id, make, model, year, price, true, cc);
    }
 
    totalVehicles++;
    saveVehicles();
    cout << "Vehicle added successfully!\n";
}
 
void viewAllVehicles() {
    if (totalVehicles == 0) {
        cout << "\nNo vehicles in the system.\n";
        return;
    }
 
    printVehicleHeader();
    for (int i = 0; i < totalVehicles; i++) {
        vehicles[i]->display();
    }
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
 
    for (int i = 0; i < totalVehicles; i++) {
        if (vehicles[i]->getId() == id) {
 
            // a rented vehicle cannot be deleted
            if (!vehicles[i]->isAvailable()) {
                cout << "Cannot delete: vehicle is currently rented!\n";
                return;
            }
 
            delete vehicles[i];
 
            // move all the vehicles after it one step back to fill the gap
            for (int j = i; j < totalVehicles - 1; j++) {
                vehicles[j] = vehicles[j + 1];
            }
            totalVehicles--;
 
            saveVehicles();
            cout << "Vehicle deleted successfully!\n";
            return;
        }
    }
 
    cout << "Vehicle not found!\n";
}
 
void viewAllRentals() {
    if (totalRentals == 0) {
        cout << "\nNo rental records.\n";
        return;
    }
 
    cout << "\nRentalID\tCustomer\tVehicleID\tDays\tTotal\tStatus\n";
    cout << "------------------------------------------------------------\n";
 
    for (int i = 0; i < totalRentals; i++) {
        string status;
        if (rentals[i].returned) {
            status = "Returned";
        } else {
            status = "Active";
        }
 
        cout << rentals[i].rentalId << "\t\t" << rentals[i].username << "\t\t"
             << rentals[i].vehicleId << "\t\t" << rentals[i].days << "\t"
             << rentals[i].total << "\t" << status << endl;
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
 
//  CUSTOMER FUNCTIONS
 
void viewAvailableVehicles() {
    bool found = false;
 
    printVehicleHeader();
    for (int i = 0; i < totalVehicles; i++) {
        if (vehicles[i]->isAvailable()) {
            vehicles[i]->display();
            found = true;
        }
    }
 
    if (!found) {
        cout << "No vehicles available right now.\n";
    }
}
 
void rentVehicle(string username) {
    if (totalRentals >= MAX_RENTALS) {
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
 
    // make a new rental record
    Rental r;
    r.rentalId = totalRentals + 1;
    r.username = username;
    r.vehicleId = id;
    r.days = days;
    r.total = days * v->getPrice();     // fee = days x price per day
    r.returned = false;
 
    rentals[totalRentals] = r;
    totalRentals++;
 
    // the vehicle is now rented
    v->setAvailable(false);
 
    saveVehicles();
    saveRentals();
 
    cout << "\nBooking successful!\n";
    cout << "Rental ID: " << r.rentalId << "  |  Estimated charge: " << r.total << endl;
}
 
void returnVehicle(string username) {
    bool hasActive = false;
 
    // first show the customer's rentals that are not returned yet
    cout << "\nYour active rentals:\n";
    for (int i = 0; i < totalRentals; i++) {
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
 
    for (int i = 0; i < totalRentals; i++) {
        if (rentals[i].rentalId == rid && rentals[i].username == username && !rentals[i].returned) {
 
            rentals[i].returned = true;
 
            // the vehicle is free again
            Vehicle *v = findVehicle(rentals[i].vehicleId);
            if (v != NULL) {
                v->setAvailable(true);
            }
 
            saveVehicles();
            saveRentals();
 
            // print the invoice
            cout << "\n========== INVOICE ==========\n";
            cout << "Rental ID : " << rentals[i].rentalId << endl;
            cout << "Customer  : " << username << endl;
            if (v != NULL) {
                cout << "Vehicle   : " << v->getName() << endl;
            }
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
 
    for (int i = 0; i < totalRentals; i++) {
        if (rentals[i].username == username) {
            string status;
            if (rentals[i].returned) {
                status = "Returned";
            } else {
                status = "Active";
            }
 
            cout << rentals[i].rentalId << "\t\t" << rentals[i].vehicleId << "\t\t"
                 << rentals[i].days << "\t" << rentals[i].total << "\t" << status << endl;
            found = true;
        }
    }
 
    if (!found) {
        cout << "You have no rental history.\n";
    }
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
 
 
// Phone is correct when it has 10 digits and starts with 97 or 98
bool isValidPhone(string phone) {
    // must be exactly 10 characters
    if (phone.length() != 10) {
        return false;
    }
 
    // every character must be a digit
    for (int i = 0; i < 10; i++) {
        if (phone[i] < '0' || phone[i] > '9') {
            return false;
        }
    }
 
    // must start with 97 or 98
    if (phone[0] != '9') {
        return false;
    }
    if (phone[1] != '7' && phone[1] != '8') {
        return false;
    }
 
    return true;
}
 
// Email is correct when it looks like this: name@domain.com
bool isValidEmail(string email) {
    int length = email.length();
    int atCount = 0;       
    int atPosition = -1;   // where the @ is
    int lastDot = -1;      // where the last . is
 
    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            atCount++;
            atPosition = i;
        }
        if (email[i] == '.') {
            lastDot = i;
        }
    }
 
    // there must be exactly one @
    if (atCount != 1) {
        return false;
    }
 
    // there must be something before the @
    if (atPosition == 0) {
        return false;
    }
 
    // there must be a . after the @, with some text between them
    if (lastDot < atPosition + 2) {
        return false;
    }
 
    // there must be at least 2 letters after the last .
    if (length - lastDot - 1 < 2) {
        return false;
    }
 
    return true;
}
 
//  REGISTER AND LOGIN
 
void registerCustomer() {
    if (totalCustomers >= MAX_CUSTOMERS) {
        cout << "Customer list is full!\n";
        return;
    }
 
    Customer c;
    cout << "\n--- Customer Registration ---\n";
 
    cout << "Username: ";
    cin >> c.username;
 
    // username must be unique
    for (int i = 0; i < totalCustomers; i++) {
        if (customers[i].username == c.username) {
            cout << "Username already taken!\n";
            return;
        }
    }
 
    cout << "Password: ";
    cin >> c.password;
 
    cout << "Name: ";
    cin >> c.name;
 
    // keep asking until the phone number is correct
    cout << "Phone (10 digits, starts with 97 or 98): ";
    cin >> c.phone;
    while (!isValidPhone(c.phone)) {
        cout << "Invalid phone number! It must be 10 digits and start with 97 or 98.\n";
        cout << "Phone: ";
        cin >> c.phone;
    }
 
    // keep asking until the email is correct
    cout << "Email (example: name@gmail.com): ";
    cin >> c.email;
    while (!isValidEmail(c.email)) {
        cout << "Invalid email! Use the format name@domain.com\n";
        cout << "Email: ";
        cin >> c.email;
    }
 
    customers[totalCustomers] = c;
    totalCustomers++;
    saveCustomers();
 
    cout << "Registration successful!\n";
}
 
void adminLogin() {
    string user, pass;
 
    cout << "\nAdmin username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
 
    if (user == ADMIN_USER && pass == ADMIN_PASS) {
        adminMenu();
    } else {
        cout << "Invalid admin credentials!\n";
    }
}
 
void customerLogin() {
    string user, pass;
 
    cout << "\nUsername: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
 
    for (int i = 0; i < totalCustomers; i++) {
        if (customers[i].username == user && customers[i].password == pass) {
            cout << "Welcome, " << customers[i].name << "!\n";
            customerMenu(user);
            return;
        }
    }
 
    cout << "Invalid username or password!\n";
}
 
//  MAIN FUNCTION (program starts here)
 
int main() {
    // load the old saved data from the files
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
 
    // free the memory we created with new
    for (int i = 0; i < totalVehicles; i++) {
        delete vehicles[i];
    }
 
    return 0;
}
 