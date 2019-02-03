#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include "chip8.h"

Chip8 mychip8;
const unsigned int W = 64;
const unsigned int H = 32;


int main(int argc, char **argv){

	mychip8.loadApplication("tetris.c8");

	std::cout << "Done!" << std::endl;
	sf::RenderWindow window(sf::VideoMode(640, 320), "Emulator");
	sf::Uint8* pxls = new sf::Uint8[W*H*4];

	sf::Texture texture;

	texture.create(W, H);

	sf::Sprite sprite (texture);
	sprite.setScale(10.0f, 10.0f);

	window.clear();
	while(window.isOpen()){

		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();
		}
		mychip8.emulateCycle();
		if(mychip8.drawFlag){
			//std::cout << "Drawing" << '\n';
			for(register int i = 0; i < 2048 * 4; i += 4) {
				pxls[i] = 255;
				pxls[i + 1] = 255;
				pxls[i + 2] = 255;
				if(mychip8.pixels[i / 4] == 0){
					pxls[i + 3] = 0;
				}
				else
					pxls[i + 3] = 255;
			}
			texture.update(pxls);
			window.clear();
			window.draw(sprite);
			//mychip8.debugRender();

			mychip8.drawFlag = false;
		}
		window.display();
	}

	return 0;
}
