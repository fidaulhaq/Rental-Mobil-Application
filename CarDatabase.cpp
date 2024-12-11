#include "CarDatabase.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Konstruktor
CarDatabase::CarDatabase(const std::string& fileName) : databaseFile(fileName) {
    loadDatabase();
}

// Memuat data dari file database
void CarDatabase::loadDatabase() {
    std::ifstream file(databaseFile);
    if (!file.is_open()) {
        std::cout << "Database file not found. Starting with an empty database." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string kode, merk, model;
        double hargaSewa;
        bool sedangDisewakan;

        if (iss >> kode >> merk >> model >> hargaSewa >> sedangDisewakan) {
            cars.emplace_back(kode, merk, model, hargaSewa, sedangDisewakan);
        }
    }

    file.close();
}

// Menyimpan data ke file database
void CarDatabase::saveDatabase() {
    std::ofstream file(databaseFile, std::ios::trunc);
    for (const auto& car : cars) {
        file << car.getKode() << " "
             << car.getMerk() << " "
             << car.getModel() << " "
             << car.getHargaSewa() << " "
             << car.getSedangDisewakan() << std::endl;
    }
    file.close();
}

// Menambahkan mobil ke database
void CarDatabase::addCar(const Car& car) {
    cars.push_back(car);
    saveDatabase(); // Simpan perubahan
}

// Mencari mobil berdasarkan kode
Car* CarDatabase::findCar(const std::string& kode) {
    for (auto& car : cars) {
        if (car.getKode() == kode) {
            return &car;
        }
    }
    return nullptr;
}

// Menampilkan semua mobil
void CarDatabase::listCars() const {
    if (cars.empty()) {
        std::cout << "Tidak ada mobil di database." << std::endl;
        return;
    }

    for (const auto& car : cars) {
        std::cout << car.toString() << std::endl;
    }
}

// Mengupdate status mobil (tersedia atau sedang disewakan)
void CarDatabase::updateCarStatus(const std::string& kode, bool sedangDisewakan) {
    Car* car = findCar(kode);
    if (car) {
        car->setSedangDisewakan(sedangDisewakan);
        saveDatabase(); // Simpan perubahan
    } else {
        std::cout << "Mobil dengan kode " << kode << " tidak ditemukan." << std::endl;
    }
}
