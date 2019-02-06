#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include "chip8.h"

Chip8 mychip8;
const unsigned int W = 64;
const unsigned int H = 32;
void getKeys();


int main(int argc, char *argv[]){

	mychip8.loadApplication(argv[1]);

	std::cout << "Done!" << std::endl;
	sf::RenderWindow window(sf::VideoMode(640, 320), "Emulator");
	sf::Uint8* pxls = new sf::Uint8[W*H*4];

	sf::Texture texture;

	texture.create(W, H);

	sf::Sprite sprite (texture);
	sprite.setScale(10.0f, 10.0f);

	window.clear();
	sf::Clock clock;
	while(window.isOpen()){

		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::KeyPressed){
				switch(event.key.code){
					case sf::Keyboard::Num1:
						mychip8.key[0] = 1;
					break;
					case sf::Keyboard::Num2:
						mychip8.key[1] = 1;
					break;
					case sf::Keyboard::Num3:
						mychip8.key[2] = 1;
					break;
					case sf::Keyboard::Num4:
						mychip8.key[3] = 1;
					break;
					case sf::Keyboard::Q:
						mychip8.key[4] = 1;
					break;
					case sf::Keyboard::W:
						mychip8.key[5] = 1;
					break;
					case sf::Keyboard::E:
						mychip8.key[6] = 1;
					break;
					case sf::Keyboard::R:
						mychip8.key[7] = 1;
					break;
					case sf::Keyboard::A:
						mychip8.key[8] = 1;
					break;
					case sf::Keyboard::S:
						mychip8.key[9] = 1;
					break;
					case sf::Keyboard::D:
						mychip8.key[10] = 1;
					break;
					case sf::Keyboard::F:
						mychip8.key[11] = 1;
					break;
					case sf::Keyboard::Z:
						mychip8.key[12] = 1;
					break;
					case sf::Keyboard::X:
						mychip8.key[13] = 1;
					break;
					case sf::Keyboard::C:
						mychip8.key[14] = 1;
					break;
					case sf::Keyboard::V:
						mychip8.key[15] = 1;
					break;
				}
			}
			if(event.type == sf::Event::KeyReleased){
				switch(event.key.code){
					case sf::Keyboard::Num1:
						mychip8.key[0] = 0;
					break;
					case sf::Keyboard::Num2:
						mychip8.key[1] = 0;
					break;
					case sf::Keyboard::Num3:
						mychip8.key[2] = 0;
					break;
					case sf::Keyboard::Num4:
						mychip8.key[3] = 0;
					break;
					case sf::Keyboard::Q:
						mychip8.key[4] = 0;
					break;
					case sf::Keyboard::W:
						mychip8.key[5] = 0;
					break;
					case sf::Keyboard::E:
						mychip8.key[6] = 0;
					break;
					case sf::Keyboard::R:
						mychip8.key[7] = 0;
					break;
					case sf::Keyboard::A:
						mychip8.key[8] = 0;
					break;
					case sf::Keyboard::S:
						mychip8.key[9] = 0;
					break;
					case sf::Keyboard::D:
						mychip8.key[10] = 0;
					break;
					case sf::Keyboard::F:
						mychip8.key[11] = 0;
					break;
					case sf::Keyboard::Z:
						mychip8.key[12] = 0;
					break;
					case sf::Keyboard::X:
						mychip8.key[13] = 0;
					break;
					case sf::Keyboard::C:
						mychip8.key[14] = 0;
					break;
					case sf::Keyboard::V:
						mychip8.key[15] = 0;
					break;
				}
			}
		}
		if(clock.getElapsedTime().asSeconds() > (1.0f/500.0f) ){
			mychip8.emulateCycle();
			clock.restart();
		}
		if(mychip8.drawFlag){
			//std::cout << "Drawing" << '\n';
			for(register int i = 0; i < 2048 * 4; i += 4) {
				pxls[i] = 255;
				pxls[i + 1] = 255;
				pxls[i + 2] = 255;
				if(mychip8.gfx[i / 4] == 0){
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
