//
// Created by kaappo on 15.2.2022.
//

#include <chrono>
#include <random>
#include "Scenes.h"
#include "Scene.h"
#include "../interface/MyOBJLoader.h"
#include "../geometry/Sphere.h"
#include "../common/mytypes.h"
#include "materials/ImageTexture.h"
#include "materials/Metal.h"
#include "materials/Dielectric.h"
#include "materials/PhongLegacy.h"
#include "materials/DebugMaterial.h"

Scene Scenes::getDebug (int windowX, int windowY) {
    Camera camera = {{-2, 1, 1}, {-0.24, 4.53, 0.27}, 2.f, {1.f, (float) windowY / windowX}, {windowX, windowY}};

    Manager<Texture> textureManager;
    Manager<Material> materialManager;


    auto schlickTest = materialManager.get<Dielectric>(1.f, 0.f, 2.f, textureManager.get<SolidTexture>(Intensity{1, 1, 1}), &SolidTextures::BUMP_UP, Intensity{0, 0, 0});
    auto target = materialManager.get<Dielectric>(1.f, 1.f, 1.0f, textureManager.get<ImageTexture>("../res/texture3.png"), &SolidTextures::BUMP_UP, Intensity{0, 0, 0});
    std::cout << *schlickTest << std::endl;
//    std::exit(0);

    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, 0, 1}, 0, materialManager.get<Dielectric>(1.f, 1.0f, 1.0f, textureManager.get<SolidTexture>(Intensity{1, 1, 1}), &SolidTextures::BUMP_UP, Intensity{0, 0, 0}));
    std::unique_ptr<Surface> sphere = std::make_unique<Sphere>(glm::vec3{2, 6, 1}, 1, schlickTest);
//    std::vector<std::unique_ptr<Surface>> cube = MyOBJLoader::readOBJ("../res/teapot2.obj", {2, 6, 1}, 0.125, {M_PI / 4, -M_PI / 2}, target);
    std::vector<std::unique_ptr<Surface>> cube = MyOBJLoader::readOBJ("../res/bmwdebug.obj", {0, 4, 0}, 0.4, {-M_PI / 4, -M_PI / 2}, target, textureManager, materialManager);
//    std::exit(0);

    std::vector<LightSource> lightSources = {
            {{-5,   4,    4}, Intensity{1, 1, 1} * 20,  0.0},
            {{1.42, 3.91, 1}, Intensity{1, 1, 0.9} * 2, 0.0}
    };

    std::vector<std::unique_ptr<Surface>> surfaces{};
    surfaces.push_back(std::move(sphere));
//    surfaces.push_back(std::move(sphere));
    for (auto& paska : cube) {
        surfaces.push_back(std::move(paska));
    }


//    auto bvh = std::make_unique<BVH>(std::move(surfaces));
    auto bvh = std::make_unique<BVH>(std::move(surfaces));

    std::vector<std::unique_ptr<Surface>> objects;
    objects.push_back(std::move(plane));
    objects.push_back(std::move(bvh));


    return Scene{std::move(objects), std::move(lightSources), camera, 3, 1, 1, std::move(textureManager), std::move(materialManager)};
}

