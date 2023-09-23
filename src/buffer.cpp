#include "buffer.hpp"
#include <stdexcept>

void validateBufferMappingAccess (GLbitfield access) {
    static constexpr GLbitfield possibleAccess = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
    static constexpr GLbitfield impossibleAccess = ~possibleAccess;
    if (access & impossibleAccess) {
        throw std::logic_error ("unrecognized access flags given");
    }
    if (!(access & GL_MAP_READ_BIT) && !(access & GL_MAP_WRITE_BIT)) {
        throw std::logic_error ("mapping has no read or write permissions");
    }
    if ((access & (GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)) && (access & GL_MAP_READ_BIT)) {
        throw std::logic_error ("cannot invalidate buffer with read permissions");
    }
    if ((access & GL_MAP_FLUSH_EXPLICIT_BIT) && !(access & GL_MAP_WRITE_BIT)) {
        throw std::logic_error ("cannot have explicit flush without write permissions");
    }
}



#ifdef DEBUG
void bufferMapping::checkPreviousMapping () {
    GLint isMapped;
    glGetNamedBufferParameteriv (name, GL_BUFFER_MAPPED, &isMapped);
    if (isMapped) {
        throw std::logic_error ("buffer already bound");
    }
}

void bufferMapping::setFlushable () {
    if (access & GL_MAP_FLUSH_EXPLICIT_BIT) {
	flushable = true;
    } else {
	flushable = false;
    }
}

void bufferMapping::assertFlushable () {
    if (!flushable) {
	throw std::logic_error ("cannot flush mapping that was not specified with GL_MAP_FLUSH_EXPLICIT_BIT flag");
    }
}

#endif

bufferMapping::bufferMapping (GLuint _name, GLintptr _offset, GLsizeiptr _length, GLbitfield _access) : name (_name), offset (_offset), length (_length), access (_access) {
#ifdef DEBUG
    validateBufferMappingAccess (access);
    checkPreviousMapping ();
    setFlushable ();
#endif
    data = glMapNamedBufferRange (name, offset, length, access);
}

bufferMapping::bufferMapping (bufferMapping && r) {
    data = r.data;
    name = r.name;
    offset = r.offset;
    length = r.length;
    access = r.access;
    r.data = nullptr;
}

void bufferMapping::flushRange (GLintptr flushOffset, GLsizeiptr flushLength) {
#ifdef DEBUG
    assertFlushable ();
#endif
    glFlushMappedNamedBufferRange (name, flushOffset, flushLength);
}

bufferMapping::~bufferMapping () {
    if (data) {
        glUnmapNamedBuffer (name);
    }
}


void fixedSizeBuffer::unbind (GLenum target) {
    glBindBuffer (target, 0);
}

#ifdef DEBUG
void fixedSizeBuffer::checkDynamic () {
    if (!dynamicStorage) {
        throw std::logic_error ("cannot use subdata on buffer without dynamic storage");
    }
}
void fixedSizeBuffer::checkMappingFlags (GLbitfield access) {
    access &= (GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    if ((~mapAccessFlags) & access) {
        throw std::logic_error ("cannot use flags for mapping that were not given in buffer storage");
    }
}
#endif

fixedSizeBuffer::fixedSizeBuffer (GLsizeiptr size, GLbitfield flags, const void * data) {
    glCreateBuffers (1, &name);
    glNamedBufferStorage (name, size, data, flags);
#ifdef DEBUG
    if (flags & GL_DYNAMIC_STORAGE_BIT) {
        dynamicStorage = true;
    }
    mapAccessFlags = flags & (GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
#endif
}

fixedSizeBuffer::fixedSizeBuffer (fixedSizeBuffer && r) {
    name = r.name;
    r.name = 0;
#ifdef DEBUG
    dynamicStorage = r.dynamicStorage;
    mapAccessFlags = r.mapAccessFlags;
#endif
}

fixedSizeBuffer::~fixedSizeBuffer () {
    glDeleteBuffers (1, &name);
}

void fixedSizeBuffer::bind (GLenum target) {
    glBindBuffer (target, name);
}

void fixedSizeBuffer::bindRange (GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) {
    glBindBufferRange (target, index, name, offset, size);
}

void fixedSizeBuffer::subdata (GLintptr offset, GLsizeiptr size, const void * data) {
#ifdef DEBUG
    checkDynamic ();
#endif
    glNamedBufferSubData (name, offset, size, data);
}

bufferMapping fixedSizeBuffer::map (GLintptr offset, GLsizeiptr length, GLbitfield access) {
#ifdef DEBUG
    checkMappingFlags(access);
#endif
    return bufferMapping (name, offset, length, access);
}

#ifdef DEBUG
void buffer::validateAccess (GLbitfield access) {
    if (access & (GL_MAP_PERSISTENT_BIT & GL_MAP_COHERENT_BIT)) {
        throw std::logic_error ("cannot use persistent bit or coherent bit on buffer of not fixed size");
    }
}
#endif

buffer::buffer () {
    glCreateBuffers (1, &name);
}

buffer::buffer (GLsizeiptr size, GLenum usage, const void * data) {
    glCreateBuffers (1, &name);
    glNamedBufferData (name, size, data, usage);
}

buffer::buffer (buffer && r) {
    name = r.name;
    r.name = 0;
}

buffer::~buffer () {
    glDeleteBuffers (1, &name);
}
void buffer::bind (GLenum target) {
    glBindBuffer (target, name);
}

void buffer::bindRange (GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) {
    glBindBufferRange (target, index, name, offset, size);
}

void buffer::data (GLsizeiptr size, const void * data, GLenum usage) {
    glNamedBufferData (name, size, data, usage);
}
void buffer::subdata (GLintptr offset, GLsizeiptr size, const void * data) {
    glNamedBufferSubData (name, offset, size, data);
}
bufferMapping buffer::map (GLintptr offset, GLsizeiptr length, GLbitfield access) {
#ifdef DEBUG
    validateBufferMappingAccess(access);
#endif
    return bufferMapping (name, offset, length, access);
}
void buffer::unbind (GLenum target) {
    glBindBuffer (target, 0);
}
