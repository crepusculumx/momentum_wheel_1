/*
 * angular.h
 *
 *  Created on: Oct 19, 2021
 *      Author: crepusculumx
 */

#ifndef INC_ANGULAR_H_
#define INC_ANGULAR_H_

#include "tim.h"

typedef struct Angular {
  // ENCODER
  TIM_HandleTypeDef *ENCODER_TIM_HANDLE;

  // var
  int angle;
} Angular;

void Angular_init(Angular *angular, TIM_HandleTypeDef *ENCODER_TIM_HANDLE);

int32_t Angular_get_cur_angle(Angular *angular);

#endif /* INC_ANGULAR_H_ */
