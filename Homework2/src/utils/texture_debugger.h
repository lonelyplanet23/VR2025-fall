#ifndef DEBUG_TEXTURE_DEBUGGER_H
#define DEBUG_TEXTURE_DEBUGGER_H

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "utils/shader.h"
#include "utils/gl/texture.h"
#include "utils/gl/vertex_array.h"
#include "utils/tools.h"

class TextureDebugger {
public:
    TextureDebugger();
    ~TextureDebugger() = default;

    void render(const Utils::GL::Texture2D& texture_to_debug);

private:
    std::unique_ptr<Utils::Shader> debug_shader;
    std::unique_ptr<Utils::GL::VertexArray> quad_va;
    std::unique_ptr<Utils::GL::ElementBuffer> quad_eb;
    std::map<std::string, std::unique_ptr<Utils::GL::VertexBuffer>> quad_vbos;
};

#endif // DEBUG_TEXTURE_DEBUGGER_H