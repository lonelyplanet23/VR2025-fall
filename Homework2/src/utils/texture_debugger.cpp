#include <memory>
#include <map>
#include <string>
#include <vector>

#include "utils/shader.h"
#include "utils/gl/texture.h"
#include "utils/gl/vertex_array.h"
#include "utils/tools.h"
#include "texture_debugger.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Eigen/Dense"

#define SHADER_DIR "C:/Users/27628/Desktop/VR2025-fall/Homework2/src//shaders"




TextureDebugger::TextureDebugger() {
    // 初始化调试着色器（传入顶点/片段着色器文件路径）
    debug_shader = std::make_unique<Utils::Shader>(SHADER_DIR"/debugger.vert", SHADER_DIR"/debugger.frag");

    // 顶点数据：位置（x,y,z）+ 纹理坐标（u,v）
    std::vector<vecf3> positions = {
        { 1.0f,  1.0f, 0.0f},  // 右上角
        { 1.0f, -1.0f, 0.0f},  // 右下角
        {-1.0f, -1.0f, 0.0f},  // 左下角
        {-1.0f,  1.0f, 0.0f}   // 左上角
    };
    std::vector<vecf2> texcoords = {
        {1.0f, 1.0f}, 
        {1.0f, 0.0f}, 
        {0.0f, 0.0f},
        {0.0f, 1.0f} 
    };
    
    // 索引数据：通过索引复用顶点，组成两个三角形
    std::vector<veci3> indices = {
        {0, 1, 3},  // 第一个三角形（右上角、右下角、左上角）
        {1, 2, 3}   // 第二个三角形（右下角、左下角、左上角）
    };

    // 创建并配置顶点数组对象（VAO）、顶点缓冲对象（VBO）和索引缓冲对象（EBO），确保顶点数据能正确传递给着色器。
    auto vb_pos = new Utils::GL::VertexBuffer(positions.size() * sizeof(vecf3), positions.data());
    quad_vbos["position"] = std::unique_ptr<Utils::GL::VertexBuffer>(vb_pos);

    auto vb_uv = new Utils::GL::VertexBuffer(texcoords.size() * sizeof(vecf2), texcoords.data());
    quad_vbos["texcoord"] = std::unique_ptr<Utils::GL::VertexBuffer>(vb_uv);

    quad_eb = std::make_unique<Utils::GL::ElementBuffer>(GL_TRIANGLES, indices.size(), (GLuint*)indices.data());

    Utils::GL::VertexArray::Format format;
    format.attr_ptrs.emplace_back(quad_vbos["position"]->attr_ptr(3, GL_FLOAT, GL_FALSE, sizeof(vecf3)));
    format.attr_ptrs.emplace_back(quad_vbos["texcoord"]->attr_ptr(2, GL_FLOAT, GL_FALSE, sizeof(vecf2)));
    format.eb = quad_eb.get();

    quad_va = std::make_unique<Utils::GL::VertexArray>(std::vector<GLuint>{0, 1}, format);
}

void TextureDebugger::render(const Utils::GL::Texture2D& texture_to_debug) {
    // 1. 临时禁用干扰性渲染状态（避免深度测试、面剔除等导致绘制失败）
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // 2. 清除颜色缓冲，设置蓝色背景（与灰度阴影贴图形成明显对比，便于观察）
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 3. 激活调试着色器程序（关键步骤，确保使用目标着色器进行渲染）
    debug_shader->active_texture(0, const_cast<Utils::GL::Texture2D*>(&texture_to_debug));
    debug_shader->set_tex("ourTexture", 0); 

    // 4. 绘制调用
    quad_va->draw(*debug_shader);

    // 5. 恢复原有渲染状态（避免影响后续其他渲染逻辑）
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
