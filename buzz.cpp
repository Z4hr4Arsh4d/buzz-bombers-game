#include <iostream>
#include <optional>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<ctime>
#include<string>
#include<fstream>
using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.

const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

int showMenu(RenderWindow& window);
void showWelcomeScreen(RenderWindow& window);
void showInstructionsScreen(RenderWindow& window);
void showLevelIntro(RenderWindow& window, int level);
std:: string playerNameInput(RenderWindow& window);
void displayMessage(RenderWindow& window, const std::string& message);
void loadAndUpdateScore(string playerName, int score);
void displayHighScore(sf::RenderWindow& window);
void loadLevel(RenderWindow& window, int level, bool& levelComplete,int& score,string playerName,bool& level1, bool& level2, bool& level3);
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
bool checkCollision(float bullet_x,float bullet_y, float bee_x, float bee_y);
void newBee(float bee_x[], float bee_y[], bool bee_active[],int bee_num, int& currentBeeCount);
void moveBee(float bee_x[],float bee_y[],bool bee_moves[],float bee_speed[],int bee_num, bool activeHoneycombs[], float honeycomb_x[], float honeycomb_y[], int maxHoneycombs,bool bee_active[],float flower_x[], float flower_y[], bool flower_active[],int& flowercount, int maxFlowers, bool& firstBeeReachedBottom,float bee_pauseTimer[], double pauseProb,float centerflower_x[], float centerflower_y[], bool centerflower_active[],int& centerflowercount, int maxCenterFlowers,bool activeRedHoneycombs[], float redhoneycomb_x[],float redhoneycomb_y[],int maxRedHoneycombs,int& beesKilled);
void moveRedBee(float fastbee_x[],float fastbee_y[],bool fastbee_moves[], float fastbee_speed[],int fastbee_num,bool fastbee_active[],float flower_x[], float flower_y[], bool flower_active[],int& flowercount, int maxFlowers, bool& firstBeeReachedBottom,int level,int& beesKilled);
void createFlower(float flower_x[], float flower_y[], bool flower_active[],int& flowercount, int maxFlowers,bool &firstBeeReachedBottom,bool bee_active[], float bee_x,int bee_num,float bee_y);
void createCenterFlower(float flower_x[], float flower_y[], bool flower_active[],int& flowercount, int maxFlowers, bool &firstBeeReachedBottom, bool bee_active[], float bee_x,int bee_num,float bee_y);
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, float flower_x[], float flower_y[],bool 
flower_active[], int& flowercount);
void drawCenterFlowers(RenderWindow& window, Sprite& centerflowerSprite, float centerflower_x[], float centerflower_y[],bool centerflower_active[], int& centerflowercount);
void updateText(Text& spraysText, Text& cansText,Text& scoreText, int currentSprays, int cans, int score);
bool allBeesInactive(bool bee_active[], int bee_num, bool fastbee_active[], int fastbee_num);
bool sprayMovement(float next_x, float player_y, float flower_x[], float flower_y[],bool flower_active[], int flowercount,float centerflower_x[], float centerflower_y[], bool centerflower_active[],int centerflowercount);
void movePlayer(float& player_x, float& player_y, float flower_x[], float flower_y[], bool flower_active[], int flowercount, float next_x,float centerflower_x[], float centerflower_y[], bool centerflower_active[],int centerflowercount);
bool useSpray(int& currentSprays, int& cans, int spraysPerCan);
void drawSprayCans(RenderWindow& window, Sprite& cansSprite, int& cans);
void drawSprayLine(RenderWindow& window, RectangleShape& groundRectangle, int cans, int sprayUsed);
int getTier(float y);
int scoreForCombs(int& score, float y,bool isRed);
void scoreForBee(int& score, bool isKillerBee);
void hitHummingBird(int& hitCount, bool& isSick, float& pauseTimer, float& hummingbird_x,float& hummingbird_y, bool& isMovingRight, bool& isMovingUp,bool& birdExist,float moveSpeed);
void hummingBird(float deltaTime, float honeycomb_x[], float honeycomb_y[],bool activeHoneycombs[], float& hummingbird_x, float& hummingbird_y, float& pauseTimer,  bool& isSick, bool& isPaused, bool& isMovingRight, float moveSpeed,int& score, Sprite& hummingbirdSprite,int maxHoneycombs,bool& isMovingUp,bool& birdExist,float redhoneycomb_x[], float redhoneycomb_y[],bool activeRedHoneycombs[],int maxRedHoneycombs);
void drawHummingbird(RenderWindow &window, Sprite& hummingbirdSprite, bool birdExist);
bool isRowFull(float flower_x[],bool flower_active[], int flowercount, int row,float flower_y[]);
void checkLevel(int &cans, int&level, float flower_x[],bool flower_active[], int& flowercount, int maxFlowers,float centerflower_x[],bool centerflower_active[], int& centerflowercount, int maxCenterFlowers,float flower_y[],float centerflower_y[],float& player_x, float& player_y);
void scoreForCans(int score,int& cans);

//////////////////////////////////////////////MAIN////////////////////////////////////////////////////////////////
int main(){
	//score
	bool level1=false;
	bool level2=false;
	bool level3=false;
	int score=0;
	 srand(time(0));
	// Declaring RenderWindow.
	RenderWindow window(VideoMode({(unsigned)resolutionX, (unsigned)resolutionY}), "Buzz Bombers", Style::Close | Style::Titlebar);
        //setting frame rate limit
        window.setFramerateLimit(60);
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(500, 200));

	// Initializing Background Music.
	Music bgMusic;
	if (!bgMusic.openFromFile("resources/Music/Music3.ogg")) {
	    cout << "Error: Could not load music file!" << endl;
	}
	bgMusic.setVolume(50);
	bgMusic.setLooping(true);
	bgMusic.play();
	
	showWelcomeScreen(window);
	showInstructionsScreen(window);
	std:: string playerName=playerNameInput(window);
	while (window.isOpen()) {
	
	int choice=showMenu(window);
	Clock clock;
	while (const std::optional event = window.pollEvent()) {
		if (event->is<Event::Closed>()) {
		return 0;
		}
	}
	int currentLevel=choice;
	bool levelComplete=false;
	switch(choice){
	
	//case 0: showHighScores(window);break;
	
	case 1:loadLevel(window, currentLevel,levelComplete,score,playerName,level1,level2,level3); break;
	case 2:loadLevel(window, currentLevel,levelComplete,score,playerName,level1,level2,level3); break;
	case 3:loadLevel(window, currentLevel,levelComplete,score,playerName,level1,level2,level3); break;
	case 4:displayHighScore(window); break;
	case 5:window.close();
	}
	}
	return 0;
	}	
/////////////////////////////////////////////////PROMPTING USER FOR NAME////////////////////////////////////////

std::string playerNameInput(RenderWindow& window){

	Font font;
	font.openFromFile("resources/fonts/OpenSans-ExtraBold.ttf");
	
	Text promptText(font, "Enter your Name", 30);
	promptText.setFillColor(Color::White);
	promptText.setPosition({(float)(resolutionX/2-3*boxPixelsX), (float)(resolutionY/2-2*boxPixelsY)});
	
	
	Text inputText(font, "", 30);
	inputText.setFillColor(Color::White);
	inputText.setPosition({(float)(resolutionX/2-3*boxPixelsX), (float)(resolutionY/2)});
	
	std::string name="";
	
	while(window.isOpen()){
	
	while(const std::optional event = window.pollEvent()){
		if(event->is<Event::Closed>()){
		window.close();
		return "";
	}
	
	if(const auto* textEntered = event->getIf<Event::TextEntered>()){
	if(textEntered->unicode==13){
	return name;
	}
	if(textEntered->unicode==8 &&!name.empty()){
	name.pop_back();
	}
	else if(textEntered->unicode<128){
	name+=static_cast<char>(textEntered->unicode);
	}
	
	inputText.setString(name);
	}
	
	}

	window.clear();
	window.draw(promptText);
	window.draw(inputText);
	window.display();
	}
	return "";
}	
/////////////////////////////////////////////////////FILE HANDLING//////////////////////////////////////////////

void loadAndUpdateScore(string playerName, int score){


	const int MAX_SCORES=10;
	const int NAME_LENGTH=200;

	string names[NAME_LENGTH];
	int scores[MAX_SCORES];
	
	ifstream file("score.txt");
	for(int i=0; i< MAX_SCORES; i++){
	
		names[i]="null";
		scores[i]=0;
	
	}

	int i=0;
	while(file>>names[i]>>scores[i]&& i<MAX_SCORES){
	i++;
	
	}
	file.close();
	
	
	bool updated=false;
	for(int i=0; i<MAX_SCORES; i++){
		if(score>scores[i]){
			for(int j=MAX_SCORES-1;j>i;j--){
			names[j]=names[j-1];
			scores[j]=scores[j-1];
			}	
	names[i]=playerName;
	scores[i]=score;
	updated=true;
	break;
		}
	}
	if(updated){
	for(int i=0; i< MAX_SCORES-1;i++){
		for(int j=0; j< MAX_SCORES-1-i;j++){
		
			if(scores[j]>scores[j+1]){
		
			int temp=scores[j];
			scores[j]=scores[j+1];
			scores[j+1]=temp;
		
		
			std:: string tempName=names[j];
			names[j]=names[j+1];
			names[j+1]=tempName;
			}		
		}
	
	}
	}
	
	
	
	ofstream outfile("score.txt");
	for(int i=0; i<MAX_SCORES; i++)
	{
	outfile<<names[i]<<" "<<scores[i]<<endl;
	}
	outfile.close();
	


}


