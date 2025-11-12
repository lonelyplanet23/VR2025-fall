#version 330 core
// 从顶点着色器接收的纹理坐标（名称、类型需与顶点着色器输出一致）
in vec2 TexCoord;
// 输出最终像素颜色
out vec4 FragColor;

// 2D纹理采样器（绑定到指定纹理单元，用于采样阴影贴图）
uniform sampler2D ourTexture;

void main() {
    // 采样阴影贴图的深度值（深度信息存储在r通道，范围[0,1]）
    float depth = texture(ourTexture, TexCoord).r;
    
    // 将深度重新映射为线性，否则贴图中几乎所有区域都接近1.0
    // 0.1/100.0是近/远平面的值，需要和main.cpp中阴影计算时的数值对应
    depth = depth * 2.0 - 1.0;
    depth = (2.0 * 0.1 * 100.0) / (100.0 + 0.1 - depth * (100.0 - 0.1));
    depth = (depth - 0.1) / (100.0 - 0.1);
    
    // 将深度值转换为灰度颜色输出（默认近物暗、远物亮）
    FragColor = vec4(vec3(depth), 1.0f);
    
    // // 调试备用：输出纯色（用于验证着色器是否正常执行）
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}       