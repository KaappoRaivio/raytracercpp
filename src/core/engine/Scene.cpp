//
// Created by kaappo on 11.9.2021.
//

#include "Scene.h"
#include "../light/Intensity.h"
#include "../common/LightSource.h"
#include "../light/IntensityBlend.h"
#include "../common/mytypes.h"
#include "ImageTexture.h"
#include "../geometry/Sphere.h"
//#include "../interface/MyOBJLoader.h"
#include "TextureManager.h"
#include "../common/MyVector3.h"
#include <glm/gtx/norm.hpp>

#include <utility>
#include <cmath>
#include <iostream>

//#include <numbers>
constexpr float PI = 3.1415926;

Scene::Scene (std::vector<std::unique_ptr<Surface>> objects, const std::vector<LightSource>& lightSources, const Camera& camera, int maxBounces, int raysPerPixel, int antialiasingScaler, TextureManager textureManager)
        : objects{std::move(objects)}, lightSources(std::move(lightSources)), camera(camera), raysPerPixel{raysPerPixel}, maxBounces(maxBounces), antialiasingScaler{antialiasingScaler}, textureManager{std::move(textureManager)} {}

#pragma clang diagnostic push
//#pragma omp declare target

std::vector<std::vector<Intensity>> Scene::trace () const {
    auto viewplane = camera.getViewplane(antialiasingScaler);

    unsigned viewport_height = viewplane.size() / antialiasingScaler;
    unsigned viewport_width = viewplane[0].size() / antialiasingScaler;

    std::vector<std::vector<Intensity>> pixels;
    pixels.reserve(viewport_height);
    for (size_t y = 0 ; y < viewport_height ; ++y) {
        std::vector<Intensity> row;
        row.reserve(viewport_width);
        for (size_t x = 0 ; x < viewport_width ; ++x) {
            row.emplace_back(0, 0, 0);
        }
        pixels.push_back(row);
    }


#pragma omp parallel for collapse(2)
//#pragma omp target map(to:viewplane), map(tofrom:pixels)
//teams distribute parallel for  collapse(2)
    for (size_t y = 0 ; y < viewport_height ; ++y) {
        for (size_t x = 0 ; x < viewport_width ; ++x) {

            IntensityBlend pixelValue;

            for (int dy = 0 ; dy < antialiasingScaler ; ++dy) {
                for (int dx = 0 ; dx < antialiasingScaler ; ++dx) {
                    const auto& pixel = viewplane[y * antialiasingScaler + dy][x * antialiasingScaler + dx];
                    const Ray ray = {camera.getOrigin(), pixel};

//                    if (x == 2 and y == 2)
//                        std::cout << "Debug!" << std::endl;

                    std::stack<float> densities;
                    densities.push(1);
                    pixelValue += calculateColor(ray, x + dx, y + dy, maxBounces, densities);

//                    if (DEBUG)
//                        std::cout << densities.size() << std::endl;
//                    densities.

                }
            }

            pixels[y][x] = pixelValue.commitBlend();
        }
    }

    return pixels;
}

