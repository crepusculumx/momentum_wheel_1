//
// Created by crepusculumx on 2021/10/19.
//

#include "controller.h"
#include "stdio.h"

void Controller_init(Controller *controller, TIM_HandleTypeDef *COUNTER_TIM_HANDLE, Motor *motor, Angular *angular) {
  controller->COUNTER_TIM_HANDLE = COUNTER_TIM_HANDLE;
  controller->motor = motor;
  controller->angular = angular;

  // start counter
  HAL_TIM_Base_Start_IT(controller->COUNTER_TIM_HANDLE);
}

void Controller_pid(Controller *controller, uint32_t rate) {
  double kp = 7, ki = 0, kd = 7;
  int32_t cur_pos = Angular_get_cur_angle(controller->angular);
//  printf("cur_pos %ld", cur_pos);
  int32_t cur_err;
  cur_err = 5000 - cur_pos;

//  printf("cur_err = %ld\n", cur_err);
  double u = kp * cur_err + ki * controller->integral + kd * (cur_err - controller->lst_err);
  double vel = -u;
//  printf("p = %lf\n", kp * cur_err);
//  printf("d = %lf\n", kd * (cur_err - controller->lst_err));
//  printf("vel = %lf\n", vel);
  if (-1000 < cur_err && cur_err < 1000) {
    Motor_set_target_state(controller->motor, vel);
  } else {
    Motor_set_pwm(controller->motor, 1000);
  }

  controller->lst_err = cur_err;
  controller->integral += cur_err;
}
