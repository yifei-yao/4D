//
// Created by Yifei Yao on 1/2/22.
//

#include "time_controller.h"
#include <chrono>

bool TimeController::IsTimeUp() const {
  return std::chrono::steady_clock::now() > deadline;
}

void TimeController::SetTime(double time) {
  deadline = std::chrono::steady_clock::now() +
             std::chrono::duration<double>(time);
}

void TimeController::StopNow() {
  deadline = std::chrono::steady_clock::now();
}

TimeController::TimeController(double time) {
  deadline = std::chrono::steady_clock::now() +
             std::chrono::duration<double>(time);
}
