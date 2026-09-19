// Include important libraries here
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace sf;

// this is where our game starts from
int main()
{
	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Making the background
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Making the tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Making the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	bool beeActive = false;
	float beeSpeed = 0.0f;

	// Making 3 clouds
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1, spriteCloud2, spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	// Set Positions
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
	bool cloud1Active = false, cloud2Active = false, cloud3Active = false;
	float cloud1Speed = 0.0f, cloud2Speed = 0.0f, cloud3Speed = 0.0f;

	// Clock
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400, timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


	// Track whether the game is running
	bool paused = true;



	// Draw some text
	int score = 0;
	Text messageText, scoreText;
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	scoreText.setFont(font);
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);
	


	while (window.isOpen()) {


		/*
		Check Player Input
		*/
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// Start Game
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			// Rest the time and the score
			score = 0;
			timeRemaining = 6;
		}

		/*
		Update Game
		*/

		if (!paused) {
			// Measure Time
			Time dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			// Losing Case
			if (timeRemaining <= 0.0f) {
				// Pause the game
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!!");

				// Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}


			// Setup Bee
			if (!beeActive) {

				// how fast is bee
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else // Move the bee 
			{

				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

				if (spriteBee.getPosition().x < -100) {
					beeActive = false;
				}

			}

			// Manage the Clouds
			// Cloud1
			if (!cloud1Active) {
				// how fast is the cloud

				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// how high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);

				// Has the cloud reached the right hand side of the screen
				if (spriteCloud1.getPosition().x > 1920) {
					// Set it up ready to be a whole new cloud
					cloud1Active = false;
				}
			}
			// Cloud2
			if (!cloud2Active) {
				// how fast is the cloud

				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// how high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

				// Has the cloud reached the right hand side of the screen
				if (spriteCloud2.getPosition().x > 1920) {
					// Set it up ready to be a whole new cloud
					cloud2Active = false;
				}
			}
			// Cloud3
			if (!cloud3Active) {
				// how fast is the cloud

				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// how high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

				// Has the cloud reached the right hand side of the screen
				if (spriteCloud3.getPosition().x > 1920) {
					// Set it up ready to be a whole new cloud
					cloud3Active = false;
				}
			}


			// Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());


		} // End if (!paused)


		/*
		Draw the scene
		*/
		window.clear();

		// Draw BG
		window.draw(spriteBackground);

		// Draw Clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// Draw Tree
		window.draw(spriteTree);

		// Draw Bee
		window.draw(spriteBee);

		// Draw the score
		window.draw(scoreText);

		// Draw the timebar
		window.draw(timeBar);
		
		// Draw message
		if (paused) {
			window.draw(messageText);
		}


		window.display();

	}

	

	return 0;
}