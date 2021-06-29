#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

using namespace sf; 
using namespace std;

void updateBranches(int seed); //function that takes in an int value
const int NUM_BRANCHES = 6; //constant global variable six
Sprite branches[NUM_BRANCHES]; //array of branches six branches in array
enum class side {LEFT, RIGHT, NONE}; //enumeration 
side branchPositions[NUM_BRANCHES]; //array of sides 

int main() {
	
	// Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game
	RenderWindow window(vm, "Norralak's Timber", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;
	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");
	// Create a sprite
	Sprite spriteBackground;
	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);
	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);
	Texture textureBackTree;
	textureBackTree.loadFromFile("graphics/tree2.png");
	Sprite backTrees[4];
	for (int i = 0; i < 4; i++) {
		backTrees[i].setTexture(textureBackTree);
	}
	backTrees[0].setPosition(0,-300);
	backTrees[1].setPosition(1600, -200);
	backTrees[2].setPosition(450, -100);
	backTrees[3].setPosition(1325, 200);
	// Making a bee sprite
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	//Check if bee is moving
	bool beeActive = false;
	float beeSpeed = 0.0f;
	float beeUp = -1; //myEdit
	int beeSide = 1; //myEdit

	//We can just create a cloud texture
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	//But then create multiple sprites of the same texture
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	// Position the clouds on the left of the screen at different heights
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
	// Are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// How fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	// Variables to control time itself
	Clock clock;
	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Green);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSec = timeBarStartWidth / timeRemaining; // 400/6

	// Track whether the game is running
	bool paused = true;
	//Draw some text
	int score = 0;
	
	Text messageText, scoreText;
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setString("Press ENTER to start!");
	scoreText.setString("Score: 0");

	messageText.setCharacterSize(65);
	scoreText.setCharacterSize(91);
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Position text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920/2.0f, 1080/2.0f);
	scoreText.setPosition(20,20);

	
	//FPS
	float fps = 0.0f;
	Text fpsText;
	fpsText.setFont(font);
	fpsText.setCharacterSize(91);
	fpsText.setString("FPS: 0");
	fpsText.setFillColor(Color::White);
	FloatRect fpsRect = fpsText.getLocalBounds();
	fpsText.setOrigin(fpsRect.left + fpsRect.width / 2.0f, fpsRect.top + fpsRect.height / 2.0f);
	fpsText.setPosition(1600, 60);
	//Branches!
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++) { //if i is less than six
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}
	
	//Player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//Player starts on left side;
	side playerSide = side::LEFT;

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 830);

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Line up axe with tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RITE = 1075;
	//Log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//Player input
	bool acceptInput = false;
	//Prepare the sounds
	// The player chopping sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sounds/nicoChopEdit.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
	// The player has met his end under a branch
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sounds/nicoBranch.wav");
	Sound death;
	death.setBuffer(deathBuffer);
	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sounds/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);
	//Nico out of time
	SoundBuffer nickOOTBuffer;
	nickOOTBuffer.loadFromFile("sounds/nicoTime.wav");
	Sound nickTime;
	nickTime.setBuffer(nickOOTBuffer);
	SoundBuffer beeBuffer;
	beeBuffer.loadFromFile("sounds/angieBee.wav");
	Sound angieBee;
	angieBee.setBuffer(beeBuffer);

	while (window.isOpen()) {

		/*
		****************************************
		Handle the players input
		****************************************
		*/

		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				acceptInput = true;

				//Hide axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)){
			window.close();
		}
		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return)){
			outOfTime.play();
			// Reset the time and the score
			//I did this first so that the game won't play nickTime after you die
			score = 0;
			timeRemaining = 6.0f;
			paused = false;
			//Make branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			//Hide grave
			spriteRIP.setPosition(675, 2000);
			//Player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}
		if (acceptInput) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				playerSide = side::RIGHT;
				score++;

				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_RITE, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);
		
				updateBranches(score);
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				// Play a chop sound
				chop.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				playerSide = side::LEFT;

				score++;
				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				updateBranches(score);
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				// Play a chop sound
				chop.play();
			}

		}
		/*
		****************************************
		Update the scene
		****************************************
		*/
		if (!paused) {
			Time dt = clock.restart();
			//subtract time remaining
			fps = 1/dt.asSeconds();
			timeRemaining -= dt.asSeconds();
			//edit size of timeBar
			//now the bar is 400/6 * time remaining which is pretty much 6 divided by timeRemaining times 400(the start)
			timeBar.setSize(Vector2f(timeBarWidthPerSec * timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f) {
				timeRemaining = 6.0f;
				paused = true;
				timeRemaining = 0.13;
				acceptInput = false;
				messageText.setString("OUT OF TIME!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				outOfTime.play();
			}
			if (!beeActive) {
				// How fast is the bee
				srand((int)time(0));//(int) casts time(0) value as an int and time(0) returns the time from the pc; srand uses that number to seed which will be used in rand()
				beeSpeed = (rand() % 200) + 200 ;
				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				if (beeSide == 1) {
					spriteBee.setPosition(1999, height);
				}
				else if (beeSide == -1) {
					spriteBee.setPosition(-99, height);
				}
				angieBee.play();
				beeActive = true;
			}

			else {
				// Move the bee vertically
				float height = spriteBee.getPosition().y, width = spriteBee.getPosition().x; //initialize y and x coordinates respectively
				// Has the bee reached the edge of the screen?
				if (!(width < -100 || width > 2000)) { //if not, move the bee
				
					if (height >= 999) { //goes up if too low
						beeUp = -1;
					}

					else if (height <= 499) { //goes down if too high
						beeUp = 1;
					}

					spriteBee.setPosition((width - beeSide * (beeSpeed * dt.asSeconds())), height + (2 * (beeUp * beeSpeed * dt.asSeconds())));
				
				}			

				else {
					beeActive = false;
					beeSide = beeSide * (-1); //turn the bee around
					spriteBee.scale(-1.0f,1.0f); //turn the bee around
				}
			}	

			// Manage the clouds
			// Cloud 1
			if (!cloud1Active) {

				// How fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = ((rand() % 200)) + 65; //myEdit sets speed to at least 66
				// How high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}

			else {
		
				spriteCloud1.setPosition(
				spriteCloud1.getPosition().x +
				(cloud1Speed * dt.asSeconds()),
				spriteCloud1.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud1.getPosition().x > 1920){
					// Set it up ready to be a whole new cloud next frame
					cloud1Active = false;
				}
		
			}
			//cloud 2
			if (!cloud2Active) {

				// How fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = ((rand() % 200)) + 65; //myEdit sets speed to at least 66
				// How high is the cloud
				srand((int)time(0) * 20);
				float height = ((rand() % 300) - 150);
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}

			else {
			
				spriteCloud2.setPosition(
				spriteCloud2.getPosition().x +
				(cloud2Speed * dt.asSeconds()),
				spriteCloud2.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud2.getPosition().x > 1920) {
					// Set it up ready to be a whole new cloud next frame
					cloud2Active = false;
				}

			}
			//cloud 3
			if (!cloud3Active) {

				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = ((rand() % 200)) + 65; //myEdit sets speed to at least 66
				// How high is the cloud
				srand((int)time(0) * 30);
				float height = ((rand() % 450) - 150);
				spriteCloud3.setPosition(2000, height);
				cloud3Active = true;
			}

			else {
				//myEdit moves cloud3 from right to left
				spriteCloud3.setPosition(
				spriteCloud3.getPosition().x -
				(cloud3Speed * dt.asSeconds()),
				spriteCloud3.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud3.getPosition().x < -200) {
					// Set it up ready to be a whole new cloud next frame
					cloud3Active = false;
				}
			}
			stringstream ss, sfps;
			sfps << "FPS: " << fps;
			fpsText.setString(sfps.str());
			ss << "Score: " << score;
			scoreText.setString(ss.str());
				
			//Branch updates
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;

				if (branchPositions[i] == side::LEFT) {
					//Move it to the left
					branches[i].setPosition(610, height);
					//Flip the sprite
					branches[i].setRotation(180);
				}

				else if (branchPositions[i] == side::RIGHT) {
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}

				else {
					
					branches[i].setPosition(3000, height); //Hide the branches out of player view
					
				}

			}

			//Handle flying log
			if (logActive) {
				
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
				//If it goes offscreen
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().y > 2000) {
						logActive = false;
						spriteLog.setPosition(810, 720);
				}
			}
			//Death
			if (branchPositions[5] == playerSide) {
				timeRemaining = 6.0f;
				paused = true;
				acceptInput = false;
				//Grave
				if (playerSide == side::LEFT) {
					spriteRIP.setPosition(525, 760);
				}
				else if (playerSide == side::RIGHT) {
					spriteRIP.setPosition(1395, 760);
				}
				//Hide player
				spritePlayer.setPosition(2000, 660);
				messageText.setString("YOU DEAD!");

				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920/2.0f, 1080/2.0f);
				death.play();
			}
			
		} //End
	 
		/*
		****************************************
		Draw the scene
		****************************************
		*/
		// Clear everything from the last frame
		window.clear();
		// Draw our game scene here
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		//Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}
		//for (int i = 0; i < 4; i++) {
			//window.draw(backTrees[i]);
		//}
		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);
		window.draw(spriteBee);
		window.draw(timeBar);
		window.draw(scoreText);
		window.draw(fpsText);
		if (paused) {
			window.draw(messageText);
		}
		// Show everything we just drew
		window.display();
		fps++;
	}

	return 0;
}

//Define functions

void updateBranches(int seed) {
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		//move the branches down
		branchPositions[j] = branchPositions[j - 1];
	}

	//Spawn a new branch at position 0
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
	
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}

}