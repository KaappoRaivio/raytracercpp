//
// Created by kaappo on 10.9.2021.
//
#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "../light/Intensity.h"
#include "../engine/Camera.h"

class MyOpenGLWindow {
private:
    inline static bool initialized;

    int m_width;
    int m_height;
    double m_gamma;
    int scale;
    const Camera& camera;

    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    MyOpenGLWindow (int width, int height, double gamma, int scale, const Camera& camera);
    ~MyOpenGLWindow ();

    void set_pixel (int x, int y, const Intensity& color) const;

    void update () const;

    void delay (int millis);

    void paint (std::vector<std::vector<Intensity>> pixels) const;
};

