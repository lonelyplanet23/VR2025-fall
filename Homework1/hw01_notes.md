# Cmake学习1
+ 文件结构
```
    my_project/
    ├── CMakeLists.txt
    ├── include/
    │   └── my_project.h
    └── src/
        └── my_project.cpp
        ├── main.cpp

```
+ 构建项目
创建构建目录
在项目根目录下创建一个 build 文件夹：

5.2 生成构建文件
运行以下命令生成构建文件：

5.3 编译项目
运行以下命令编译项目：

## VS Code 中使用 CMake GUI 插件
2️⃣ 初始化配置

打开项目 → 点击底部状态栏的 “⚙️ CMake: [Debug]”
会提示：

选择编译器（g++, clang, msvc）

自动创建 build/ 文件夹

3️⃣ 常见操作（插件按钮）
操作	说明
🧱 Configure	检查并生成构建配置
🔨 Build	编译项目，生成可执行文件
▶️ Run	运行
🐞 Debug	启动调试器

CMake Tools 插件会自动检测你的 CMakeLists.txt，显示在 GUI 面板里。

# C++ learning
1. static_cast<Type> 强制类型转换


