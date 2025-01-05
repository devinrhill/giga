#include <format>
#include <filesystem>
#include <GL/glew.h>
#include "giga/error.h"
#include "giga/shader.h"

namespace giga {
namespace gfx {

Shader::~Shader() {
    destroy();
}

void Shader::openFile(const std::string& filename) {
    if(filename.empty()) {
        throw err::FormatException<std::invalid_argument>("Filename is empty");
    }

    GLenum shaderType;
    if(filename.ends_with(".vert")) { // vertex shader
        shaderType = GL_VERTEX_SHADER;
    } else if(filename.ends_with(".geom")) { // geometry shader
        shaderType = GL_GEOMETRY_SHADER;
    } else if(filename.ends_with(".frag")) { // fragment shader
        shaderType = GL_FRAGMENT_SHADER;
    } else { // unknown shader
        throw err::FormatException<std::runtime_error>(std::format("Unknown shaderfile type '{}'", std::string(std::filesystem::path(filename).extension())));
    }

    Bytestream bytestream;
    try {
        bytestream.openFile(filename);
    } catch(const std::exception& e) {
        throw std::runtime_error("error opening file via Bytestream: " + std::string(e.what()));
    }
    const std::uint8_t* tmpCode = bytestream.getBuf();

    _id = glCreateShader(shaderType);
    glShaderSource(_id, 1, reinterpret_cast<const GLchar**>(&tmpCode), nullptr);
    glCompileShader(_id);

    int success;
    char log[512];
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(_id, 512, nullptr, log);
        throw std::runtime_error("couldn't compile shader: " + std::string(log));
    }
}

void Shader::destroy() {
    glDeleteShader(_id);
}

GLuint Shader::getId() const noexcept {
    return _id;
}

ShaderProgram::~ShaderProgram() {
    destroy();
}

void ShaderProgram::finalize() {
    _id = glCreateProgram();

    for(const Shader& shader: *this) {
        glAttachShader(_id, shader.getId());
    }

    glLinkProgram(_id);
}

void ShaderProgram::destroy() {
    glDeleteProgram(_id);
}

GLuint ShaderProgram::getId() const noexcept {
    return _id;
}

} // namespace gfx
} // namespace giga
