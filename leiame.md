# Sistema de Desenvolvimento Híbrido (Topografia & Gestão)

## 📌 Visão Geral
Repositório central focado em precisão matemática absoluta. O núcleo (Core) é desenvolvido em C++ Nativo (Standard Library) para garantir portabilidade e performance.

---

## 🛠 Configuração do Ambiente
- **Linguagem:** C++ Nativo (Padrão ISO/IEC 14882 - STL).
- **Bibliotecas:** Uso prioritário de `<vector>`, `<string>`, `<cstdint>`, `<fstream>`.
- **Unidade Base:** Décimo de milímetro (0.1 mm) usando `int64_t`.
- **Aritmética:** Inteiros e Deslocamento de Bits (Bit Shifting).
- **Interface:** Qt 6.x restrito apenas à camada de UI (janelas e eventos).

---

## 📂 Estrutura de Pastas (Mapa)

C:/DESENV/
├── leiame.md                   # O presente arquivo (Memória Técnica).
├── CPP/                        # Tudo em C++ / Qt
│   ├── TST/                    # Teste e validação
│   ├── TOP/                    # Projetos de Topografia (Cálculos de Campo).
│   ├── ADM/                    # Projetos de Gestão (Orçamento e Obras).
│   └── LIB/                    # Bibliotecas Compartilhadas (Core C++ Nativo)
│       ├── MAT/                # Matemática: Cálculos via std::int64_t.
│       ├── COM/                # Comunicação: Hardware (Serial via C Nativo/WinAPI).
│       ├── ARQ/                # Arquivos: Parsers DXF/CSV via std::fstream e std::string.
│       ├── TIP/                # Tipos: Structs de Pontos e definições de escala.
│       ├── CAD/                # Motor Gráfico: Lógica de desenho independente de UI.
│       └── GUI/                # Interface: Único local onde o framework Qt é permitido.
└── LSP/                        # Biblioteca de rotinas .lsp
    ├── UTILS/                  # Utilidades: Camadas e Textos.
    ├── TOP/                    # Topografia: Pontos e Perfis.
    ├── DES/                    # Desenho: Automação de Pranchas.
    └── INT/                    # Integração: Exportação Lisp -> C++.

## .. executado:

C:/DESENV/
├── leiame.md                   # O presente arquivo (Memória Técnica).
├── CPP/                        # Tudo em C++ / Qtntrole).
│   ├── TST/                    # Teste e validação
│   └── LIB/                    # Bibliotecas Compartilhadas
│       └── COM/                # Comunicação: Hardware (Serial via C Nativo/WinAPI).
│	        ├── MotorComunicacao.h
│	        └── PortaSerialWin.h
**25/04/26 13:43

---

## 📏 Padrões de Nomenclatura (Regras Definitivas)

### 1. Escopo e Variáveis (Sem Sublinhas)


| Entidade | Padrão | Exemplo |
| :--- | :--- | :--- |
| **Classe / Struct** | **PascalCase** | `class CalculoVolume` |
| **Variável Membro** | **m** + PascalCase | `mDistanciaDecimetros` |
| **Constantes** | **k** + PascalCase | `kFatorEscalaBits` |
| **Parâmetro / Local**| **camelCase** | `cotaEntrada` |
| **Arquivos** | **PascalCase** | `ProcessadorDxf.cpp` |

### 2. Unidades e Processamento Nativo
- **Inteiros:** Uso obrigatório de `int64_t` (da `<cstdint>`) para coordenadas.
- **Buffers de Dados:** Usar `std::vector<uint8_t>` ou `std::string` para leitura de hardware/arquivos.
- **Aislamiento:** O código em `LIB/MAT`, `LIB/ARQ` e `LIB/COM` não deve incluir headers do Qt (`#include <Q...>`), garantindo que o processamento seja C++ Puro.

---

## 📝 Notas de Versão e Decisões
- **25/04/26:** Definição do uso de C++ Standard para o Core.
- **Decisão:** O Qt será usado como uma "casca" para a interface, mas os dados trafegam e são processados via Standard C++.
- **Decisão:** Aritmética de inteiros para evitar erros de ponto flutuante em grandes coordenadas.

---
*Documento atualizado em: [INSIRA A DATA]*
