//	TesteIntegracao main.cpp
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include "../../LIB/COM/portaSerialWin.h"

std::vector<std::string> listarCabosConectados()
{
    std::vector<std::string> portas;
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        char nomeValor[255];
        BYTE dadosValor[255];
        DWORD dwIndex = 0, tamNome, tamDados, tipo;
        while (true)
        {
            tamNome = 255;
            tamDados = 255;
            if (RegEnumValueA(hKey, dwIndex, nomeValor, &tamNome, NULL, &tipo, dadosValor, &tamDados) == ERROR_SUCCESS)
            {
                portas.push_back((char*)dadosValor); // Ex: "COM3"
                dwIndex++;
            }
            else break;
        }
        RegCloseKey(hKey);
    }
    return portas;
}

void verificarCodificacao(const char* dados, int32_t tamanho) {
    for (int32_t i = 0; i < tamanho; ++i) {
        unsigned char c = static_cast<unsigned char>(dados[i]);
        if (c >= 32 && c <= 126) std::cout << c;
        else if (c == '\r') std::cout << "[CR]";
        else if (c == '\n') std::cout << "[LF]\n";
        else std::cout << "[0x" << std::hex << (int)c << std::dec << "]";
    }
}

int main()
{
    auto portas = listarCabosConectados();
    if (portas.empty())
    {
        std::cout << "Nenhum cabo serial detectado. Verifique a conexao fisica." << std::endl;
        return -1;
    }
    std::cout << "Cabos detectados: " << std::endl;
    for (size_t i = 0; i < portas.size(); ++i) std::cout << i << " - " << portas[i] << std::endl;
    std::string portaEscolhida = portas[0];
    int numPorta = std::stoi(portaEscolhida.substr(3)); // Converte "COM3" para 3
    PortaSerialWin motor;
    ConfigSerial config{9600, 8, Paridade::Nenhum, StopBits::Um};
    std::cout << "\nTentando conectar em " << portaEscolhida << "..." << std::endl;

    if (motor.conectar(numPorta, config))
    {
        std::cout << "Sucesso! Cabo " << portaEscolhida << " aberto." << std::endl;
        std::cout << "Aguardando transmissao (ASCII)..." << std::endl;
        char buffer[1024];
        auto ultimaRecepcao = std::chrono::steady_clock::now();
        const int limiteInatividadeSegundos = 5;
        while (true)
        {
            int32_t lidos = motor.lerParaBuffer(buffer, sizeof(buffer));
            if (lidos > 0)
            {
                verificarCodificacao(buffer, lidos);
                ultimaRecepcao = std::chrono::steady_clock::now();
            }
            else
            {
                auto agora = std::chrono::steady_clock::now();
                auto decorrido = std::chrono::duration_cast<std::chrono::seconds>(agora - ultimaRecepcao).count();
                if (decorrido >= limiteInatividadeSegundos)
                {
                    std::cout << "\n\n[INFO] Tempo limite atingido. Encerrando recepcao com seguranca..." << std::endl;
                    break; // Sai do loop
                }
            }
            Sleep(10);
        }
    }
    else std::cout << "Erro ao abrir " << portaEscolhida << ". Porta em uso ou cabo desconectado." << std::endl;
    motor.desconectar();
    std::cout << "Porta fechada com sucesso." << std::endl;
	return 0;
}