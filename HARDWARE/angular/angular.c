//
// Created by crepusculumx on 2021/10/19.
//

#include "angular.h"

void Angular_init(Angular *angular, TIM_HandleTypeDef *ENCODER_TIM_HANDLE) {
  angular->ENCODER_TIM_HANDLE = ENCODER_TIM_HANDLE;

  // start encoder
  HAL_TIM_Encoder_Start(angular->ENCODER_TIM_HANDLE, TIM_CHANNEL_ALL);
}

int32_t Angular_get_cur_angle(Angular *angular) {
  return (int32_t) __HAL_TIM_GET_COUNTER(angular->ENCODER_TIM_HANDLE);
}