//Scene Scenes::getSceneOne (int windowX, int windowY) {
//    Camera camera = {{0, -5, 7}, {1, 4, 2}, 0.5, {1, (float) windowY / windowX}, {windowX, windowY}};
//
//
//    Manager textureManager;
//
//    auto triangleTexture = textureManager.getImageTexture("../res/texture3.png");
//    auto earthTexture = textureManager.getImageTexture("../res/earth.png");
//    auto earthBump = textureManager.getImageTexture("../res/earth_bump.jpg");
//
//    auto planeTexture = textureManager.getSolidTexture(Intensity{1, 1, 1});
//
////
//    Material triangleMaterial{planeTexture, 0.5, 1};
//    Material planeMaterial{planeTexture};
//    Material mirror{&SolidTextures::WHITE, 1, 1};
//    Material earthSurface{earthTexture, 0.2, earthBump};
//    Material testSurface{triangleTexture, 0.2, &SolidTextures::BUMP_UP};
//
//    auto triangle = std::make_unique<Triangle>(
//            glm::vec3{-5, 6, 3},
//            glm::vec3{0, 0, 3},
//            glm::vec3{5, 4, 4},
//            triangleMaterial
//    );
//
//    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, 0, 1}, 0, planeMaterial);
//    std::unique_ptr<Surface> mirrorSphere = std::make_unique<Sphere>(glm::vec3{1, 6, 5}, 2, mirror);
//    std::unique_ptr<Surface> earth = std::make_unique<Sphere>(glm::vec3{1.5, 1, 1}, 1.5, earthSurface);
//    std::unique_ptr<Surface> test = std::make_unique<Sphere>(glm::vec3{-2, 0.5, 1}, 1.5, testSurface);
//
////    std::vector<std::unique_ptr<Surface>> polygons;
//
//    std::vector<std::unique_ptr<Surface>> polygons = MyOBJLoader::readOBJ("../res/teapot2.obj", {4, 4, 2}, 0.25, {M_PI / 4, -M_PI / 2}, &Materials::BLUE_GLOSSY);
//    std::vector<std::unique_ptr<Surface>> polygons2 = MyOBJLoader::readOBJ("../res/uvmaptest.obj", {-2, 4, 2}, 0.25, {M_PI / 4, -M_PI / 2}, &Materials::RED_GLOSSY);
////
//    for (auto& pointer : polygons2) {
//        polygons.push_back(std::move(pointer));
//    }
//
//    polygons.push_back(std::move(mirrorSphere));
//    polygons.push_back(std::move(earth));
//    polygons.push_back(std::move(test));
//
//    std::unique_ptr<Surface> bvh = std::make_unique<BVH>(std::move(polygons));
//
//    std::vector<std::unique_ptr<Surface>> objects;
//
//    objects.push_back(std::move(plane));
//    objects.push_back(std::move(bvh));
//
//    double radius = 0;
//    std::vector<LightSource> lights = {
//            {{-2, 1,   3},  Intensity{1, 1, 1} * 21,  radius},
//            {{10, -40, 40}, Intensity{1, 1, 1} * 300, radius * 50},
//    };
//
//    return {std::move(objects), lights, camera, 5, 1, 4, std::move(textureManager)};
//}
//
//Scene Scenes::getSceneTwo (int windowX, int windowY) {
//    Camera camera = {{0, -5, 7}, {1, 4, 2}, 1, {1, (float) windowY / windowX}, {windowX, windowY}};
//
//    Manager textureManager;
//
//    auto triangleTexture = textureManager.getImageTexture("../res/texture3.png");
//    auto earthTexture = textureManager.getImageTexture("../res/earth.png");
//    auto earthBump = textureManager.getImageTexture("../res/test.png");
//
//    auto planeTexture = textureManager.getSolidTexture(Intensity{1, 1, 1});
//
//    Material planeMaterial{planeTexture};
//    Material mirror{&SolidTextures::WHITE, 1, 1};
//    Material earthSurface{earthTexture, 0.4, earthBump};
//    Material testSurface{triangleTexture, 0.2, &SolidTextures::BUMP_UP};
//
//    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, 0, 1}, 0, planeMaterial);
//    std::unique_ptr<Surface> mirrorSphere = std::make_unique<Sphere>(glm::vec3{1, 6, 5}, 2, mirror);
//    std::unique_ptr<Surface> earth = std::make_unique<Sphere>(glm::vec3{1.5, 1, 1}, 1.5, earthSurface);
//    std::unique_ptr<Surface> test = std::make_unique<Sphere>(glm::vec3{-2, 0.5, 1}, 1.5, testSurface);
//
//    std::vector<std::unique_ptr<Surface>> polygons = MyOBJLoader::readOBJ("../res/teapot2.obj", {4, 4, 2}, 0.25, {M_PI / 4, -M_PI / 2}, &Materials::BLUE_GLOSSY);
//    std::vector<std::unique_ptr<Surface>> polygons2 = MyOBJLoader::readOBJ("../res/uvmaptest.obj", {-2, 4, 2}, 0.25, {M_PI / 4, -M_PI / 2}, &Materials::RED_GLOSSY);
////
////    for (auto& pointer : polygons2) {
////        polygons.push_back(std::move(pointer));
////    }
//
//    polygons.push_back(std::move(mirrorSphere));
//    polygons.push_back(std::move(earth));
//    polygons.push_back(std::move(test));
//
//    std::unique_ptr<Surface> bvh = std::make_unique<BVH>(std::move(polygons));
//
//    std::vector<std::unique_ptr<Surface>> objects;
//
//    objects.push_back(std::move(plane));
//    objects.push_back(std::move(bvh));
//
//    double radius = 0;
//    std::vector<LightSource> lights = {
//            {{-2, 1,   3},  Intensity{1, 1, 1} * 21,  radius},
//            {{10, -40, 40}, Intensity{1, 1, 1} * 300, radius * 50},
//    };
//
//    return {std::move(objects), lights, camera, 5, 1, 2, std::move(textureManager)};
//}
//
//Scene Scenes::getSceneThree (int windowX, int windowY) {
//    Camera camera{{-10, -10, 3}, {0, 5, 3}, 1, {1, (float) windowY / windowX,}, {windowX, windowY}};
//
//    Manager textureManager;
//
//    auto planeTexture = textureManager.getImageTexture("../res/texture3.png");
//
//    Material planeMaterial{planeTexture};
//    Material transparent{&SolidTextures::WHITE, 0, 0.2f};
//    transparent.opticalDensity = 1.5;
//
//    Material transparent2{&SolidTextures::WHITE, 0, 0.2f};
//    transparent2.opticalDensity = 1000;
//
//    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, -1, 0}, 10, planeMaterial);
//    std::unique_ptr<Surface> plane2 = std::make_unique<Plane>(glm::vec3{0, 0, 1}, 0, Materials::WHITE);
//    std::unique_ptr<Surface> transparentSphere = std::make_unique<Sphere>(glm::vec3{0, 3, 3}, 2, transparent);
//    std::unique_ptr<Surface> transparentSphere2 = std::make_unique<Sphere>(glm::vec3{0, 6, 3}, 1, transparent2);
//    std::unique_ptr<Surface> transparentTriangle = std::make_unique<Triangle>(glm::vec3{-4, 1, 7}, glm::vec3{-4, 1, 3}, glm::vec3{4, 1, 7}, transparent);
//    std::unique_ptr<Surface> transparentTriangle2 = std::make_unique<Triangle>(glm::vec3{-4, 1, 3}, glm::vec3{4, 1, 3}, glm::vec3{4, 1, 7}, transparent);
//
//    std::cout << glm::to_string(plane->getUVAt({0, 1, 0})) << std::endl;
////    std::cout << transparentSphere->refract()
////    std::exit(0);
//
//    std::stack<double> a;
//    a.push(1);
////    auto ray = Ray{{0,    -5,   7},
////                   {0.28, 0.85, -0.43}};
////    const std::optional<Intersection>& intersection = transparentSphere->getIntersection(ray, <#initializer#>);
//
////    std::cout << intersection->distance << std::endl;
////    std::cout << intersection->position << std::endl;
////    std::cout << transparentSphere->refract(intersection.value().position, ray.getDirection(), a) << std::endl;
//
//
////    std::exit(0);
//
//
//    std::vector<std::unique_ptr<Surface>> polygons;
//    polygons.push_back(std::move(transparentSphere));
//    polygons.push_back(std::move(transparentSphere2));
//    auto teapot = MyOBJLoader::readOBJ("../res/uvmaptest.obj", glm::vec3{1.f, 9.f, 3.f}, 0.15, {0, -M_PI / 2}, &Materials::WHITE);
//    for (auto& face : teapot) {
//        polygons.push_back(std::move(face));
//    }
////    polygons.push_back();
////    polygons.push_back(std::move(transparentTriangle));
////    polygons.push_back(std::move(transparentTriangle2));
//    std::unique_ptr<Surface> bvh = std::make_unique<BVH>(std::move(polygons));
//
//    std::vector<std::unique_ptr<Surface>> objects;
//
//    objects.push_back(std::move(plane));
//    objects.push_back(std::move(plane2));
//    objects.push_back(std::move(bvh));
//
//    double radius = 0;
//    std::vector<LightSource> lights = {
//            {{-2, 20,  3},   Intensity{1, 1, 1} * 210,  radius},
//            {{10, -40, 100}, Intensity{1, 1, 1} * 3000, radius * 50},
//    };
//
//
//    return {std::move(objects), lights, camera, 2, 1, 4, std::move(textureManager)};
//}
//
//Scene Scenes::getSceneFour (int windowX, int windowY) {
//
//    Camera camera = {{0, -5, 7}, {0.341747, -5 + 0.808307, 7 - 0.479426}, 1, {1, (float) windowY / windowX}, {windowX, windowY}};
//
//    Manager textureManager;
//
//    auto planeTexture = textureManager.getSolidTexture(Intensity{1, 1, 1});
//
//    Material planeMaterial{planeTexture, 0.0, 1.0};
//
//    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, 0, 1}, 0, planeMaterial);
//
//    std::vector<std::unique_ptr<Surface>> polygons = MyOBJLoader::readOBJ("../res/uvmaptest.obj", {4, 4, 2}, 0.25, {M_PI / 4, -M_PI / 2}, &Materials::BLUE);
//    std::unique_ptr<Surface> bvh = std::make_unique<BVH>(std::move(polygons));
//
//    std::vector<std::unique_ptr<Surface>> objects;
//    objects.push_back(std::move(plane));
//    objects.push_back(std::move(bvh));
//
//    double radius = 0;
//    std::vector<LightSource> lights = {
//            {{-2,  1,   3},   Intensity{1, 1, 1} * 21,   radius},
//            {{-10, -40, 100}, Intensity{1, 1, 1} * 3000, radius * 25},
//    };
//
//    return {std::move(objects), lights, camera, 3, 1, 1, std::move(textureManager)};
//}
//
//Scene Scenes::getRaytracinginaweekendtestscene (int windowX, int windowY) {
//    Manager manager;
//
//    auto ground = Material{manager.getSolidTexture({1, 0.8, 0.0}), 0, 1.0};
//    auto center = Material{manager.getSolidTexture({0.7, 0.3, 0.3}), 0.8, 1.0};
//    auto left = Material{manager.getSolidTexture({0.8, 0.8, 0.8}), 1, 0.5};
//    left.opticalDensity = 1.5;
//    auto right = Material{manager.getSolidTexture({0.8, 0.6, 0.2}), 0.8, 0.5};
//
//    std::vector<std::unique_ptr<Surface>> o;
////    o.push_back(std::make_unique<Sphere>(glm::vec3{0.0, 0.0, -1.0}, 0.5, center));
//    o.push_back(std::make_unique<Sphere>(glm::vec3{-1.0, 5.0, -1.0}, 0.5, left));
////    o.push_back(std::make_unique<Sphere>(glm::vec3{1.0, 0.0, -1.0}, 0.5, right));
//
//    std::vector<std::unique_ptr<Surface>> objects{};
//    objects.push_back(std::make_unique<BVH>(std::move(o)));
//    objects.push_back(std::make_unique<Plane>(glm::vec3{0, 0, 1}, 1.6, ground));
//
////    std::cout << *objects[1] << std::endl;
//
//    std::vector<LightSource> lights = {
//            {{100, -40, 40}, Intensity{1, 1, 1} * 10000, 0},
//    };
//
//    Camera camera{{0, 0, -1.4}, {-1, 5, -1}, 3, {1, (float) windowY / windowX}, {windowX, windowY}};
//
//    return {std::move(objects), lights, camera, 3, 1, 1, std::move(manager)};
//
//}
//
Scene Scenes::getBezierScene (int windowX, int windowY, const SplineSequence& sequence) {
    Manager<Texture> textureManager;
    Manager<Material> materialManager;


    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::uniform_real_distribution<float> d(10, 20);
    std::uniform_real_distribution<float> radiusDistribution(1, 2);
    std::uniform_real_distribution<float> colorDistribution(0.5, 1);
    std::uniform_real_distribution<float> glossinessDistribution(0.5, 0.8);
    std::uniform_real_distribution<float> alphaDistribution(0, 0);
    std::uniform_real_distribution<float> sideDistribution(-1, 1);


    std::vector<std::unique_ptr<Surface>> spheres;

    for (float t = 0 ; t < 1 ; t += 0.025f) {
        const Ray& ray = sequence.apply(t, true);
        auto forward = ray.getDirection();
        auto side = glm::normalize(glm::cross(forward, {0.f, 0.f, 1.f}));

        auto& gen = MyRandom::generator;
        float alpha = alphaDistribution(gen);

        Intensity color{
                colorDistribution(gen),
                colorDistribution(gen),
                colorDistribution(gen)
        };
//        Material material{textureManager.get<SolidTexture>(color), 0.01, 0.1f};

        auto material = materialManager.get<Dielectric>(0.f, alpha, 1.2f, textureManager.get<SolidTexture>(color), &SolidTextures::BUMP_UP, Intensity{0, 0, 0});

//        material.opticalDensity = 1.2;



        spheres.push_back(std::make_unique<Sphere>(ray.getOrigin() + 3.f * (sideDistribution(gen) > 0 ? 1 : -1) * side, radiusDistribution(gen), material));
    }

    std::unique_ptr<Surface> bvh = std::make_unique<BVH>(std::move(spheres));

    std::vector<std::unique_ptr<Surface>> objects;
    objects.push_back(std::move(bvh));

    std::vector<LightSource> lights = {
            {{100, -40, 40}, Intensity{1, 1, 1} * 5000, 0},
    };

    auto planeTexture = textureManager.get<ImageTexture>("../res/texture3.png");
    auto planeMaterial = materialManager.get<Dielectric>(1.0f, 1.0f, 1.0f, planeTexture, &SolidTextures::BUMP_UP, Intensity{0, 0, 0});
    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, 0, 1}, 0, planeMaterial);
    objects.push_back(std::move(plane));

    Camera camera{sequence.apply(0, false).getOrigin(), sequence.apply(0, false).getDirection(), 1, {1.f, (float) windowY / windowX}, {windowX, windowY}};
    return {std::move(objects), lights, camera, 2, 1, 1, std::move(textureManager), std::move(materialManager)};
}

