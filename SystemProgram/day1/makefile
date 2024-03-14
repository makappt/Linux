BUILT_DIR = built
TARGET = $(BUILT_DIR)/app
SRCS = $(wildcard *.cpp) 
OBJS = $(patsubst %.cpp,$(BUILT_DIR)/%.o,$(SRCS)) 
$(TARGET): $(OBJS) | $(BUILT_DIR) 
	g++ $(OBJS) -o $(TARGET)

$(BUILT_DIR)/%.o: %.cpp | $(BUILT_DIR)
	g++ -c $< -o $@

$(BUILT_DIR):
	mkdir -p $(BUILT_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILT_DIR)
