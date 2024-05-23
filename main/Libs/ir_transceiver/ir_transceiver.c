//
// Created by nikita on 19.05.24.
//

#include "driver/rmt.h"
#include "esp_log.h"
#include "Libs/ac_command/ac_command.h"
#include "Libs/internal_led/internal_led.h"

#define IR_PIN 26
#define LED_BLUE 23

static const char* LOG_TAG = "TEST_LOG";

const rmt_channel_t RMT_CHANNEL = RMT_CHANNEL_0;
const int RMT_TX_CARRIER_EN = 1;

void setup_rmt_config()
{
    rmt_config_t rmtConfig;

    rmtConfig.rmt_mode = RMT_MODE_TX;  								//transmit mode
    rmtConfig.channel = RMT_CHANNEL;  								//channel to use 0 - 7
    rmtConfig.clk_div = 80;  										//clock divider 1 - 255. source clock is 80MHz -> 80MHz/80 = 1MHz -> 1 tick = 1 us
    rmtConfig.gpio_num = IR_PIN; 									//pin to use
    rmtConfig.mem_block_num = 1; 									//memory block size

    rmtConfig.tx_config.loop_en = 0; 								//no loop
    rmtConfig.tx_config.carrier_freq_hz = 38000;  					//38kHz carrier frequency
    rmtConfig.tx_config.carrier_duty_percent = 33; 					//duty
    rmtConfig.tx_config.carrier_level =  RMT_CARRIER_LEVEL_HIGH; 	//carrier level
    rmtConfig.tx_config.carrier_en =RMT_TX_CARRIER_EN ;  			//carrier enable
    rmtConfig.tx_config.idle_level =  RMT_IDLE_LEVEL_LOW ; 			//signal level at idle
    rmtConfig.tx_config.idle_output_en = true;  					//output if idle

    rmt_config(&rmtConfig);
    rmt_driver_install(rmtConfig.channel, 0, 0);
}

static void rmtAcCtrlTask()
{
    internal_led_on(LED_BLUE);

    setup_rmt_config();
    esp_log_level_set(LOG_TAG, ESP_LOG_INFO);

    int data_length = get_data_length();
    uint32_t* data = get_data();

    const int send_data_length = (data_length + 2)/2;

    rmt_item32_t send_data[send_data_length]; 		//Data to send

    for(int i = 0, t = 0; i < (send_data_length*2)-1; i += 2, t++)
    {
        //Odd bit High
        send_data[t].duration0 = data[i];
        send_data[t].level0 = 1;
        //Even bit Low
        send_data[t].duration1 = data[i+1];
        send_data[t].level1 = 0;
    }

    ESP_LOGI(LOG_TAG, "SEND RMT DATA");

    rmt_write_items(RMT_CHANNEL, send_data, send_data_length, true);

    //some circuit design corrections using software, since RMT does not care about the state of the IR pin, you have to install it manually
    gpio_set_level(IR_PIN, 1);

    internal_led_off(LED_BLUE);

    vTaskDelete(NULL);
}

void rmt_transceive()
{
    xTaskCreate(rmtAcCtrlTask, "rmtAcCtrlTask", 10000, NULL, 10, NULL);
}