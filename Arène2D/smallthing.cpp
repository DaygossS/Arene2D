#include <SFML/Graphics.hpp>
#include <iostream>

inline sf::Color rainbowing(float& ticks) {
    //RGB (0-255r 256-511rg 512-767g 768-1023gb 1024-1279b 1280-1535br )
    sf::Color coloring = sf::Color::Black;

    if (ticks <= 255) {
        float tick = ticks - 0;
        coloring.r = 255;
        coloring.g = 0;
        coloring.b = 255 - tick;
    }
    else if (ticks <= 511 ) {
        float tick = ticks - 255;
        coloring.r = 255;
        coloring.g = tick;
        coloring.b = 0;
    }
    else if (ticks <= 727) {
        float tick = ticks - 511;
        coloring.r = 255 - tick;
        coloring.g = 255;
        coloring.b = 0;
    }
    //why do you glitch???
    else if (ticks <= 1023 ) {
        float tick = ticks - 727;
        coloring.r = 0;
        coloring.g = 255;
        coloring.b = tick;
    }
    else if (ticks <= 1279 ) {
        float tick = ticks - 1023;
        coloring.r = 0;
        coloring.g = 255 - tick;
        coloring.b = 255;
    }
    else if (ticks <= 1535) {
        float tick = ticks - 1279;
        coloring.r = tick;
        coloring.g = 0;
        coloring.b = 255;
    }
    else {
        coloring = sf::Color::Magenta;
        ticks = 0;
    }



    return coloring;
}

