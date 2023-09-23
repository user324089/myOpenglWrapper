#include <GL/glew.h>


template <unsigned int DIMENTIONS>
class textureDimentionPolicy {};

template <>
class textureDimentionPolicy <1> {
    public:
        static void checkTypeValidity (GLenum type);
        struct textureDataInfo {
            int usedWidth;
            int usedX;
            int alignment;
            GLenum dataType;
            GLenum format;
            GLint internalFormat;
            const void* data;
        };
        static void texImage (const textureDataInfo & info, GLenum type, GLint level);
        struct textureStorageInfo {
            int width;
            int levels;
            GLenum internalFormat;
        };
        static void texStorage (const textureStorageInfo & info, GLuint name) {
            glTextureStorage1D (name, info.levels, info.internalFormat, info.width);
        }
};

template <>
class textureDimentionPolicy <2> {
    public:
        static void checkTypeValidity (GLenum type);
        struct textureDataInfo {
            int totalWidth;
            int usedWidth, usedHeight;
            int usedX, usedY;
            int alignment;
            GLenum dataType;
            GLenum format;
            GLint internalFormat;
            const void* data;
        };
        static void texImage (const textureDataInfo & info, GLenum type, GLint level);
        struct textureStorageInfo {
            int width, height;
            int levels;
            GLenum internalFormat;
        };
        static void texStorage (const textureStorageInfo & info, GLuint name) {
            glTextureStorage2D (name, info.levels, info.internalFormat, info.width, info.height);
        }
};

template <>
class textureDimentionPolicy <3> {
    public:
        static void checkTypeValidity (GLenum type);
        struct textureDataInfo {
            int totalWidth, totalHeight;
            int usedWidth, usedHeight, usedDepth;
            int usedX, usedY, usedZ;
            int alignment;
            GLenum dataType;
            GLenum format;
            GLint internalFormat;
            const void* data;
        };
        static void texImage (const textureDataInfo & info, GLenum type, GLint level);
        struct textureStorageInfo {
            int width, height, depth;
            int levels;
            GLenum internalFormat;
        };
        static void texStorage (const textureStorageInfo & info, GLuint name) {
            glTextureStorage3D (name, info.levels, info.internalFormat, info.width, info.height, info.depth);
        }
};

template <unsigned int DIMENTIONS>
class texture {
    private:
        GLuint name = 0;
        GLenum type;
    public:
        typedef typename textureDimentionPolicy<DIMENTIONS>::textureDataInfo textureDataInfo;
        texture (GLenum _type);
        void setParameter (GLenum parameter, GLint value);
        void bind (int unit);
        static void unbind (int unit);
        void data (const textureDataInfo& info, int level);
	GLuint getName () {return name;}
        ~texture ();
};

template <unsigned int DIMENTIONS>
class fixedSizeTexture {
    private:
        GLuint name = 0;
        GLenum type;
    public:
        typedef typename textureDimentionPolicy<DIMENTIONS>::textureStorageInfo textureStorageInfo;
        fixedSizeTexture (GLenum _type, const textureStorageInfo & storageInfo);
        ~fixedSizeTexture ();
        void clear (GLint level, GLenum format, GLenum dataType, const void *data);
        void setParameter (GLenum parameter, GLint value);
        void bind (int unit);
	GLuint getName () {return name;}
        static void unbind (int unit);
};


typedef texture<3> texture3D;
typedef texture<2> texture2D;
typedef texture<1> texture1D;

typedef fixedSizeTexture<3> fixedSizeTexture3D;
typedef fixedSizeTexture<2> fixedSizeTexture2D;
typedef fixedSizeTexture<1> fixedSizeTexture1D;
