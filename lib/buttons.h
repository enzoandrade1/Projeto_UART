#ifndef BUTTONS_H
#define BUTTONS_H

#include "pico/stdlib.h"

// Definições dos pinos dos botões
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// Funções de inicialização e controle dos botões
void init_buttons();

// Funções para verificar o estado dos botões
bool is_button_a_pressed();
bool is_button_b_pressed();

#endif // BUTTONS_H
