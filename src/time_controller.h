//
// Created by Yifei Yao on 1/2/22.
//

#ifndef INC_4D_TIME_CONTROLLER_H
#define INC_4D_TIME_CONTROLLER_H

#include <chrono>

class TimeController {
public:
  explicit TimeController(double time = 0);

  void SetTime(double time);

  void StopNow();

  [[nodiscard]] bool IsTimeUp() const;

private:
  typedef typeof(std::chrono::steady_clock::now() +
                 std::chrono::duration<double>()) TimePoint;

  TimePoint deadline;
};


#endif //INC_4D_TIME_CONTROLLER_H
