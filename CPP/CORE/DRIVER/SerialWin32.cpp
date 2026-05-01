//  SerialWin32.cpp
#include "SerialWin32.h"
#include <iostream>

SerialWin32::SerialWin32(): m_hSerial(INVALID_HANDLE_VALUE), m_threadH(NULL), m_ativo(false){}

SerialWin32::~SerialWin32(){fechar();}

bool SerialWin32::abrir(const char* porta, const SerialParams& params)
{
    // 1. Abre o túnel com o driver do Windows
    m_hSerial = CreateFileA(porta, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (m_hSerial == INVALID_HANDLE_VALUE) return false;

    // 2. Prepara a estrutura de controle (DCB)
    DCB dcb{};
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(m_hSerial, &dcb)) return false;

    // 3. Injeta a parametrização via bitmasking (Idioma WinAPI)
    // Aqui o custo de CPU é quase zero: apenas deslocamento de bits.
    dcb.BaudRate = params.winBaud();   // Extrai os 16 bits iniciais
    dcb.Parity   = params.winParity(); // Extrai os bits de paridade
    dcb.StopBits = params.winStop();   // Extrai os bits de parada
    dcb.ByteSize = params.winSize();   // Extrai o tamanho do byte

    // Desativa controles de fluxo de hardware para evitar bloqueios por cabo
    dcb.fOutxCtsFlow = FALSE;
    dcb.fRtsControl  = RTS_CONTROL_ENABLE;

    // Aplica a configuração no chip UART
    if (!SetCommState(m_hSerial, &dcb)) return false;

    // 4. Configura os Timeouts (Crucial para o modo "nervoso" da Thread)
    COMMTIMEOUTS timeouts{};
    timeouts.ReadIntervalTimeout = MAXDWORD; // Retorna imediatamente o que tiver

    timeouts.ReadTotalTimeoutMultiplier  = 0;
    timeouts.ReadTotalTimeoutConstant    = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant   = 0;

    if (!SetCommTimeouts(m_hSerial, &timeouts)) return false;

    // 5. Cria o arquivo .raw (Soberania do dado bruto)
    // Usamos um nome genérico ou baseado na porta para o teste
    m_hArquivoRaw = CreateFileA("campo_coleta.raw", GENERIC_WRITE, FILE_SHARE_READ, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (m_hArquivoRaw == INVALID_HANDLE_VALUE) return false;

    // 6. Dispara o "Vigia" (Thread)
    m_ativo = true;
    m_threadH = CreateThread(NULL, 0, threadLeitura, this, 0, NULL);

    if (m_threadH == NULL)
    {
        fechar(); // Limpa tudo se a thread falhar
        return false;
    }


    return true;
}

void SerialWin32::fechar()
{
    // 1. Sinaliza para o loop da thread parar
    m_ativo = false;

    // 2. Se a thread existir, espera ela encerrar (timeout de 2s para segurança)
    if (m_threadH != NULL)
    {
        WaitForSingleObject(m_threadH, 2000);
        CloseHandle(m_threadH);
        m_threadH = NULL;
    }

    // 3. Fecha o acesso ao hardware
    if (m_hSerial != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hSerial);
        m_hSerial = INVALID_HANDLE_VALUE;
    }
    // 4. Fecha o arquivo de log bruto
    if (m_hArquivoRaw != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hArquivoRaw);
        m_hArquivoRaw = INVALID_HANDLE_VALUE;
    }
}

DWORD WINAPI SerialWin32::threadLeitura(LPVOID param)
{
    // Recupera a instância da classe
    SerialWin32* instancia = static_cast<SerialWin32*>(param);

    uint8_t byteLido;
    DWORD bytesRecebidos;

    while (instancia->m_ativo)
    {
        // Tentativa de leitura de 1 byte (Modo não-bloqueante devido aos Timeouts)
        if (ReadFile(instancia->m_hSerial, &byteLido, 1, &bytesRecebidos, NULL))
        {
            if (bytesRecebidos > 0)
            {
                DWORD bytesEscritos;
                // Escrita imediata no disco via WinAPI
                WriteFile(instancia->m_hArquivoRaw, &byteLido, 1, &bytesEscritos, NULL);

                // Opcional: FlushFileBuffers(instancia->m_hArquivoRaw);
                // Use o flush apenas se a queda de energia for um risco crítico,
                // pois reduz um pouco a vida útil do SSD/SD.

                //------------------------------------------------------------
                //  --PRA VISUALIZAR TESTE--
                // 2. Visualização (Para validação)
                // Se o byte for imprimível, mostra o caractere; senão, mostra o valor hexadecimal
                if (byteLido >= 32 && byteLido <= 126)
                    std::cout << (char)byteLido;
                else if (byteLido == '\r') std::cout << "\\r";
                else if (byteLido == '\n') std::cout << "\\n\n"; // Quebra linha no console
                else std::cout << "[0x" << std::hex << (int)byteLido << "]";

                std::cout.flush(); // Garante que apareça na tela na hora
                //------------------------------------------------------------
            }
        }

        // Sleep(1) evita que o núcleo da CPU rode a 100% em idle.
        // 1ms de latência é irrelevante para a velocidade da Serial (9600-115200 bps).
        Sleep(1);
    }
    return 0;
}