Scene Scenes::getBMWScene (int windowX, int windowY) {
    Camera camera = {{0, 0, 0.5}, {0, 4, 0.5}, 1.f, {1.f, (float) windowY / windowX}, {windowX, windowY}};

    Manager<Texture> textureManager;
    Manager<Material> materialManager;


    auto schlickTest = materialManager.get<Dielectric>(0.f, 0.f, 2.f, textureManager.get<SolidTexture>(Intensity{1, 1, 1}), &SolidTextures::BUMP_UP, Intensity{0, 0, 0});
    auto target = materialManager.get<Dielectric>(1.f, 1.f, 1.0f, &ImageTextures::DEBUG_TEXTURE, &SolidTextures::BUMP_UP, Intensity{1, 1, 1} * 2000);
    std::cout << *schlickTest << std::endl;
//    std::exit(0);

    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, 0, 1}, 0, materialManager.get<Dielectric>(1.f, 1.0f, 1.0f, textureManager.get<SolidTexture>(Intensity{1, 1, 1}), &SolidTextures::BUMP_UP, Intensity{0, 0, 0}));
    std::unique_ptr<Surface> backdrop = std::make_unique<Plane>(glm::vec3{0, -1, 0}, 10, materialManager.get<Dielectric>(1.f, 1.0f, 1.0f, &ImageTextures::DEBUG_TEXTURE, &SolidTextures::BUMP_UP, Intensity{1, 1, 1} * 0.5));
    std::unique_ptr<Surface> sphere = std::make_unique<Sphere>(glm::vec3{2, 6, 1}, 1, schlickTest);
