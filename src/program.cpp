#include "program.hpp"

shader::shader (GLenum type) {
    name = glCreateShader(type);
}
shader::~shader () {
    glDeleteShader (name);
}
void shader::make (GLsizei count, const GLchar *const *string) {
    glShaderSource (name, count, string, nullptr);
    glCompileShader (name);

    GLint success;
    glGetShaderiv (name, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar buffer [512];
        glGetShaderInfoLog (name, 512, nullptr, buffer);
        throw std::runtime_error (std::string ("error with shader compilation: ") + buffer);
    }
}

program::program () {
    name = glCreateProgram ();
}
void program::makeSingular (GLenum type, GLsizei count, const GLchar *const *strings) {
    glDeleteProgram (name);
    name = glCreateShaderProgramv (type, count, strings);
}
void program::use () {
    glUseProgram (name);
}
GLint program::getUniformLocation (const char * uniformName) {
    return glGetUniformLocation (name, uniformName);
}
program::~program () {
    glDeleteProgram (name);
}
void program::unUse () {
    glUseProgram (0);
}

programPipeline::programPipeline () {
    glCreateProgramPipelines (1, &name);
}
void programPipeline::useProgram (program& p, GLbitfield stages) {
    glUseProgramStages (name, stages, p.getName());
}
void programPipeline::activeProgram (program& p) {
    glActiveShaderProgram (name, p.getName());
}
void programPipeline::activeProgram (GLenum type) {
    GLint shaderName;
    glGetProgramPipelineiv (name, type, &shaderName);
    glActiveShaderProgram (name, shaderName);
}
void programPipeline::use () {
    glBindProgramPipeline (name);
}
programPipeline::~programPipeline () {
    glDeleteProgramPipelines (1, &name);
}
void programPipeline::unUse () {
    glBindProgramPipeline (0);
}
