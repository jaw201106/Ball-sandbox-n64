V=1
include $(N64_INST)/include/n64.mk

PROG_NAME = ball_physics

BUILD_DIR = build
SRCS = main.cpp
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Pass mandatory C++ build configurations into libdragon architecture
CXXFLAGS += -std=gnu++17

all: $(PROG_NAME).z64

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "    [CXX] $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROG_NAME).elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.elf *.z64

.PHONY: all clean

-include $(OBJS:%.o=%.d)
