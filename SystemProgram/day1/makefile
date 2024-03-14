CXX = g++
CXXFLAGS = -Wall -std=c++11
BUILT_DIR = built

# 使用函数获取所有cpp文件，为每个文件定义目标
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=$(BUILT_DIR)/%.o)
TARGETS = $(SRCS:%.cpp=$(BUILT_DIR)/%)

# 首个目标是默认目标，这里设置为编译所有组件
all: $(TARGETS)

# 为每个目标文件定义规则
$(BUILT_DIR)/%: %.cpp | $(BUILT_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(@:%=%).o
	$(CXX) $(@:%=%).o -o $@

# 创建输出目录
$(BUILT_DIR):
	mkdir -p $@

# 清理编译产物
clean:
	rm -rf $(BUILT_DIR)

.PHONY: all clean
