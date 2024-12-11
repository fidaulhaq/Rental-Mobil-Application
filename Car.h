#ifndef CAR_H
#define CAR_H

#include <string>

class Car {
private:
    std::string kode;
    std::string merk;
    std::string model;
    double hargaSewa;
    bool sedangDisewakan; // Status mobil: true jika sedang disewakan, false jika tersedia

public:
    Car(std::string kode, std::string merk, std::string model, double hargaSewa, bool sedangDisewakan = false);

    std::string getKode() const;
    std::string getMerk() const;
    std::string getModel() const;
    double getHargaSewa() const;
    bool getSedangDisewakan() const;
    void setSedangDisewakan(bool status);

    std::string toString() const;
};

#endif // CAR_H
