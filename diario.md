**diario.md**
# Diário de Bordo - ECApp

## 30/04/26	21:40
- **Setup do Ambiente**: Projeto configurado no Qt Creator com Qt 6.9.1 e MinGW. 
- **Compilação Inicial**: Sucesso na execução do primeiro binário C++20.
- **Estruturação do Core**:
    - Definido `tipPonto.h` com structs `Ponto` e `MetaPonto` alinhadas.
    - Implementada lógica de coordenadas em décimos de milímetro (Ponto Fixo).
    - Esboçada a arquitetura do `Dicionario.h` para gestão centralizada de strings.
- **Decisão Técnica**: Abandono da libserialport em favor da WinAPI nativa para comunicação com estações totais/GPS.

"Parametrização da Serial via Bitmasking (Idioma WinAPI) novidade com sucesso."
"Decisão de projeto: Implementação de Raw Logging para garantir a integridade dos dados de campo antes do processamento."
"Definição do formato .raw como camada de persistência imediata para garantir a imutabilidade dos dados de campo."
"Finalizada a camada de Driver Serial: Implementação completa de leitura, thread e log binário bruto (Raw Logging) via WinAPI."
"Próxima meta: Arquitetura de tradução offline (Parsers Modulares)."
"Criação da infraestrutura de validação permanente em /VALIDA com lógica de encerramento por timeout de inatividade."
"Arquitetura Bi-Partida: O CORE deve permanecer agnóstico à interface. O uso do Qt é restrito à camada de apresentação para garantir portabilidade futura para WinAPI pura."

C:/DESENV/
├── diretrizes.md               # A "Constituição" do projeto
├── diario.md                   # Registro de progresso e decisões
├── leiame.md
└── CPP/                        # Pasta raiz do código-fonte
    ├── ÉticaConstrutora.pro    # Projeto Qt (qmake)
    ├── main.cpp                # Ponto de entrada (Bootstrap do Qt)
    │
    ├── TYPES/                  # Tipos fundamentais e Dicionário
    │   ├── tipPonto.h          # Structs Ponto e MetaPonto
    │   └── Dicionario.h        # Cabeçalho do motor de strings
    │
    ├── CORE/                   # Onde ficarão as futuras implementações
    │   ├── DRIVER/             # (Vazio) Planejado: WinAPI Serial
    │   ├── PROTOCOL/           # (Vazio) Planejado: Parsers
    │   └── GEOM/               # (Vazio) Planejado: Motores de Cálculo
    │
    └── UI/                     # (Vazio) Planejado: Camada Visual

