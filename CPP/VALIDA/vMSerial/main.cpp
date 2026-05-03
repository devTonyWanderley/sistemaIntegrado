//	main.cpp
#include <iostream>
#include <iomanip>
#include "../../CORE/DRIVER/SerialModerno.h"

void printHex(std::span<const uint8_t> dados) {
    for (auto b : dados) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << (int)b << " ";
    }
    std::cout << std::dec << std::endl;
}

int main() {
    SerialModerno serial;
    
    // Configuramos para 9600, 8N1 (Padrão Estação Total)
    SerialParams params = { .raw = SerialParams::build(9600, 0, 0, 8) };

    std::cout << "--- Validando SerialModerno (C++20) ---" << std::endl;

    // Abrimos a porta com uma Lambda "capturadora"
    bool ok = serial.Abrir("COM3", params, [](std::span<const uint8_t> dados) {
        std::cout << "[Recebido " << dados.size() << " bytes]: ";
        
        // Exemplo: Se for texto (NMEA/GSI), mostra string. Se for binário, mostra HEX.
        std::string texto(reinterpret_cast<const char*>(dados.data()), dados.size());
        std::cout << texto; 
        
        // Validação extra em HEX para ver se não há perda de frames
        // printHex(dados); 
    });

    if (!ok) {
        std::cerr << "ERRO: Nao foi possivel abrir a COM3. Verifique o cabo!" << std::endl;
        return -1;
    }

    std::cout << "Porta aberta. Digite algo para enviar ou 'sair' para encerrar." << std::endl;

    std::string entrada;
    while (true) {
        std::getline(std::cin, entrada);
        if (entrada == "sair") break;

        // Validando o envio via span do C++20
        auto buffer = std::span(reinterpret_cast<const uint8_t*>(entrada.data()), entrada.size());
        serial.Enviar(buffer);
    }

    std::cout << "Encerrando e limpando threads..." << std::endl;
    return 0;
}
