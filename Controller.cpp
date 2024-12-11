#include "Controller.h"
#include "CarDatabase.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>

extern CarDatabase carDb;

void Controller::commandLoop() {
    char choice;
    do {
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "1. Customer" << std::endl;
        std::cout << "2. Admin" << std::endl;
        std::cout << "Press any other key to exit." << std::endl;
        choice = getch();

        switch (choice) {
            case '1': customer(); break;
            case '2': admin(); break;
            default: std::cout << "Exiting program." << std::endl; break;
        }
    } while (choice == '1' || choice == '2');
}

void Controller::customer() {
    char choice;
    do {
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "1. List Mobil" << std::endl;
        std::cout << "2. Ajukan Sewa Mobil" << std::endl;
        std::cout << "3. Pembayaran" << std::endl;
        std::cout << "4. Status" << std::endl;
        std::cout << "Press any other key to exit." << std::endl;
        choice = getch();

        switch (choice) {
            case '1': listMobil(); break;
            case '2': ajukanSewa(); break;
            case '3': pembayaran(); break;
            case '4': status(); break;

            default: std::cout << "Exiting customer menu." << std::endl; break;
        }
    } while (choice == '1' || choice == '2' || choice == '3'|| choice == '4');
}

void Controller::admin() {
    char choice;
    do {
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "1. List Mobil" << std::endl;
        std::cout << "2. Update Mobil" << std::endl;
        std::cout << "3. Konfirmasi Pembayaran" << std::endl;
        std::cout << "Press any other key to exit." << std::endl;
        choice = getch();

        switch (choice) {
            case '1': listMobil(); break;
            case '2': updateMobil(); break;
            case '3': konfirmBayar(); break;
            default: std::cout << "Exiting admin menu." << std::endl; break;
        }
    } while (choice == '1' || choice == '2' || choice == '3');
}

void Controller::listMobil() {
    carDb.listCars();
}

void Controller::updateMobil() {
    std::string kode, merk, model;
    double harga;
    std::cout << "Masukkan Kode Mobil: "; std::cin >> kode;
    std::cout << "Masukkan Merk: "; std::cin >> merk;
    std::cout << "Masukkan Model: "; std::cin >> model;
    std::cout << "Masukkan Harga Sewa: "; std::cin >> harga;

    Car newCar(kode, merk, model, harga);
    carDb.addCar(newCar);
    std::cout << "Mobil berhasil ditambahkan ke database." << std::endl;
}

void Controller::ajukanSewa() {
    std::string kode, nama, noHP, alamat;
    int durasi;

    std::cout << "Masukkan kode mobil yang ingin disewa: ";
    std::cin >> kode;

    Car* car = carDb.findCar(kode);
    if (car && !car->getSedangDisewakan()) {
        std::cout << "Mobil ditemukan: " << car->getMerk() << " " << car->getModel()
                  << " dengan harga Rp " << car->getHargaSewa() << "/hari." << std::endl;

        std::cout << "Masukkan nama penyewa: "; std::cin.ignore(); std::getline(std::cin, nama);
        std::cout << "Masukkan nomor HP: "; std::getline(std::cin, noHP);
        std::cout << "Masukkan alamat: "; std::getline(std::cin, alamat);
        std::cout << "Masukkan durasi sewa (hari): "; std::cin >> durasi;

        // Buat file struk penyewaan
        std::ofstream strukFile(nama + ".txt");
        strukFile << "Nama: " << nama << "\nNo HP: " << noHP
                  << "\nAlamat: " << alamat << "\nKode Mobil: " << kode
                  << "\nDurasi: " << durasi << " hari"
                  << "\nHarga: " << car->getHargaSewa()
                  << "\nStatus Pembayaran: false"
                  << "\nKonfirmasi Admin: false" << std::endl;
        strukFile.close();

        std::cout << "Pengajuan sewa berhasil dicatat." << std::endl;
    } else if (car && car->getSedangDisewakan()) {
        std::cout << "Mobil dengan kode " << kode << " sedang disewakan." << std::endl;
    } else {
        std::cout << "Mobil tidak ditemukan." << std::endl;
    }
}

