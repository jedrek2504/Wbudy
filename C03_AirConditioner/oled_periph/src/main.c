#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_timer.h"
#include "light.h"
#include "oled.h"
#include "temp.h"
#include "acc.h"
#include "rgb.h"

static uint32_t msTicks = 0;

static void SysTick_Handler(void);
static uint32_t getTicks(void);
static void init_ssp(void);
static void init_i2c(void);
static void init_adc(void);
static void PWM_Init(void);
static void delay(uint32_t milliseconds);

int main(void);

static void SysTick_Handler(void)
{
    msTicks++;
}

static uint32_t getTicks(void)
{
    return msTicks;
}

static void init_ssp(void)
{
    SSP_CFG_Type SSP_ConfigStruct;
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 7;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 8;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 9;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Funcnum = 0;
    PinCfg.Portnum = 2;
    PinCfg.Pinnum = 2;
    PINSEL_ConfigPin(&PinCfg);

    SSP_ConfigStructInit(&SSP_ConfigStruct);

    SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

    SSP_Cmd(LPC_SSP1, ENABLE);
}

static void init_i2c(void)
{
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 2;
    PinCfg.Pinnum = 10;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 11;
    PINSEL_ConfigPin(&PinCfg);

    I2C_Init(LPC_I2C2, 100000);

    I2C_Cmd(LPC_I2C2, ENABLE);
}

static void init_adc(void)
{
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 23;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);

    ADC_Init(LPC_ADC, 200000);
    ADC_IntConfig(LPC_ADC, ADC_CHANNEL_0, DISABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
}

static void PWM_Init(void)
{
    LPC_SC->PCONP |= (1U << 6);
    LPC_PWM1->PR = 0x18;
    LPC_PWM1->MR0 = 0x3e8;
    LPC_PWM1->MR1 = 0x0;
    LPC_PINCON->PINSEL4 |= (1U << 0);
    LPC_PWM1->PCR |= (1UL << 9);
    LPC_PWM1->TCR = 0x9;
    LPC_PWM1->PC = 0x1;
    LPC_PWM1->LER = 0x3;
}

static void delay(uint32_t milliseconds)
{
    uint32_t startTicks = msTicks;
    while ((msTicks - startTicks) < milliseconds) {}
}

int main(void)
{
    int32_t t = 0;
    uint32_t lux = 0;
    uint32_t trim = 0;

    init_i2c();
    init_ssp();
    init_adc();
    rgb_init();
    oled_init();
    light_init();
    acc_init();
    temp_init(&getTicks);
    PWM_Init();

    if (SysTick_Config(SystemCoreClock / 1000) != 0) {
        while (1) {} // Capture error
    }

    LPC_SC->PCONP |= (1UL << 15U);
    LPC_GPIO2->FIODIR &= ~(1UL << 10U);
    LPC_GPIO2->FIOPIN |= (1UL << 10U);

    light_enable();
    light_setRange(LIGHT_RANGE_4000);

    oled_clearScreen(OLED_COLOR_WHITE);
    oled_putString(1, 1, (uint8_t*)"Temp : ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
    oled_putString(1, 20, (uint8_t*)"Swiat : ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);

    while (1)
    {
        char str[10];
        char str2[10];

        t = temp_read();
        int ret = sprintf(str, "%.1f", t / 10.0);

        lux = light_read();
        ret = sprintf(str2, "%3d", lux);

        uint16_t ledOn = 0;
        uint16_t ledOff = 0;

        if (t >= 265) {
            rgb_setLeds(0x06);
            LPC_PWM1->MR1 = 1000;
            LPC_PWM1->LER = 0x2;
        }

        if ((t >= 245) && (t < 265)) {
            rgb_setLeds(0x04);
            LPC_PWM1->MR1 = 750;
            LPC_PWM1->LER = 0x2;
        }

        if (t < 245) {
            rgb_setLeds(0x05);
            LPC_PWM1->MR1 = 400;
            LPC_PWM1->LER = 0x2;
        }

        if (lux < 10U) {
            oled_inverse(0);
        } else {
            oled_inverse(1);
        }

        oled_fillRect((1 + (9 * 6)), 1, 80, 8, OLED_COLOR_WHITE);
        oled_putString((1 + (9 * 6)), 1, str, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        oled_putString((1 + (9 * 6)), 20, str2, OLED_COLOR_BLACK, OLED_COLOR_WHITE);


        delay(200);
    }
}
