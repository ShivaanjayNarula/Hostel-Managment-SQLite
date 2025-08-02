#include <iostream>
#include <sqlite3.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Database file
const char* db_file = "hostel_management.db";
sqlite3* db;

// Function prototypes
void newStu();
void update();
void display();
void deleteStu();
void exitApp();
void login();
void signIn();
void menu();
bool connectToDatabase();
void disconnectFromDatabase();
bool initializeDatabase();
void showMainMenu();

struct Date {
    int month, day, year;
};

struct Student {
    string name;
    int age;
    string city;
    string mail;
    string hostel_name;
    string room;
    string father_name;
    string mother_name;
    long long phone;
    long long parent_phone;
    long long roll;
    Date dob;
} add, upd, rem, students[100], temp;

long long check;   // for entry check through roll numbers
long long i, j, mexit;

int main() {
    // Connect to SQLite database
    if (!connectToDatabase()) {
        cout << "Failed to connect to database. Exiting..." << endl;
        return 1;
    }
    
    // Initialize database tables
    if (!initializeDatabase()) {
        cout << "Failed to initialize database. Exiting..." << endl;
        return 1;
    }
    
    showMainMenu();
    disconnectFromDatabase();
    return 0;
}

// Function to connect to SQLite database
bool connectToDatabase() {
    int rc = sqlite3_open(db_file, &db);
    if (rc != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    cout << "Connected to SQLite database successfully!" << endl;
    return true;
}

// Function to disconnect from SQLite database
void disconnectFromDatabase() {
    if (db != NULL) {
        sqlite3_close(db);
    }
}

// Function to initialize database tables
bool initializeDatabase() {
    const char* create_users_sql = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    const char* create_students_sql = 
        "CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "roll_number INTEGER UNIQUE NOT NULL,"
        "name TEXT NOT NULL,"
        "dob DATE NOT NULL,"
        "age INTEGER NOT NULL,"
        "city TEXT NOT NULL,"
        "phone INTEGER NOT NULL,"
        "email TEXT NOT NULL,"
        "father_name TEXT NOT NULL,"
        "parent_phone INTEGER NOT NULL,"
        "room_number TEXT NOT NULL,"
        "hostel_name TEXT NOT NULL,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    char* errMsg = 0;
    
    // Create users table
    int rc = sqlite3_exec(db, create_users_sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating users table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    // Create students table
    rc = sqlite3_exec(db, create_students_sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating students table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    // Insert default users if table is empty
    const char* check_users_sql = "SELECT COUNT(*) FROM users;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, check_users_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            if (count == 0) {
                const char* insert_default_users = 
                    "INSERT INTO users (username, password) VALUES "
                    "('admin', 'password'),"
                    "('Narula', '1234'),"
                    "('Krish', '1234'),"
                    "('Shivam', '12345678');";
                
                rc = sqlite3_exec(db, insert_default_users, 0, 0, &errMsg);
                if (rc != SQLITE_OK) {
                    cout << "Error inserting default users: " << errMsg << endl;
                    sqlite3_free(errMsg);
                }
            }
        }
    }
    sqlite3_finalize(stmt);
    
    // Insert sample students if table is empty
    const char* check_students_sql = "SELECT COUNT(*) FROM students;";
    rc = sqlite3_prepare_v2(db, check_students_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            if (count == 0) {
                const char* insert_sample_students = 
                    "INSERT INTO students (roll_number, name, dob, age, city, phone, email, father_name, parent_phone, room_number, hostel_name) VALUES "
                    "(2210992302, 'Shivaanjay', '2004-02-25', 20, 'Dharamshala', 8580457634, 'shivaanjaynarula@gmail.com', 'Sanjay Narula', 8219654843, '328', 'Archimides'),"
                    "(2210992270, 'Saurabh Middha', '2003-11-30', 20, 'GangaNagar', 9888803908, 'saurabh.com', 'andom.Random', 9888809898, '508', 'Armstrong');";
                
                rc = sqlite3_exec(db, insert_sample_students, 0, 0, &errMsg);
                if (rc != SQLITE_OK) {
                    cout << "Error inserting sample students: " << errMsg << endl;
                    sqlite3_free(errMsg);
                }
            }
        }
    }
    sqlite3_finalize(stmt);
    
    cout << "Database initialized successfully!" << endl;
    return true;
}

