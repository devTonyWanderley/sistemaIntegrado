//	main.cpp
#include <QApplication>
#include <QWidget>
#include "../UI/IFSerial.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    IFSerial tela;
    tela.setWindowTitle("Ética Construtora - Captura de Campo");
    tela.resize(400, 300);
    tela.show();
//	[	...		]
    return app.exec();
}
