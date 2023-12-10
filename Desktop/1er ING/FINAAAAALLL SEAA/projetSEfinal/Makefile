SRC_DIR := algorithms/src
BUILD_DIR := algorithms/build
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
CFLAGS := -Wall -g

all: $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%) main

main: main.c
	gcc $(CFLAGS) main.c -o main
	@chmod a+x main

$(BUILD_DIR)/%: $(SRC_DIR)/%.c $(SRC_DIR)/main.h
	@mkdir -p $(@D)
	gcc $(CFLAGS) $< -o $@
	@chmod a+x $@
clean:
	rm -rf algorithms/build
	rm -f main