// Function to show the main login menu
void showMainMenu() {
    int ch;
    
    cout << "************ WELCOME TO CHITKARA UNIVERSITY ************" << endl;
    cout << "************         HOSTEL ALLOTMENT       ************" << endl;
    cout << "************ 1. LOGIN                       ************" << endl;
    cout << "************ 2. SIGN IN                     ************" << endl;
    cout << "************ 3. EXIT                        ************" << endl;
    cout << "Enter your choice: ";
    cin >> ch;
    
    switch (ch) {
        case 1:
            login();
            break;
        case 2:
            signIn();
            break;
        case 3:
            exitApp();
            break;
        default:
            cout << "Invalid choice. Please try again.\n" << endl;
            showMainMenu();
    }
}

// Function to display the main menu
void menu() {
    int choice;
    system("clear");  // Clear the screen for Unix-based systems like macOS
    cout << "************WELCOME TO CHITKARA UNIVERSITY************" << endl;
    cout << "**************     HOSTEL ALLOTMENT    ***************" << endl;
    cout << "************           MAIN MENU          ************" << endl;
    cout << "1. ENTER NEW STUDENT DETAILS" << endl;
    cout << "2. UPDATE STUDENT DETAILS" << endl;
    cout << "3. DISPLAY STUDENT DETAILS" << endl;
    cout << "4. REMOVE STUDENT DETAILS" << endl;
    cout << "5. EXIT" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            newStu();
            break;
        case 2:
            update();
            break;
        case 3:
            display();
            break;
        case 4:
            deleteStu();
            break;
        case 5:
            exitApp();
            break;
        default:
            cout << "Invalid choice. Please try again.\n" << endl;
            menu();
    }
}

// Login function to authenticate users
void login() {
    system("clear");
    string username, password;
    
    cout << "************      LOGIN     ************" << endl;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Prepare SQL query
    const char* sql = "SELECT * FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
        cout << "Press any key to continue...";
        cin.ignore();
        cin.get();
        showMainMenu();
        return;
    }
    
    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    // Execute query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Login successful!" << endl;
        sqlite3_finalize(stmt);
        menu();  // Direct to the main menu after successful login
    } else {
        cout << "Invalid credentials, please try again." << endl;
        sqlite3_finalize(stmt);
        cout << "Press any key to continue...";
        cin.ignore();
        cin.get();
        showMainMenu();  // Return to main menu instead of calling main()
    }
}

// Sign-in function for registering new users
void signIn() {
    system("clear");
    string new_username, new_password;

    cout << "************      SIGN IN     ************" << endl;
    cout << "Enter new username: ";
    cin >> new_username;
    cout << "Enter new password: ";
    cin >> new_password;

    // Prepare SQL query to insert new user
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
        cout << "Press any key to continue...";
        cin.ignore();
        cin.get();
        showMainMenu();
        return;
    }
    
    // Bind parameters
    sqlite3_bind_text(stmt, 1, new_username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, new_password.c_str(), -1, SQLITE_STATIC);
    
    // Execute query
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << "Sign-in successful! Please login." << endl;
    } else {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
    }
    
    cout << "Press any key to continue...";
    cin.ignore();
    cin.get();
    showMainMenu();  // Return to main menu instead of calling main()
}

