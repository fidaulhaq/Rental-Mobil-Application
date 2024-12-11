#ifndef CARDATABASE_H
#define CARDATABASE_H

#include "Car.h"
#include <vector>
#include <string>

class CarDatabase {
private:
    std::vector<Car> cars;         // List mobil
    std::string databaseFile;      // Nama file database

    void loadDatabase();           // Memuat data dari file
    void saveDatabase();           // Menyimpan data ke file

public:
    CarDatabase(const std::string& fileName); // Konstruktor dengan parameter
    void addCar(const Car& car);              // Menambahkan mobil ke database
    Car* findCar(const std::string& kode);    // Mencari mobil berdasarkan kode
    void listCars() const;                    // Menampilkan semua mobil
    void updateCarStatus(const std::string& kode, bool sedangDisewakan); // Update status
};

#endif // CARDATABASE_H