#pragma clang diagnostic pop
Intensity Scene::calculateColor (const Ray& ray, int x, int y, int bounces_left, std::stack<float>& opticalDensities) const {
    Intersection intersection;
    bool intersects = getClosestIntersection(ray, 0, intersection);
//    std::cout << intersection.value() << std::endl;
    if constexpr(DEBUG) {
        if (y % 100 == 0 && x == 0) {
            std::cout << "Row " << y << std::endl;
        }
    }


    if (not intersects) {
        return Intensity{0, 0, 0};
    } else {

    if constexpr(DEBUG) {
        std::cout << "hit!" << std::endl;
    }

        //        std::cout << closest.position << std::endl;
        const Material* material = intersection.material;
        const auto* const surface = intersection.hitSurface;

        const Intensity& albedo = material->getAlbedoAt(surface->getUVAt(intersection.position));

//        Intensity diffuse_light = {0, 0, 0};
        IntensityBlend diffuse_light;
        IntensityBlend specular_light;
        const glm::vec3& face_normal = surface->getBumpedNormalAt(intersection.position);
        const glm::vec3& N = face_normal;

//        if (N * ray.getDirection() >= 0) {
//            return calculateColor({closest.position, ray.getDirection()}, x, y, bounces_left, opticalDensities);
//        }


        const glm::vec3& d = intersection.ray.getDirection();
        const glm::vec3& R = glm::normalize(glm::reflect(d, face_normal));

        for (int i = 0 ; i < raysPerPixel ; ++i) {
            for (const auto& lightSource : lightSources) {
                const glm::vec3& vector_to_light = VectorOperations::rotateInsideCone(lightSource.position - intersection.position, lightSource.radius);
                const glm::vec3& V = glm::normalize(vector_to_light);

                Intersection _;
                bool anyHits = getClosestIntersection({intersection.position, V}, vector_to_light.length(), _);

                if (not anyHits) {
                    float distance_coefficient = 1.0 / glm::length2(vector_to_light);

                    float diffuse_direction_coefficient = lambertianDiffuseReflection(N, V, d);
//                    float diffuse_direction_coefficient = orenNayarDiffuseReflection(N, V, R, 1 - material.glossiness);
//                    float diffuse_direction_coefficient = 0;

                    float specular_direction_coefficient = calculate_beckmann_distribution(R, V, material->glossiness);
                    diffuse_light += lightSource.intensity / raysPerPixel * distance_coefficient * diffuse_direction_coefficient;
                    specular_light += lightSource.intensity / raysPerPixel * distance_coefficient * specular_direction_coefficient;
                }
            }
        }

        Intensity underlying{0, 0, 0};
        if (bounces_left > 0) {
//            std::cout << opticalDensities.top() / material->opticalDensity << std::endl;
//            float reflectance = 0.5;
            float reflectance = getReflectance(glm::abs(glm::dot(N, d)), opticalDensities.top() / material->opticalDensity);
//            std::cout << glm::abs(glm::dot(N, d)) << ", " << reflectance << std::endl;
//            std::cout << glm::abs(glm::dot(N, d)) << std::endl;
            float refractance = 1 - reflectance;

//            std::cout << reflectance << std::endl;

            if (material->glossiness > 0 and reflectance > 0) {
//                specular_light += calculateColor({intersection.position, R}, x, y, bounces_left - 1, opticalDensities) * reflectance * 2;
            }
            if (material->alpha < 1 and refractance > 0) {
                glm::vec3 refracted = glm::normalize(surface->refract(intersection.position, d, opticalDensities));
//                std::cout << refracted << std::endl;
//                std::cout << calculateColor({intersection.position + refracted * 0.01f, refracted}, x, y, bounces_left - 1, opticalDensities) * refractance * (1 - material->alpha) << std::endl;
                underlying = calculateColor({intersection.position + refracted * 0.01f, refracted}, x, y, bounces_left - 1, opticalDensities) * refractance * albedo * 2;
                //            std::cout << underlying << std::endl;
            }
        }


        const Intensity& specular_intensity = specular_light.commitSum();
        const Intensity& diffuse_intensity = diffuse_light.commitSum();

        return albedo * material->alpha * (specular_intensity * material->glossiness + diffuse_intensity * (1 - material->glossiness))
               + underlying * (1 - material->alpha);
    }

}

float Scene::orenNayarDiffuseReflection (const glm::vec3& face_normal, const glm::vec3& vector_to_light, const glm::vec3& vector_from_camera, float roughness) {
    const auto& n = glm::normalize(face_normal);
    const auto& v = glm::normalize(vector_to_light);
    const auto& d = glm::normalize(vector_from_camera);
    float sigma = roughness;
    float variance = std::pow(sigma, 2);

    float s = glm::dot(n, v) * glm::dot(d, n) - glm::dot(v, d);
    float t = s > 0 ? std::max(glm::dot(n, v), -glm::dot(n, d)) : 1;

    float A = 1 / M_PI * (1 - 0.5 * variance / (variance + 0.17) * variance / (variance + 0.13));
    float B = 1 / M_PI * (0.45 * variance / (variance + 0.09));

    float d1 = glm::dot(d, n) * (A + B * s / t);
//    std::cout << d1 << std::endl;
    return std::clamp(d1, 0.0f, 1.0f);
}


