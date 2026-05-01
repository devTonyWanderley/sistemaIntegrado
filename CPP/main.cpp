//	main.cpp
#include <QApplication>
#include <QDebug>
#include <QFile>
#include "SerialWin32.h"
#include "DeviceConfigs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Configuração Paramétrica (Baud: 9600, Par: None, Stop: 1, Size: 8)
    SerialParams config;
    config.raw = SerialParams::build(9600, 0, 0, 8);

    SerialWin32 serial;

    qDebug() << "--- Iniciando Teste de Validação ---";

    // 2. Teste de Abertura (Tente abrir uma porta que existe ou use um emulador como com0com)
    // Se não tiver nada conectado, o CreateFileA deve retornar false.
    if (serial.abrir("COM3", config)) {
        qDebug() << "Sucesso: Porta aberta e Thread disparada.";

        // Simulação de tempo de escuta (5 segundos)
        qDebug() << "Aguardando 5 segundos para captura de ruído/dados...";
        Sleep(5000);

        serial.fechar();
        qDebug() << "Sucesso: Porta fechada e Thread encerrada.";
    } else {
        qWarning() << "Falha: Nao foi possivel abrir a COM3 (Verifique se existe ou esta em uso).";
    }

    // 3. Validação do Arquivo Raw
    QFile file("campo_coleta.raw");
    if (file.exists()) {
        qDebug() << "Sucesso: Arquivo 'campo_coleta.raw' foi criado no disco.";
        qDebug() << "Tamanho do arquivo:" << file.size() << "bytes.";
    } else {
        qWarning() << "Erro: O arquivo .raw nao foi gerado.";
    }

    qDebug() << "--- Fim do Teste ---";

    return 0;
}
