CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -g -std=c11

GUI_SRC = $(wildcard gui/cli/*.c)
TETRIS_SRC = $(wildcard brick_game/*.c)

GUI_HDR = $(wildcard gui/cli/*.h)
TETRIS_HDR = $(wildcard brick_game/*.h)

GUI_OBJ = $(patsubst %.c, %.o, $(GUI_SRC))
TETRIS_OBJ = $(patsubst %.c, %.o, $(TETRIS_SRC))

PROJECT = tetris
TEST_DIR = tests
BUILD_DIR = build

OS = $(shell uname)

ifeq ($(OS),Linux)
    det_OS = -lcheck -lrt -lpthread -lsubunit -lm -lncurses
else
    det_OS = -lcheck -lpthread -lncurses
endif

all: gcov_report dist

check:
	cp ../materials/linters/.clang-format ./gui/cli/
	cp ../materials/linters/.clang-format ./brick_game/
	clang-format -style=Google -n $(GUI_SRC) $(GUI_HDR)
	clang-format -style=Google -n $(TETRIS_SRC) $(TETRIS_HDR)
	clang-format -style=Google -i $(GUI_SRC) $(GUI_HDR)
	clang-format -style=Google -i $(TETRIS_SRC) $(TETRIS_HDR)
	rm ./gui/cli/.clang-format
	rm ./brick_game/.clang-format
	cppcheck --enable=all --suppress=missingIncludeSystem  $(GUI_SRC) $(TETRIS_SRC) $(TETRIS_HDR) $(GUI_HDR)

tetris: tetris.a $(GUI_OBJ)
	$(CC) $(CFLAGS) $(GUI_OBJ) tetris.a -o $@ $(det_OS)

tetris.a: $(TETRIS_OBJ)
	ar rcs tetris.a $(TETRIS_OBJ)
	ranlib tetris.a

$(GUI_OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(det_OS)

$(TETRIS_OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(det_OS)

gcov_report: clean test
	lcov -t "result" -o res.info -c -d .
	genhtml -o report res.info
	open report/index.html

test: tetris.a
	$(CC) $(CFLAGS) --coverage tests/tests.c $(TETRIS_SRC) -o $(TEST_DIR)/$@ $(det_OS)
	./tests/test

valgrind: clean test
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./tests/test

install:
	make tetris && mkdir -p $(BUILD_DIR) && mv ./$(PROJECT) ./$(BUILD_DIR)

unistall:
	rm -rf ./$(BUILD_DIR)

play:
	./$(BUILD_DIR)/$(PROJECT)

clean:
	rm -rf report **/*.gc* **/**/*.o **/*.o *.info *.a test.dSYM tetris.dSYM tetris a.out

dvi: 
	doxygen
	google-chrome documentation/html/index.html

dist: install dvi
	mkdir -p dist
	cp ./$(BUILD_DIR)/$(PROJECT) ./dist/$(PROJECT)
	cp -R ./documentation ./dist/documentation
	cd ./dist && tar cvzf $(PROJECT).tgz *
	cd ./dist && rm -rf $(PROJECT) documentation

origin: clean
	make unistall 
	rm -rf documentation dist **/test 

install_doxygen:
	sudo apt-get update
	sudo apt-get install graphviz
	sudo apt-get install doxygen 


