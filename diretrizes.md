**diretrizes.md**
# Diretrizes de Projeto - Ética Construtora (ECApp)

## 1. Filosofia de Desenvolvimento
- **Minimalismo de Terceiros**: Usar o mínimo possível de bibliotecas externas. Priorizar WinAPI e C++ Nativo.
- **Soberania do Código**: O desenvolvedor deve ter controle total sobre a lógica e a memória.
- **Agnosticismo de Interface**: O CORE do sistema deve ser independente do Qt (C++ puro).

## 2. Tecnologias e Ferramentas
- **Linguagem**: C++20 (Uso de conceitos modernos para performance).
- **Compilador**: MinGW 64-bit (via Qt Creator 6.9.1).
- **Gerenciamento de Build**: qmake (.pro) pela simplicidade e legibilidade.

## 3. Gestão de Dados e Performance
- **Aritmética**: Proibido o uso de ponto flutuante no processamento interno. 
- **Unidade Padrão**: Inteiro de 32 bits representando décimos de milímetro (0.1mm).
- **Alinhamento de Memória**: Uso de `alignas(16)` para structs geométricas (foco em cache/SIMD).
- **Persistência**: Formato binário customizado (mapeamento direto de memória para disco).
- **Gerenciamento de Texto**: Pool de strings com indireção (Dicionário) para economia de RAM e suporte ao SINAPI.

## 4. Hardware e Comunicação
- **Serial**: Implementação direta via WinAPI (CreateFile, ReadFile) para eliminar camadas intermediárias.


**seção de rascunhos**
C:/DESENV/
├── diretrizes.md               # A "Constituição" do projeto
├── diario.md                   # Registro de progresso e decisões
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