void displayHighScore(sf::RenderWindow& window){


	const int MAX_SCORES=10;
	string names[MAX_SCORES];
	int scores[MAX_SCORES];

	ifstream file("score.txt");
	
	for(int i=0; i< MAX_SCORES; i++){
	names[i]="Null";
	scores[i]=0;
	
	}
	
	int i=0;
	while(file>>names[i]>>scores[i]&& i<MAX_SCORES){
	i++;
	}
	file.close();
	
	sf::Font font;
	font.openFromFile("resources/fonts/OpenSans-ExtraBold.ttf");
	sf::Text text(font);
	text.setCharacterSize(24);
	text.setFillColor(Color::White);
	
	float positionY=50;
	bool backToMenu=false;
	window.clear();
	for(int i=0; i<MAX_SCORES; i++){
	
	string scoreText= names[i]+"---"+ to_string(scores[i]);
	text.setString(scoreText);
	text.setPosition({100.f, positionY});
	window.draw(text);
	window.display();
	positionY+=40;
	
	}
	
		while(!backToMenu&&window.isOpen()){
		 while(const std::optional event = window.pollEvent()){
			if(event->is<sf::Event::Closed>()){
			window.close();
			return;
			}
			if(event->is<sf::Event::KeyPressed>()){
			backToMenu=true;
			}		
		   }	
		}
		
		if(backToMenu){
		showMenu(window);
		}
		
	}
		
////////////////////////////////////////////////////MENU////////////////////////////////////////////////////////
	


/////////////////////////////////////////////// LEVEL INTRO SCREEN //////////////////////////////////////////////
void showLevelIntro(RenderWindow& window, int level){
	Font font;
	if(!font.openFromFile("resources/fonts/OpenSans-ExtraBold.ttf")){
		cout << "Error: could not load font" << endl;
	}

	// Bee decoration
	Texture beeTex;
	beeTex.loadFromFile("resources/Textures/Regular_bee.png");
	Sprite beeSprite(beeTex);
	beeSprite.setTextureRect(IntRect({0, 0}, {boxPixelsX, boxPixelsY}));
	beeSprite.setScale({2.5f, 2.5f});

	// "LEVEL N" big neon title
	Text glow(font, "LEVEL " + std::to_string(level), 90);
	glow.setFillColor(Color(255, 0, 150, 70));
	Text title(font, "LEVEL " + std::to_string(level), 90);
	title.setFillColor(Color(0, 255, 200));
	title.setOutlineColor(Color(255, 0, 150));
	title.setOutlineThickness(4.f);
	FloatRect tb = title.getLocalBounds();
	float tx = (resolutionX - tb.size.x)/2.f;
	title.setPosition({tx, 230.f});

	// A little flavor line per level
	std::string flavor;
	if(level==1) flavor = "The swarm approaches...";
	else if(level==2) flavor = "Faster bees incoming!";
	else flavor = "Final challenge!";
	Text sub(font, flavor, 28);
	sub.setFillColor(Color(255, 230, 0));
	FloatRect sbf = sub.getLocalBounds();
	sub.setPosition({(resolutionX - sbf.size.x)/2.f, 350.f});

	Text getReady(font, "GET READY", 34);
	getReady.setFillColor(Color::White);
	FloatRect gb = getReady.getLocalBounds();
	getReady.setPosition({(resolutionX - gb.size.x)/2.f, 440.f});

	Clock clock;
	Clock pulseClock;
	while(window.isOpen() && clock.getElapsedTime().asSeconds() < 2.2f){
		while(const std::optional event = window.pollEvent()){
			if(event->is<Event::Closed>()){ window.close(); return; }
		}
		float t = pulseClock.getElapsedTime().asSeconds();

		// pulsing glow behind title
		float g = 1.f + 0.05f * sin(t * 3.f);
		glow.setScale({g, g});
		FloatRect gbb = glow.getLocalBounds();
		glow.setPosition({(resolutionX - gbb.size.x*g)/2.f, 230.f - 8});

		// two bees drifting in from the sides
		float off = t * 120.f;
		beeSprite.setPosition({100.f + off, 120.f});
		window.clear(Color(10, 5, 25));
		window.draw(glow);
		window.draw(title);
		window.draw(sub);
		int alpha = (int)(155 + 100 * sin(t * 4.f));
		getReady.setFillColor(Color(255,255,255,alpha));
		window.draw(getReady);
		window.draw(beeSprite);
		beeSprite.setPosition({resolutionX - 160.f - off, 120.f});
		window.draw(beeSprite);
		window.display();
	}
}

/////////////////////////////////////////////// WELCOME SCREEN //////////////////////////////////////////////////
// A neon-retro title screen: dark background, glowing "BUZZ BOMBERS" title, a bee mascot,
// and a pulsing "PRESS ENTER" prompt.
void showWelcomeScreen(RenderWindow& window){
	Font font;
	if(!font.openFromFile("resources/fonts/OpenSans-ExtraBold.ttf")){
		cout << "Error: could not load font" << endl;
	}

	// Bee mascot decoration
	Texture beeTex;
	beeTex.loadFromFile("resources/Textures/Regular_bee.png");
	Sprite beeSprite(beeTex);
	beeSprite.setTextureRect(IntRect({0, 0}, {boxPixelsX, boxPixelsY}));
	beeSprite.setScale({3.f, 3.f});
	beeSprite.setPosition({(float)(resolutionX/2 - boxPixelsX*1.5f), 120.f});

	// --- Glowing title: draw the same text several times, offset & dim, then bright on top ---
	Text titleGlow(font, "BUZZ BOMBERS", 72);
	titleGlow.setFillColor(Color(255, 0, 150, 60));   // pink glow, translucent
	Text title(font, "BUZZ BOMBERS", 72);
	title.setFillColor(Color(0, 255, 200));           // bright neon cyan
	title.setOutlineColor(Color(255, 0, 150));        // pink outline
	title.setOutlineThickness(3.f);
	// center it
	FloatRect tb = title.getLocalBounds();
	float titleX = (resolutionX - tb.size.x)/2.f;
	float titleY = 240.f;
	title.setPosition({titleX, titleY});

	Text subtitle(font, "The Bee-Blasting Arcade Classic", 24);
	subtitle.setFillColor(Color(255, 230, 0));        // neon yellow
	FloatRect sb = subtitle.getLocalBounds();
	subtitle.setPosition({(resolutionX - sb.size.x)/2.f, 340.f});

	Text prompt(font, "PRESS ENTER TO START", 30);
	prompt.setFillColor(Color::White);
	FloatRect pb = prompt.getLocalBounds();
	prompt.setPosition({(resolutionX - pb.size.x)/2.f, 480.f});

	Clock pulseClock;
	while(window.isOpen()){
		while(const std::optional event = window.pollEvent()){
			if(event->is<Event::Closed>()){
				window.close();
				return;
			}
			if(const auto* key = event->getIf<Event::KeyPressed>()){
				if(key->code == Keyboard::Key::Enter) return;
			}
		}

		// pulse the prompt's brightness with a sine wave
		float t = pulseClock.getElapsedTime().asSeconds();
		int alpha = (int)(155 + 100 * sin(t * 3.f));
		prompt.setFillColor(Color(255, 255, 255, alpha));

		// gentle glow scaling behind the title
		float g = 1.f + 0.04f * sin(t * 2.f);
		titleGlow.setScale({g, g});
		FloatRect gb = titleGlow.getLocalBounds();
		titleGlow.setPosition({(resolutionX - gb.size.x * g)/2.f, titleY - 6});

		window.clear(Color(10, 5, 25));   // deep purple-black
		window.draw(titleGlow);
		window.draw(title);
		window.draw(subtitle);
		window.draw(beeSprite);
		window.draw(prompt);
		window.display();
	}
}

/////////////////////////////////////////////// INSTRUCTIONS SCREEN /////////////////////////////////////////////
void showInstructionsScreen(RenderWindow& window){
	Font font;
	if(!font.openFromFile("resources/fonts/OpenSans-ExtraBold.ttf")){
		cout << "Error: could not load font" << endl;
	}

	Text heading(font, "HOW TO PLAY", 54);
	heading.setFillColor(Color(0, 255, 200));
	heading.setOutlineColor(Color(255, 0, 150));
	heading.setOutlineThickness(3.f);
	FloatRect hb = heading.getLocalBounds();
	heading.setPosition({(resolutionX - hb.size.x)/2.f, 60.f});

	// Instruction lines
	const int LINES = 6;
	std::string lines[LINES] = {
		"MOVE:    Left / Right Arrow Keys",
		"SHOOT:   Spacebar (uses spray)",
		"GOAL:    Blast the bees before they",
		"         swarm the flowers!",
		"WATCH:   You have limited spray cans.",
		"JUMP:    You can hop over flowers."
	};

	Text lineText[LINES] = {
		Text(font), Text(font), Text(font), Text(font), Text(font), Text(font)
	};
	for(int i=0; i<LINES; i++){
		lineText[i].setString(lines[i]);
		lineText[i].setCharacterSize(26);
		lineText[i].setFillColor(i % 2 == 0 ? Color(255, 230, 0) : Color(0, 220, 255));
		lineText[i].setPosition({140.f, 170.f + i * 50.f});
	}

	Text prompt(font, "PRESS ENTER TO CONTINUE", 28);
	prompt.setFillColor(Color::White);
	FloatRect pb = prompt.getLocalBounds();
	prompt.setPosition({(resolutionX - pb.size.x)/2.f, 500.f});

	Clock pulseClock;
	while(window.isOpen()){
		while(const std::optional event = window.pollEvent()){
			if(event->is<Event::Closed>()){
				window.close();
				return;
			}
			if(const auto* key = event->getIf<Event::KeyPressed>()){
				if(key->code == Keyboard::Key::Enter) return;
			}
		}

		float t = pulseClock.getElapsedTime().asSeconds();
		int alpha = (int)(155 + 100 * sin(t * 3.f));
		prompt.setFillColor(Color(255, 255, 255, alpha));

		window.clear(Color(10, 5, 25));
		window.draw(heading);
		for(int i=0; i<LINES; i++) window.draw(lineText[i]);
		window.draw(prompt);
		window.display();
	}
}


