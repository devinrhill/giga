#ifndef GIGA_SHADER_H
#define GIGA_SHADER_H

#include <GL/gl.h>
#include <iostream>
#include <string>
#include "bytestream.h"

namespace giga {
namespace gfx {

class Shader {
public:
    ~Shader();

    void openFile(const std::string& filename);
    void destroy();

    // Getters
    GLuint getId() const noexcept;
private:
    GLuint _id = 0;
};

class ShaderProgram: public std::vector<Shader> {
public:
    ~ShaderProgram();
    
    void finalize();
    void destroy();

    // Getters
    GLuint getId() const noexcept;
private:
    GLuint _id = 0;
};

} // namespace gfx
} // namespace giga

#endif // GIGA_SHADER_H
