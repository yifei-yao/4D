//
// Created by Yifei Yao on 12/30/21.
//

#ifndef INC_4D_INFINITY_H
#define INC_4D_INFINITY_H

#include <limits>

namespace infinity {
  static constexpr int kInfinity = std::numeric_limits<int>::max();

  static constexpr int kNegativeInfinity = std::numeric_limits<int>::min() + 1;
}

#endif //INC_4D_INFINITY_H