int showMenu(sf::RenderWindow& window){


const int menuOptions=5;
std::string options[menuOptions]={
	"1.Start Level 1",
	"2.Start Level 2",
	"3.Start Level 3",
	"4.View High Scores",
	"5. EXIT"
};	
	
	
	
	 Font font;
	font.openFromFile("resources/fonts/OpenSans-ExtraBold.ttf");
	
	Text menuText[menuOptions] = { Text(font), Text(font), Text(font), Text(font), Text(font) };
	for(int i=0; i< menuOptions;i++){
	
	menuText[i].setString(options[i]);
	menuText[i].setCharacterSize(32);
	FloatRect mb = menuText[i].getLocalBounds();
	menuText[i].setPosition({(resolutionX - mb.size.x)/2.f, (float)(200+i*60)});
	}

	// Neon menu title
	Text menuTitle(font, "BUZZ BOMBERS", 60);
	menuTitle.setFillColor(Color(0, 255, 200));
	menuTitle.setOutlineColor(Color(255, 0, 150));
	menuTitle.setOutlineThickness(3.f);
	FloatRect mtb = menuTitle.getLocalBounds();
	menuTitle.setPosition({(resolutionX - mtb.size.x)/2.f, 70.f});

	Text hint(font, "Use UP / DOWN arrows, ENTER to select", 20);
	hint.setFillColor(Color(150, 150, 180));
	FloatRect hnb = hint.getLocalBounds();
	hint.setPosition({(resolutionX - hnb.size.x)/2.f, 560.f});

	int currentSelection=0;
	
	while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
                return 5; // Exit option
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code==sf::Keyboard::Key::Up) {
                    currentSelection=(currentSelection - 1 + menuOptions) % menuOptions;  // Move up
                }
                if (keyPressed->code== sf::Keyboard::Key::Down) {
                    currentSelection=(currentSelection + 1) % menuOptions;  // Move down
                }
                if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    return currentSelection + 1;  // Return the selected option (1-based index)
                }
            }
        }

        // Update text colors based on the current selection
        for (int i = 0; i < menuOptions; i++) {
            if (i == currentSelection) {
                menuText[i].setFillColor(sf::Color(255, 230, 0));      // neon yellow highlight
                menuText[i].setOutlineColor(sf::Color(255, 100, 0));   // orange glow
                menuText[i].setOutlineThickness(2.f);
            } else {
                menuText[i].setFillColor(sf::Color(0, 200, 255));      // neon blue
                menuText[i].setOutlineThickness(0.f);
            }
        }

        // Draw the Menu
        window.clear(sf::Color(10, 5, 25));   // deep purple-black
        window.draw(menuTitle);
        window.draw(hint);
        for (int i = 0; i < menuOptions; i++) {
            window.draw(menuText[i]);
        }
        window.display();
    }

    return -1;  // If the loop exits, return an invalid option
	
}
	
void displayMessage(RenderWindow& window, const std:: string& message){

	Font font;
	font.openFromFile("resources/fonts/OpenSans-ExtraBold.ttf");
	
	Text text(font);
	
	text.setString(message);
	text.setCharacterSize(48);
	text.setPosition({(float)(resolutionX/2 -100), (float)(resolutionY/2-50)});	


	Clock clock;
	while(clock.getElapsedTime().asSeconds()<2){
	window.clear();
	window.draw(text);
	window.display();
	
	}
}	
		
	
////////////////////////////////////////////////FUNCTION DEFINITIONS//////////////////////////////////////////////

