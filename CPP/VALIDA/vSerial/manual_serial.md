**manual_serial.md**
# Manual de Uso: Módulo SerialWin32

## Objetivo
Validar a recepção de dados brutos de equipamentos de campo (Estações Totais/GPS).

## Configuração do Equipamento
Para este teste funcionar, a estação deve estar configurada com:
- Baud Rate: 9600
- Paridade: Nenhuma (None)
- Bits de Dados: 8
- Bits de Parada: 1

## Como Executar
1. Conecte o cabo na porta física COM3.
2. Execute o binário `v_serial.exe`.
3. Dispare a transmissão na estação total.
4. O programa encerrará automaticamente após 3 segundos de silêncio.

## Resultado Esperado
Um arquivo chamado `validacao_estacao.raw` será criado com o conteúdo binário idêntico ao enviado pelo hardware.
