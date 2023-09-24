#ifndef PROGRAM
#define PROGRAM

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class shader {
    private:
        GLuint name;
    public:
        shader (GLenum type);
        shader (const shader & r) = delete;
        shader & operator = (const shader & r) = delete;
        shader (shader && r);
        ~shader ();
        void make (GLsizei count, const GLchar *const *string);
        GLuint getName () { return name;}
};


class program {
    private:
	GLuint name;
    public:
	program ();
	program (const program & r) = delete;
	program & operator = (const program & r) = delete;
    program (program && r);
	template <typename... T>
	    void make (T&... shaders);
	void makeSingular (GLenum type, GLsizei count, const GLchar *const *strings);
	void use ();
	GLuint getName () { return name; }
	GLint getUniformLocation (const char * uniformName);
	~program ();
	static void unUse ();
};

template <typename... T>
void program::make (T&... shaders) {
    (... , glAttachShader (name, shaders.getName()));
    glLinkProgram (name);
    GLint success;
    glGetProgramiv (name, GL_LINK_STATUS, &success);
    (... , glDetachShader (name, shaders.getName()));
    if (!success) {
	GLchar buffer [512];
	glGetProgramInfoLog (name, 512, nullptr, buffer);
	throw std::runtime_error (std::string("error with program linking: ") + buffer);
    }
}

class programPipeline {
    private:
        GLuint name;
    public:
        programPipeline ();
        programPipeline (const programPipeline & r) = delete;
        programPipeline & operator = (const programPipeline & r) = delete;
        programPipeline (programPipeline && r);
        void useProgram (program& p, GLbitfield stages);
        void activeProgram (program& p);
        void activeProgram (GLenum type);
        void use ();
        ~programPipeline ();
        static void unUse ();
	GLuint getName () { return name; }
};

#endif
