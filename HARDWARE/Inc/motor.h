/*
 * motor.h
 *
 *  Created on: 2021年10月14日
 *      Author: crepusculumx
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "tim.h"
#include "gpio.h"

typedef enum bool {
  true = 1, false = 0
} bool;

typedef enum rot_dir {
  CW, CCW
} rot_dir;

typedef struct Motor {
  // PWM
  TIM_HandleTypeDef *PWM_TIM_HANDLE;
  uint32_t PWM_TIM_CHANNEL;

  // ENCODER
  TIM_HandleTypeDef *ENCODER_TIM_HANDLE;

  // COUNTER
  TIM_HandleTypeDef *COUNTER_TIM_HANDLE;

  // EN
  GPIO_TypeDef *EN_GPIO_Port;
  uint16_t EN_GPIO_Pin;

  // DIR
  GPIO_TypeDef *DIR_GPIO_Port;
  uint16_t DIR_GPIO_Pin;

  // var

  // state
//  double cur_vel;
//  double cur_acc;
//  rot_dir cur_dir;

  double lst_vel;

  double target_vel;// -1000 to 1000
  rot_dir target_dir;

  uint32_t pwm;

  // encoder
  uint32_t lst_encoder_cnt;
} Motor;

void Motor_init(Motor *motor,
                TIM_HandleTypeDef *PWM_TIM_HANDLE,
                uint32_t PWM_TIM_CHANNEL,
                TIM_HandleTypeDef *ENCODER_TIM_HANDLE,
                TIM_HandleTypeDef *COUNTER_TIM_HANDLE,
                GPIO_TypeDef *EN_GPIO_Port,
                uint16_t EN_GPIO_Pin,
                GPIO_TypeDef *DIR_GPIO_Port,
                uint16_t DIR_GPIO_Pin
);
void Motor_set_pwm(Motor *motor, uint32_t pwm);
void Motor_set_cur_state(Motor *motor, double vel, double acc, rot_dir dir);
void Motor_set_dir(Motor *motor, rot_dir dir);
void Motor_set_target_state(Motor *motor, double target_vel);
void Motor_update_cur_state(Motor *motor, uint32_t rate);
#endif /* INC_MOTOR_H_ */
