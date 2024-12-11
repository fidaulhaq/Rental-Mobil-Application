#include "Car.h"
#include <sstream>

Car::Car(std::string kode, std::string merk, std::string model, double hargaSewa, bool sedangDisewakan)
    : kode(kode), merk(merk), model(model), hargaSewa(hargaSewa), sedangDisewakan(sedangDisewakan) {}

std::string Car::getKode() const { return kode; }
std::string Car::getMerk() const { return merk; }
std::string Car::getModel() const { return model; }
double Car::getHargaSewa() const { return hargaSewa; }
bool Car::getSedangDisewakan() const { return sedangDisewakan; }
void Car::setSedangDisewakan(bool status) { sedangDisewakan = status; }

std::string Car::toString() const {
    std::ostringstream oss;
    oss << "Kode: " << kode
        << ", Merk: " << merk
        << ", Model: " << model
        << ", Harga Sewa: Rp " << hargaSewa
        << ", Status: " << (sedangDisewakan ? "Sedang Disewakan" : "Tersedia");
    return oss.str();
}
