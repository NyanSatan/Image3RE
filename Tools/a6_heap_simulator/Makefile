PROJ_NAME = a6_heap_simulator

SRC_ROOT = src
BUILD_ROOT = build

RESULT = $(BUILD_ROOT)/$(PROJ_NAME)

SOURCES = \
	$(SRC_ROOT)/arm.c \
	$(SRC_ROOT)/heap-1145.3.c \
	$(SRC_ROOT)/dummy_heap.c \
	$(SRC_ROOT)/scenario.c \
	$(SRC_ROOT)/ll.c \
	$(SRC_ROOT)/main.c

OBJECTS = $(addprefix $(BUILD_ROOT)/, $(SOURCES:.c=.o))


CC = clang
CFLAGS = -I$(SRC_ROOT)/include
CFLAGS += -fsanitize=address
CFLAGS += -MMD

LDFLAGS = -lc -lbsd
LDFLAGS += -fsanitize=address

DIR_HELPER = mkdir -p $(@D)

all: $(RESULT)

$(RESULT): $(OBJECTS)
	@$(DIR_HELPER)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_ROOT)/%.o: %.c
	@$(DIR_HELPER)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(OBJECTS:.o=.d)

.PHONY: clean

clean:
	$(RM) -rf $(BUILD_ROOT)
