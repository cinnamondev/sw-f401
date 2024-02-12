#include "Quadrature.hpp"
#include "STM32F4xx_HAL_Driver/stm32f4xx_hal_tim.h"
#include "STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.h"

Quadrature::Quadrature(TIM_TypeDef* timer) {
    //https://github.com/STMicroelectronics/STM32CubeF4/blob/ec8744184587ef76b0e1704d1ee3e391f1aa2b90/Projects/STM324xG_EVAL/Examples/TIM/TIM_Encoder/Src/main.c
    // configuration code based on ^^
    // Quadrature encoder configuration
    encoder.Instance = timer;
    encoder.Init.Period = 65535;
    encoder.Init.Prescaler = 0;
    encoder.Init.ClockDivision = 0;
    encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
    encoder.Init.RepetitionCounter = 0;

    encoderCfg.EncoderMode = TIM_ENCODERMODE_TI12; // x4 encoder //Test
    
    encoderCfg.IC1Polarity = TIM_ICPOLARITY_RISING;
    encoderCfg.IC1Prescaler = TIM_ICPSC_DIV1;
    encoderCfg.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    encoderCfg.IC1Filter = 0;

    encoderCfg.IC2Polarity = TIM_ICPOLARITY_RISING;
    encoderCfg.IC1Prescaler = TIM_ICPSC_DIV1;
    encoderCfg.IC2Selection = TIM_ICSELECTION_INDIRECTTI;
    encoderCfg.IC2Filter = 0;

    if (HAL_TIM_Encoder_Init(&encoder, &encoderCfg) != HAL_OK) {
        throw invalid_argument("Timer used is not valid");
    }

    HAL_TIM_Encoder_Start(&encoder, TIM_CHANNEL_ALL);
}   
void Quadrature::tick(void) {
    unsigned long encT2 = encoder.Instance->CNT;
    degs = ((float)encT2-encT1)*1406.25; // deg/s
    encT1 = encT2; // new value is now old value in future
    // 1406.25 = (360 degrees / 256 steps) = deg/ms * 1000 = deg/s
    
}
bool Quadrature::getDirection(void) {
    return __HAL_TIM_IS_TIM_COUNTING_DOWN(&encoder);
}

void Quadrature::start(void) {
    active = true;
    encoderRead.attach(callback(this, Quadrature::tick), 1ms);
}

void Quadrature::stop(void) {
    encoderRead.detach();
    active = false;
}

bool Quadrature::isActive(void) {
    return active;
}

float Quadrature::rotarySpeed(void) {
    return degs;  
}