#version 330 core  
// 输入顶点属性：位置（3分量）和纹理坐标（2分量）
layout (location = 0) in vec3 aPos;       // 顶点位置（与VAO属性索引0绑定）
layout (location = 1) in vec2 aTexCoord;  // 纹理坐标（与VAO属性索引1绑定）

// 输出到片段着色器的纹理坐标（需与片段着色器输入变量完全匹配）
out vec2 TexCoord;

void main()
{
    // 直接使用输入位置作为NDC坐标，确保四边形全屏显示
    gl_Position = vec4(aPos, 1.0);
    // 将纹理坐标传递给片段着色器，用于后续纹理采样
    TexCoord = aTexCoord;
}