float Scene::lambertianDiffuseReflection (const glm::vec3& N, const glm::vec3& L, const glm::vec3& d) {
    float dot1 = -glm::dot(d, N);
    float dot2 = glm::dot(L, N);

    if (dot2 > 0 and dot1 > 0) {
        return dot2;
    } else {
        return 0;
    }






//    return std::max(0.0, N * L);

    if ((dot2 < 0) == (dot1 < 0)) {
        return std::abs(glm::dot(N, L));
    } else {
        return 0;
    }

}

bool Scene::getClosestIntersection (const Ray& ray, float max_distance, Intersection& out) const {
    std::vector<Intersection> intersections;

    for (const auto& object : objects) {
//        std::cout << *object << std::endl;
        Intersection temp;
        bool intersects = object->getIntersection(ray, temp);
        if (intersects && (max_distance == 0 || temp.distance < max_distance)) {
            intersections.push_back(temp);
        }
    }

    if (intersections.empty()) { return false; }
    else {
        Intersection intersection = *std::min_element(intersections.begin(), intersections.end(), [] (const Intersection& a, const Intersection& b) {
            return a.distance < b.distance;
        });

        out = intersection;
        return true;
    }
//
//
//    float closest_distance = 1e308;
//    Intersection* closest = nullptr;
//
//    for (const auto& object: objects) {
//        std::optional<Intersection> possibleIntersection = object.getIntersection(ray);
//        if (possibleIntersection && possibleIntersection->distance < closest_distance && (max_distance == 0 || possibleIntersection->distance < max_distance)) {
////            std::cout << (*possibleIntersection).sceneObject.getSurface() << std::endl;
//            closest_distance = possibleIntersection->distance;
//            closest = &(*possibleIntersection);
//        }
//    }
//
//    if (closest == nullptr) {
//        return std::nullopt;
//    } else {
//        return {*closest};
//    }
}

float Scene::calculate_beckmann_distribution (const glm::vec3& R, const glm::vec3& V, float glossiness) {
    float roughness = 1 - glossiness;
    if (roughness == 0) {
        return 0;
    }

    float cosine = glm::dot(R, V);


    return std::exp(-(1 - std::pow(cosine, 2)) / (std::pow(cosine * roughness, 2))) / (PI * std::pow(roughness, 2) * std::pow(cosine, 4));
//    / (std::numbers::pi_v<float> * std::pow(roughness, 2) * std::pow(cosine, 4));


}

float Scene::getReflectance (float cosine, float refractionRatio) {
    // Use Schlick's approximation for reflectance.
    float r0 = glm::pow((1 - refractionRatio) / (1 + refractionRatio), 2);
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5);
}

std::ostream& operator<< (std::ostream& os, const Scene& scene) {
    os << "objects: {";
    for (const auto& i : scene.objects) {
        os << *i << ", ";
    }
    os << "}";

    os << "lightSources: {";
    for (const auto& i : scene.lightSources) {
        os << i << ", ";
    }
    os << "}";

    os << " camera: " << scene.camera << " raysPerPixel: " << scene.raysPerPixel << " antialiasingScaler: " << scene.antialiasingScaler;
    return os;
}

//void Scene::executeCameraMove (SplineSequence sequence, float deltaT, auto onFrameRendered) {
//    for (float t = 0; t < 1; t += deltaT) {
//        camera.origin = sequence.apply(t).getOrigin();
//        camera.direction = sequence.apply(t).getDirection();
//        auto&& pixels = trace();
//        onFrameRendered(std::move(pixels));
//    }
//}

//#pragma omp end declare target
