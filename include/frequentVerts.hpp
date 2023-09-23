namespace frequentVerts {
    struct vert2d {
        float coords[2];
    };
    struct vert3d {
        float coords[3];
    };
    constexpr static vert2d squareLocations [] = {
        {-1.0f, -1.0f},
        { 1.0f, -1.0f},
        {-1.0f,  1.0f},
        { 1.0f,  1.0f}
    };
    constexpr static vert2d squareTexturePositions [] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f}
    };
};