//    std::vector<std::unique_ptr<Surface>> cube = MyOBJLoader::readOBJ("../res/teapot2.obj", {2, 6, 1}, 0.125, {M_PI / 4, -M_PI / 2}, target);
    std::vector<std::unique_ptr<Surface>> cube = MyOBJLoader::readOBJ("../res/bmw27_cpu.obj", {0, 4, 0}, 0.5, {-M_PI / 4 - M_PI / 8, -M_PI / 2}, target, textureManager, materialManager);
//    std::exit(0);



    auto v1 = glm::vec3(-0.1784, 3.1024, 0.5804);
    auto v2 = glm::vec3(-0.1770, 3.1571, 0.5938);
    auto v3 = glm::vec3(-0.1952, 3.1079, 0.5813);

    auto normal = glm::normalize(glm::cross(v3 - v1, v2 - v1));

    Ray newRay{v1, glm::reflect({-0.0561, 0.9981, 0.02703}, normal)};


    auto lightSourcePosition = newRay.apply(10);

    std::vector<LightSource> lightSources = {
//            {{-5, 4, 4}, Intensity{1, 1, 1} * 20, 0},
            {lightSourcePosition, Intensity(1, 1, 1) * 200,   0},
            {{1.42, 6, 10},       Intensity{1, 1, 0.9} * 200, 0},
            {{-2,   0, 5},        Intensity{1, 1, 0.9} * 200, 0}
    };

    std::vector<std::unique_ptr<Surface>> surfaces{};
    surfaces.push_back(std::move(sphere));
