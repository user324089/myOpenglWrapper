#include "texture.hpp"
#include <stdexcept>

void textureDimentionPolicy<1>::checkTypeValidity (GLenum type) {
    if (type == GL_TEXTURE_1D || type == GL_PROXY_TEXTURE_1D) {
	return;
    }
    throw std::runtime_error ("invalid texture type");
}
void textureDimentionPolicy<1>::texImage (const textureDataInfo & info, GLenum type, GLint level) {
    glPixelStorei (GL_UNPACK_ALIGNMENT, info.alignment);

    glPixelStorei (GL_UNPACK_SKIP_PIXELS, info.usedX);

    glTexImage1D (type, level, info.internalFormat, info.usedWidth, 0, info.format, info.dataType, info.data); 
}


void textureDimentionPolicy<2>::checkTypeValidity (GLenum type) {
    constexpr static GLenum validTypes [] = {
	GL_TEXTURE_2D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE,
	GL_PROXY_TEXTURE_2D, GL_PROXY_TEXTURE_1D_ARRAY, GL_PROXY_TEXTURE_RECTANGLE,
	GL_PROXY_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
    for (unsigned int i = 0; i < sizeof(validTypes) / sizeof(GLenum); i++) {
	if (type == validTypes[i]) {
	    return;
	}
    }
    throw std::runtime_error ("invalid texture type");
}

void textureDimentionPolicy<2>::texImage (const textureDataInfo & info, GLenum type, GLint level) {
    glPixelStorei (GL_UNPACK_ALIGNMENT, info.alignment);

    glPixelStorei (GL_UNPACK_ROW_LENGTH, info.totalWidth);

    glPixelStorei (GL_UNPACK_SKIP_PIXELS, info.usedX);
    glPixelStorei (GL_UNPACK_SKIP_ROWS, info.usedY);

    glTexImage2D (type, level, info.internalFormat, info.usedWidth, info.usedHeight, 0, info.format, info.dataType, info.data);
}


void textureDimentionPolicy<3>::checkTypeValidity (GLenum type) {
    constexpr static GLenum validTypes [] = {
	GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_CUBE_MAP_ARRAY, GL_PROXY_TEXTURE_CUBE_MAP_ARRAY,
	GL_PROXY_TEXTURE_3D, GL_PROXY_TEXTURE_2D_ARRAY
    };
    for (unsigned int i = 0; i < sizeof(validTypes) / sizeof(GLenum); i++) {
	if (type == validTypes[i]) {
	    return;
	}
    }
    throw std::runtime_error ("invalid texture type");
}

void textureDimentionPolicy<3>::texImage (const textureDataInfo & info, GLenum type, GLint level) {
    glPixelStorei (GL_UNPACK_ALIGNMENT, info.alignment);

    glPixelStorei (GL_UNPACK_ROW_LENGTH, info.totalWidth);
    glPixelStorei (GL_UNPACK_IMAGE_HEIGHT, info.totalHeight);

    glPixelStorei (GL_UNPACK_SKIP_PIXELS, info.usedX);
    glPixelStorei (GL_UNPACK_SKIP_ROWS, info.usedY);
    glPixelStorei (GL_UNPACK_SKIP_IMAGES, info.usedZ);
    glTexImage3D (type, level, info.internalFormat, info.usedWidth, info.usedHeight, info.usedDepth, 0, info.format, info.dataType, info.data); 
}


template <unsigned int DIMENTIONS>
texture<DIMENTIONS>::texture (GLenum _type) : type (_type) {
#ifdef DEBUG
    textureDimentionPolicy<DIMENTIONS>::checkTypeValidity (type);
#endif
    glCreateTextures (type, 1, &name);
    glTextureParameteri (name, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri (name, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri (name, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri (name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri (name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

template <unsigned int DIMENTIONS>
texture<DIMENTIONS>::texture (texture&& r) : name (r.name), type (r.type) {
    r.name = 0;
}

template <unsigned int DIMENTIONS>
void texture<DIMENTIONS>::setParameter (GLenum parameter, GLint value) {
    glTextureParameteri (name, parameter, value);
}
template <unsigned int DIMENTIONS>
void texture<DIMENTIONS>::bind (int unit) {
    glBindTextureUnit (unit, name);
}
template <unsigned int DIMENTIONS>
void texture<DIMENTIONS>::unbind (int unit) {
    glBindTextureUnit (unit, 0);
}
template <unsigned int DIMENTIONS>
void texture<DIMENTIONS>::data (const textureDataInfo& info, int level) {
    glBindTexture (type, name);

    textureDimentionPolicy<DIMENTIONS>::texImage (info, type, level);

    glBindTexture (type, 0);
}
template <unsigned int DIMENTIONS>
texture<DIMENTIONS>::~texture () {
    glDeleteTextures (1, &name);
}

template <unsigned int DIMENTIONS>
fixedSizeTexture<DIMENTIONS>::fixedSizeTexture (GLenum _type, const textureStorageInfo & storageInfo) : type (_type) {
    glCreateTextures (type, 1, &name);
#ifdef DEBUG
    textureDimentionPolicy<DIMENTIONS>::checkTypeValidity (type);
#endif
    textureDimentionPolicy<DIMENTIONS>::texStorage (storageInfo, name);
}

template <unsigned int DIMENTIONS>
fixedSizeTexture<DIMENTIONS>::fixedSizeTexture (fixedSizeTexture && r) : name (r.name), type (r.type) {
    r.name = 0;
}

template <unsigned int DIMENTIONS>
fixedSizeTexture<DIMENTIONS>::~fixedSizeTexture () {
    glDeleteTextures (1, &name);
}
template <unsigned int DIMENTIONS>
void fixedSizeTexture<DIMENTIONS>::clear (GLint level, GLenum format, GLenum dataType, const void *data) {
    glClearTexImage (name, level, format, dataType, data);
}
template <unsigned int DIMENTIONS>
void fixedSizeTexture<DIMENTIONS>::setParameter (GLenum parameter, GLint value) {
    glTextureParameteri (name, parameter, value);
}
template <unsigned int DIMENTIONS>
void fixedSizeTexture<DIMENTIONS>::bind (int unit) {
    glBindTextureUnit (unit, name);
}
template <unsigned int DIMENTIONS>
void fixedSizeTexture<DIMENTIONS>::unbind (int unit) {
    glBindTextureUnit (unit, 0);
}

template class texture<1>;
template class texture<2>;
template class texture<3>;

template class fixedSizeTexture<1>;
template class fixedSizeTexture<2>;
template class fixedSizeTexture<3>;
