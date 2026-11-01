#include "stm32f10x.h"
#include "Port.h"
#include "cfg_port.h"
#include "Dio.h"
// Hàm delay đơn giản (không dùng timer, không chính xác)
void Delay(__IO uint32_t nCount) {
    for(; nCount != 0; nCount--);
}
#define BUTTON_DEBOUNCE_DELAY 5000
int main(void) {
     /* =======================
     * 1. Khởi tạo Port Driver
     * ======================= */
    Port_ConfigType portConfig = {
        .PinConfigs = PortCfg_Pins,
        .PinCount = PortCfg_PinsCount};
    Port_Init(&portConfig);

    /* =======================
     * 2. Khởi tạo các Driver ngoại vi
     * ======================= */
    // Adc_Init(); // TODO: Hiện thực module Adc theo chuẩn AUTOSAR
    // Pwm_Init(); // TODO: Hiện thực module Pwm theo chuẩn AUTOSAR

    /* =======================
     * 3. Ứng dụng thực tế
     * ======================= */
    uint8_t led_state = 0;
    uint32_t button_cnt = 0;

    while (1)
    {
        /* ----- Đọc nút nhấn (PC13) để bật/tắt LED số (PA0) ----- */
        if (Dio_ReadChannel(DIO_CHANNEL(DIO_PORT_C, 13)) == STD_LOW)
        {
            /* Đơn giản debounce bằng bộ đếm */
            button_cnt++;
            if (button_cnt > BUTTON_DEBOUNCE_DELAY)
            {
                led_state = !led_state; // Đảo trạng thái
                Dio_WriteChannel(DIO_CHANNEL(DIO_PORT_A, 0), led_state ? STD_HIGH : STD_LOW);
                while (Dio_ReadChannel(DIO_CHANNEL(DIO_PORT_C, 13)) == STD_LOW)
                    ; // Đợi thả nút
                button_cnt = 0;
            }
        }
        else
        {
            button_cnt = 0;
        }

    }
    Dio_ReadChannel(CHANNEL_1);
    Dio_WriteChannel(CHANNEL_1, STD_HIGH);
}
