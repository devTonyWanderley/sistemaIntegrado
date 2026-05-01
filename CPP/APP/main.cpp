//	main.cpp
#include <QApplication>
#include <QWidget>
#include "SerialWin32.h"
#include "Dicionario.h"

// Exemplo de como o sistema vai "ouvir" a serial na produção
// No futuro, isso alimentará o Parser e não apenas o console
void callbackProducao(uint8_t byte) {
    // Aqui o dado entra no fluxo de processamento do sistema
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 1. Inicialização do "Cérebro"
    // Dicionario dicionario;
    // dicionario.prealocar(1024 * 1024, 1000); // 1MB para strings iniciais

    // 2. Configuração da Janela Principal (Qt)
    QWidget janelaPrincipal;
    janelaPrincipal.setWindowTitle("Ética Construtora - Gestão de Obras");
    janelaPrincipal.resize(1024, 768);
    janelaPrincipal.show();

    // O sistema fica aguardando eventos da interface aqui
    return app.exec();
}
