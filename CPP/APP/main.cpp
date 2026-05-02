//	main.cpp
#include <QApplication>
#include <QWidget>
#include "PainelConexao.h"

// Exemplo de como o sistema vai "ouvir" a serial na produção
// No futuro, isso alimentará o Parser e não apenas o console
void callbackProducao([[maybe_unused]] uint8_t byte) {
    // Aqui o dado entra no fluxo de processamento do sistema
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Em vez de QWidget window, usamos a sua classe:
    PainelConexao painel;

    painel.setWindowTitle("Ética Construtora - Captura de Campo");
    painel.resize(400, 300); // Ajuste um tamanho inicial razoável
    painel.show();

    return app.exec();
}
