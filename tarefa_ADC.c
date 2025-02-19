/*          EMBARCATECH
Código desenvolvido para atendimento da proposta da tarefa: Conversores A/D
Residente.: Jackson da Silva Carneiro   TIC370100907
*/
                       //inclusão das bibliotecas

#include <stdio.h>            
#include "pico/stdlib.h"      
#include "hardware/adc.h"  
#include "hardware/pwm.h"  

                    // GPIOs para o joystick, botão e LEDs
#define joyVRX_PIN 26           
#define joyVRY_PIN 27         
#define BUTTON_SW_PIN 22    
#define LED_PIN_GREEN 11   
#define LED_PIN_BLUE 12   
#define LED_PIN_RED 13   
#define P_BUTTON_A 5

const float DIVISOR_PWM = 16.0;
const uint16_t PERIODO_PWM = 4096;
uint16_t level_led_blue, level_led_red = 100;
uint slice_led_blue, slice_led_red;
const int ADC_CHANNEL_0 = 0;
const int ADC_CHANNEL_1 = 1;

void config_joystick(){   // FUNÇÃO PARA CONFIGURAR O JOYSTICK

    adc_gpio_init(joyVRX_PIN); // Configura adc do GP26 (ADC0) para o eixo X do joystick.
    adc_gpio_init(joyVRY_PIN); // Configura adc do GP27 (ADC1) para o eixo Y do joystick.

    // Configuração do pino do botão do joystick como entrada digital.
    gpio_init(BUTTON_SW_PIN);
    gpio_set_dir(BUTTON_SW_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_SW_PIN); // Habilitação de pull-up interno.

}
void configura_GPIO(){   //  FUNÇÃO PARA CONFIGURAR OS LED'S E PUSH BUTTON A
    
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

void config_led_pwm(uint led, uint *slice, uint16_t level){

    gpio_set_function(led, GPIO_FUNC_PWM);   // configuração do pino do led como saída
    *slice = pwm_gpio_to_slice_num(led);    // obtenção do slice associado ao pino
    pwm_set_clkdiv(*slice, DIVISOR_PWM);   //  definição do divisor de clock
    pwm_set_wrap(*slice, PERIODO_PWM);    //   definição do período
    pwm_set_gpio_level(led, level);      //  definição do nível inicial do pwm
    pwm_set_enabled(*slice, true);      //   habilitação do pwm no slice correspondente

}

void joystick_read(uint16_t *vrx_value, uint16_t *vry_value){

    adc_select_input(ADC_CHANNEL_0); // canal adc para eixo X
    sleep_us(2);
    *vrx_value = adc_read();  //lê o valor do eixo X

    adc_select_input(ADC_CHANNEL_1);  // canal adc para eixo Y
    sleep_us(2);
    *vry_value = adc_read();   // lê o valor do eixo Y
}

int main() {
    
    stdio_init_all();
    adc_init();
    config_joystick();
    configura_GPIO();
    uint16_t vrx_value, vry_value;
                        // configuração do pwm para os led's azul e vermelho
    setup_pwm_led(LED_PIN_BLUE, &slice_led_blue, level_led_blue);
    setup_pwm_led(LED_PIN_RED, &slice_led_red, level_led_red);

    
    while (true) {

        // Leitura do estado do botão do joystick (SW)
        bool sw_value = gpio_get(BUTTON_SW_PIN) == 0; // 0 indica que o botão está pressionado.

joystick_read(&vrx_value, &vry_value);

pwm_set_gpio_level(LED_PIN_BLUE, vry_value);
pwm_set_gpio_level(LED_PIN_RED, vrx_value);

        if (sw_value) { // Verifica se o botão no pino 22 está pressionado
            gpio_put(LED_PIN_GREEN, true); // Liga o LED3.
        } else {
            gpio_put(LED_PIN_GREEN, false); // Desliga o LED3.
        }
        // atraso de 500 ms antes de repetir a leitura.
        sleep_ms(500);
    }

    return 0;
}