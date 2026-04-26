//  motorComunicacao.h
#pragma once
#include <cstdint>

enum class Paridade { Nenhum = 0, Impar = 1, Par = 2 };
enum class StopBits { Um = 0, UmMeio = 1, Dois = 2 };

struct ConfigSerial {
    uint32_t baudRate;
    uint8_t byteSize;  // Geralmente 8
    Paridade paridade;
    StopBits stopBits;
};

class MotorComunicacao
{
public:
    virtual ~MotorComunicacao() {}  // Retorna o número de bytes lidos. 0 se vazio, -1 se erro.
    virtual int32_t lerParaBuffer(char* bufferDestino, uint32_t tamanhoMaximo) = 0;
    virtual bool enviarDoBuffer(const char* bufferOrigem, uint32_t tamanho) = 0;
    virtual bool conectar(int portaNumero, const ConfigSerial& config) = 0;
    virtual void desconectar() = 0;
    virtual bool estaConectado() const = 0;
};
//Q_DECLARE_METATYPE(ConfigSerial)