void loadLevel(sf::RenderWindow& window, int level, bool& levelComplete, int& score,string playerName, bool& level1, bool& level2, bool& level3){

	
	showLevelIntro(window, level);
	float player_x = (gameRows / 2) * boxPixelsX;
	float player_y =(gameColumns - 4) * boxPixelsY;
	Texture playerTexture;
	playerTexture.loadFromFile("resources/Textures/spray.png");
	Sprite playerSprite(playerTexture);
	 

	// Initializing Bullet and Bullet Sprites
	// Data for bullet / Spray pellet

	float bullet_x = player_x;
	float bullet_y = player_y;
	bool bullet_exists = false;
	Clock bulletClock;
	Texture bulletTexture;
	bulletTexture.loadFromFile("resources/Textures/bullet.png");
	Sprite bulletSprite(bulletTexture);
	bulletSprite.setScale({3.f, 3.f});
	bulletSprite.setTextureRect(sf::IntRect({0, 0}, {boxPixelsX, boxPixelsY}));

	
	//initialization of spray cans
	int spraysPerCan=56;
	int currentSprays= spraysPerCan;
	int cans=3;
	Texture cansTexture;
	cansTexture.loadFromFile("resources/Textures/spray.png");
	Sprite cansSprite(cansTexture);
	cansSprite.setScale({0.5f, 0.5f});


	//For printing no of cans
	Font font;
	if(!font.openFromFile("resources/fonts/OpenSans-ExtraBold.ttf")){
	cout<<"error loading";
	}

	Text spraysText(font);
	spraysText.setCharacterSize(16);
	spraysText.setFillColor(Color::White);
	spraysText.setPosition({10.f, 10.f});
	Text cansText(font);
	cansText.setCharacterSize(16);
	cansText.setFillColor(Color::White);
	cansText.setPosition({10.f, 40.f});
	Text scoreText(font);
	scoreText.setCharacterSize(16);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition({10.f, 70.f});

	//initialization of bees
	int bee_num=20;
	int maxActiveBees=6;
	int size=50;
	float bee_x[size];
	float bee_y[size];
	float bee_speed[size];
	bool bee_moves[size];
	bool bee_active[size]={true};
	bool firstBeeReachedBottom=false;
	int currentBeeCount=0;
	float bee_pauseTimer[size]={0};
	double pauseProb=0.000001f;
	Texture beeTexture;
	beeTexture.loadFromFile("resources/Textures/Regular_bee.png");
	Sprite beeSprite(beeTexture);
	beeSprite.setTextureRect(IntRect({0, 0}, {boxPixelsX, boxPixelsY}));
	int beesKilled=0;
	
	//initialization of fast bees
	int fastbee_num=3;
	int maxActivefastBees=3;
	float fastbee_x[size];
	float fastbee_y[size];
	float fastbee_speed[size];
	bool fastbee_moves[size];
	bool fastbee_active[size]={true};
	int currentFastBeeCount=0;
	Texture fastbeeTexture;
	fastbeeTexture.loadFromFile("resources/Textures/Fast_bee.png");
	Sprite fastbeeSprite(fastbeeTexture);
	fastbeeSprite.setTextureRect(IntRect({0, 0}, {boxPixelsX, boxPixelsY}));


	//initialization of honey combs
	int maxHoneycombs=200;
	float honeycomb_x[maxHoneycombs];
	float honeycomb_y[maxHoneycombs];
	bool activeHoneycombs[maxHoneycombs]={false};
	Texture honeycombTexture;
	honeycombTexture.loadFromFile("resources/Textures/honeycomb.png");
	Sprite honeycombSprite(honeycombTexture);
	honeycombSprite.setScale({0.9f, 0.9f});
	
	
	//initialization of red honeycombs
	int maxRedHoneycombs=200;
	float redhoneycomb_x[maxRedHoneycombs];
	float redhoneycomb_y[maxRedHoneycombs];
	bool activeRedHoneycombs[maxRedHoneycombs]={false};
	Texture redhoneycombTexture;
	redhoneycombTexture.loadFromFile("resources/Textures/honeycomb_red.png");
	Sprite redhoneycombSprite(redhoneycombTexture);
	redhoneycombSprite.setScale({0.9f, 0.9f});

	//initialization of flowers
	int maxFlowers=1000;
	float flower_x[maxFlowers];
	float flower_y[maxFlowers];
	bool flower_active[maxFlowers]={false};
	int flowercount=0;
	Texture flowerTexture;
	flowerTexture.loadFromFile("resources/Textures/obstacles.png");
	Sprite flowerSprite(flowerTexture);
	flowerSprite.setScale({0.9f, 0.9f});

	for(int i=0; i<maxFlowers;i++){
	
	flower_x[i]=-50;
	flower_y[i]=-50;
	flower_active[i]=false;
	
	}
	
	//centre flower
	int maxCenterFlowers=10000;
	float centerflower_x[maxCenterFlowers];
	float centerflower_y[maxCenterFlowers];
	bool centerflower_active[maxCenterFlowers]={false};
	int centerflowercount=0;
	Texture centerflowerTexture;
	Sprite centerflowerSprite(flowerTexture);
	centerflowerTexture.loadFromFile("resources/Textures/obstacles.png");
	centerflowerSprite.setScale({0.9f, 0.9f});

	for(int i=0; i<maxCenterFlowers;i++){
	
	centerflower_x[i]=-50;
	centerflower_y[i]=-50;
	centerflower_active[i]=false;
	
	}
	
	//Initialization of Humming bird
	float hummingbird_x= resolutionX/2;
	float hummingbird_y= resolutionY -100;
	bool isSick=false;
	int hitCount=0;
	float moveSpeed=3.0f;
	float pauseTimer=0.0f;
	bool isPaused=false;
	bool birdExist=true;
	bool isMovingRight=true;
	bool isMovingUp=true;	
	Texture hummingbirdTexture;
	hummingbirdTexture.loadFromFile("resources/Textures/bird.png");
	Sprite hummingbirdSprite(hummingbirdTexture);
	hummingbirdSprite.setScale({0.9f, 0.9f});

	
	// The ground on which player moves
	RectangleShape groundRectangle(Vector2f(960, 64));
	groundRectangle.setPosition({0.f, (float)((gameColumns - 2) * boxPixelsY)});
	groundRectangle.setFillColor(Color::Green);
	
////////////////////////////////////////////LEVEL 1//////////////////////////////////////////////////////////////	
	
	if(level==1){
	
	 
	
	 bee_num=20;
	 maxActiveBees=6;
	 maxHoneycombs=200;
	 levelComplete=false;
	 int spraysPerCan=56;
	int currentSprays= spraysPerCan;
	int cans=3;
	
	 //bees intialization
	for(int i=0; i<bee_num;i++){
	bee_x[i]=-100000000;
	bee_y[i]=-100000000;
	bee_speed[i]=4.0f+static_cast<float>(rand()%3);
	bee_moves[i]= i%2==0;
	bee_active[i]=false;
	}
	
	//yellow honey combs
	for(int i=0;i<maxHoneycombs;i++){
	activeHoneycombs[i]=false;
	honeycomb_x[i]=-100000000;
	honeycomb_y[i]=-100000000;
	}
	
	 //initializing 3 honeycombs
	int count=0;
	for(int i=0;i<3;i++){
	float x=rand()%(resolutionX - boxPixelsX);
	float y=rand()%resolutionY/2;
	
	bool isOccupied=false;
	for(int j=0; j<count; j++){
	    if(x==honeycomb_x[j]&& y==honeycomb_y[j]){
		isOccupied=true;
		break;
	
	    }
	}
		if(!isOccupied){
		honeycomb_x[count]=x;
		honeycomb_y[count]=y;
		activeHoneycombs[count]=true;
		count++;
		}
		else {
		 i--;
		 }
	 }
	
	while (!levelComplete && window.isOpen()) {
	Clock clock;
	while (const std::optional event = window.pollEvent()) {
		if (event->is<Event::Closed>()) {
		 window.close();
		}
	}
	if(Keyboard::isKeyPressed(Keyboard::Key::Left)&& player_x>0){
  	float next_x=player_x-15;
  	movePlayer(player_x,player_y,flower_x,flower_y,flower_active,flowercount,next_x,centerflower_x, centerflower_y,centerflower_active,centerflowercount);
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Key::Right)&& player_x< resolutionX- boxPixelsX){
	   float next_x=player_x+15;
	   movePlayer(player_x,player_y,flower_x,flower_y,flower_active,flowercount,next_x,centerflower_x, centerflower_y,centerflower_active,centerflowercount);
	}
        
        //Bullet launch
	if(Keyboard::isKeyPressed(Keyboard::Key::Space) && !bullet_exists){
		if(useSpray(currentSprays,cans,spraysPerCan)){
		bullet_exists=true;
		bullet_x=player_x+15;
		bullet_y=player_y+8;
		}
	}

	if (bullet_exists == true){
	moveBullet(bullet_y, bullet_exists, bulletClock);
	drawBullet(window, bullet_x, bullet_y, bulletSprite);
	}
	else
	{
	bullet_x = player_x;
	bullet_y = player_y;
	}


	//updating spray and can numbers
	updateText(spraysText, cansText, scoreText, currentSprays,cans,score);

	window.draw(spraysText);
	window.draw(cansText);
	window.draw(scoreText);

	for(int i=0; i<maxActiveBees; i++)
	{
		if(!bee_active[i] && currentBeeCount <bee_num)
		{	
		newBee(bee_x,bee_y,bee_active,bee_num,currentBeeCount);
		}
	}
	
	for(int i=0; i<maxActiveBees; i++){
	if(currentBeeCount==maxActiveBees && !bee_active[i]){ 
	bee_x[i]=-100000;
	bee_y[i]=-100000;
		}
	}
	// bee movement
	moveBee(bee_x,bee_y,bee_moves,bee_speed,bee_num,activeHoneycombs,  honeycomb_x, honeycomb_y, maxHoneycombs,bee_active,flower_x,flower_y,flower_active,flowercount,maxFlowers,firstBeeReachedBottom, bee_pauseTimer,pauseProb,centerflower_x,centerflower_y,centerflower_active,centerflowercount,maxCenterFlowers, activeRedHoneycombs,redhoneycomb_x,redhoneycomb_y,maxRedHoneycombs,beesKilled);
	
	for(int i=0;i<bee_num;i++){
	beeSprite.setPosition({bee_x[i], bee_y[i]});
	window.draw(beeSprite);
	}

	//check collision with bees
	for(int i=0; i<bee_num; i++){
		if(bullet_exists && checkCollision (bullet_x,bullet_y,bee_x[i],bee_y[i]))
		{
		bullet_exists=false;
		bee_active[i]=false;
		scoreForBee(score,false);
		beesKilled++;

			for(int j=0;j<maxHoneycombs;j++){
				if(!activeHoneycombs[j])
				{
				activeHoneycombs[j]=true;
				honeycomb_x[j]=bee_x[i];
				honeycomb_y[j]=bee_y[i];
				bee_x[i]=-100000;
				bee_y[i]=-100000;
				break;
    				}
    			}

 		}
	}

	for(int i=0;i<bee_num;i++){
		if(honeycomb_x[i]!=-700 &&activeHoneycombs[i]==true)
		{
		honeycombSprite.setPosition({honeycomb_x[i], honeycomb_y[i]});
		window.draw(honeycombSprite);
		}
         }


	//check collision with honeycombs
	for(int i=0;i<maxHoneycombs;i++){
		if(activeHoneycombs[i]&&bullet_exists&&
		checkCollision(bullet_x,bullet_y,honeycomb_x[i],honeycomb_y[i]))
		{
		bullet_exists=false;
		activeHoneycombs[i]=false;
		honeycomb_x[i]=-100000000;
		honeycomb_y[i]=-100000000;
		}

	}
	
	levelComplete=true;
	for(int i=0;i<bee_num;i++){
	if(bee_active[i]){
	levelComplete=false;
	}
	}
	
	if(levelComplete){
	if(cans==0 || beesKilled<20){
	levelComplete=false;
	displayMessage(window,"YOU LOSS");
	loadAndUpdateScore(playerName,score);
	score=0;
	showMenu(window);
	break;
	}else{
	displayMessage(window,"Level Complete");
	levelComplete=false;
	level1=true;
	loadLevel(window,2,levelComplete,score,playerName,level1,level2,level3);
	break;
	}
	}
	//humming bird sick
	 if(bullet_exists && checkCollision(bullet_x,  bullet_y, hummingbird_x, hummingbird_y)){
	 bullet_exists=false;
	hitHummingBird(hitCount,isSick,pauseTimer,hummingbird_x,hummingbird_y,isMovingRight,isMovingUp,birdExist,moveSpeed);
	 }
	 scoreForCans(score,cans);
	//humming bird
	 float deltaTime=clock.restart().asSeconds();
	 hummingBird(deltaTime,honeycomb_x,honeycomb_y,activeHoneycombs,
	 hummingbird_x,hummingbird_y,pauseTimer,isSick,isPaused,isMovingRight,moveSpeed,score,hummingbirdSprite,
	 maxHoneycombs,isMovingUp,birdExist,redhoneycomb_x,redhoneycomb_y,activeRedHoneycombs,maxRedHoneycombs);
	 
	drawHummingbird(window, hummingbirdSprite,birdExist);
	window.draw(groundRectangle);
	drawCenterFlowers(window,centerflowerSprite,centerflower_x,centerflower_y,centerflower_active,
	centerflowercount);
	drawFlowers(window, flowerSprite, flower_x, flower_y, flower_active,flowercount);
	drawSprayCans(window, cansSprite, cans);
	drawPlayer(window, player_x, player_y, playerSprite);
	window.display();
	window.clear();

      }
  }
  
 ///////////////////////////////LEVEL 2////////////////////////////////////////////////////////////////////////
 if(level==2){
 
 	bee_num=15;
	maxActiveBees=6; 
 	maxHoneycombs=200;
 	fastbee_num=5;
 	levelComplete=false;
 	int spraysPerCan=56;
	int currentSprays= spraysPerCan;
	int cans=3;
 	
	for(int i=0; i<bee_num;i++){
	bee_x[i]=-100000000;
	bee_y[i]=-100000000;
	bee_speed[i]=4.0f+static_cast<float>(rand()%3);
	bee_moves[i]= i%2==0;
	bee_active[i]=false;
	}
	
	//redbees initialization
	for(int i=0; i<fastbee_num;i++){
	fastbee_x[i]=-100000000;
	fastbee_y[i]=-100000000;
	fastbee_speed[i]=4.0f+static_cast<float>(rand()%3);
	fastbee_moves[i]= i%2==0;
	fastbee_active[i]=false;
	}
	
	//yellow honey combs
	for(int i=0;i<maxHoneycombs;i++){
	activeHoneycombs[i]=false;
	honeycomb_x[i]=-100000000;
	honeycomb_y[i]=-100000000;
	}
	
	 //initializing 9 honeycombs
	int count=0;
	for(int i=0;i<9;i++){
	float x=rand()%(resolutionX - boxPixelsX);
	float y=rand()%resolutionY/2;
	
	bool isOccupied=false;
	for(int j=0; j<count; j++){
	    if(x==honeycomb_x[j]&& y==honeycomb_y[j]){
		isOccupied=true;
		break;
	
	    }
	}
		if(!isOccupied){
		honeycomb_x[count]=x;
		honeycomb_y[count]=y;
		activeHoneycombs[count]=true;
		count++;
		}
		else {
		 i--;
		 }
	 }
	 
	 //red honey combs
	for(int i=0;i<maxRedHoneycombs;i++){
	activeRedHoneycombs[i]=false;
	redhoneycomb_x[i]=-100000000;
	redhoneycomb_y[i]=-100000000;
	}
	
	while (window.isOpen()) {
	Clock clock;
	while (const std::optional event = window.pollEvent()) {
		if (event->is<Event::Closed>()) {
		 window.close();
		}
	}
	if(Keyboard::isKeyPressed(Keyboard::Key::Left)&& player_x>0){
  	float next_x=player_x-15;
  	movePlayer(player_x,player_y,flower_x,flower_y,flower_active,flowercount,next_x,centerflower_x, centerflower_y,centerflower_active,centerflowercount);
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Key::Right)&& player_x< resolutionX- boxPixelsX){
	   float next_x=player_x+15;
	  movePlayer(player_x,player_y,flower_x,flower_y,flower_active,flowercount,next_x,centerflower_x, centerflower_y,centerflower_active,centerflowercount);
	}
        
        //Bullet launch
	if(Keyboard::isKeyPressed(Keyboard::Key::Space) && !bullet_exists){
		if(useSpray(currentSprays,cans,spraysPerCan)){
		bullet_exists=true;
		bullet_x=player_x+15;
		bullet_y=player_y+8;
		}
	}

	if (bullet_exists == true){
	moveBullet(bullet_y, bullet_exists, bulletClock);
	drawBullet(window, bullet_x, bullet_y, bulletSprite);
	}
	else
	{
	bullet_x = player_x;
	bullet_y = player_y;
	}


	//updating spray and can numbers
	updateText(spraysText, cansText,scoreText, currentSprays,cans,score);

	window.draw(spraysText);
	window.draw(cansText);
	window.draw(scoreText);

	for(int i=0; i<maxActiveBees; i++)
	{
		if(!bee_active[i] && currentBeeCount <bee_num)
		{	
		newBee(bee_x,bee_y,bee_active,bee_num,currentBeeCount);
		}
	}
	
	for(int i=0; i<maxActiveBees; i++){
	if(currentBeeCount==maxActiveBees && !bee_active[i]){ 
	bee_x[i]=-100000;
	bee_y[i]=-100000;
		}
	}
	// bee movement
	moveBee(bee_x,bee_y,bee_moves,bee_speed,bee_num,activeHoneycombs,  honeycomb_x, honeycomb_y, maxHoneycombs,bee_active,flower_x,flower_y,flower_active,flowercount,maxFlowers,firstBeeReachedBottom, bee_pauseTimer,pauseProb,centerflower_x,centerflower_y,centerflower_active,centerflowercount,maxCenterFlowers, activeRedHoneycombs,redhoneycomb_x,redhoneycomb_y,maxRedHoneycombs,beesKilled);
	
	for(int i=0;i<bee_num;i++){
	beeSprite.setPosition({bee_x[i], bee_y[i]});
	window.draw(beeSprite);
	}

	//check collision with bees
	for(int i=0; i<bee_num; i++){
		if(bullet_exists && checkCollision (bullet_x,bullet_y,bee_x[i],bee_y[i]))
		{
		bullet_exists=false;
		bee_active[i]=false;
		scoreForBee(score,false);
		beesKilled++;
			for(int j=0;j<maxHoneycombs;j++){
				if(!activeHoneycombs[j])
				{
				activeHoneycombs[j]=true;
				honeycomb_x[j]=bee_x[i];
				honeycomb_y[j]=bee_y[i];
				bee_x[i]=-100000;
				bee_y[i]=-100000;
				break;
    				}
    			}

 		}
	}

	for(int i=0;i<bee_num;i++){
		if(honeycomb_x[i]!=-700 &&activeHoneycombs[i]==true)
		{
		honeycombSprite.setPosition({honeycomb_x[i], honeycomb_y[i]});
		window.draw(honeycombSprite);
		}
         }


	//check collision with honeycombs
	for(int i=0;i<maxHoneycombs;i++){
		if(activeHoneycombs[i]&&bullet_exists&&
		checkCollision(bullet_x,bullet_y,honeycomb_x[i],honeycomb_y[i]))
		{
		bullet_exists=false;
		activeHoneycombs[i]=false;
		honeycomb_x[i]=-100000000;
		honeycomb_y[i]=-100000000;
		}

	}
	
	for(int i=0; i<maxActivefastBees; i++)
	{
		if(!fastbee_active[i] && currentFastBeeCount <fastbee_num)
		{	
		newBee(fastbee_x,fastbee_y,fastbee_active,fastbee_num,currentFastBeeCount);
		}
	}
	
	//red bee movement
	moveRedBee(fastbee_x,fastbee_y,fastbee_moves,fastbee_speed,fastbee_num,fastbee_active,flower_x,flower_y,
	flower_active,flowercount,maxFlowers,firstBeeReachedBottom,level,beesKilled);
	
	for(int i=0;i<fastbee_num;i++){
	if(fastbee_active[i]){
	fastbeeSprite.setPosition({fastbee_x[i], fastbee_y[i]});
	window.draw(fastbeeSprite);
	}
	}

	for(int i=0; i<fastbee_num; i++){
		if(bullet_exists && checkCollision (bullet_x,bullet_y,fastbee_x[i],fastbee_y[i]))
		{
		bullet_exists=false;
		fastbee_active[i]=false;
		scoreForBee(score,true);
		beesKilled++;

			for(int j=0;j<maxRedHoneycombs;j++){
				if(!activeRedHoneycombs[j])
				{
				activeRedHoneycombs[j]=true;
				redhoneycomb_x[j]=fastbee_x[i];
				redhoneycomb_y[j]=fastbee_y[i];
				fastbee_x[i]=-100000000;
				fastbee_y[i]=-100000000;
				break;
    				}
    			}

 		}
	}
	
	for(int i=0;i<maxRedHoneycombs;i++){
		if(activeRedHoneycombs[i]&&bullet_exists&&
		checkCollision(bullet_x,bullet_y,redhoneycomb_x[i],redhoneycomb_y[i]))
		{
		bullet_exists=false;
		activeRedHoneycombs[i]=false;
		redhoneycomb_x[i]=-100000000;
		redhoneycomb_y[i]=-100000000;
		}

	}
	
	
	for(int i=0;i<fastbee_num;i++){
		if(activeRedHoneycombs[i]==true)
		{
		redhoneycombSprite.setPosition({redhoneycomb_x[i], redhoneycomb_y[i]});
		window.draw(redhoneycombSprite);
		}
         }
         
         
         
	scoreForCans(score,cans);
	
	//humming bird sick
	 if(bullet_exists && checkCollision(bullet_x,  bullet_y, hummingbird_x, hummingbird_y)){
	 bullet_exists=false;
	hitHummingBird(hitCount,isSick,pauseTimer,hummingbird_x,hummingbird_y,isMovingRight,isMovingUp,birdExist,moveSpeed);
	 }
	 levelComplete=true;
	for(int i=0;i<bee_num;i++){
	if(bee_active[i]||fastbee_active[i]){
	levelComplete=false;
	}
	}
	
	if(levelComplete){
	if(cans==0 || beesKilled<20){
	levelComplete=false;
	displayMessage(window,"YOU LOSS");
	loadAndUpdateScore(playerName,score);
	score=0;
	showMenu(window);
	break;
	}else{
	displayMessage(window,"Level Complete");
	levelComplete=false;
	level2=true;
	loadLevel(window,3,levelComplete,score,playerName,level1,level2,level3);
	break;
	}
	}
	//humming bird
	 float deltaTime=clock.restart().asSeconds();
	 hummingBird(deltaTime,honeycomb_x,honeycomb_y,activeHoneycombs,
	 hummingbird_x,hummingbird_y,pauseTimer,isSick,isPaused,isMovingRight,moveSpeed,score,hummingbirdSprite,
	 maxHoneycombs,isMovingUp,birdExist,redhoneycomb_x,redhoneycomb_y,activeRedHoneycombs,maxRedHoneycombs);
	 
	drawHummingbird(window, hummingbirdSprite,birdExist);
	
	levelComplete=allBeesInactive(bee_active,bee_num,fastbee_active,fastbee_num);
	window.draw(groundRectangle);
	drawCenterFlowers(window,centerflowerSprite,centerflower_x,centerflower_y,centerflower_active,
	centerflowercount);
	drawFlowers(window, flowerSprite, flower_x, flower_y, flower_active,flowercount);
	drawSprayCans(window, cansSprite, cans);
	drawPlayer(window, player_x, player_y, playerSprite);
	window.display();
	window.clear(); 
 		} 
	}

