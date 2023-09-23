#ifndef BUFFER
#define BUFFER

#include <GL/glew.h>

class bufferMapping {
    private:
        GLuint name;
        GLintptr offset;
        GLsizeiptr length;
        GLbitfield access; // available access flags: GL_MAP_READ_BIT, GL_MAP_WRITE_BIT,
                           // GL_MAP_INVALIDATE_RANGE_BIT, GL_MAP_INVALIDATE_BUFFER_BIT,
                           // GL_MAP_FLUSH_EXPLICIT_BIT, GL_MAP_UNSYNCHRONIZED_BIT,
                           // GL_MAP_PERSISTENT_BIT, GL_MAP_COHERENT_BIT
        void * data = nullptr;

#ifdef DEBUG
	bool flushable = false;
        void checkPreviousMapping ();
	void setFlushable ();
	void assertFlushable ();
#endif // DEBUG
    public:
        bufferMapping (GLuint _name, GLintptr _offset, GLsizeiptr _length, GLbitfield _access);
        bufferMapping (const bufferMapping & r) = delete;
        bufferMapping & operator = (const bufferMapping & r) = delete;
        bufferMapping (bufferMapping && r);
	void flushRange (GLintptr flushOffset, GLsizeiptr flushLength);
        void * getData () { return data; }
        ~bufferMapping ();
};

class fixedSizeBuffer {
    private:
        GLuint name;

#ifdef DEBUG
        bool dynamicStorage = false;
        GLbitfield mapAccessFlags = 0;
        void checkDynamic ();
        void checkMappingFlags (GLbitfield access);
#endif
    public:
        fixedSizeBuffer (GLsizeiptr size, GLbitfield flags, const void * data = nullptr);
        fixedSizeBuffer (const fixedSizeBuffer & r) = delete;
        fixedSizeBuffer & operator = (const fixedSizeBuffer & r) = delete;
        fixedSizeBuffer (fixedSizeBuffer && r);
        ~fixedSizeBuffer ();
        void bind (GLenum target);
        void bindRange (GLenum target, GLuint index, GLintptr offset, GLsizeiptr size);
        void subdata (GLintptr offset, GLsizeiptr size, const void * data);
        bufferMapping map (GLintptr offset, GLsizeiptr length, GLbitfield access);
        GLuint getName() {return name;}
        static void unbind (GLenum target);
};

class buffer {
    private:
        GLuint name;
#ifdef DEBUG
        void validateAccess (GLbitfield access);
#endif
    public:
        buffer ();
	buffer (GLsizeiptr size, GLenum usage, const void * data = nullptr);
        buffer (const buffer & r) = delete;
        buffer & operator = (const buffer & r) = delete;
        buffer (buffer && r);
        ~buffer ();
        void bind (GLenum target);
	void bindRange (GLenum target, GLuint index, GLintptr offset, GLsizeiptr size);
        void data (GLsizeiptr size, const void * data, GLenum usage);
        void subdata (GLintptr offset, GLsizeiptr size, const void * data);
        bufferMapping map (GLintptr offset, GLsizeiptr length, GLbitfield access);
        GLuint getName() {return name;}
        static void unbind (GLenum target);
};


#endif // BUFFER
