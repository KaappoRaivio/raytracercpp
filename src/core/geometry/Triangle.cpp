//
// Created by kaappo on 11.9.2021.
//

#include "Triangle.h"
#include <glm/ext.hpp>
#include "glm/gtx/string_cast.hpp"


Triangle::Triangle (const glm::vec3& t1, const glm::vec3& t2, const glm::vec3& t3, const Material* material, const glm::vec3& texture1, const glm::vec3& texture2, const glm::vec3& texture3)
        : t1{t1}, t2{t2}, t3{t3}, plane{Plane::from_three_points(t1, t2, t3, *material)}, tu{texture1}, tv{texture2}, tw{texture3}, v0{t2 - t1}, v1{t3 - t1} {

//    v0 = t2 - t1;
//    v1 = t3 - t1;
    d00 = glm::dot(v0, v0);
    d01 = glm::dot(v0 , v1);
    d11 = glm::dot(v1, v1);
    invDenom = 1.0 / (d00 * d11 - d01 * d01);
}

std::ostream& Triangle::print (std::ostream& os) const {
    return os << "Triangle{" << glm::to_string(t1) << ", " << glm::to_string(t2) << ", " << glm::to_string(t3) << "}";
}


Triangle::Triangle (const glm::vec3& t1, const glm::vec3& t2, const glm::vec3& t3, const Material* material) : Triangle{t1, t2, t3, material, {0, 0, 0}, {1, 0, 0}, {0, 1, 0}} {

}

float Triangle::getIntersectionDistance (const Ray& ray, const Surface*& hitSurface, const Material*& hitMaterial) const {
//    std::cout << "Raydestroyed: " << glm::to_string(ra)y << std::endl;
    auto possible_intersection = plane.getIntersectionDistance(ray, hitSurface, hitMaterial);
    if (possible_intersection == 0 or glm::dot(ray.getDirection(), plane.getNormal()) >= 0) {
//        std::cout << "Early return" << std::endl;
        return 0.0;
    }

    auto position = ray.apply(possible_intersection);
    if (includes(position)) {
        hitSurface = this;
        hitMaterial = getMaterial();
        return possible_intersection;
    } else {
        return 0.0;
    }
}

bool Triangle::includes (const glm::vec3& vector) const {
    return plane.includes(vector) && check_bounds(vector);
}

bool Triangle::check_bounds (const glm::vec3& P) const {
    const glm::vec3& v2 = P - t1;
    float d02 = glm::dot(v0, v2);
    float d12 = glm::dot(v1, v2);

    float u = (d11 * d02 - d01 * d12) * invDenom;
    float v = (d00 * d12 - d01 * d02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);


}

glm::vec3 Triangle::getNormalAt (const glm::vec3& position) const {
    return plane.getNormal();
}

glm::vec3 Triangle::getUVAt (const glm::vec3& P) const {
    glm::vec3 v2 = P - t1;

    float d02 = glm::dot(v0, v2);
    float d12 = glm::dot(v1, v2);

    float v = (d11 * d02 - d01 * d12) * invDenom;
    float w = (d00 * d12 - d01 * d02) * invDenom;
    float u = 1.0f - v - w;

    return tu * u + tv * v + tw * w;

}

std::ostream& operator<< (std::ostream& os, const Triangle& triangle) {
    os << "Triangle{" << glm::to_string(triangle.t1) << ", " << glm::to_string(triangle.t2) << ", " << glm::to_string(triangle.t3) << "}";
    return os;
}

AABB Triangle::getBoundingBox () const {
    float smallestX = std::min(t1.x, std::min(t2.x, t3.x));
    float greatestX = std::max(t1.x, std::max(t2.x, t3.x));

    float smallestY = std::min(t1.y, std::min(t2.y, t3.y));
    float greatestY = std::max(t1.y, std::max(t2.y, t3.y));

    float smallestZ = std::min(t1.z, std::min(t2.z, t3.z));
    float greatestZ = std::max(t1.z, std::max(t2.z, t3.z));

    return AABB{{smallestX, smallestY, smallestZ},
                {greatestX, greatestY, greatestZ}};
}

const Material* Triangle::getMaterial () const {
    return plane.getMaterial();
}


