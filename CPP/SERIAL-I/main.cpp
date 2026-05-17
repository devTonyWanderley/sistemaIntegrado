//  main.cpp
#include <QApplication>
#include "GUISerial.hpp"
#include <filesystem>

int main(int argc, char *argv[])
{
    std::filesystem::create_directories("./TMP/");
    QApplication a(argc, argv);
    GuiSerial w;
    w.setWindowTitle("Título");
    w.show();
    return a.exec();
}
