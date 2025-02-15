# Projeto: Comunicação Serial e Controle de Periféricos com RP2040

## Descrição
Este projeto tem como objetivo consolidar os conceitos de comunicação serial, manipulação de LEDs comuns e endereçáveis WS2812, bem como a utilização de botões e um display SSD1306 na placa BitDogLab.

## Estrutura do Projeto
O código-fonte está organizado da seguinte maneira:

```
├── animation/      # Animações dos números para a matriz de LEDs WS2812
│   ├── num0.c
│   ├── num1.c
│   ├── ...
│   └── num9.c
│  
│
├── inc/            # Arquivos de cabeçalho
│   ├── font.h      # Definição das fontes (inclui caracteres minúsculos)
│   └── ssd1306.h   # Configuração do display OLED SSD1306
│   
│
├── lib/            # Bibliotecas auxiliares
│   ├── matriz_leds.c   # Controle da matriz de LEDs WS2812
│   ├── buttons.c       # Implementação das interrupções e debounce dos botões
│   ├── buttons.h       # Definições dos botões e suas funções
│   └── matriz_leds.h   # Definições da matriz de LEDs
│
├── Projeto_UART.c      # Código principal do projeto
│
└── README.md       # Documentação do projeto
```

## Componentes Utilizados
- **Matriz de LEDs WS2812** (5x5) conectada à GPIO 7
- **LED RGB** (GPIOs 11, 12, 13)
- **Botões:**
  - Botão A conectado à GPIO 5
  - Botão B conectado à GPIO 6
- **Display OLED SSD1306** conectado via I2C (GPIO 14 e GPIO 15)

## Funcionalidades Implementadas
1. **Exibição de Caracteres no Display SSD1306**
   - Os caracteres digitados via Serial Monitor são exibidos no display.
   - A biblioteca `font.h` foi modificada para incluir caracteres minúsculos.
   
2. **Exibição de Números na Matriz de LEDs WS2812**
   - Quando um número entre 0 e 9 é digitado no Serial Monitor, a matriz exibe a animação correspondente.
   
3. **Interação com os Botões**
   - **Botão A**:
     - Alterna o LED Verde (GPIO 11).
     - Mensagem correspondente é exibida no display e no Serial Monitor.
   - **Botão B**:
     - Alterna o LED Azul (GPIO 12).
     - Mensagem correspondente é exibida no display e no Serial Monitor.

## Requisitos Técnicos
- **Uso de Interrupções** para os botões (implementado em `buttons.c`).
- **Debounce via software** para evitar múltiplas detecções de um único pressionamento.
- **Controle de LEDs** convencionais e endereçáveis WS2812.
- **Utilização do Display OLED 128x64** para exibir mensagens.
- **Envio de dados pela UART** para comunicação via Serial Monitor.
- **Código estruturado e bem comentado** para facilitar a compreensão.

## Demonstração
- **Vídeo explicativo do projeto:** https://drive.google.com/file/d/1WueSvV-GPB_ypt91-hFDdWFOou8unbTm/view?usp=sharing

## Considerações Finais
Este projeto é uma ótima oportunidade para aprender e consolidar conhecimentos em microcontroladores, protocolos de comunicação e manipulação de hardware. 