/////////////////////////////////////////////LEVEL 3////////////////////////////////////////////////////////////	
if(level==3){
 
 	bee_num=20;
	maxActiveBees=6; 
 	maxHoneycombs=200;
 	fastbee_num=10;
 	levelComplete=false;
 	int spraysPerCan=56;
	int currentSprays= spraysPerCan;
	int cans=3;
 	
	for(int i=0; i<bee_num;i++){
	bee_x[i]=-100000000;
	bee_y[i]=-100000000;
	bee_speed[i]=4.0f+static_cast<float>(rand()%3);
	bee_moves[i]= i%2==0;
	bee_active[i]=false;
	}
	
	//redbees initialization
	for(int i=0; i<fastbee_num;i++){
	fastbee_x[i]=-100000000;
	fastbee_y[i]=-100000000;
	fastbee_speed[i]=4.0f+static_cast<float>(rand()%3);
	fastbee_moves[i]= i%2==0;
	fastbee_active[i]=false;
	}
	
	//yellow honey combs
	for(int i=0;i<maxHoneycombs;i++){
	activeHoneycombs[i]=false;
	honeycomb_x[i]=-100000000;
	honeycomb_y[i]=-100000000;
	}
	
	 //initializing 15 honeycombs
	int count=0;
	for(int i=0;i<15;i++){
	float x=rand()%(resolutionX - boxPixelsX);
	float y=rand()%resolutionY/2;
	
	bool isOccupied=false;
	for(int j=0; j<count; j++){
	    if(x==honeycomb_x[j]&& y==honeycomb_y[j]){
		isOccupied=true;
		break;
	
	    }
	}
		if(!isOccupied){
		honeycomb_x[count]=x;
		honeycomb_y[count]=y;
		activeHoneycombs[count]=true;
		count++;
		}
		else {
		 i--;
		 }
	 }
	 
	 //red honey combs
	for(int i=0;i<maxRedHoneycombs;i++){
	activeRedHoneycombs[i]=false;
	redhoneycomb_x[i]=-100000000;
	redhoneycomb_y[i]=-100000000;
	}
	
	while (window.isOpen()) {
	Clock clock;
	while (const std::optional event = window.pollEvent()) {
		if (event->is<Event::Closed>()) {
		 window.close();
		}
	}
	if(Keyboard::isKeyPressed(Keyboard::Key::Left)&& player_x>0){
  	float next_x=player_x-15;
  	movePlayer(player_x,player_y,flower_x,flower_y,flower_active,flowercount,next_x,centerflower_x, centerflower_y,centerflower_active,centerflowercount);
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Key::Right)&& player_x< resolutionX- boxPixelsX){
	   float next_x=player_x+15;
	   movePlayer(player_x,player_y,flower_x,flower_y,flower_active,flowercount,next_x,centerflower_x, centerflower_y,centerflower_active,centerflowercount);
	}
        
        //Bullet launch
	if(Keyboard::isKeyPressed(Keyboard::Key::Space) && !bullet_exists){
		if(useSpray(currentSprays,cans,spraysPerCan)){
		bullet_exists=true;
		bullet_x=player_x+15;
		bullet_y=player_y+8;
		}
	}

	if (bullet_exists == true){
	moveBullet(bullet_y, bullet_exists, bulletClock);
	drawBullet(window, bullet_x, bullet_y, bulletSprite);
	}
	else
	{
	bullet_x = player_x;
	bullet_y = player_y;
	}


	//updating spray and can numbers
	updateText(spraysText, cansText, scoreText, currentSprays,cans, score);

	window.draw(spraysText);
	window.draw(cansText);
	window.draw(scoreText);

	for(int i=0; i<maxActiveBees; i++)
	{
		if(!bee_active[i] && currentBeeCount <bee_num)
		{	
		newBee(bee_x,bee_y,bee_active,bee_num,currentBeeCount);
		}
	}
	
	for(int i=0; i<maxActiveBees; i++){
	if(currentBeeCount==maxActiveBees && !bee_active[i]){ 
	bee_x[i]=-100000;
	bee_y[i]=-100000;
		}
	}
	// bee movement
	moveBee(bee_x,bee_y,bee_moves,bee_speed,bee_num,activeHoneycombs,  honeycomb_x, honeycomb_y, maxHoneycombs,bee_active,flower_x,flower_y,flower_active,flowercount,maxFlowers,firstBeeReachedBottom, bee_pauseTimer,pauseProb,centerflower_x,centerflower_y,centerflower_active,centerflowercount,maxCenterFlowers, activeRedHoneycombs,redhoneycomb_x,redhoneycomb_y,maxRedHoneycombs,beesKilled);
	
	for(int i=0;i<bee_num;i++){
	beeSprite.setPosition({bee_x[i], bee_y[i]});
	window.draw(beeSprite);
	}

	//check collision with bees
	for(int i=0; i<bee_num; i++){
		if(bullet_exists && checkCollision (bullet_x,bullet_y,bee_x[i],bee_y[i]))
		{
		
		bullet_exists=false;
		bee_active[i]=false;
		scoreForBee(score,false);
		beesKilled++;

			for(int j=0;j<maxHoneycombs;j++){
				if(!activeHoneycombs[j])
				{
				activeHoneycombs[j]=true;
				honeycomb_x[j]=bee_x[i];
				honeycomb_y[j]=bee_y[i];
				bee_x[i]=-100000;
				bee_y[i]=-100000;
				break;
    				}
    			}

 		}
	}

	for(int i=0;i<bee_num;i++){
		if(honeycomb_x[i]!=-700 &&activeHoneycombs[i]==true)
		{
		honeycombSprite.setPosition({honeycomb_x[i], honeycomb_y[i]});
		window.draw(honeycombSprite);
		}
         }


	//check collision with honeycombs
	for(int i=0;i<maxHoneycombs;i++){
		if(activeHoneycombs[i]&&bullet_exists&&
		checkCollision(bullet_x,bullet_y,honeycomb_x[i],honeycomb_y[i]))
		{
		bullet_exists=false;
		activeHoneycombs[i]=false;
		honeycomb_x[i]=-100000000;
		honeycomb_y[i]=-100000000;
		}

	}
	
	for(int i=0; i<maxActivefastBees; i++)
	{
		if(!fastbee_active[i] && currentFastBeeCount <fastbee_num)
		{	
		newBee(fastbee_x,fastbee_y,fastbee_active,fastbee_num,currentFastBeeCount);
		}
	}
	
	//red bee movement
	moveRedBee(fastbee_x,fastbee_y,fastbee_moves,fastbee_speed,fastbee_num,fastbee_active,flower_x,flower_y,
	flower_active,flowercount,maxFlowers,firstBeeReachedBottom,level,beesKilled);
	
	for(int i=0;i<fastbee_num;i++){
	if(fastbee_active[i]){
	fastbeeSprite.setPosition({fastbee_x[i], fastbee_y[i]});
	window.draw(fastbeeSprite);
	}
	}

	for(int i=0; i<fastbee_num; i++){
		if(bullet_exists && checkCollision (bullet_x,bullet_y,fastbee_x[i],fastbee_y[i]))
		{
		bullet_exists=false;
		fastbee_active[i]=false;
		scoreForBee(score,true);
		beesKilled++;

			for(int j=0;j<maxRedHoneycombs;j++){
				if(!activeRedHoneycombs[j])
				{
				activeRedHoneycombs[j]=true;
				redhoneycomb_x[j]=fastbee_x[i];
				redhoneycomb_y[j]=fastbee_y[i];
				fastbee_x[i]=-100000000;
				fastbee_y[i]=-100000000;
				break;
    				}
    			}

 		}
	}
	
	for(int i=0;i<maxRedHoneycombs;i++){
		if(activeRedHoneycombs[i]&&bullet_exists&&
		checkCollision(bullet_x,bullet_y,redhoneycomb_x[i],redhoneycomb_y[i]))
		{
		bullet_exists=false;
		activeRedHoneycombs[i]=false;
		redhoneycomb_x[i]=-100000000;
		redhoneycomb_y[i]=-100000000;
		}

	}
	
	
	for(int i=0;i<fastbee_num;i++){
		if(activeRedHoneycombs[i]==true)
		{
		redhoneycombSprite.setPosition({redhoneycomb_x[i], redhoneycomb_y[i]});
		window.draw(redhoneycombSprite);
		}
         }
         
         
         
	
	scoreForCans(score,cans);
	//humming bird sick
	 if(bullet_exists && checkCollision(bullet_x,  bullet_y, hummingbird_x, hummingbird_y)){
	 bullet_exists=false;
	hitHummingBird(hitCount,isSick,pauseTimer,hummingbird_x,hummingbird_y,isMovingRight,isMovingUp,birdExist,moveSpeed);
	 }
	 
	//humming bird
	 float deltaTime=clock.restart().asSeconds();
	 hummingBird(deltaTime,honeycomb_x,honeycomb_y,activeHoneycombs,
	 hummingbird_x,hummingbird_y,pauseTimer,isSick,isPaused,isMovingRight,moveSpeed,score,hummingbirdSprite,
	 maxHoneycombs,isMovingUp,birdExist,redhoneycomb_x,redhoneycomb_y,activeRedHoneycombs,maxRedHoneycombs);
	 levelComplete=true;
	 checkLevel(cans,level,flower_x,flower_active,flowercount,maxFlowers,centerflower_x, centerflower_active,centerflowercount,maxCenterFlowers,flower_y,centerflower_y,player_x,player_y);
	for(int i=0;i<bee_num;i++){
	if(bee_active[i]||fastbee_active[i]){
	levelComplete=false;
	}
	}
	
	if(levelComplete){
	if(cans==0 || beesKilled<30){
	levelComplete=false;
	displayMessage(window,"YOU LOSS");
	loadAndUpdateScore(playerName,score);
	score=0;
	showMenu(window);
	break;
	}else{
	displayMessage(window,"Level Complete");
	loadAndUpdateScore(playerName,score);
	score=0;
	level3=true;
	levelComplete=false;
	if(level1&&level2&&level3){
	
	displayMessage(window,"YOUU WON!!");
	displayHighScore(window);
	}
	else{
	displayMessage(window,"GAME OVER!!");
	displayHighScore(window);
	}
	break;
	}
	}
	
	 
	drawHummingbird(window, hummingbirdSprite,birdExist);
	
	levelComplete=allBeesInactive(bee_active,bee_num,fastbee_active,fastbee_num);
	window.draw(groundRectangle);
	drawCenterFlowers(window,centerflowerSprite,centerflower_x,centerflower_y,centerflower_active,
	centerflowercount);
	drawFlowers(window, flowerSprite, flower_x, flower_y, flower_active,flowercount);
	drawSprayCans(window, cansSprite, cans);
	drawPlayer(window, player_x, player_y, playerSprite);
	window.display();
	window.clear(); 
 		} 
	}
}

