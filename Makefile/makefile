BUILT_DIR = built
TARGET = $(BUILT_DIR)/app
INCLUDE_DIR = -Iinclude # 搜索头文件
SRC_DIR = SRC  
SRCS = $(wildcard $(SRC_DIR)/*.cpp) # 搜索源文件
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILT_DIR)/%.o,$(SRCS)) # 搜索.o文件

$(TARGET): $(OBJS) | $(BUILT_DIR) # 这里的BUILT_DIR是先决条件，表示在构建项目之前，要确保built目录的创建
	g++ $(OBJS) -o $(TARGET) $(INCLUDE_DIR)

$(BUILT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILT_DIR)
	g++ -c $< -o $@ $(INCLUDE_DIR)

$(BUILT_DIR):
	mkdir -p $(BUILT_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILT_DIR)

# 自动变量:
# $^  在依赖项中，所有不重复的依赖文件，这些文件之间以空格分开
# $@ 表示目标文件的名称，包含文件扩展名
# $< 表示以来像中第一个依赖文件的名称