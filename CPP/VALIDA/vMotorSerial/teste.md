# 🛠️ MotorSerial Validation Tool

> **Módulo de Captura e Validação de Fluxo Serial de Alta Precisão**

Este utilitário realiza a validação de hardware e software para a classe `MotorSerial`. Ele é projetado especificamente para capturar transmissões de equipamentos de campo (como Estações Totais) e garantir a persistência íntegra dos dados em arquivos temporários.

---

## 📋 Funcionalidades Principais

*   **Auto-Detecção**: Varredura automática de portas COM ativas no Windows.
*   **Dump em Tempo Real**: Visualização sanitizada de caracteres no console (converte binários para `.` para manter a legibilidade).
*   **Persistência Automática**: Gravação em fluxo direto (`flush`) para arquivos `.tmp` datados.
*   **Encerramento Inteligente**: Detecção automática de fim de transmissão por silêncio (timeout de inatividade).
*   **Gerenciamento de Pastas**: Criação automática da árvore de diretórios `C:/DESENV/CPP/INSTANCIA/TMP/`.

---

## 🚀 Como Executar

### 1. Preparação do Ambiente
O sistema utiliza o seguinte caminho para armazenamento:
`C:\DESENV\CPP\INSTANCIA\TMP\`

### 2. Compilação (Ambiente Qt/qmake)
Certifique-se de que o compilador suporta **C++20**:
```bash
qmake vMotorSerial.pro
make
```

### 3. Configuração de Hardware
Antes de rodar, verifique se a Estação Total está configurada com os mesmos parâmetros definidos no `main.cpp`:
*   **Baud**: 9600 (ajustável na variável `baud`)
*   **Data Bits**: 8
*   **Parity**: None
*   **Stop Bits**: 1

---

## 🔍 Interpretação de Resultados

Ao final de cada captura, o sistema apresenta um resumo técnico no console:


| Métrica | Descrição |
| :--- | :--- |
| **Total de Bytes** | Volume bruto recebido. Deve coincidir com o tamanho do arquivo no disco. |
| **Total de Pacotes** | Quantidade de vezes que o sistema operacional entregou blocos de dados. |
| **Arquivo Gerado** | Nome baseado no timestamp da abertura (`AAAAMMDDHHMMSS.tmp`). |

---

## 🛠 Diagnóstico de Falhas (Troubleshooting)

### O arquivo .tmp não foi criado?
*   Verifique se o usuário tem permissão de escrita no drive `C:/`.
*   O arquivo só é criado fisicamente no momento em que o **primeiro byte** é recebido.

### Dados aparecem como pontos `....`?
*   Isso indica dados binários não imprimíveis. Se o dado deveria ser texto (ASCII), verifique se o **Baud Rate** está correto.

### O teste fecha sozinho antes da hora?
*   O critério de encerramento é de **5 segundos de silêncio**. Se a estação total demora mais que isso entre blocos de dados, aumente o valor de `inatividadeSegundos` no `main.cpp`.

---

## 📁 Estrutura de Saída
O arquivo gerado é um **binário bruto (raw)**. Para análise manual:
1. Vá até a pasta de saída.
2. Abra o arquivo com um editor de texto (Notepad++) ou um Hex Editor para validar o protocolo.

---
**Status:** Validado e Estável  
**Versão:** 2.0 (Integração de Persistência)
