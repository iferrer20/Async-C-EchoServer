TARGET    := cserver

SRC_DIR   := src/
OBJ_DIR   := build/
SRC_FILES := $(wildcard $(SRC_DIR)*.c) $(wildcard $(SRC_DIR)*/*.c)

# obj files
OBJ_FILES := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.d,$(SRC_FILES))

# compiler options
CFLAGS += -O2 -Wall

# link options
LDFLAGS += -l ssl -lpthread

all: $(TARGET)
segf: CFLAGS += -Og -g -fsanitize=address
segf: LDFLAGS += -Og -g -fsanitize=address
segf: $(TARGET)

$(TARGET): $(OBJ_FILES)
	gcc -o $@ $^ $(LDFLAGS)
	
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	gcc -MMD -MP $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ_FILES) $(DEP_FILES)
	
-include $(DEP_FILES)
