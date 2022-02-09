//
// Created by kaappo on 12.9.2021.
//

#pragma once

#include <memory>
#include <ostream>
#include "../common/MyVector3.h"
#include "../light/Intensity.h"
#include "Texture.h"
#include "SolidTexture.h"

struct Material {
    const Texture* albedo;
    const Texture* bump;
    double glossiness;

    constexpr Material (const Texture* const albedo, double glossiness, const Texture* bump) : albedo{albedo}, glossiness{glossiness}, bump{bump} {}
    constexpr Material (const Texture* const albedo, double glossiness) : albedo{albedo}, glossiness{glossiness}, bump{&SolidTextures::BUMP_UP} {}
    explicit Material (const Texture* const albedo) : albedo{albedo}, glossiness{0}, bump{&SolidTextures::BUMP_UP} {}

    Material(const Material& other) = delete;
    void operator=(const Material& other) = delete;

public:
    Intensity getAlbedoAt (const MyVector3& uv) const;
    MyVector3 getBumpAt (const MyVector3& uv, const MyVector3& normal) const;

    friend std::ostream& operator<< (std::ostream& os, const Material& material);
};


namespace Materials {
    constexpr Material WHITE_GLOSSY{&SolidTextures::WHITE, 0.8};
    constexpr Material BLUE_GLOSSY{&SolidTextures::BLUE, 0.8};
    constexpr Material RED_GLOSSY{&SolidTextures::RED, 0.8};
//    constexpr Material BLUE_GLOSSY{&SolidTextures::BLUE, 0.8};
    constexpr Material YELLOW_GLOSSY{&SolidTextures::YELLOW, 0.8};
    constexpr Material GREEN_GLOSSY{&SolidTextures::GREEN, 0.8};
    constexpr Material WHITE{&SolidTextures::WHITE, 0};
}