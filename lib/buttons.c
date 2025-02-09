#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "buttons.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;
static absolute_time_t last_a_time;  // Debounce
static absolute_time_t last_b_time;

// Única callback para tratar ambos os botões
void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A_PIN) {

        // Debounce: ignora eventos com menos de 500ms do último
        if (absolute_time_diff_us(last_a_time, get_absolute_time()) > 500000) {
            button_a_pressed = true;
            last_a_time = get_absolute_time();
        }
    } 
    else if (gpio == BUTTON_B_PIN) {
        if (absolute_time_diff_us(last_b_time, get_absolute_time()) > 500000) {
            button_b_pressed = true;
            last_b_time = get_absolute_time();
        }
    }
}

void init_buttons() {
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    // Habilita interrupções para ambos os botões
    gpio_set_irq_enabled(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Configura a callback global 
    gpio_set_irq_callback(&button_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);  // Habilita interrupções do GPIO
}

// Retorna se o botão A foi pressionado
bool is_button_a_pressed() {
    if (button_a_pressed) {
        button_a_pressed = false;  // Reseta flag
        return true;
    }
    return false;
}

// Retorna se o botão B foi pressionado
bool is_button_b_pressed() {
    if (button_b_pressed) {
        button_b_pressed = false;  // Reseta flag
        return true;
    }
    return false;
}



