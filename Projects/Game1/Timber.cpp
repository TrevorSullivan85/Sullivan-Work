// Include important libraries here
#include <SFML/Graphics.hpp>

using namespace sf;

// this is where our game starts from
int main()
{
	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	Texture textureBackground;

	textureBackground.loadFromFile("graphics/background.png");

	Sprite spriteBackground;

	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	while (window.isOpen()) {
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		window.clear();

		window.draw(spriteBackground);

		window.display();

	}

	

	return 0;
}