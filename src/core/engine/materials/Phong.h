//
// Created by kaappo on 11.4.2022.
//

#pragma once


#include "Material.h"

class Phong : public Material {
private:
public:
    Phong (const Texture* albedoAmbient, const Texture* albedoDiffuse, const Texture* albedoSpecular, const Texture* albedoFresnel, float specularPower);

private:
    const Texture* albedoAmbient;
    float specularPower;


protected:
    const Texture* albedoDiffuse;
    const Texture* albedoSpecular;
    const Texture* albedoFresnel;
public:
    int scatter (const glm::vec3& position, const glm::vec3& normal, const Intersection& intersection, float currentOpticalDensity, std::array<Interface, Config::MAX_SCATTER>& scatteredRays) const override;

    Intensity shade (const glm::vec3& position, const glm::vec3& normal, const Intersection& intersection, const std::vector<LightSource>& visibleLightSources, float currentOpticalDensity) const override;

    std::ostream& print (std::ostream& s) const override;
};