void Controller::pembayaran() {
    std::string nama;
    std::cout << "Masukkan nama penyewa untuk membayar: ";
    //std::cin.ignore();
    //std::getline(std::cin, nama);//perbarui
    std::getline(std::cin >> std::ws, nama);

    // Nama file yang sesuai
    std::string fileName = nama + ".txt";

    // Membuka file untuk membaca
    std::ifstream strukFileIn(fileName);
    if (!strukFileIn.is_open()) {
        std::cerr << "Struk penyewaan tidak ditemukan: " << fileName << std::endl;
        return;
    }

    // Membaca isi file
    std::ostringstream buffer;
    buffer << strukFileIn.rdbuf();
    std::string fileContent = buffer.str();
    strukFileIn.close();

    // Cari dan perbarui status pembayaran
    size_t statusPos = fileContent.find("Status Pembayaran: ");
    if (statusPos == std::string::npos) {
        std::cerr << "Format file tidak valid: Tidak ditemukan 'Status Pembayaran'." << std::endl;
        return;
    }

    size_t nextLinePos = fileContent.find('\n', statusPos);
    if (nextLinePos == std::string::npos) nextLinePos = fileContent.length();

    // Perbarui status pembayaran menjadi 'true'
    std::string updatedContent = fileContent.substr(0, statusPos)
                                + "Status Pembayaran: true"
                                + fileContent.substr(nextLinePos);

    // Menulis ulang file dengan status pembayaran diperbarui
    std::ofstream strukFileOut(fileName);
    if (!strukFileOut.is_open()) {
        std::cerr << "Gagal membuka file untuk menulis: " << fileName << std::endl;
        return;
    }

    strukFileOut << updatedContent;
    strukFileOut.close();

    std::cout << "Pembayaran berhasil dicatat dan diperbarui untuk penyewa: " << nama << std::endl;
}

void Controller::status() {
    std::string namaPenyewa;
    std::cout << "Masukkan nama penyewa (nama file tanpa .txt): ";
    std::getline(std::cin >> std::ws, namaPenyewa);

    std::string fileName = namaPenyewa + ".txt";

    // Membuka file untuk membaca
    std::ifstream fileIn(fileName);
    if (!fileIn.is_open()) {
        std::cerr << "File tidak ditemukan: " << fileName << std::endl;
        return;
    }

    // Membaca isi file
    std::ostringstream buffer;
    buffer << fileIn.rdbuf();
    std::string fileContent = buffer.str();
    fileIn.close();

    // Menampilkan seluruh informasi dari struk
    std::cout << "\nInformasi Penyewaan:\n" << fileContent << std::endl;

//edit kebawah

    // Mengambil informasi harga dan durasi
    size_t hargaPos = fileContent.find("Harga: ");
    size_t durasiPos = fileContent.find("Durasi: ");

    if (hargaPos == std::string::npos || durasiPos == std::string::npos) {
        std::cerr << "Format file tidak valid: Informasi harga atau durasi tidak ditemukan." << std::endl;
        return;
    }

    size_t hargaEnd = fileContent.find('\n', hargaPos);
    size_t durasiEnd = fileContent.find('\n', durasiPos);

    double hargaSewa = std::stod(fileContent.substr(hargaPos + 7, hargaEnd - (hargaPos + 7)));
    int durasi = std::stoi(fileContent.substr(durasiPos + 8, durasiEnd - (durasiPos + 8)));

    double jumlahBayar = hargaSewa * durasi;
    std::cout << "\nJumlah yang harus dibayarkan: Rp " << jumlahBayar << std::endl;
}



