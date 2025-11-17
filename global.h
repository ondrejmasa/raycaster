#pragma once
#include <array>
#include <map>

namespace gbl
{
    constexpr double PI = 3.14159265358979323846;

    namespace screen
    {
        constexpr unsigned short width = 1280;
        constexpr unsigned short height = 720;
        //constexpr unsigned short width = 800;
        //constexpr unsigned short height = 600;
    }

    namespace map
    {
        constexpr float maxRayLength = 40.f;
    }
}