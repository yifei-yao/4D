cmake_minimum_required(VERSION 3.14)
project(4D)

set(CMAKE_CXX_STANDARD 17)

add_executable(4D src/main.cpp src/board.cpp src/board.h src/piece.h src/color.h
        src/piece.cpp src/color.cpp src/move.cpp src/move.h src/move_gen.cpp
        src/move_gen.h src/position.cpp src/position.h src/game.cpp src/game.h
        src/infinity.h src/move_order.cpp src/move_order.h src/zobrist.cpp
        src/zobrist.h src/transposition_table.cpp src/transposition_table.h
        src/eval.cpp src/eval.h src/time_controller.cpp src/time_controller.h
        src/uci.cpp src/uci.h src/search.cpp src/search.h)