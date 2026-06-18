#pragma once
#include <iostream>
#include <sqlite3.h>
#include <string>

class Database {
private:
    sqlite3* DB;

public:
    Database(const char* db_name) {
        if (sqlite3_open(db_name, &DB) != SQLITE_OK) {
            std::cerr << "Error: Failed to open database!" << std::endl;
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
        std::string usersTable =
            "CREATE TABLE IF NOT EXISTS Users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "fristNmae TEXT NOT NULL UNIQUE, "
            "lastName TEXT, "
            "password TEXT NOT NULL, "
            "phone TEXT NOT NULL);";

        std::string animalsTable =
            "CREATE TABLE IF NOT EXISTS Animals ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "age INTEGER NOT NULL, "
            "health_status TEXT NOT NULL, "
            "type TEXT NOT NULL, "
            "user_id INTEGER, "
            "FOREIGN KEY(user_id) REFERENCES Users(id));";

        sqlite3_exec(DB, usersTable.c_str(), NULL, 0, NULL);
        sqlite3_exec(DB, animalsTable.c_str(), NULL, 0, NULL);
    }

    int loginUser(std::string firstName, std::string password) {
        std::string query = "SELECT id FROM Users WHERE fristNmae = '" + firstName + "' AND password = '" + password + "';";

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

    bool registerUser(std::string firstName, std::string lastName, std::string password, std::string phone) {
        std::string query = "INSERT INTO Users (fristNmae, lastName, password, phone) VALUES ('"
            + firstName + "', '" + lastName + "', '" + password + "', '" + phone + "');";

        int result = sqlite3_exec(DB, query.c_str(), NULL, 0, NULL);
        return (result == SQLITE_OK);
    }

    bool addAnimal(std::string name, int age, std::string health, std::string type) {
        std::string query = "INSERT INTO Animals (name, age, health_status, type, user_id) VALUES ('"
            + name + "', " + std::to_string(age) + ", '" + health + "', '" + type + "', NULL);";

        int result = sqlite3_exec(DB, query.c_str(), NULL, 0, NULL);
        return (result == SQLITE_OK);
    }

    bool adoptAnimal(int userId, int animalId) {
        std::string query = "UPDATE Animals SET user_id = " + std::to_string(userId) +
            " WHERE id = " + std::to_string(animalId) + " AND user_id IS NULL;";

        int result = sqlite3_exec(DB, query.c_str(), NULL, 0, NULL);
        return (result == SQLITE_OK);
    }
    void showAvailableAnimals() {
        std::string query = "SELECT id, name, age, health_status, type FROM Animals WHERE user_id IS NULL;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            std::cout << "\n================ Available Animals ================\n";
            std::cout << "ID\t| Name\t| Age\t| Health Status\t| Type\n";
            std::cout << "---------------------------------------------------\n";

            bool found = false;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                found = true;
                int id = sqlite3_column_int(stmt, 0);
                std::string name = (char*)sqlite3_column_text(stmt, 1);
                int age = sqlite3_column_int(stmt, 2);
                std::string health = (char*)sqlite3_column_text(stmt, 3);
                std::string type = (char*)sqlite3_column_text(stmt, 4);

                std::cout << id << "\t| " << name << "\t| " << age << "\t| " << health << "\t| " << type << "\n";
            }
            if (!found) {
                std::cout << "No animals available for adoption right now!\n";
            }
            std::cout << "===================================================\n";
        }
        sqlite3_finalize(stmt);
    }

    void showMyAdoptedAnimals(int userId) {
        std::string query = "SELECT id, name, age, health_status, type FROM Animals WHERE user_id = " + std::to_string(userId) + ";";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            std::cout << "\n================ Your Adopted Animals ================\n";
            std::cout << "ID\t| Name\t| Age\t| Health Status\t| Type\n";
            std::cout << "------------------------------------------------------\n";

            bool found = false;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                found = true;
                int id = sqlite3_column_int(stmt, 0);
                std::string name = (char*)sqlite3_column_text(stmt, 1);
                int age = sqlite3_column_int(stmt, 2);
                std::string health = (char*)sqlite3_column_text(stmt, 3);
                std::string type = (char*)sqlite3_column_text(stmt, 4);

                std::cout << id << "\t| " << name << "\t| " << age << "\t| " << health << "\t| " << type << "\n";
            }
            if (!found) {
                std::cout << "You haven't adopted any animals yet!\n";
            }
            std::cout << "======================================================\n";
        }
        sqlite3_finalize(stmt);
    }
};