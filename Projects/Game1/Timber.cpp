// Include important libraries here
#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>
using namespace sf;

// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch
// Top or Bottom
enum class side { TOP, BOTTOM, NONE};

side branchPositions[NUM_BRANCHES];
float branchXPositions[NUM_BRANCHES];

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
	spriteTree.setOrigin(spriteTree.getLocalBounds().width / 2, spriteTree.getLocalBounds().height / 2);
	spriteTree.setPosition(1920 / 2, 1080 / 2);
	spriteTree.setRotation(90);
	spriteTree.setScale(1, 3);


	// Making the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	bool beeActive = false;
	float beeSpeed = 0.0f;

	// Making 3 clouds

	const int NUM_CLOUDS = 3;
	Sprite clouds[NUM_CLOUDS];
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	bool cloudsActive[NUM_CLOUDS];
	float cloudsSpeeds[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++) {
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, (i + 1) * 150);
		cloudsActive[i] = false;
		cloudsSpeeds[i] = 0;
	}



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

	// Prepare 5 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// Set the sprite's origin to dead center
		branches[i].setOrigin(220, 20);

		// Rotate branch for horizontal game
		branches[i].setRotation(90);
	}

	// Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// The player starts on the Left
	side playerSide = side::TOP;

	// Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line the axe up with the tree
	const float AXE_POSITION_TOP = 700;
	const float AXE_POSITION_BOTTOM = 1075;

	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 1080 / 2);
	spriteLog.setOrigin(spriteLog.getLocalBounds().width / 2, spriteLog.getLocalBounds().height / 2);
	spriteLog.setRotation(90);

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control player input
	bool acceptInput = false;

	// Prepare the sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
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

				// hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// Start Game
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			// Rest the time and the score
			score = 0;
			timeRemaining = 6;

			// Make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			// Make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			// Move the player into position
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}

		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput) {
			// handle pressing right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right)) {

				// Make sure the player is on the right
				playerSide = side::BOTTOM;

				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_TOP, spriteAxe.getPosition().y);
				
				spritePlayer.setPosition(1200, 720);

				// Update the branches
				updateBranches(score);

				// Set the log flying to the Left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				chop.play();

			}

			// Handle the left cursor key

			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				// Make sure the player is on the left
				playerSide = side::TOP;

				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_BOTTOM, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				// update the branches
				updateBranches(score);

				// set the log flying
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				chop.play();
			}
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

				// Play out of time sound
				outOfTime.play();
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
			for (int i = 0; i < NUM_CLOUDS; i++) {
				if (!cloudsActive[i]) {
					// how fast is the cloud
					srand((int)time(0) * 10 * (i + 1));
					cloudsSpeeds[i] = (rand() % 200);

					// how high is the cloud
					srand((int)time(0) * 10 * (i + 1));
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;
				}
				else {
					clouds[i].setPosition(clouds[i].getPosition().x + (cloudsSpeeds[i] * dt.asSeconds()), clouds[i].getPosition().y);

					// Has the cloud reached the right hand side of the screen
					if (clouds[i].getPosition().x > 1920) {
						// Set it up ready to be a whole new cloud
						cloudsActive[i] = false;
					}
				}
			}


			// Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// Update branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++) {

				float height = i * 150;

				if (branchPositions[i] == side::TOP) {
					// Move the sprite to the Top side
					branches[i].setPosition(height, 300);

					// Flip the sprite round the other way
					branches[i].setRotation(270);

				}
				else if (branchPositions[i] == side::BOTTOM) {
					// Move sprite to the right side
					branches[i].setPosition(height, 800);
					branches[i].setRotation(90);
				}
				else {
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}


			// Handle a flying log
			if (logActive) {

				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
					// Set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide) {
				// death
				paused = true;
				acceptInput = false;

				// Draw the gravestone
				spriteRIP.setPosition(525, 760);

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

		} // End if (!paused)


		/*
		Draw the scene
		*/
		window.clear();

		// Draw BG
		window.draw(spriteBackground);

		// Draw Clouds
		for (int i = 0; i < NUM_CLOUDS; i++) {
			window.draw(clouds[i]);
		}

		// Draw Branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		// Draw Tree
		window.draw(spriteTree);

		// Draw the Player
		window.draw(spritePlayer);

		// Draw the Axe
		window.draw(spriteAxe);

		// Draw the flying log
		window.draw(spriteLog);

		// Draw the gravestone
		window.draw(spriteRIP);

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

// Function definition
void updateBranches(int seed) {
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		branchPositions[0] = side::TOP;
		break;

	case 1:
		branchPositions[0] = side::BOTTOM;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}