//////////////////////////OTHER FUNCTION DEFINITIONS////////////////////////////////////////////////////////////


	
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) {
playerSprite.setPosition({player_x, player_y});
window.draw(playerSprite);
}

void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
	return;

	bulletClock.restart();
	bullet_y -= 20;
		if (bullet_y < -32){
		bullet_exists = false;
		}
}


void drawBullet(sf::RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
bulletSprite.setPosition({bullet_x, bullet_y});
window.draw(bulletSprite);
}

bool checkCollision(float bullet_x,float bullet_y, float bee_x, float bee_y)
{

        if(bullet_x+boxPixelsX>bee_x&& bullet_x < bee_x+ boxPixelsX && bullet_y+
boxPixelsY> bee_y&& bullet_y<bee_y+boxPixelsY){
	return true;}
else
	return false;

}


void newBee(float bee_x[],float bee_y[], bool bee_active[], int bee_num,int& currentBeeCount)
{
	for(int i=0;i<bee_num; i++)
	{
		if(!bee_active[i]){

			if(currentBeeCount%2==0)
			bee_x[i]=0;
			else
			bee_x[i]=resolutionX-boxPixelsX;
			bee_y[i]=0;
			bee_active[i]=true;
			currentBeeCount++;
			break;
 		 }
	}
}


