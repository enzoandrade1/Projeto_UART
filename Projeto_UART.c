#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "lib/matriz_leds.h" 
#include "lib/buttons.h"    

// Definições de pinos
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
#define LED_VERDE_PIN 11
#define LED_AZUL_PIN 12
#define LED_VERMELHO_PIN 13
#define MATRIZ_PIN 7

// Constantes gerais
#define ANIMACAO 1

// Declaração das animações dos números de 0 a 9 
extern uint32_t num0[][25];
extern uint32_t num1[][25];
extern uint32_t num2[][25];
extern uint32_t num3[][25];
extern uint32_t num4[][25];
extern uint32_t num5[][25];
extern uint32_t num6[][25];
extern uint32_t num7[][25];
extern uint32_t num8[][25];
extern uint32_t num9[][25];

// Array de ponteiros para as animações
uint32_t (*numeros[])[25] = {num0, num1, num2, num3, num4, num5, num6, num7, num8, num9};

// Variáveis globais
uint8_t ssd[ssd1306_buffer_length];
struct render_area frame_area;

// Função para exibir o número na matriz de LEDs
void exibir_numero(PIO pio, uint sm, uint8_t num) {
    if (num < 10) {
        rgb_led rgb_data[ANIMACAO][25];
        hex_to_rgb(numeros[num], rgb_data, ANIMACAO); // Converte cores hex para RGB
        ajustar_brilho(rgb_data, 0.8, ANIMACAO);     // Ajusta brilho para 80%
        enviar_animacao(rgb_data, pio, sm, ANIMACAO); // Envia dados para a matriz
    }
}

// Inicializa os LEDs
void inicializarLEDs() {
    gpio_init(LED_VERDE_PIN);
    gpio_init(LED_AZUL_PIN);
    gpio_init(LED_VERMELHO_PIN);
    gpio_set_dir(LED_VERDE_PIN, GPIO_OUT);
    gpio_set_dir(LED_AZUL_PIN, GPIO_OUT);
    gpio_set_dir(LED_VERMELHO_PIN, GPIO_OUT);
}

// Controla a cor dos LEDs
void colorir(uint8_t r, uint8_t g, uint8_t b) {
    gpio_put(LED_VERMELHO_PIN, r);
    gpio_put(LED_VERDE_PIN, g);
    gpio_put(LED_AZUL_PIN, b);
}

// Inicializa o display OLED
void inicializar_display() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();

    // Configura a área de renderização para 128x64 pixels
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;      // 127
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;      // 7 (para 64 pixels)
    calculate_render_area_buffer_length(&frame_area);

    // Limpa o display
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

void exibir_caracter(char *text) {
    memset(ssd, 0, ssd1306_buffer_length); // Limpa o buffer do display

    // Calcula a posição central para 128x64 pixels
    int x = (ssd1306_width - 6) / 2;       // Centraliza horizontalmente (128px - 6px do caractere)
    int y = (ssd1306_n_pages - 1) / 2;     // Centraliza verticalmente (64px / 8px por página = 8 páginas → página 3)

    // Exibe cada caractere centralizado
    for (uint i = 0; i < strlen(text); i++) {
        if (text[i] != ' ') { // Verifica se o caractere não é um espaço
            ssd1306_draw_char(ssd, x, y, text[i]);
        }
        x += 8; // Aumenta para 8px para garantir um espaçamento extra entre os caracteres
        if (x > ssd1306_width - 6) { // Quebra de linha se necessário
            x = (ssd1306_width - 6) / 2;
            y += 1; // Próxima linha (próxima página)
        }
    }

    render_on_display(ssd, &frame_area); // Renderiza
}


void exibir_texto_multilinha(char *linhas[], uint8_t num_linhas) {
    // Limpa o buffer do display
    memset(ssd, 0, ssd1306_buffer_length);
    
    // Define o deslocamento vertical inicial (por exemplo, 0)
    int y = 0;
    
    // Itera sobre as linhas de texto e as desenha
    for (uint8_t i = 0; i < num_linhas; i++) {
        ssd1306_draw_string(ssd, 5, y, linhas[i]);
        y += 8;  // Incrementa 8 pixels para a próxima linha (cada linha tem 8px de altura)
    }
    
    // Renderiza o conteúdo no display OLED
    render_on_display(ssd, &frame_area);
}

// Variáveis globais para armazenar o estado dos LEDs
bool led_verde_estado = false;
bool led_azul_estado = false;

int main() {
    char comando[2];
    stdio_init_all();
    inicializarLEDs();               // Inicializa os LEDs
    PIO pio = pio0;                  // Usa o bloco PIO 0
    uint sm = configurar_matriz(pio, MATRIZ_PIN); // Configura a matriz de LEDs
    init_buttons();                  // Inicializa os botões
    inicializar_display();           // Inicializa o display OLED


while (1) {
    int c = getchar_timeout_us(0);
    if (c != PICO_ERROR_TIMEOUT) {
        comando[0] = (char)c;
        comando[1] = '\0';

        if (comando[0] >= '0' && comando[0] <= '9') {
            exibir_numero(pio, sm, comando[0] - '0');
        } else if (comando[0] >= 'a' && comando[0] <= 'z') {
            exibir_caracter(comando);
        }
    }

    // Alterna LED Verde
    if (is_button_a_pressed()) {
        led_verde_estado = !led_verde_estado;  // Inverte o estado do LED Verde
        led_azul_estado = false;              // Garante que o LED Azul será desligado
        colorir(0, led_verde_estado, 0);      // Atualiza os LEDs

        char *msg_verde[] = { " led verde ", led_verde_estado ? "   ligado   " : "  desligado " };
        exibir_texto_multilinha(msg_verde, 2);

        printf("LED Verde %s\n", led_verde_estado ? "Ligado" : "Desligado");
        sleep_ms(300);  // Debounce
    }

    // Alterna LED Azul
    if (is_button_b_pressed()) {
        led_azul_estado = !led_azul_estado;  // Inverte o estado do LED Azul
        led_verde_estado = false;            // Garante que o LED Verde será desligado
        colorir(0, 0, led_azul_estado);      // Atualiza os LEDs

        char *msg_azul[] = { "  led azul ", led_azul_estado ? "   ligado   " : "  desligado " };
        exibir_texto_multilinha(msg_azul, 2);

        printf("LED Azul %s\n", led_azul_estado ? "Ligado" : "Desligado");
        sleep_ms(300);  // Debounce
    }

    sleep_ms(10);
}


    return 0;
}