// Add new student details
void newStu() {
    system("clear");
    
    // Roll number validation
    roll:
    cout << "************      ADD STUDENT DETAILS     ************" << endl;
    cout << "ENTER ROLL NUMBER: ";
    cin >> check;

    // Check if roll number already exists
    const char* check_sql = "SELECT roll_number FROM students WHERE roll_number = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    sqlite3_bind_int64(stmt, 1, check);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Student's roll number is already in use" << endl;
        sqlite3_finalize(stmt);
        goto roll;
    }
    sqlite3_finalize(stmt);

    add.roll = check;
    cout << "ENTER THE NAME: ";
    cin.ignore();
    getline(cin, add.name);

    // Validate date of birth
    bool valid = false;
    while (!valid) {
        cout << "ENTER THE DATE OF BIRTH (DD MM YYYY): ";
        cin >> add.dob.day >> add.dob.month >> add.dob.year;

        // Date validation
        if (add.dob.year >= 1900 && add.dob.year <= 9999) {
            if (add.dob.month >= 1 && add.dob.month <= 12) {
                if (((add.dob.day >= 1 && add.dob.day <= 31) && (add.dob.month == 1 || add.dob.month == 3 || add.dob.month == 5 || add.dob.month == 7 || add.dob.month == 8 || add.dob.month == 10 || add.dob.month == 12)) ||
                    ((add.dob.day >= 1 && add.dob.day <= 30) && (add.dob.month == 4 || add.dob.month == 6 || add.dob.month == 9 || add.dob.month == 11)) ||
                    ((add.dob.day >= 1 && add.dob.day <= 28) && (add.dob.month == 2)) || (add.dob.day == 29 && add.dob.month == 2 && (add.dob.year % 400 == 0 || (add.dob.year % 4 == 0 && add.dob.year % 100 != 0)))) {
                    valid = true;
                } else {
                    cout << "Day is invalid.\n";
                }
            } else {
                cout << "Month is not valid.\n";
            }
        } else {
            cout << "Year is not valid.\n";
        }
    }

    cout << "ENTER THE AGE: ";
    cin >> add.age;
    cout << "ENTER THE CITY: ";
    cin.ignore();
    getline(cin, add.city);
    cout << "ENTER THE PHONE NUMBER: ";
    cin >> add.phone;
    cout << "ENTER THE EMAIL ID: ";
    cin.ignore();
    getline(cin, add.mail);
    cout << "ENTER THE FATHER'S NAME: ";
    cin.ignore();
    getline(cin, add.father_name);
    cout << "ENTER THE PARENT'S PHONE NUMBER: ";
    cin >> add.parent_phone;
    cout << "ENTER THE ROOM NUMBER: ";
    cin.ignore();
    getline(cin, add.room);
    cout << "ENTER THE HOSTEL NAME: ";
    getline(cin, add.hostel_name);

    // Format date for SQLite
    stringstream date_ss;
    date_ss << setfill('0') << setw(4) << add.dob.year << "-"
             << setfill('0') << setw(2) << add.dob.month << "-"
             << setfill('0') << setw(2) << add.dob.day;
    string dob_str = date_ss.str();

    // Prepare SQL query to insert new student
    const char* insert_sql = "INSERT INTO students (roll_number, name, dob, age, city, phone, email, father_name, parent_phone, room_number, hostel_name) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    // Bind parameters
    sqlite3_bind_int64(stmt, 1, add.roll);
    sqlite3_bind_text(stmt, 2, add.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, dob_str.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, add.age);
    sqlite3_bind_text(stmt, 5, add.city.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 6, add.phone);
    sqlite3_bind_text(stmt, 7, add.mail.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, add.father_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 9, add.parent_phone);
    sqlite3_bind_text(stmt, 10, add.room.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 11, add.hostel_name.c_str(), -1, SQLITE_STATIC);
    
    // Execute query
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE) {
        cout << "***********  STUDENT ADDED SUCCESSFULLY   ************" << endl;
    } else {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
    }

    cout << "ENTER 1 TO GO TO MAIN MENU AND 0 TO EXIT: ";
    cin >> mexit;

    if (mexit == 1)
        menu();
    else if (mexit == 0)
        exitApp();
}

