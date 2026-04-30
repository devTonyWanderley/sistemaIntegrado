--diretrizes.md--

1.	Utilizar o mínimo possível de camadas de terceiros;


--seção de rascunhos--
C:/DESENV/CPP/
├── CORE/                       # Código C++ Puro (Sem Qt)
│   ├── DRIVER/                 # WinAPI Serial (CreateFile, ReadFile)
│   ├── PROTOCOL/               # Parsers (Máquinas de estado p/ Bytes)
│   ├── GEOM/                   # Octree, Delaunay, Cálculos CAD
│   └── TYPES/                  # Suas Structs Binárias (tipPonto.h, etc)
│
├── UI/                         # Camada Visual (Qt Widgets)
│   ├── Main/                   # Janelas Principais
│   └── Components/             # Desenho técnico usando QPainter (Nativo)
│
├── APP/                        # Onde o sistema se une
│   ├── TopoApp.pro             # Arquivo de projeto qmake
│   └── main.cpp                # Ponto de entrada
│
└── diretrizes.md               # Suas regras de ouro
