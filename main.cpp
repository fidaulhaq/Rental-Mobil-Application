#include "Controller.h"
#include "CarDatabase.h"

CarDatabase carDb("database.txt"); // Global object untuk mengelola database

int main() {
    Controller controller;
    controller.commandLoop();
    return 0;
}
