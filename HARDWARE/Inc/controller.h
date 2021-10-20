//
// Created by crepusculumx on 2021/10/19.
//

#ifndef MOMENTUM_WHEEL_1_HARDWARE_INC_CONTROLLER_H_
#define MOMENTUM_WHEEL_1_HARDWARE_INC_CONTROLLER_H_

#include "motor.h"
#include "angular.h"

typedef struct Controller {
  Motor *motor;
  Angular *angular;

  // COUNTER
  TIM_HandleTypeDef *COUNTER_TIM_HANDLE;

  // var
  int_fast32_t integral;
  int_fast32_t lst_err;
} Controller;

void Controller_init(Controller *controller, TIM_HandleTypeDef *COUNTER_TIM_HANDLE, Motor *motor, Angular *angular);
void Controller_pid(Controller *controller, uint32_t rate);

#endif //MOMENTUM_WHEEL_1_HARDWARE_INC_CONTROLLER_H_
