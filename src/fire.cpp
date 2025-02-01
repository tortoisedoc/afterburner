#include <SFML/Graphics.hpp>
#include "fire_engine.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>

const int WIDTH = 640;
const int HEIGHT = 480;
const int NUM_PIXELS = WIDTH * HEIGHT;
const int FADING = 1;

// Spoiler : Some ancient algorithm knowledge out of the depth of time
// https://web.archive.org/web/20160418004150/http://freespace.virgin.net/hugo.elias/models/m_fire.htm
// https://web.archive.org/web/20160418004149/http://freespace.virgin.net/hugo.elias/graphics/x_warp.htm

int main() {
    sf::Texture fireTexture;
    //Stack should not overflow with SD just yet..
    std::vector<unsigned int> pixels(NUM_PIXELS);
    std::vector<unsigned int> newPixels(NUM_PIXELS);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fire Simulation");
    FireEngine * engine = new FireEngine (WIDTH, HEIGHT);
    fireTexture.create(WIDTH, HEIGHT);

    sf::Sprite sprite;
    sprite.setTexture(fireTexture);
    while (window.isOpen()) {
        std::cout << ".";
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        engine->Scroll(&pixels[0], 1);
        engine->Ignite(&pixels[0], HEIGHT - 1);
        engine->Burn(&pixels[0], &newPixels[0], 1, 1);

        // Draw the fire as texture via a sprite; load in bytes
        fireTexture.update((sf::Uint8*)&newPixels[0]);
        memcpy(&pixels[0], &newPixels[0], NUM_PIXELS*4);

        window.clear();
        window.draw(sprite);
	window.display();
        usleep(16667);
    }
    return 0;
}