// Update student details
void update() {
    system("clear");
    int choice;
    bool recordFound = false;

    cout << "ENTER THE ROLL NUMBER YOU WANT TO CHANGE THE DETAILS OF: ";
    cin >> upd.roll;

    // Check if student exists
    const char* check_sql = "SELECT * FROM students WHERE roll_number = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    sqlite3_bind_int64(stmt, 1, upd.roll);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        recordFound = true;
        cout << "Record found for roll number: " << upd.roll << endl;
        cout << "ENTER 1 TO UPDATE ROOM NUMBER OR 2 TO UPDATE PHONE NUMBER: ";
        cin >> choice;

        const char* update_sql;
        if (choice == 1) {
            cout << "ENTER NEW ROOM NUMBER: ";
            cin.ignore();
            getline(cin, upd.room);
            update_sql = "UPDATE students SET room_number = ? WHERE roll_number = ?;";
        } else if (choice == 2) {
            cout << "ENTER NEW PHONE NUMBER: ";
            cin >> upd.phone;
            update_sql = "UPDATE students SET phone = ? WHERE roll_number = ?;";
        } else {
            cout << "Invalid choice!" << endl;
            sqlite3_finalize(stmt);
            return;
        }
        
        sqlite3_finalize(stmt);
        
        rc = sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            cout << "Error: " << sqlite3_errmsg(db) << endl;
            return;
        }
        
        if (choice == 1) {
            sqlite3_bind_text(stmt, 1, upd.room.c_str(), -1, SQLITE_STATIC);
        } else {
            sqlite3_bind_int64(stmt, 1, upd.phone);
        }
        sqlite3_bind_int64(stmt, 2, upd.roll);
        
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        if (rc == SQLITE_DONE) {
            cout << "Changes saved successfully!" << endl;
        } else {
            cout << "Error: " << sqlite3_errmsg(db) << endl;
        }
    } else {
        sqlite3_finalize(stmt);
    }

    if (!recordFound) {
        cout << "Record not found!" << endl;
    }

    cout << "ENTER 1 TO RETURN TO MAIN MENU AND 0 TO EXIT: ";
    cin >> mexit;

    if (mexit == 1)
        menu();
    else
        exitApp();
}

// Display all student details
void display() {
    system("clear");
    
    const char* sql = "SELECT * FROM students ORDER BY roll_number;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    cout << "************ STUDENT RECORD ************" << endl;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Roll Number: " << sqlite3_column_int64(stmt, 1) << endl;
        cout << "Name: " << (const char*)sqlite3_column_text(stmt, 2) << endl;
        cout << "DOB: " << (const char*)sqlite3_column_text(stmt, 3) << endl;
        cout << "Age: " << sqlite3_column_int(stmt, 4) << endl;
        cout << "City: " << (const char*)sqlite3_column_text(stmt, 5) << endl;
        cout << "Phone: " << sqlite3_column_int64(stmt, 6) << endl;
        cout << "Email: " << (const char*)sqlite3_column_text(stmt, 7) << endl;
        cout << "Father's Name: " << (const char*)sqlite3_column_text(stmt, 8) << endl;
        cout << "Parent's Phone: " << sqlite3_column_int64(stmt, 9) << endl;
        cout << "Room: " << (const char*)sqlite3_column_text(stmt, 10) << endl;
        cout << "Hostel: " << (const char*)sqlite3_column_text(stmt, 11) << endl;
        cout << "--------------------------------------" << endl;
    }
    
    sqlite3_finalize(stmt);

    cout << "ENTER 1 TO RETURN TO MAIN MENU AND 0 TO EXIT: ";
    cin >> mexit;

    if (mexit == 1)
        menu();
    else
        exitApp();
}

// Delete a student record
void deleteStu() {
    system("clear");
    long long roll;
    bool recordFound = false;

    cout << "ENTER ROLL NUMBER OF STUDENT TO REMOVE: ";
    cin >> roll;

    // Check if student exists
    const char* check_sql = "SELECT roll_number FROM students WHERE roll_number = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    sqlite3_bind_int64(stmt, 1, roll);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        recordFound = true;
    }
    sqlite3_finalize(stmt);

    if (recordFound) {
        // Delete the student
        const char* delete_sql = "DELETE FROM students WHERE roll_number = ?;";
        
        rc = sqlite3_prepare_v2(db, delete_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            cout << "Error: " << sqlite3_errmsg(db) << endl;
            return;
        }
        
        sqlite3_bind_int64(stmt, 1, roll);
        
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        if (rc == SQLITE_DONE) {
            cout << "STUDENT RECORD DELETED!" << endl;
        } else {
            cout << "Error: " << sqlite3_errmsg(db) << endl;
        }
    } else {
        cout << "Record not found!" << endl;
    }

    cout << "ENTER 1 TO RETURN TO MAIN MENU AND 0 TO EXIT: ";
    cin >> mexit;

    if (mexit == 1)
        menu();
    else
        exitApp();
}

// Exit the application
void exitApp() {
    cout << "THANK YOU FOR USING THE SYSTEM!" << endl;
    disconnectFromDatabase();
    exit(0);
} 