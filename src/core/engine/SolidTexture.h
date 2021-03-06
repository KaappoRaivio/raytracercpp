//
// Created by kaappo on 21.9.2021.
//

#pragma once


#include "Texture.h"

class SolidTexture : public Texture {
private:
    Intensity intensity;
public:
    constexpr explicit SolidTexture (const Intensity& intensity) : intensity{intensity} {};

public:
    Intensity getPixelAt (const glm::vec3& uv) const override;
};

namespace SolidTextures {
    constexpr SolidTexture BUMP_UP {{0.5, 0.5, 1}};
    constexpr SolidTexture WHITE {{1, 1, 1}};
    constexpr SolidTexture BLUE {{0.1, 0.1, 1}};
    constexpr SolidTexture RED {{1, 0.1, 0.1}};
    constexpr SolidTexture YELLOW {{1, 1, 0.5}};
    constexpr SolidTexture GREEN {{1, 0.5, 0.5}};
}