//    surfaces.push_back(std::move(sphere));
    for (auto& paska : cube) {
        surfaces.push_back(std::move(paska));
    }


    auto bvh = std::make_unique<BVH>(std::move(surfaces));

    std::vector<std::unique_ptr<Surface>> objects;
    objects.push_back(std::move(plane));
    objects.push_back(std::move(bvh));
    objects.push_back(std::move(backdrop));


    return Scene{std::move(objects), std::move(lightSources), camera, 2, 1, 2, std::move(textureManager), std::move(materialManager)};
}

Scene Scenes::getClassroomScene (int windowX, int windowY) {
//    Camera camera = {{1, 1, 1}, {1.5, 4, 3}, 0.2f, {1.f, (float) windowY / windowX}, {windowX, windowY}};
    Camera camera = {{1, 1, 1}, {0.5, 4, 1}, 0.5f, {1.f, (float) windowY / windowX}, {windowX, windowY}};

    Manager<Texture> textureManager;
    Manager<Material> materialManager;


    auto schlickTest = materialManager.get<Dielectric>(0.f, 1.f, 1.f, textureManager.get<SolidTexture>(Intensity{1, 1, 1}), &SolidTextures::BUMP_UP, Intensity{0, 0, 0});
    std::cout << dynamic_cast<const Dielectric*>(schlickTest)->absorbance;
//    std::exit(0);
    auto target = materialManager.get<Dielectric>(1.f, 1.f, 1.0f, &ImageTextures::DEBUG_TEXTURE, &SolidTextures::BUMP_UP, Intensity{1, 1, 1});
    std::cout << *schlickTest << std::endl;
//    std::exit(0);

    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, 0, 1}, 0, materialManager.get<Dielectric>(1.f, 1.0f, 1.0f, textureManager.get<SolidTexture>(Intensity{1, 1, 1}), &SolidTextures::BUMP_UP, Intensity{0, 0, 0}));
    std::unique_ptr<Surface> backdrop = std::make_unique<Plane>(glm::vec3{0, -1, 0}, 10, materialManager.get<Dielectric>(1.f, 1.0f, 1.0f, &ImageTextures::DEBUG_TEXTURE, &SolidTextures::BUMP_UP, Intensity{1, 1, 1} * 0.5));
    std::unique_ptr<Surface> sphere = std::make_unique<Sphere>(glm::vec3{2, 6, 1}, 1, schlickTest);
