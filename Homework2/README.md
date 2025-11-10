# 4. 光照模型和阴影贴图

欢迎大家来到《实时三维图形基础》课程的第四次作业！

**尽量不要在目录中包含中文字符！**

## 作业说明

渲染（Rendering），是计算机图形学的重要内容。本次作业旨在让大家实现一些基础的实时渲染功能：
- 实现 MVP 变换，理解矩阵在图形学中的应用；
- 实现 Phong Shading，理解光照模型的具体计算方法；
- 实现 Shadow Mapping，在三维场景中实现较真实的阴影功能；
- （可选）使用纹理贴图可视化的方法对阴影贴图进行调试（见 DEBUG.md 文件）。

## 作业提交

- 提交内容：程序代码和实验报告PPT。程序代码仅需要提交 src/ 下的内容。
- 提交位置：https://bhpan.buaa.edu.cn/link/AA43F0DCA76D9740269BE92EC8EF130D0A，提交至“第二次作业”子目录下。

## 作业要求

修改和完善 `src/maim.cpp` 和 `src/shaders/light_shadow.frag`，实现 **Phong 着色模型** 和 **阴影贴图** 的功能。

## 项目结构

本项目目录包含以下若干子目录：

- `src/` - 作业项目的源代码：
    - `main.cpp` - 函数的入口。除了基本的显示功能外，通过键盘和鼠标事件，利用 camera 实现了视角和移动的功能。
    - `shaders/` - 本次作业中所用到的着色器
    - `utils/` - 作业项目中用到的工具函数和工具类，其中 `gl/` 文件夹下对 opengl 的一些功能进行了封装。
- `deps/` - 本次作业的依赖文件，请不要随意修改和移动这部分的文件。
- `resources/` - 本次作业需要小牛模型、平面模型，以及他们的材质。

## 作业指导

### 2.1

1. 在 `utils/transform.cpp` 中，完成 `get_scale_matrix` 、 `get_trans_matrix` 和 `rotate_with` 函数（2.1.1），实现物体在空间中的变换；
2. 在 `utils/transform.cpp` 中，完成 `perspective` 、 `orthographic` 和 `look_at` 函数（2.1.2），将物体变换到相机的局部坐标和裁剪空间中；
3. 完成 2.1 部分后，可以使用键盘的 wasd 对视角进行操控。

### 2.2

1. 在 `shaders/light_shadow.frag` 中，完成Phong Shading相关的代码（2.2.1）；
2. 在 `main.cpp`和`shaders/light_shadow.frag` 中，完成 Shadow Mapping 相关的代码（2.2.2）；
3. 使用空格切换是否有阴影的状态。

注意事项：
- 相机的参数可以直接通过 `Camera` 类获取；
- `draw_imgui` 函数实现了一些调试功能，可以直观地看到 `view` 矩阵和 `projection` 矩阵的变化，如果有其他需要观察的参数，也可以在这里添加；
- 可以在整个项目中搜索 `TODO` 关键字找到需要完成的部分；

## 作业参考

1. Eigen库的[官方文档](http://eigen.tuxfamily.org/dox/modules.html)以及[中文文档](https://github.com/qixianyu-buaa/EigenChineseDocument)
2. [GAMES101课程](https://www.bilibili.com/video/BV1X7411F744)中的第三讲和第四讲