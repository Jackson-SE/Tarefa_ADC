/*          EMBARCATECH
Código desenvolvido para atendimento da proposta da tarefa: Conversores A/D
Residente.: Jackson da Silva Carneiro
*/
                       //inclusão das bibliotecas

#include <stdio.h>            
#include "pico/stdlib.h"      
#include "hardware/adc.h"    

                    // GPIOs para o joystick, botão e LEDs
#define joyVRX_PIN 26           
#define joyVRY_PIN 27         
#define BUTTON_SW_PIN 22    
#define LED_PIN_GREEN 11   
#define LED_PIN_BLUE 12   
#define LED_PIN_RED 13   
#define P_BUTTON_A 5

void configura_GPIO(){
    // Configuração dos pinos GPIO 26 e 27 para leitura analógica do ADC.
    adc_gpio_init(joyVRX_PIN); // Configura adc do GP26 (ADC0) para o eixo X do joystick.
    adc_gpio_init(joyVRY_PIN); // Configura adc do GP27 (ADC1) para o eixo Y do joystick.

    // Configuração do pino do botão do joystick como entrada digital.
    gpio_init(BUTTON_SW_PIN);
    gpio_set_dir(BUTTON_SW_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_SW_PIN); // Habilitação de pull-up interno.

    // Configuração dos pinos dos LEDs como saída.
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
  
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);

    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);

     //configutação do push button A como entrada
    gpio_init(P_BUTTON_A);
    gpio_set_dir(P_BUTTON_A, GPIO_IN);
    gpio_pull_up(P_BUTTON_A); //Habilitação de pull-up interno.

}

int main() {
    
    stdio_init_all();
    adc_init();
    configura_GPIO();

    
    while (true) {
        // Leitura do valor do ADC para Eixo X do joystick
        adc_select_input(0); // Seleciona canal 0 (GP26 - VRX)
        uint16_t vrx_value = adc_read(); 

        // Leitura do valor do ADC para Eixo Y do joystick
        adc_select_input(1); // Seleciona canal 1 (GP27 - VRY)
        uint16_t vry_value = adc_read(); 

        // Leitura do estado do botão do joystick (SW)
        bool sw_value = gpio_get(BUTTON_SW_PIN) == 0; // 0 indica que o botão está pressionado.

        // Controle do LED1 baseado no valor do ADC0 (VRX)
        if (vrx_value > 2100) {
            gpio_put(LED_PIN_RED, true); // Liga o LED1.
        } else {
            gpio_put(LED_PIN_RED, false); // Desliga o LED1.
        }

        // Controle do LED2 baseado no valor do ADC1 (VRY)
        if (vry_value > 2100) {
            gpio_put(LED_PIN_BLUE, true); // Liga o LED2.
        } else {
            gpio_put(LED_PIN_BLUE, false); // Desliga o LED2.
        }

        // Controle do LED3 baseado no estado do botão (SW_PIN)
        if (sw_value) { // Verifica se o botão no pino 22 está pressionado
            gpio_put(LED_PIN_GREEN, true); // Liga o LED3.
        } else {
            gpio_put(LED_PIN_GREEN, false); // Desliga o LED3.
        }


        // Introduz um atraso de 500 milissegundos antes de repetir a leitura.
        sleep_ms(500);
    }

    return 0;
}