//    std::vector<std::unique_ptr<Surface>> cube = MyOBJLoader::readOBJ("../res/teapot2.obj", {2, 6, 1}, 0.125, {M_PI / 4, -M_PI / 2}, target);
    std::vector<std::unique_ptr<Surface>> cube = MyOBJLoader::readOBJ("../res/classroom/classroom/classroom.obj", {0, 4, 0}, 1, {M_PI / 4 - M_PI / 8, -M_PI / 2}, target, textureManager, materialManager);
//    std::exit(0);



    auto v1 = glm::vec3(-0.1784, 3.1024, 0.5804);
    auto v2 = glm::vec3(-0.1770, 3.1571, 0.5938);
    auto v3 = glm::vec3(-0.1952, 3.1079, 0.5813);

    auto normal = glm::normalize(glm::cross(v3 - v1, v2 - v1));

    Ray newRay{v1, glm::reflect({-0.0561, 0.9981, 0.02703}, normal)};


    auto lightSourcePosition = newRay.apply(10);


    float radius = 0.f;
    std::vector<LightSource> lightSources = {
//            {{2.394355, 4.556603, 2.205824-0.1}, Intensities::INCANDESCENT_2 * 10, radius},
//            {{0, 5.51, 2.859690-0.1}, Intensities::INCANDESCENT_2 * 10, radius},
            {{-0.808237, 3.625171, 2.205287 - 0.1}, Intensities::INCANDESCENT_2 * 40, radius},
//            {{1.604023, 2.675619, 2.218865-0.1}, Intensities::INCANDESCENT_2 * 10, radius},
    };

    std::vector<std::unique_ptr<Surface>> surfaces{};
    surfaces.push_back(std::move(sphere));
//    surfaces.push_back(std::move(sphere));
    for (auto& paska : cube) {
        surfaces.push_back(std::move(paska));
    }


    auto bvh = std::make_unique<BVH>(std::move(surfaces));

    std::vector<std::unique_ptr<Surface>> objects;
    objects.push_back(std::move(plane));
    objects.push_back(std::move(bvh));
    objects.push_back(std::move(backdrop));


    return Scene{std::move(objects), std::move(lightSources), camera, 2, 1, 1, std::move(textureManager), std::move(materialManager)};
}

