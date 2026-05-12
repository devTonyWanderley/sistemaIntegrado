//  main.cpp
#include <QApplication>
#include "GUISerial.hpp"

int main(int argc, char *argv[])
{
    qDebug() << "entrou";
    QApplication a(argc, argv);
    GuiSerial w;
    w.setWindowTitle("Título");
    w.show();
    return a.exec();
}
