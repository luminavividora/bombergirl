# bombergirl Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic
LIBS = -lncurses
SRC_DIR  = ./source

SRC = $(SRC_DIR)/main.cpp \
      $(SRC_DIR)/board.cpp \
      $(SRC_DIR)/bomb.cpp \
      $(SRC_DIR)/enemies.cpp \
      $(SRC_DIR)/game.cpp \
      $(SRC_DIR)/leaderboard.cpp \
      $(SRC_DIR)/level.cpp \
      $(SRC_DIR)/maps.cpp \
      $(SRC_DIR)/menu.cpp \
      $(SRC_DIR)/name_entry.cpp \
      $(SRC_DIR)/parser.cpp \
      $(SRC_DIR)/player.cpp \
      $(SRC_DIR)/powerup.cpp \
      $(SRC_DIR)/random.cpp \
      $(SRC_DIR)/render.cpp

OUT = bombergirl

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)

debug:
	$(CXX) $(CXXFLAGS) -g -O0 -DDEBUG_MODE $(SRC) -o $(OUT)_debug $(LIBS)


clean:
	rm -f $(OUT) $(OUT)_debug