void moveBee(float bee_x[],float bee_y[],bool bee_moves[], float bee_speed[],int bee_num,bool activeHoneycombs[], float honeycomb_x[],float honeycomb_y[],int maxHoneycombs,bool bee_active[],float flower_x[], float flower_y[], bool flower_active[],int& flowercount, int maxFlowers, bool& firstBeeReachedBottom, float bee_pauseTimer[], double pauseProb,float centerflower_x[], float centerflower_y[], bool centerflower_active[],int& centerflowercount, int maxCenterFlowers,bool activeRedHoneycombs[], float redhoneycomb_x[],float redhoneycomb_y[],int maxRedHoneycombs, int& beesKilled)
{
for(int i=0; i<bee_num;i++){
	if(!bee_active[i]) continue;
	
	
	
	if(rand()%100< pauseProb*100){
	if(bee_pauseTimer[i]<=0){
	bee_pauseTimer[i]=(rand()%6+5)/10.0f;
	
	}
	}
	
	
	if(bee_pauseTimer[i]>0){
	bee_pauseTimer[i]-=0.1f;
	continue;
	}
		if(bee_moves[i])
		{
		bee_x[i]+=bee_speed[i];
   			if(bee_x[i]>=resolutionX- boxPixelsX){
   			bee_moves[i]=false;
   			bee_y[i]+=boxPixelsY;
   		 	}
   		}
   		else
   		{
   		bee_x[i]-=bee_speed[i];
   			if(bee_x[i]<=0){
   			bee_moves[i]=true;
      			bee_y[i]+=boxPixelsY;
    			}  
    		 }
     
     	//if(bee_x[i]>=(resolutionX/2-boxPixelsX)&&bee_x[i]<=resolutionX/2&&
     	//bee_y[i]>=(resolutionY/2-boxPixelsY)&&bee_y[i]<=resolutionY/2){
     	
     // Centerflowers removed: no longer spawn a flower when a bee crosses the centre.
     // if(bee_x[i]==resolutionX/2&&bee_y[i]==resolutionY/2){ createCenterFlower(...); }
     
     	if(bee_y[i]>=resolutionY-2*boxPixelsY){
     	bee_active[i]=false;
     	createFlower(flower_x,flower_y,flower_active,flowercount,maxFlowers,firstBeeReachedBottom, bee_active,bee_x[i], bee_num,bee_y[i]);

     	
     	}
     	
     
     for(int j=0;j<maxHoneycombs;j++)
     {
     	if(activeHoneycombs[j]&&checkCollision(bee_x[i],bee_y[i],honeycomb_x[j],honeycomb_y[j]))
     	{
     	bee_moves[i]=!bee_moves[i];
     	bee_y[i]+=boxPixelsY;
     	break;
       }
     }
     for(int j=0;j<maxRedHoneycombs;j++)
     {
     	if(activeRedHoneycombs[j]&&checkCollision(bee_x[i],bee_y[i],redhoneycomb_x[j],redhoneycomb_y[j]))
     	{
     	bee_moves[i]=!bee_moves[i];
     	bee_y[i]+=boxPixelsY;
     	break;
       }
     }
   }
}

void moveRedBee(float fastbee_x[],float fastbee_y[],bool fastbee_moves[], float fastbee_speed[],int fastbee_num,bool fastbee_active[],float flower_x[], float flower_y[], bool flower_active[],int& flowercount, int maxFlowers, bool& firstBeeReachedBottom,int level, int &beesKilled)
{
for(int i=0; i<fastbee_num;i++){
	if(!fastbee_active[i]) continue;
	
	
		if(fastbee_moves[i])
		{
		fastbee_x[i]+=fastbee_speed[i];
   			if(fastbee_x[i]>=resolutionX- boxPixelsX){
   			fastbee_moves[i]=false;
   			fastbee_y[i]+=boxPixelsY;
   		 	}
   		}
   		else
   		{
   		fastbee_x[i]-=fastbee_speed[i];
   			if(fastbee_x[i]<=0){
   			fastbee_moves[i]=true;
      			fastbee_y[i]+=boxPixelsY;
    			}  
    		 }
   
     	if(fastbee_y[i]>=resolutionY-2*boxPixelsY && level!=3){
     	fastbee_active[i]=false;
     	createFlower(flower_x,flower_y,flower_active,flowercount,maxFlowers,firstBeeReachedBottom, fastbee_active,fastbee_x[i], fastbee_num,fastbee_y[i]);
     	}
     	
   
   }
}

void createCenterFlower(float centerflower_x[], float centerflower_y[], bool centerflower_active[],int& centerflowercount, int maxCenterFlowers, bool &firstBeeReachedBottom, bool bee_active[], float bee_x,int bee_num,float bee_y){
{
	if(centerflowercount>=maxCenterFlowers) return; // Avoid exceeding max flowers
    
    
    	centerflower_x[centerflowercount]=resolutionX/2 -boxPixelsX/2; // Rightmost, 2nd flower
        centerflower_y[centerflowercount]=resolutionY-3*boxPixelsY;
        centerflower_active[centerflowercount]=true;
        centerflowercount++;
    
    
    }


}

void createFlower(float flower_x[], float flower_y[], bool flower_active[],int& flowercount, int maxFlowers, bool &firstBeeReachedBottom, bool bee_active[], float bee_x,int bee_num,float bee_y)
{

	if(flowercount>=maxFlowers) return;
    
      if(!firstBeeReachedBottom) {

        flower_x[flowercount]=0; 
        flower_y[flowercount]=resolutionY-3*boxPixelsY;
        flower_active[flowercount]=true;
        flowercount++;

        flower_x[flowercount]=boxPixelsX; 
        flower_y[flowercount]=resolutionY-3*boxPixelsY;
        flower_active[flowercount]=true;
        flowercount++;

        flower_x[flowercount]=resolutionX-boxPixelsX;
        flower_y[flowercount]=resolutionY-3*boxPixelsY;
        flower_active[flowercount]=true;
        flowercount++;

        flower_x[flowercount]=resolutionX-2 * boxPixelsX;
        flower_y[flowercount]=resolutionY-3*boxPixelsY;
        flower_active[flowercount]=true;
        flowercount++;

        firstBeeReachedBottom=true;
    }
        
else {
	if(bee_x<resolutionX/2){
	
	float maxLeftX=-1;
        for(int i=0;i<flowercount;i++){
        
            if(flower_active[i]&&flower_x[i]<resolutionX/2){
		
		if(flower_x[i]>maxLeftX){
		maxLeftX=flower_x[i];
               		 }
                     }
                }
                
           if(maxLeftX+boxPixelsX<resolutionX/2&&flowercount<maxFlowers){
           
           flower_x[flowercount]=maxLeftX + boxPixelsX;
           flower_y[flowercount]=resolutionY-3*boxPixelsY;
           flower_active[flowercount]=true;
           flowercount++;
           	}
           }  
           
           else
           {
           
           float minRightX=resolutionX;
           
           for(int i=0;i<flowercount;i++)
           {
           if(flower_active[i] && flower_x[i]>= resolutionX/2){
           if(flower_x[i]< minRightX)
           {
           
           	minRightX=flower_x[i];
          	 }
              }
           }
           
           if(minRightX-boxPixelsX>resolutionX/2 && flowercount<maxFlowers)
           {
           flower_x[flowercount]= minRightX- boxPixelsX;
           flower_y[flowercount]= resolutionY-3*boxPixelsY;
           flower_active[flowercount]=true;
           flowercount++;
           }
        } 
      }
      
      
    
    } 
           
           
bool allBeesInactive(bool bee_active[], int bee_num, bool fastbee_active[], int fastbee_num){
	for(int i=0; i< bee_num; i++){
		if(bee_active[i]){
		return false;
		}
	}
	for(int i=0; i< fastbee_num; i++){
		if(fastbee_active[i]){
		return false;
		}
	}
	return true;

}           
           
          
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, float flower_x[], float flower_y[],bool flower_active[], int& flowercount)
{
	for(int i=0; i<flowercount;i++)
	{
		if(flower_active[i]){

		flowerSprite.setPosition({flower_x[i], flower_y[i]});
		window.draw(flowerSprite);
		}
     }
}

void drawCenterFlowers(RenderWindow& window, Sprite& centerflowerSprite, float centerflower_x[], float centerflower_y[],bool centerflower_active[], int& centerflowercount)
{
	for(int i=0; i<centerflowercount;i++)
	{
		if(centerflower_active[i]){

		centerflowerSprite.setPosition({centerflower_x[i], centerflower_y[i]});
		window.draw(centerflowerSprite);
		}
     }
}


bool useSpray(int& currentSprays, int& cans, int spraysPerCan){

  if(currentSprays>0){
  currentSprays--;
  return true;
   } 
   else if(cans>1){
   cans--;
   currentSprays= spraysPerCan-1;
   return true;
   }
   else
   return false;


}

