#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

class Controller {
public:
    void commandLoop();
    void customer();
    void admin();
    void listMobil();
    void updateMobil();
    void konfirmBayar();
    void ajukanSewa();
    void pembayaran();//belum
    void status();//belum
};

#endif
