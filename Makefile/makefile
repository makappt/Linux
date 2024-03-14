BUILT_DIR = built
TARGET = $(BUILT_DIR)/app
INCLUDE_DIR = -Iinclude
SRC_DIR = SRC
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILT_DIR)/%.o,$(SRCS))

$(TARGET): $(OBJS) | $(BUILT_DIR)
	g++ $^ -o $@ $(INCLUDE_DIR)

$(BUILT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILT_DIR)
	g++ -c $< -o $@ $(INCLUDE_DIR)

$(BUILT_DIR):
	mkdir -p $(BUILT_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILT_DIR)
