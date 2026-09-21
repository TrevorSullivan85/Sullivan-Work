// Include important libraries here
#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>
using namespace sf;

// Function declaration
void spawnAsteroids(int seed);

const int NUM_ASTEROIDS = 10;
Sprite asteroids[NUM_ASTEROIDS];

// Where is the player/branch
// Top or Bottom
enum class side { TOP, BOTTOM, MIDDLE, NONE};

side branchPositions[NUM_ASTEROIDS];
float branchXPositions[NUM_ASTEROIDS];

// this is where our game starts from
int main()
{
	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Random number centralized
	srand(time(0));

	// Making the Space background
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/black.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);


	// Clock
	Clock clock;

	// Time
	float timeSurvived = 0.0f;

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

	// TODO: Prepare Asteroids
	Texture textureAsteroid;
	textureAsteroid.loadFromFile("graphics/meteorBrown_big1.png");
	int const ASTEROID_SPEED = 500;
	float const GROWTH_RATE = 20;
	float const SPINNING_SPEED = 250;


	// Y locations for the player and the asteroids
	float const TOP_Y = 180;
	float const MIDDLE_Y = 545;
	float const BOTTOM_Y = 910;

	// Set the texture for each asteroid
	for (int i = 0; i < NUM_ASTEROIDS; i++) {
		asteroids[i].setTexture(textureAsteroid);
		asteroids[i].setOrigin(asteroids[i].getLocalBounds().width / 2, asteroids[i].getLocalBounds().height / 2);
		asteroids[i].setPosition(-300, -300);// Hide
		asteroids[i].setScale(1.5, 1.5);
	}

	// Prepare the Ship
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/playerShip1_blue.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setOrigin(spritePlayer.getLocalBounds().width / 2, spritePlayer.getLocalBounds().height / 2);
	spritePlayer.setPosition(1700, MIDDLE_Y);
	spritePlayer.setScale(1.5, 1.5);
	spritePlayer.setRotation(-90);

	// The player starts on the MIDDLE
	side playerSide = side::MIDDLE;

	// Control player input
	bool acceptInput = false;

	// Prepare the sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/fly.wav");
	Sound fly;
	fly.setBuffer(chopBuffer);
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);


	while (window.isOpen()) {
		/*
		Check Player Input
		*/

		Event event;

		while (window.pollEvent(event)) {

			if (event.type == Event::KeyReleased && !paused) {

				// Listen for key presses again
				acceptInput = true;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// Start Game
		if (Keyboard::isKeyPressed(Keyboard::Return) && !acceptInput) {
			paused = false;

			// Rest the time and the score
			score = 0;

			timeSurvived = 0;

			// Put player back on screen
			spritePlayer.setPosition(1700, MIDDLE_Y);

			// Ready Asteroids
			for (int i = 0; i < NUM_ASTEROIDS; i++) {
				asteroids[i].setPosition(-2000 - (i * 350), -2000);
			}

			spawnAsteroids(timeSurvived);

			acceptInput = true;
		}

		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput) {

			// Player presses up arrow
			if (Keyboard::isKeyPressed(Keyboard::Down) && playerSide != side::BOTTOM) {

				// Move the player accordingly
				if (playerSide == side::MIDDLE) {
					playerSide = side::BOTTOM;
					spritePlayer.setPosition(1700, BOTTOM_Y);
				}
				else {
					playerSide = side::MIDDLE;
					spritePlayer.setPosition(1700, MIDDLE_Y);
				}

				acceptInput = false;

				// Play a fly sound
				fly.play();

			}

			// Player presses down arrow
			if (Keyboard::isKeyPressed(Keyboard::Up) && playerSide != side::TOP) {
				// Move the player accordingly
				if (playerSide == side::MIDDLE) {
					playerSide = side::TOP;
					spritePlayer.setPosition(1700, TOP_Y);
				}
				else {
					playerSide = side::MIDDLE;
					spritePlayer.setPosition(1700, MIDDLE_Y);
				}


				


				acceptInput = false;

				score++;

				// Play a fly sound
				fly.play();
			}
		}

		/*
		Update Game
		*/


		if (!paused) {
			// Measure Time
			Time dt = clock.restart();

			timeSurvived += dt.asSeconds();


			// Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			spawnAsteroids(timeSurvived);

			// Update branch sprites
			for (int i = 0; i < NUM_ASTEROIDS; i++) {


				// Moving Branches
				float conveyorSpeed = ASTEROID_SPEED + (GROWTH_RATE * timeSurvived);
				float positionX = asteroids[i].getPosition().x + (conveyorSpeed * dt.asSeconds());

				if (branchPositions[i] == side::TOP) {
					// Move the sprite to the Top side
					asteroids[i].setPosition(asteroids[i].getPosition().x, TOP_Y);
				}
				else if (branchPositions[i] == side::MIDDLE) {
					// Move sprite to the Middle
					asteroids[i].setPosition(asteroids[i].getPosition().x, MIDDLE_Y);
				}
				else if (branchPositions[i] == side::BOTTOM) {
					// Move sprite to the bottom
					asteroids[i].setPosition(asteroids[i].getPosition().x, BOTTOM_Y);
				}
				else {
					// Leave Branch off screen
					asteroids[i].setPosition(asteroids[i].getPosition().x, 3000);
				}
				// Move Branches no matter what
				asteroids[i].setPosition(positionX, asteroids[i].getPosition().y);
				asteroids[i].rotate(SPINNING_SPEED * dt.asSeconds());

			}

			// Check all asteroids to see if one is hitting the player
			for (int i = 0; i < NUM_ASTEROIDS;i++) {
				// has the player been squished by a branch?
				if (branchPositions[i] == playerSide && (1800 > asteroids[i].getPosition().x && asteroids[i].getPosition().x > 1650)) {
					// death
					paused = true;
					acceptInput = false;

					// hide the player
					spritePlayer.setPosition(2000, 660);

					// Change the text of the message
					messageText.setString("SQUISHED!!");

					// Center it on the screen
					FloatRect textRect = messageText.getLocalBounds();

					messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

					messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

					// Play the death sound
					death.play();

				}
			}

			

		} // End if (!paused)


		/*
		Draw the scene
		*/
		window.clear();

		// Draw BG
		window.draw(spriteBackground);

		// Draw Asteroids
		for (int i = 0; i < NUM_ASTEROIDS; i++) {
			window.draw(asteroids[i]);
		}

		// Draw the Player
		window.draw(spritePlayer);

		// Draw the score
		window.draw(scoreText);
		
		// Draw message
		if (paused) {
			window.draw(messageText);
		}


		window.display();

	}

	

	return 0;
}

// Function definition
void spawnAsteroids(int seed) {
	// Move all the asteroids down one place
	for (int i = 0; i < NUM_ASTEROIDS; i++) {
		// If branch went off screen
		// Respawn it
		if (asteroids[i].getPosition().x > 2000) {

			// Need to spawn branch behind leftmost branch to ensure equal gap
			int leftmost = (i + NUM_ASTEROIDS - 1) % NUM_ASTEROIDS;

			asteroids[i].setPosition(asteroids[leftmost].getPosition().x - 350, asteroids[i].getPosition().y);
			// Spawn a new branch
			int r = (rand() % 3); // No chance for NONE when % 3

			switch (r) {
			case 0:
				branchPositions[i] = side::TOP;
				break;

			case 1:
				branchPositions[i] = side::MIDDLE;
				break;

			case 2:
				branchPositions[i] = side::BOTTOM;
				break;
			default:
				branchPositions[i] = side::NONE;
				break;
			}

		}
	}


}
