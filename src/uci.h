//
// Created by Yifei Yao on 1/2/22.
//

#ifndef INC_4D_UCI_H
#define INC_4D_UCI_H

#include "color.h"
#include "board.h"
#include <iostream>
#include <sstream>
#include <map>

namespace uci {
  void UCIMainLoop();

  std::map<std::string, double> ParseGo(std::stringstream &ss);

  double GetTime(const std::map<std::string, double> &options,
                 color::Color color_to_move, const Board &board);
}

#endif //INC_4D_UCI_H