bool sprayMovement(float next_x, float player_y, float flower_x[], float flower_y[],bool flower_active[], int flowercount,float centerflower_x[], float centerflower_y[], bool centerflower_active[],int centerflowercount){
	for(int i=0; i<flowercount; i++){
		if(flower_active[i]){
		   if(next_x+boxPixelsX>flower_x[i]&& next_x<flower_x[i]+boxPixelsX && 
		      player_y+boxPixelsY> flower_y[i] && player_y< flower_y[i]+ boxPixelsY){
		      return true;
		      }
		}
	}
	for(int i=0; i<centerflowercount; i++){
		if(centerflower_active[i]){
		   if(next_x+boxPixelsX>centerflower_x[i]&& next_x<centerflower_x[i]+boxPixelsX && 
		      player_y+boxPixelsY> centerflower_y[i] && player_y< centerflower_y[i]+ boxPixelsY){
		      return true;
		      }
		}
	}
	return false;	
}

void movePlayer(float& player_x, float& player_y, float flower_x[], float flower_y[], bool flower_active[], int flowercount, float next_x,float centerflower_x[], float centerflower_y[], bool centerflower_active[],int centerflowercount){
	// Original behaviour: flowers block movement. If nothing is in the way, move.
	if(!sprayMovement(next_x,player_y,flower_x,flower_y,flower_active,flowercount,centerflower_x,centerflower_y, centerflower_active,centerflowercount)){
		player_x=next_x;
	}
}

void updateText(Text& spraysText, Text& cansText, Text& scoreText, int currentSprays, int cans, int score){

	spraysText.setString("Sprays Remaining: "+ to_string(currentSprays));
	cansText.setString("Cans Remaining: "+ to_string(cans));
	scoreText.setString("Score: "+ to_string(score));
	}


	
void drawSprayCans(RenderWindow& window, Sprite& cansSprite, int& cans)
{
	float spacing= 40;
		for(int i=0;i<cans;i++)
		{
		cansSprite.setPosition({(float)(10+(i* spacing)), (float)(resolutionY-50)});
		window.draw(cansSprite);
		}
}

void hitHummingBird(int& hitCount, bool& isSick, float& pauseTimer, float& hummingbird_x,float& hummingbird_y, bool& isMovingRight, bool& isMovingUp, bool& birdExist,float moveSpeed){

	hitCount++;
	if(hitCount>=3){
	isSick=true;
	hitCount=0;
	}
	
	

}

int getTier(float y){

	const int tier1=2*boxPixelsY;
	const int tier2=5*boxPixelsY;
	const int tier3=14*boxPixelsY;
	
	
	if(y<tier1) return 1;
	if(y<tier2) return 2;
	if(y<tier3) return 3;
	return 4;
	}


int scoreForCombs(int& score, float y,bool isRed){
	int tier=getTier(y);

	if(isRed){
	
		if(tier==1) score+=2000;
		else if(tier==2) score+=1800;
		else score+=1500;
	}else{
		if(tier==1) score+=1000;
		else if (tier==2) score +=800;
		else score+=500;
	
	}
	
	return score;
}			

void scoreForBee(int& score, bool isKillerBee){

	if(isKillerBee){
	   score+=1000;
	} else {
	   score+=100;
	}
}
void hummingBird(float deltaTime, float honeycomb_x[], float honeycomb_y[],bool activeHoneycombs[], float& hummingbird_x, float& hummingbird_y, float& pauseTimer,  bool& isSick, bool& isPaused, bool& isMovingRight, float moveSpeed,int& score, Sprite& hummingbirdSprite, int maxHoneycombs, bool& isMovingUp,bool& birdExist,float redhoneycomb_x[], float redhoneycomb_y[],bool activeRedHoneycombs[],int maxRedHoneycombs)
{

	if(isSick){	
		 hummingbird_x+=(isMovingRight?moveSpeed:-moveSpeed);
		 hummingbird_y+=(isMovingUp?-moveSpeed:moveSpeed);
	
		if(hummingbird_x>resolutionX+ boxPixelsX||hummingbird_x<-boxPixelsX ||
		hummingbird_y>resolutionY+ boxPixelsY||hummingbird_y<-boxPixelsY ){
		isPaused=true;
		isSick=false;
		pauseTimer=0.0005f;
		}
	}
	 
		else if(isPaused){
		pauseTimer-=deltaTime;
			if(pauseTimer<=0){
			isPaused= false;
			if(isSick){
			hummingbird_x= resolutionX/2;
			hummingbird_y=resolutionY -100;
			isMovingRight=(rand()%2==0);
			isMovingUp=(rand()%2==0);
			}
		    }	
		}
		else{
		// --- Seek the nearest honeycomb (like the real game), with a little wander. ---
		int   targetIdx = -1;
		bool  targetRed = false;
		float bestDist  = 1e9f;
		// nearest yellow honeycomb
		for(int i=0; i<maxHoneycombs; i++){
			if(activeHoneycombs[i]){
				float dx = honeycomb_x[i]-hummingbird_x;
				float dy = honeycomb_y[i]-hummingbird_y;
				float d  = dx*dx+dy*dy;
				if(d<bestDist){ bestDist=d; targetIdx=i; targetRed=false; }
			}
		}
		// nearest red honeycomb (compare against the same best distance)
		for(int i=0; i<maxRedHoneycombs; i++){
			if(activeRedHoneycombs[i]){
				float dx = redhoneycomb_x[i]-hummingbird_x;
				float dy = redhoneycomb_y[i]-hummingbird_y;
				float d  = dx*dx+dy*dy;
				if(d<bestDist){ bestDist=d; targetIdx=i; targetRed=true; }
			}
		}

		if(targetIdx!=-1){
			// steer toward the target honeycomb
			float tx = targetRed ? redhoneycomb_x[targetIdx] : honeycomb_x[targetIdx];
			float ty = targetRed ? redhoneycomb_y[targetIdx] : honeycomb_y[targetIdx];
			float dx = tx - hummingbird_x;
			float dy = ty - hummingbird_y;
			float len = sqrt(dx*dx+dy*dy);
			if(len<1.f) len=1.f;
			// move toward it, plus a small random wander so it isn't a perfectly straight line
			float wanderX = ((rand()%100)-50)/100.0f;   // -0.5..0.5
			float wanderY = ((rand()%100)-50)/100.0f;
			hummingbird_x += (dx/len)*moveSpeed + wanderX*moveSpeed*0.4f;
			hummingbird_y += (dy/len)*moveSpeed + wanderY*moveSpeed*0.4f;
		} else {
			// no honeycombs around: drift in the old bouncing style
			hummingbird_x+=(isMovingRight?moveSpeed:-moveSpeed);
			hummingbird_y+=(isMovingUp?-moveSpeed:moveSpeed);
			if(hummingbird_x<=0) isMovingRight=true;
			else if(hummingbird_x>=resolutionX-boxPixelsX) isMovingRight=false;
			if(hummingbird_y<=0) isMovingUp=false;
			else if(hummingbird_y>=resolutionY-3*boxPixelsY) isMovingUp=true;
		}
		// keep the bird on-screen
		if(hummingbird_x<0) hummingbird_x=0;
		if(hummingbird_x>resolutionX-boxPixelsX) hummingbird_x=resolutionX-boxPixelsX;
		if(hummingbird_y<0) hummingbird_y=0;
		if(hummingbird_y>resolutionY-3*boxPixelsY) hummingbird_y=resolutionY-3*boxPixelsY;
		
	for(int i=0; i<maxHoneycombs; i++){
	if(activeHoneycombs[i]&& checkCollision(hummingbird_x,hummingbird_y,honeycomb_x[i],honeycomb_y[i])){
	
	activeHoneycombs[i]=false;
	scoreForCombs(score,honeycomb_y[i],false);
	   }
	}
	
	for(int i=0; i<maxRedHoneycombs; i++){
	if(activeRedHoneycombs[i]&& checkCollision(hummingbird_x,hummingbird_y,redhoneycomb_x[i],
	redhoneycomb_y[i])){
	
	activeRedHoneycombs[i]=false;
	scoreForCombs(score,honeycomb_y[i],true);
	   }
	}
	
	
	
	
	}
	hummingbirdSprite.setPosition({hummingbird_x, hummingbird_y});
	}
     
void drawHummingbird(RenderWindow &window, Sprite& hummingbirdSprite, bool birdExist){
if(birdExist){
window.draw(hummingbirdSprite);}

}

bool isRowFull(float flower_x[],bool flower_active[], int flowercount, int row, float flower_y[]){

	int startX=0;
	int endX=resolutionX;
	
	
	for(int x=startX; x<endX; x+=boxPixelsX){
	bool flowerFound=false;
	for(int i=0; i<flowercount;i++){
	if(flower_active[i]&&flower_x[i]==x &&(flower_y[i]==(resolutionY-3*boxPixelsY))){
	flowerFound=true;
	break;
	}
	}
	if(!flowerFound) return false;
	
	}

	return true;
}


void checkLevel(int &cans, int&level, float flower_x[],bool flower_active[], int& flowercount, int maxFlowers,float centerflower_x[],bool centerflower_active[], int& centerflowercount, int maxCenterFlowers,float flower_y[],float centerflower_y[],float& player_x, float& player_y){
if(isRowFull(flower_x,flower_active,flowercount, resolutionY-3*boxPixelsY,flower_y)){

if(cans>1){
	cans--;

}
flowercount=0;
level=level;
for(int i=0; i<maxFlowers;i++){
flower_active[i]=false;
flower_x[i]=-10000;
flower_y[i]=-10000;


}

centerflowercount=0;
level=level;
for(int i=0; i<maxCenterFlowers;i++){
centerflower_active[i]=false;
centerflower_x[i]=-10000;
centerflower_y[i]=-10000;
}

player_x=(gameRows/2)*boxPixelsX;
player_y=(gameColumns - 4) * boxPixelsY;
}

}

void scoreForCans(int score,int& cans){

    if (score>=20000 && score<40000){
        cans++;  
    }
    else if (score>=40000 && score<80000){
        cans++;  
    }
    else if (score>= 80000) {
        cans++;
	 if(score%80000==0){
        cans++;
    }
    }
}