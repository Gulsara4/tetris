
CXX1 := g++
CXX2 := gcc
CXXFLAGS := -Wall -Wextra -Werror 
LDFLAGS := -lncursesw
TEST_FLAGS := -lgtest -lgtest_main -pthread -fprofile-arcs -ftest-coverage

# Директории
SRC_DIR1 := brick_game/snake
SRC_DIR2 := brick_game/tetris
TEST_DIR1 := tests/snake
TEST_DIR2 := tests/tetris
CLI_DIR1 := gui/cli/snake
CLI_DIR2 := gui/cli/tetris
GUI_DIR1 :=gui/desktop/desktop_snake
GUI_DIR2 :=gui/desktop/desktop_tetris
STATIC_LIB =s21_snake_lib.a

TEX = documentation_snake.tex
PDF = documentation_snake.pdf
SRC1 := $(filter-out $(SRC_DIR1)/main_snake.cpp, $(wildcard $(SRC_DIR1)/*.cpp))
SRC2 := $(filter-out $(SRC_DIR2)/main_tetris.c, $(wildcard $(SRC_DIR2)/*.c))

OBJ1 := $(SRC1:.cpp=.o)
OBJ2 := $(SRC2:.c=.o)

MAIN1 := $(SRC_DIR1)/main_snake.cpp
MAIN2 := $(SRC_DIR2)/main_tetris.c

all: $(STATIC_LIB) install 

$(STATIC_LIB): $(OBJ1)
	ar rc $@ $(OBJ1)
	ranlib $@

install: $(STATIC_LIB) $(MAIN1)
	$(CXX1) $(MAIN1) $(CLI_DIR1)/frontend.cpp $(STATIC_LIB) -DNOT_TEST $(CXXFLAGS) -std=c++17 -o snake $(LDFLAGS)

uninstall:
	rm snake

clean:
	rm -rf $(SRC_DIR1)/*.o  $(SRC_DIR2)/*.o
	rm -rf snake tetris test menu
	rm -rf *.a
	rm -rf *.gcda *.gcno
	rm -rf $(SRC_DIR1)/*.html $(TEST_DIR1)/*.html
	rm -rf coverage.info
	rm -rf coverage-report
	rm -rf snake.tar.gz
	rm -rf tmp1 tmp2 *.exe
	
dvi: 
	pdflatex $(TEX)
dist:
	tar -czvf snake.tar.gz $(SRC1) $(MAIN1) $(CLI_DIR1)/frontend.cpp Makefile $(TEX) $(TEST_DIR1)/test.cpp FSM_snake.png 

tetris: $(OBJ2) $(MAIN2)
	$(CXX2) -std=c11 $(OBJ2) $(MAIN2) $(CLI_DIR2)/frontend.c  $(CXXFLAGS) -o tetris $(LDFLAGS)

test:
	$(CXX1) $(TEST_DIR1)/test.cpp $(SRC1) $(TEST_FLAGS) -o test $(LDFLAGS)
	./test
	
report:
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage-report
	
menu1: 	install tetris
	$(CXX1) main.cpp -DCLI -o menu1 
	./menu1
menu2: install tetris desktop_snake desktop_tetris
	$(CXX1) main.cpp -DDESKTOP -o menu2 
	./menu2
clang:
	clang-format -i  $(SRC_DIR1)/*.cpp $(SRC_DIR1)/*.h  
	clang-format -i $(SRC_DIR2)/*.c  $(SRC_DIR2)/*.h 
	clang-format -i $(TEST_DIR1)/*.cpp $(TEST_DIR2)/*.c
	clang-format -i $(CLI_DIR1)/*.cpp $(CLI_DIR2)/*.c 
	clang-format -i $(GUI_DIR1)/*.cpp $(GUI_DIR2)/*.cpp 
	clang-format -i $(GUI_DIR1)/*.h $(GUI_DIR2)/*.h
	clang-format -i main.cpp


desktop_tetris:
	mkdir -p tmp1
	cd tmp1 && cmake ../gui/desktop/desktop_tetris
	cd tmp1 && ninja
	mv tmp1/desktop_tetris.exe .
desktop_snake:
	mkdir -p tmp2
	cd tmp2 && cmake ../gui/desktop/desktop_snake
	cd tmp2 && ninja
	mv tmp2/snake_gui.exe .