void Controller::konfirmBayar() {
    std::string namaPenyewa;
    std::cout << "Masukkan nama penyewa (nama file tanpa .txt): ";
    std::getline(std::cin >> std::ws, namaPenyewa); // Bersihkan whitespace sebelum membaca input

    std::string fileName = namaPenyewa + ".txt";

    // Membuka file untuk membaca
    std::ifstream fileIn(fileName);
    if (!fileIn.is_open()) {
        std::cerr << "File tidak ditemukan: " << fileName << std::endl;
        return;
    }

    // Membaca isi file
    std::ostringstream buffer;
    buffer << fileIn.rdbuf();
    std::string fileContent = buffer.str();
    fileIn.close();

    // Cari dan tampilkan status pembayaran
    size_t pembayaranPos = fileContent.find("Status Pembayaran: ");
    if (pembayaranPos == std::string::npos) {
        std::cerr << "Format file tidak valid: Tidak ditemukan 'Status Pembayaran'." << std::endl;
        return;
    }

    size_t pembayaranEnd = fileContent.find('\n', pembayaranPos);
    if (pembayaranEnd == std::string::npos) pembayaranEnd = fileContent.length();

    std::string statusPembayaran = fileContent.substr(pembayaranPos + 19, pembayaranEnd - (pembayaranPos + 19));
    std::cout << "Status Pembayaran: " << statusPembayaran << std::endl;

    // Cari dan tampilkan status konfirmasi admin
    size_t konfirmasiPos = fileContent.find("Konfirmasi Admin: ");
    if (konfirmasiPos == std::string::npos) {
        std::cerr << "Format file tidak valid: Tidak ditemukan 'Konfirmasi Admin'." << std::endl;
        return;
    }

    size_t konfirmasiEnd = fileContent.find('\n', konfirmasiPos);
    if (konfirmasiEnd == std::string::npos) konfirmasiEnd = fileContent.length();

    std::string statusKonfirmasi = fileContent.substr(konfirmasiPos + 18, konfirmasiEnd - (konfirmasiPos + 18));
    std::cout << "Konfirmasi Admin: " << statusKonfirmasi << std::endl;

    // Konfirmasi status pembayaran
    if (statusPembayaran == "false") {
        std::cout << "Pembayaran belum dilakukan. Tidak dapat mengonfirmasi." << std::endl;
        return;
    }

    char konfirmasiInput;
    std::cout << "Konfirmasi admin untuk menyewakan mobil? (y/n): ";
    std::cin >> konfirmasiInput;

    if (konfirmasiInput == 'y' || konfirmasiInput == 'Y') {
        // Update status konfirmasi admin
        std::string updatedContent = fileContent.substr(0, konfirmasiPos)
                                    + "Konfirmasi Admin: true"
                                    + fileContent.substr(konfirmasiEnd);

        // Tulis ulang file
        std::ofstream fileOut(fileName);
        if (!fileOut.is_open()) {
            std::cerr << "Gagal membuka file untuk menulis: " << fileName << std::endl;
            return;
        }

        fileOut << updatedContent;
        fileOut.close();

        // Cari kode mobil
        size_t kodePos = fileContent.find("Kode Mobil: ");
        if (kodePos != std::string::npos) {
            size_t kodeEnd = fileContent.find('\n', kodePos);
            if (kodeEnd == std::string::npos) kodeEnd = fileContent.length();

            std::string kodeMobil = fileContent.substr(kodePos + 12, kodeEnd - (kodePos + 12));

            // Update status mobil di database
            carDb.updateCarStatus(kodeMobil, true);
        }

        std::cout << "Konfirmasi admin berhasil diperbarui dan status mobil diupdate." << std::endl;
    } else {
        std::string updatedContent = fileContent.substr(0, konfirmasiPos)
                                    + "Konfirmasi Admin: false"
                                    + fileContent.substr(konfirmasiEnd);

        std::ofstream fileOut(fileName);
        if (!fileOut.is_open()) {
            std::cerr << "Gagal membuka file untuk menulis: " << fileName << std::endl;
            return;
        }

        fileOut << updatedContent;
        fileOut.close();

        // Cari kode mobil
        size_t kodePos = fileContent.find("Kode Mobil: ");
        if (kodePos != std::string::npos) {
            size_t kodeEnd = fileContent.find('\n', kodePos);
            if (kodeEnd == std::string::npos) kodeEnd = fileContent.length();

            std::string kodeMobil = fileContent.substr(kodePos + 12, kodeEnd - (kodePos + 12));

            // Update status mobil di database
            carDb.updateCarStatus(kodeMobil, false);
        }

        std::cout << "Konfirmasi dibatalkan, status mobil dikembalikan menjadi tersedia." << std::endl;
    }
}
