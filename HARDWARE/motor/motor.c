//
// Created by crepusculumx on 2021/10/14.
//

#include <stdio.h>
#include "motor.h"

uint32_t max(uint32_t a, uint32_t b) {
  return a > b ? a : b;
}

uint32_t min(uint32_t a, uint32_t b) {
  return a > b ? b : a;
}

void Motor_init(Motor *motor,
                TIM_HandleTypeDef *PWM_TIM_HANDLE,
                uint32_t PWM_TIM_CHANNEL,
                TIM_HandleTypeDef *ENCODER_TIM_HANDLE,
                TIM_HandleTypeDef *COUNTER_TIM_HANDLE,
                GPIO_TypeDef *EN_GPIO_Port,
                uint16_t EN_GPIO_Pin,
                GPIO_TypeDef *DIR_GPIO_Port,
                uint16_t DIR_GPIO_Pin
) {
  motor->PWM_TIM_HANDLE = PWM_TIM_HANDLE;
  motor->PWM_TIM_CHANNEL = PWM_TIM_CHANNEL;

  motor->ENCODER_TIM_HANDLE = ENCODER_TIM_HANDLE;

  motor->COUNTER_TIM_HANDLE = COUNTER_TIM_HANDLE;

  motor->EN_GPIO_Port = EN_GPIO_Port;
  motor->EN_GPIO_Pin = EN_GPIO_Pin;

  motor->DIR_GPIO_Port = DIR_GPIO_Port;
  motor->DIR_GPIO_Pin = DIR_GPIO_Pin;

  // start pwm
  HAL_TIM_PWM_Start(motor->PWM_TIM_HANDLE, motor->PWM_TIM_CHANNEL);

  // start encoder
  motor->lst_encoder_cnt = 0;
  HAL_TIM_Encoder_Start(motor->ENCODER_TIM_HANDLE, TIM_CHANNEL_ALL);

  // start counter
  HAL_TIM_Base_Start_IT(motor->COUNTER_TIM_HANDLE);

  // start EN
  HAL_GPIO_WritePin(motor->EN_GPIO_Port, motor->EN_GPIO_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(motor->DIR_GPIO_Port, motor->DIR_GPIO_Pin, GPIO_PIN_SET);
  // start

  // default target
  motor->target_vel = 0;
  Motor_set_pwm(motor, 1000);
  Motor_set_dir(motor, CCW);
}

void Motor_set_pwm(Motor *motor, uint32_t pwm) {
  motor->pwm = pwm;
  if (motor->pwm > 1000) motor->pwm = 1000;

    __HAL_TIM_SetCompare(motor->PWM_TIM_HANDLE, motor->PWM_TIM_CHANNEL, pwm);
}

void Motor_set_dir(Motor *motor, rot_dir dir) {
  switch (dir) {
    case CCW:HAL_GPIO_WritePin(motor->DIR_GPIO_Port, motor->DIR_GPIO_Pin, GPIO_PIN_SET);
      break;
    case CW:HAL_GPIO_WritePin(motor->DIR_GPIO_Port, motor->DIR_GPIO_Pin, GPIO_PIN_RESET);
      break;
    default:return;
  }
}

void Motor_set_cur_state(Motor *motor, double vel, double acc, rot_dir dir) {
//  motor->cur_vel = vel;
//  motor->cur_dir = dir;
}

void Motor_set_target_state(Motor *motor, double target_vel) {
  motor->target_vel = target_vel;

  int32_t pwm = (int32_t) target_vel;
  if (target_vel > 1000) pwm = 1000;
  else if (target_vel < -1000) pwm = -1000;

  if (target_vel >= 0) {
    Motor_set_pwm(motor, 1000 - pwm);
    Motor_set_dir(motor, CCW);
  } else {
    Motor_set_pwm(motor, 1000 + pwm);
    Motor_set_dir(motor, CW);
  }
}

void Motor_update_cur_state(Motor *motor, uint32_t rate) {
  uint32_t cur_cnt = __HAL_TIM_GET_COUNTER(motor->ENCODER_TIM_HANDLE);
  uint32_t cur_dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(motor->ENCODER_TIM_HANDLE);
  uint32_t d_cnt = max(motor->lst_encoder_cnt, cur_cnt) - min(motor->lst_encoder_cnt, cur_cnt);
  double cur_vel = (double) d_cnt / 400 * rate;
  double cur_acc = (motor->lst_vel - cur_vel) * rate;
  Motor_set_cur_state(motor, cur_vel, cur_acc, cur_dir);

//  if (cur_vel >= 0 && cur_vel <= 8000 && cur_acc < 100000) {
//    printf("vel %lf\n", cur_vel);
//    printf("acc %lf\n", cur_acc);
//  }

  motor->lst_encoder_cnt = cur_cnt;
  motor->lst_vel = cur_vel;
}
