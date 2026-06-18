#include <iostream>
#include "Database.h"
using namespace std;


int main() {
        
    Database db("AdoptionStore.db");

    int choice,id;

    while (true) {
        cout << "\n--- Animal Adoption Store ---\n";
        cout << "1. Register a new user\n";
        cout << "2. login\n";
        cout << "your choice :";cin >> choice;

        if (choice == 1) {
            string fname, lname, pass, phone;
            cout << "Enter first name: "; cin >> fname;
            cout << "Enter last name: "; cin >> lname;
            cout << "Enter password: "; cin >> pass;
            cout << "Enter phone number: "; cin >> phone;

            if (db.registerUser(fname, lname, pass, phone)) {
                cout << "Registration successful!\n";
            }
            else {
                cout << "Registration failed (Username might already exist).\n";
            }
        }
        else if (choice == 2) {
            
            string fname, pass;
            cout << "Enter first name: "; cin >> fname;
            cout << "Enter password: "; cin >> pass;

            id = db.loginUser(fname, pass);
            if (id != -1) {
                cout << "login successful!\n";
                
                break;
            }
            else {
                cout << "login failed (wrong data).\n";
                continue;
            }
        }
    }
    while (true) {
        cout << "\n--- Animal Adoption Store ---\n";
        cout << "1. Add a new animal to the store\n";
        cout << "2. Adopt an animal\n";
        cout << "3. Available animals\n";
        cout << "4. Your adopted animals\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

         if (choice == 1) {
            string name, health, type;
            int age;
            cout << "Enter animal name: ";   cin >> name;
            cout << "Enter age: ";           cin >> age;
            cout << "Enter health status: "; cin >> health;
            cout << "Enter type : ";         cin >> type;

            if (db.addAnimal(name, age, health, type)) {
                cout << "Animal added to the store successfully!\n";
            }
        }

        else if (choice == 2) {
            int  animalId;
            cout << "Enter the Animal ID: "; cin >> animalId;

            if (db.adoptAnimal(id, animalId)) {
                cout << "Adoption completed successfully! Congratulations.\n";
            }
            else {
                cout << "Operation failed. Please check the IDs or ensure the animal is not already adopted.\n";
            }
        }
        else if (choice == 3) {
                 db.showAvailableAnimals();
             }

        else if (choice == 4) {
                 db.showMyAdoptedAnimals(id);
             }

        else if (choice == 5) {
            cout << "Exiting the program. Goodbye!\n";
            break;
        }

        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}