Scene Scenes::getLegacyScene (int windowX, int windowY) {
    Camera camera{{0, -5, 7}, {0, -4, 7 - 0.3}, 0.7, {1.f, (float) windowY / windowX}, {windowX, windowY}};

    Manager<Texture> textureManager;
    Manager<Material> materialManager;


//    const Material* simpleLambert = materialManager.get<Phong>(&SolidTextures::WHITE, &SolidTextures::WHITE, &SolidTextures::BLACK, &SolidTextures::BLACK, 1.f);
    const Texture* t1 = textureManager.get<SolidTexture>(Intensity{1, 1, 1} * 0.7);
    const Texture* triangleTexture = textureManager.get<SolidTexture>(Intensity{1, 1, 1} * 0.5);

//    const Material* triangleMaterial = materialManager.get<PhongLegacy>(triangleTexture, triangleTexture, t1, &SolidTextures::BLACK, 1.f);
//    const Material* m2 = materialManager.get<PhongLegacy>(&SolidTextures::WHITE, &SolidTextures::WHITE, t1, &SolidTextures::BLACK, 1.f);
//    const Material* m3 = materialManager.get<PhongLegacy>(&SolidTextures::WHITE, &SolidTextures::WHITE, &SolidTextures::BLACK, &SolidTextures::BLACK, 1.f);
//    const Material* m4 = materialManager.get<PhongLegacy>(&SolidTextures::WHITE, &SolidTextures::WHITE, &SolidTextures::WHITE, &SolidTextures::BLACK, 1.f);


    const Material* triangleMaterial = materialManager.get<DebugMaterial>();
    const Material* m2 = materialManager.get<DebugMaterial>();
    const Material* m3 = materialManager.get<DebugMaterial>();
    const Material* m4 = materialManager.get<DebugMaterial>();

    std::unique_ptr<Surface> triangle = std::make_unique<Triangle>(glm::vec3{-5, 6, 5}, glm::vec3{0, 0, 3}, glm::vec3{5, 6, 3}, triangleMaterial);
    std::unique_ptr<Surface> sphere1 = std::make_unique<Sphere>(glm::vec3{-2.5, 4, 4.5}, 0.3, m2);
    std::unique_ptr<Surface> sphere2 = std::make_unique<Sphere>(glm::vec3{-1, 4, 4.3}, 0.6, m2);
    std::unique_ptr<Surface> sphere3 = std::make_unique<Sphere>(glm::vec3{1, 4, 4}, 1, m2);
    std::unique_ptr<Surface> sphere4 = std::make_unique<Sphere>(glm::vec3{0.5, 2, 3}, 0.5, m2);
    std::unique_ptr<Surface> sphere5 = std::make_unique<Sphere>(glm::vec3{-0.75, 2, 3.25}, 0.4, m2);
    std::unique_ptr<Surface> sphere6 = std::make_unique<Sphere>(glm::vec3{0, 6, 6}, 1.5, m4);




    std::unique_ptr<Surface> plane = std::make_unique<Plane>(glm::vec3{0, 0, 1}, -2, m3);
    std::vector<std::unique_ptr<Surface>> surfaces;
    surfaces.push_back(std::move(triangle));
    surfaces.push_back(std::move(sphere1));
    surfaces.push_back(std::move(sphere2));
    surfaces.push_back(std::move(sphere3));
    surfaces.push_back(std::move(sphere4));
    surfaces.push_back(std::move(sphere5));
    surfaces.push_back(std::move(sphere6));


    auto bvh = std::make_unique<BVH>(std::move(surfaces));
    std::vector<std::unique_ptr<Surface>> objects;
    objects.push_back(std::move(bvh));
//    objects.push_back(std::move(plane));


    std::vector<LightSource> lightSources {
//        {{0, 5, 5}, Intensity{1, 0, 0.25} * 5, 0},
        {{0, 0, 7}, Intensity{0.5, 0.5, 1} * 25, 0},
        {{-2, 3, 10}, Intensity{1, 0.1, 0} * 20, 0},
//        {{-2, 3, 10, Intensity{1, 0, 0.25} * 5, 1},
    };

    return Scene{std::move(objects), lightSources, camera, 3, 1, 1, std::move(textureManager), std::move(materialManager)};

}