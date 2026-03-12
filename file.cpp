#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <fstream>
#include <ctime>

using namespace std;

// Function to convert citizenship input to lowercase
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Function to generate a random Voter ID
string generateVoterID() {
    static bool seeded = false;  // ensures srand runs only once
    if (!seeded) {
        srand(time(0));          // seed random generator with system time
        seeded = true;
    }

    int randomNum = rand() % 9000 + 1000; // generate 4-digit random number
    return "IND-VTR-" + to_string(randomNum);
}

// Function to calculate age using Date of Birth and current system date
int calculateAge(int day, int month, int year) {
    time_t t = time(0);
    tm* now = localtime(&t);

    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    int age = currentYear - year;

    // Adjust age if birthday hasn't occurred yet this year
    if (currentMonth < month || 
       (currentMonth == month && currentDay < day)) {
        age--;
    }

    return age;
}

// Function to validate Date of Birth input
int getValidatedDOB() {
    int day, month, year;

    while (true) {
        cout << "Enter Date of Birth (DD MM YYYY): ";
        cin >> day >> month >> year;

        // Check for non numeric input
        if (cin.fail()) {
            cout << "Invalid input! Please enter numeric values.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Validate date ranges
        if (day < 1 || day > 31 || 
            month < 1 || month > 12 || 
            year < 1900 || year > 2026) {
            cout << "Invalid Date! Please enter correct DOB.\n";
            continue;
        }

        // Calculate age using DOB
        int age = calculateAge(day, month, year);

        // Validate realistic age range
        if (age < 0 || age > 120) {
            cout << "Invalid age calculated. Please re-enter DOB.\n";
            continue;
        }

        return age; 
    }
}

// Function to store eligible voter details in a text file
void storeVoter(string name, int age, string voterID) {
    ofstream file("voter_records.txt", ios::app);   // append mode
    file << "Name: " << name 
         << " | Age: " << age 
         << " | Voter ID: " << voterID << endl;
    file.close();
}

// Function that pauses program and returns to main menu
void returnToMainMenu() {
    int exitChoice;

    cout << "\n------------------------------------\n";

    while (true) {
        cout << "Press 0 to Exit and Return to Main Menu: ";
        cin >> exitChoice;

        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! ";
            continue;
        }

        // Only accept 0 to return
        if (exitChoice == 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        cout << "Invalid input! ";
    }
}

// Main function - program execution starts here
int main() {
    int choice;

    // Menu loop continues until user chooses Exit
    do {
        cout << "\n=========================================\n";
        cout << "          VOTING ELIGIBILITY SYSTEM\n";
        cout << "=========================================\n";
        cout << "1. Verify Voting Eligibility\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Validate menu input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter 1 or 2.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Option 1: Verify voter eligibility
        if (choice == 1) {
            string name, citizenship;

            cout << "\nEnter Name: ";
            getline(cin, name); // allows full name with spaces

            int age = getValidatedDOB(); // get validated age from DOB

            cout << "Enter Citizenship: ";
            cin >> citizenship;

            citizenship = toLowerCase(citizenship); // normalize input

            cout << "\n------------------------------------\n";

            // Check eligibility conditions
            if (age >= 18 && citizenship == "indian") {
                string voterID = generateVoterID();

                cout << "STATUS: ELIGIBLE\n";
                cout << "Calculated Age: " << age << endl;
                cout << "Voter ID Generated: " << voterID << endl;

                // Store voter data in file
                storeVoter(name, age, voterID);
            }
            else {
                cout << "STATUS: NOT ELIGIBLE\n";
                cout << "Calculated Age: " << age << endl;

                // Show reason for ineligibility
                if (age < 18)
                    cout << "Reason: Minimum age is 18.\n";
                else
                    cout << "Reason: Only Indian citizens can vote.\n";
            }

            cout << "------------------------------------\n";

            // Wait for user before returning to main menu
            returnToMainMenu();
        }

        // Option 2: Exit program
        else if (choice == 2) {
            cout << "\nExiting System... Thank You!\n";
        }

        // Handle invalid menu choices
        else {
            cout << "Invalid choice! Please select 1 or 2.\n";
        }

    } while (choice != 2);

    return 0;
}