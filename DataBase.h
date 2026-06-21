#pragma once
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <iomanip>

using namespace std;

class Database {
private:
    sqlite3* DB;

public:
    Database(const char* db_name) {
        if (sqlite3_open(db_name, &DB) != SQLITE_OK) {
            cerr << "Error: Failed to open database!" << endl;
        }
        else {
            sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", NULL, 0, NULL);
            createTables();
        }
    }

    ~Database() {
        sqlite3_close(DB);
    }

    void createTables() {
        string usersTable =
            "CREATE TABLE IF NOT EXISTS Users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "firstName TEXT NOT NULL, "
            "lastName TEXT, "
            "password TEXT NOT NULL, "
            "phone TEXT NOT NULL,"
            "Email TEXT NOT NULL UNIQUE);";

        string animalsTable =
            "CREATE TABLE IF NOT EXISTS Animals ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "age REAL NOT NULL, "
            "health_status TEXT NOT NULL, "
            "type TEXT NOT NULL, "
            "user_id INTEGER, "
            "FOREIGN KEY(user_id) REFERENCES Users(id));";

        sqlite3_exec(DB, usersTable.c_str(), NULL, 0, NULL);
        sqlite3_exec(DB, animalsTable.c_str(), NULL, 0, NULL);
    }

    int loginUser(string email, string password) {
        string query = "SELECT id FROM Users WHERE Email = '" + email + "' AND password = '" + password + "';";

        sqlite3_stmt* stmt;
        int userId = -1;

        if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                userId = sqlite3_column_int(stmt, 0);
            }
        }

        sqlite3_finalize(stmt);
        return userId;
    }

    bool registerUser(string firstName, string lastName, string password, string phone, string email) {
        string query = "INSERT INTO Users (firstName, lastName, password, phone,Email) VALUES ('"
            + firstName + "', '" + lastName + "', '" + password + "', '" + phone + "', '" + email + "');";

        int result = sqlite3_exec(DB, query.c_str(), NULL, 0, NULL);
        return (result == SQLITE_OK);
    }

    string getUserName(int userId) {
        string query = "SELECT firstName || ' ' || lastName FROM Users WHERE id = " + to_string(userId) + ";";
        sqlite3_stmt* stmt;
        string userName = "";

        if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                userName = (char*)sqlite3_column_text(stmt, 0);
            }
        }

        sqlite3_finalize(stmt);
        return userName;
    }

    bool addAnimal(string name, float age, string health, string type) {
        string query = "INSERT INTO Animals (name, age, health_status, type, user_id) VALUES ('"
            + name + "', " + to_string(age) + ", '" + health + "', '" + type + "', NULL);";

        int result = sqlite3_exec(DB, query.c_str(), NULL, 0, NULL);
        return (result == SQLITE_OK);
    }

    bool adoptAnimal(int userId, int animalId) {
        string query = "UPDATE Animals SET user_id = " + to_string(userId) +
            " WHERE id = " + to_string(animalId) + " AND user_id IS NULL;";

        int result = sqlite3_exec(DB, query.c_str(), NULL, 0, NULL);
        if (result == SQLITE_OK) {
            int rowsAffected = sqlite3_changes(DB);

            if (rowsAffected > 0) {
                return true;
            }
            else {
                return false;
            }
        }
        return false;
    }
    void showAvailableAnimals() {
        string query = "SELECT id, name, age, health_status, type FROM Animals WHERE user_id IS NULL;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            cout << "\n======================== Available Animals ========================\n";
            cout << " " << left << setw(6) << "ID"
                << "| " << setw(15) << "Name"
                << "| " << setw(8) << "Age"
                << "| " << setw(15) << "Health Status"
                << "| " << setw(12) << "Type" << "\n";
            cout << "-------------------------------------------------------------------\n";

            bool found = false;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                found = true;
                int id = sqlite3_column_int(stmt, 0);
                string name = (char*)sqlite3_column_text(stmt, 1);
                float age = sqlite3_column_double(stmt, 2);
                string health = (char*)sqlite3_column_text(stmt, 3);
                string type = (char*)sqlite3_column_text(stmt, 4);

                cout << " " << left << setw(6) << id
                    << "| " << setw(15) << name
                    << "| " << setw(8) << age
                    << "| " << setw(15) << health
                    << "| " << setw(12) << type << "\n";
            }
            if (!found) {
                cout << "No animals available for adoption right now!\n";
            }
            cout << "===================================================================\n";
        }
        sqlite3_finalize(stmt);
    }

    void showMyAdoptedAnimals(int userId) {
        string query = "SELECT id, name, age, health_status, type FROM Animals WHERE user_id = " + to_string(userId) + ";";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            cout << "\n===================== Your Adopted Animals ========================\n";
            cout << " " << left << setw(6) << "ID"
                << "| " << setw(15) << "Name"
                << "| " << setw(8) << "Age"
                << "| " << setw(15) << "Health Status"
                << "| " << setw(12) << "Type" << "\n";
            cout << "-------------------------------------------------------------------\n";

            bool found = false;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                found = true;
                int id = sqlite3_column_int(stmt, 0);
                string name = (char*)sqlite3_column_text(stmt, 1);
                float age = sqlite3_column_double(stmt, 2);
                string health = (char*)sqlite3_column_text(stmt, 3);
                string type = (char*)sqlite3_column_text(stmt, 4);

                cout << " " << left << setw(6) << id
                    << "| " << setw(15) << name
                    << "| " << setw(8) << age
                    << "| " << setw(15) << health
                    << "| " << setw(12) << type << "\n";
            }
            if (!found) {
                cout << "You haven't adopted any animals yet!\n";
            }
            cout << "===================================================================\n";
        }
        sqlite3_finalize(stmt);
    }
};
