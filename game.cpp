
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include<fstream>

using namespace std;


sf::Music backgroundMusic;
bool musicEnabled = true;


bool initializeMusic() {
    if (!backgroundMusic.openFromFile("Sounds/candycrush.ogg")) {
        cout << "Failed to load background music" << endl;
        return false;
    }
    
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(30.0f); 
    
    if (musicEnabled) {
        backgroundMusic.play();
    }
    
    return true;
}


class Candy {
protected:
    char symbol;
public:
    Candy(char symbol) : symbol(symbol) {}
    virtual char getSymbol() const = 0;
    virtual ~Candy() {}
};

class JellyBean : public Candy {
public:
    JellyBean() : Candy('R') {}
    char getSymbol() const override { return symbol; }
};
class Lozenge : public Candy {
public:
    Lozenge() : Candy('O') {}
    char getSymbol() const override { return symbol; }
};
class LemonDrop : public Candy {
public:
    LemonDrop() : Candy('Y') {}
    char getSymbol() const override { return symbol; }
};
class GumSquare : public Candy {
public:
    GumSquare() : Candy('G') {}
    char getSymbol() const override { return symbol; }
};
class LollipopHead : public Candy {
public:
    LollipopHead() : Candy('B') {}
    char getSymbol() const override { return symbol; }
};
class JujubeCluster : public Candy {
public:
    JujubeCluster() : Candy('P') {}
    char getSymbol() const override { return symbol; }
};


class Board {
    Candy* board[8][8];
public:
    Board() {
        srand((time(0)));
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = randomCandies();
    }

    Candy* randomCandies() {
        int r = rand() % 6;
        switch (r) {
        case 0: return new JellyBean();
        case 1: return new Lozenge();
        case 2: return new LemonDrop();
        case 3: return new GumSquare();
        case 4: return new LollipopHead();
        default: return new JujubeCluster();
        }
    }

    char getSymbol(int i, int j) const {
        return board[i][j]->getSymbol();
    }

    bool validatePosition(int r, int c) {
        return r >= 0 && r < 8 && c >= 0 && c < 8;
    }

    bool adjacentCandies(int r1, int c1, int r2, int c2) {
        return (r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1);
    }

    void swappingCandies(int r1, int c1, int r2, int c2) {
        Candy* temp = board[r1][c1];
        board[r1][c1] = board[r2][c2];
        board[r2][c2] = temp;
    }

    int matchingCandiesAtOnce(bool& found) {
        bool matchBoard[8][8] = {};
        found = false;
        int score = 0;

        // Horizontal
        for (int i = 0; i < 8; i++) {
            int count = 1;
            for (int j = 0; j < 7; j++) {
                if (board[i][j]->getSymbol() == board[i][j + 1]->getSymbol()) {
                    count++;
                } else {

                    if (count >= 3) {
                        for (int k = 0; k < count; k++)
                            matchBoard[i][j - k] = true;
                        found = true;
                        score += (count == 3 ? 60 : count == 4 ? 120 : 200);
                    
                    }
                    count = 1;
                }
            }
            if (count >= 3) {
                for (int k = 0; k < count; k++)
                    matchBoard[i][7 - k] = true;
                found = true;
                score += (count == 3 ? 60 : count == 4 ? 120 : 200);
            }
        }

        // Vertical
        for (int j = 0; j < 8; j++) {
            int count = 1;
            for (int i = 0; i < 7; i++) {
                if (board[i][j]->getSymbol() == board[i + 1][j]->getSymbol()) {
                    count++;
                } else {
                    if (count >= 3) {
                        for (int k = 0; k < count; k++)
                            matchBoard[i - k][j] = true;
                        found = true;
                        score += (count == 3 ? 60 : count == 4 ? 120 : 200);
                    }
                    count = 1;
                }
            }
            if (count >= 3) {
                for (int k = 0; k < count; k++)
                    matchBoard[7 - k][j] = true;
                found = true;
                score += (count == 3 ? 60 : count == 4 ? 120 : 200);
            }
        }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (matchBoard[i][j]) {
                    delete board[i][j];
                    board[i][j] = randomCandies();
                }

        return score;
    }

    int matchAndClearAll() {
        bool found;
        int total = 0;
        do {
            int s = matchingCandiesAtOnce(found);
            total += s;
        } while (found);
        return total;
    }

    ~Board() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                delete board[i][j];
    }
};

class Player {
    string name;
    int score;
public:
    Player(string name) : name(name), score(0) {}
    void updateScore(int s) { score += s; }
    int getScoreVal() const { return score; }
    string getName() const { return name; }
};

class SoundEffects {
private:
    sf::SoundBuffer matchSoundBuffer;
    sf::SoundBuffer selectSoundBuffer;
    sf::SoundBuffer errorSoundBuffer;
    sf::Sound matchSound;
    sf::Sound selectSound;
    sf::Sound errorSound;
    bool enabled;

public:
    SoundEffects() : enabled(true) {
      
        if (!matchSoundBuffer.loadFromFile("Sounds/match.wav")) {
            cout << "Failed to load match sound effect" << endl;
        }
        if (!selectSoundBuffer.loadFromFile("Sounds/select.wav")) {
            cout << "Failed to load select sound effect" << endl;
        }
        if (!errorSoundBuffer.loadFromFile("Sounds/error.wav")) {
            cout << "Failed to load error sound effect" << endl;
        }

        matchSound.setBuffer(matchSoundBuffer);
        selectSound.setBuffer(selectSoundBuffer);
        errorSound.setBuffer(errorSoundBuffer);
        
        // Set volumes
        matchSound.setVolume(50.0f);
        selectSound.setVolume(40.0f);
        errorSound.setVolume(40.0f);
    }

    void playMatchSound() {
        if (enabled) matchSound.play();
    }

    void playSelectSound() {
        if (enabled) selectSound.play();
    }

    void playErrorSound() {
        if (enabled) errorSound.play();
    }

    void setEnabled(bool state) {
        enabled = state;
    }

    bool isEnabled() const {
        return enabled;
    }
};



class MainMenu{

    protected:
    
    sf::RenderWindow &window;
     int windowWidth;
     int windowHeight;
    sf::Font &font;
    sf::Font &font2;
    SoundEffects &soundFx;
    string &playerName;
    int totalMoves;
    friend class MainGame;

    void setupButton(sf::RectangleShape &button, sf::Text &text, sf::Font& font, string boxText, float px, float py, int size=24){
    
        button.setPosition(px, py);
        button.setFillColor(sf::Color(255,120,180));
        button.setOutlineThickness(3);
        button.setOutlineColor(sf::Color(200,60,120));

        text.setFont(font);
        text.setString(boxText);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);

        sf::FloatRect textBound= text.getLocalBounds();
        text.setPosition( px+ (button.getSize().x - textBound.width)/2.0f, py+(button.getSize().y -textBound.height)/2.0f-5.0f);
        
    }
bool isMouseOver(const sf::RectangleShape& shape, const sf::Vector2f& mousePos) {
    return shape.getGlobalBounds().contains(mousePos);
}
sf::Color getHoverColor(){
return sf::Color(100,50,150,220);
}

bool validatePlayerName(string name){

    if(name.length()<3 || name.length()>15){
    return false;
    }
    else{
    return true;
    }

}

public:

MainMenu(sf::RenderWindow &window, sf::Font &font, sf::Font &font2, SoundEffects &soundFx, string &playerName, bool keepFullscreen= false):window(window), font(font), font2(font2), soundFx(soundFx), playerName(playerName), totalMoves(0){

  windowWidth = keepFullscreen ? sf::VideoMode::getDesktopMode().width : 1024;
    windowHeight = keepFullscreen ? sf::VideoMode::getDesktopMode().height : 800;
    
    // Only recreate the window if not keeping fullscreen mode
    if (!keepFullscreen) {
        window.create(sf::VideoMode(windowWidth, windowHeight), "M & I Candy Pop", 
                      keepFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
        window.setFramerateLimit(120);
    }
    window.setView(sf::View(sf::FloatRect(0,0,windowWidth,windowHeight)));

}

void loadMenuGraphics(){

sf:: Texture bgtexture;
sf::Sprite background;

if(!bgtexture.loadFromFile("assets/Main menu.jpg")){
cout<< "Failed to load menu background."<<endl;
}

background.setTexture(bgtexture);
background.setScale(windowWidth/(float)bgtexture.getSize().x, windowHeight/(float)bgtexture.getSize().y);

sf::Text title("M & I CandyPop!", font, 90);
   title.setPosition((windowWidth - title.getLocalBounds().width) / 2, 80);
     title.setFillColor(sf::Color(255, 50, 120)); // Bright pink
    title.setOutlineThickness(3);
    title.setOutlineColor(sf::Color::White);


sf::RectangleShape nameBox(sf::Vector2f(500,60));
    nameBox.setPosition((windowWidth - 500) / 2, 220);
    nameBox.setFillColor(sf::Color(100, 100, 100, 200)); // Semi-transparent white
    nameBox.setOutlineThickness(2);
    nameBox.setOutlineColor(sf::Color(200, 100, 150));

 sf::Text namePrompt("Enter your name: ", font2, 20);
    namePrompt.setPosition((windowWidth - namePrompt.getLocalBounds().width) / 2, 180);
    namePrompt.setFillColor(sf::Color::White);
    namePrompt.setOutlineThickness(1);
    namePrompt.setOutlineColor(sf::Color(100, 50, 80));

sf::Text nameText("", font2, 32);
    nameText.setPosition((windowWidth - 460) / 2, 230);
    nameText.setFillColor(sf::Color(100, 50, 150)); // 
    
sf::Text errorMsg("", font2, 24);
    errorMsg.setFillColor(sf::Color(255, 50, 50)); // Red
    errorMsg.setPosition((windowWidth - 500) / 2, 290);


sf::RectangleShape easyButton(sf::Vector2f(350,70));
sf::RectangleShape mediumButton(sf::Vector2f(350,70));
sf::RectangleShape hardButton(sf::Vector2f(350,70));
sf::RectangleShape exitButton(sf::Vector2f(350,70));

sf:: Text easyText;
sf::Text mediumText;
sf::Text hardText;
sf::Text exitText;

setupButton(easyButton, easyText, font2, "Easy Mode", (windowWidth-350)/2,350,28);
setupButton(mediumButton, mediumText, font2, "Medium Mode", (windowWidth-350)/2,440,28);
setupButton(hardButton, hardText, font2, "Hard Mode", (windowWidth-350)/2,530,28);
setupButton(exitButton, exitText, font2, "Exit Game", (windowWidth-350)/2,620,28);

string nameInput="";
while(window.isOpen()){

    sf::Event ev;
    
    while(window.pollEvent(ev)){
    
      if(ev.type== sf::Event::TextEntered && nameInput.length()<15){
    
        if(ev.text.unicode==8 && !nameInput.empty()){
        
            nameInput.pop_back();
            errorMsg.setString("");
        
        }
        else if(ev.text.unicode <128 && isprint(ev.text.unicode)){
        nameInput+= static_cast<char>(ev.text.unicode);
        errorMsg.setString("");
        }
        nameText.setString(nameInput);
    
    }
   if(ev.type== sf::Event::MouseMoved){
                sf::Vector2f mousePosition(sf::Mouse :: getPosition(window));
                  hoverEffects(mousePosition, easyButton, mediumButton, hardButton, exitButton);
    }

    if(ev.type== sf::Event::MouseButtonPressed){
    sf::Vector2f pressedPosition(sf::Mouse::getPosition(window));
totalMoves= loadMoves(pressedPosition, easyButton, mediumButton, hardButton, exitButton, nameBox, nameInput, errorMsg);

if(totalMoves!=0){return;}
    }
}

window.clear(); 

window.draw(background);
window.draw(title);
window.draw(nameBox);
window.draw(namePrompt);
window.draw(nameText);
window.draw(errorMsg);

window.draw(easyButton); window.draw(easyText);
window.draw(mediumButton);
window.draw(mediumText);
window.draw(hardButton);
window.draw(hardText);
window.draw(exitButton);
window.draw(exitText);

window.display();
}


}


 void hoverEffects( sf::Vector2f &mousePos, sf::RectangleShape &button1, sf::RectangleShape &button2, sf::RectangleShape &button3, sf::RectangleShape &button4){
    
        sf:: RectangleShape *hoverBox= nullptr;
               button1.setFillColor(sf::Color(255, 120, 180));
    button2.setFillColor(sf::Color(255, 120, 180));
    button3.setFillColor(sf::Color(255, 120, 180));
    button4.setFillColor(sf::Color(255, 120, 180));

                 if(isMouseOver(button1, mousePos)){
                button1.setFillColor((getHoverColor()));
                    hoverBox= &button1;
                }
               else if(isMouseOver(button2, mousePos)){
                button2.setFillColor((getHoverColor()));
                    hoverBox= &button2;
                }
               else if(isMouseOver(button3, mousePos)){
                button3.setFillColor((getHoverColor()));
                    hoverBox= &button3;
                }
              else  if(isMouseOver(button4, mousePos)){
                button4.setFillColor((getHoverColor()));
                    hoverBox= &button4;
                }

            }
        
        
    int loadMoves(sf::Vector2f &mousePos, sf::RectangleShape &button1, sf::RectangleShape &button2, sf::RectangleShape &button3, sf::RectangleShape &button4,sf::RectangleShape &nameBox, string nameInput, sf::Text &err){
    
        
        bool nameActive= false;

        if(isMouseOver(button1, mousePos)){
        soundFx.playSelectSound();
        if(nameInput.empty()){
         err.setString("Please enter name first!");  
         return 0; 
        }
        else if(!validatePlayerName(nameInput)){
        err.setString("Name exceeds number limit. Enter between 3-15 characters.");
        return 0;
        }
        else{
        playerName= nameInput;
        return 15;}
    
    }

        if(isMouseOver(button2, mousePos)){
        soundFx.playSelectSound();
        if(nameInput.empty()){
         err.setString("Please enter name first!");   
         return 0;
        }
        else if(!validatePlayerName(nameInput)){
        err.setString("Name exceeds number limit. Enter between 3-15 characters.");
        return 0;
        }
        else{
        playerName= nameInput;
        return 10;
        }
    }

        if(isMouseOver(button3, mousePos)){
        soundFx.playSelectSound();
        if(nameInput.empty()){
         err.setString("Please enter name first!");   
         return 0;
        }
        else if(!validatePlayerName(nameInput)){
        err.setString("Name exceeds number limit. Enter between 3-15 characters.");
        return 0;
        }
        else{
        playerName= nameInput;
        return 5;
        }
    }
        if(isMouseOver(button4, mousePos)){
        soundFx.playSelectSound();
        window.close();
        return -1;
        }

        if(isMouseOver(nameBox, mousePos)){
        nameActive= true;
        nameBox.setOutlineColor(sf::Color(100,100,150));
        nameBox.setFillColor(sf::Color( 255,255,255,200));
        }
        else{
            nameActive= false;
            nameBox.setFillColor(sf::Color(100,100,100,200));
        }

        return 0;
        
        }
    
    int getMoves(){
return totalMoves;
}

};



class MainGame{

protected:
sf::RenderWindow &window;
sf::Texture textures[6];
int windowWidth;
int windowHeight;
int tileSize;
int boardSize;
int boardPixelSize;
int boardStartX, boardStartY;
int uiHeight;
int selectedRow;
int selectedColumn;
float animationTime;
bool gameOver;
bool savedScore;

SoundEffects &soundFx;
MainMenu *menu;


public:

MainGame(sf::RenderWindow &window,  int tileSize, int boardSize, int uiHeight, SoundEffects &soundFx, MainMenu *menu): window(window), tileSize(tileSize), boardSize(boardSize), uiHeight(uiHeight), soundFx(soundFx), menu(menu){

   sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        windowWidth = desktop.width;
        windowHeight = desktop.height;
        
window.create(sf::VideoMode(windowWidth, windowHeight), "M & I CandyPop!", sf::Style::Fullscreen );
window.setView(sf::View(sf::FloatRect(0,0,windowWidth, windowHeight)));
boardPixelSize= tileSize*boardSize;
boardStartX= (windowWidth- boardPixelSize)/2;
boardStartY= (windowHeight- boardPixelSize-uiHeight)/2;

selectedRow=-1;
selectedColumn=-1;
animationTime= 0;
gameOver= false;
savedScore= false;

 for(int i=0; i<6; i++){
    if(!textures[i].loadFromFile("assets/"+to_string(i+1)+ ".png")){
    cout<< "Unable to load candies!"<<endl;
    }
    }


}

  void saveToFile(string filename, Player &player){
        ofstream file(filename, ios::app);
        if(file.is_open()){
    
        file<< player.getName()<< " | Score: "<< player.getScoreVal() <<" "<<endl;
            file.close();
        }
            else{
                cerr<<"Unable to open file!"<<endl;
            }
    
}

void playGame(sf::Vector2f &mousePos, Board &board, Player &player){

    int row= (mousePos.y - boardStartY)/tileSize;
    int col= (mousePos.x - boardStartX)/ tileSize;

    if(row>=0 && row<8 && col>=0 && col<8){
    
        if(selectedRow== -1 || selectedColumn==-1){
        selectedRow = row;
        selectedColumn= col;
        soundFx.playSelectSound();
        }
        else if(selectedRow== row && selectedColumn == col){
        selectedRow= -1; 
        selectedColumn = -1;
        soundFx.playSelectSound();
        }
        else if(board.adjacentCandies(selectedRow, selectedColumn, row, col)){
        board.swappingCandies(selectedRow,selectedColumn,row, col);

        bool found;
        int score= board.matchingCandiesAtOnce(found);
        if(found){
        player.updateScore(score);
        soundFx.playMatchSound();

        while(true){
        bool moreMatches;
        int moreScore= board.matchingCandiesAtOnce(moreMatches);
        if(!moreMatches){break;}
        player.updateScore(moreScore);
        }
        menu->totalMoves--;
        } // if found
        else{
        board.swappingCandies(row, col, selectedRow, selectedColumn);
        soundFx.playErrorSound();
        }

        selectedRow=-1, selectedColumn=-1;
    
        }
        else{
        selectedRow =row;
        selectedColumn = col;
        soundFx.playSelectSound();
    }
    
    
    }



}
void loadSprites(Board &board, sf::RectangleShape &selectedCandy){

    sf::Sprite sprites;
    char symbol;
     int index;
    for(int i =0 ; i<8; i++){
        for(int j=0 ; j<8; j++){
        symbol= board.getSymbol(i,j);
        switch(symbol){
        case 'R':
        index= 0;
        break;
        case 'O':
        index= 1;
        break;
        case 'Y':
        index= 2;
        break;
        case 'G':
        index= 3;
        break;
        case 'B':
        index= 4;
        break;
        case 'P':
        index= 5;
        break;
        default:
        index =0;
        } 
        
        sprites.setTexture(textures[index]);
        sprites.setOrigin(sprites.getTexture()->getSize().x/2.0f, sprites.getTexture()->getSize().y/2.0f);

        float cellCenterX= boardStartX + j*tileSize + tileSize/2.0f;
        float cellCenterY= boardStartY + i*tileSize + tileSize/2.0f;
        float animationOffset= sin(animationTime * 2.0f + (i*j))*1.5f;
        sprites.setPosition(cellCenterX,cellCenterY+animationOffset);

        float rotation= sin(animationTime+(i*j))* 2.0f;
        sprites.setRotation(rotation);

        float maxDimension= max(sprites.getTexture()->getSize().x, sprites.getTexture()->getSize().y);
        float scaleFactor= (tileSize*0.8f)/maxDimension;

        if(i== selectedRow && j==selectedColumn){
        float pulseScale= 1.0f+ sin(animationTime * 5.0f)* 0.1f;
        sprites.setScale(scaleFactor * pulseScale, scaleFactor*pulseScale);

        selectedCandy.setPosition(boardStartX+j*tileSize, boardStartY+i *tileSize);
        window.draw(selectedCandy);
        
        }
        else{
        sprites.setScale(scaleFactor,scaleFactor);
        }

        window.draw(sprites);
        }
    }

}
void loadGameGraphics(sf::Font &font, sf::Font font2, Board &board, Player &player){

        sf::Texture bgTexture;
        sf::Sprite background;

        if(!bgTexture.loadFromFile("assets/background.jpg")){
        
        cout<<"Unable to load background image."<<endl;
        }
        background.setTexture(bgTexture);
        background.setScale(windowWidth/(float)bgTexture.getSize().x, windowHeight/(float)bgTexture.getSize().y);


        sf::RectangleShape boardOverlay(sf::Vector2f(boardPixelSize,boardPixelSize));
        boardOverlay.setFillColor(sf::Color(255,255,255,30));
            boardOverlay.setPosition(boardStartX, boardStartY);


    sf::RectangleShape displayPanel(sf::Vector2f(windowWidth, uiHeight));
    displayPanel.setPosition(0, windowHeight-uiHeight);
    displayPanel.setFillColor(sf::Color(100,50,150,220));

    sf::Text panelText("", font2, 32);
    panelText.setFillColor(sf::Color::White);
    panelText.setOutlineThickness(1);
    panelText.setOutlineColor(sf::Color(100,0,100));
    panelText.setPosition(20, windowHeight-uiHeight+20);

    sf::RectangleShape bgGrid(sf::Vector2f(boardPixelSize,boardPixelSize));
    bgGrid.setPosition(boardStartX,boardStartY);
    bgGrid.setFillColor(sf::Color::Transparent);
    bgGrid.setOutlineThickness(3);
    bgGrid.setOutlineColor(sf::Color(255,150,200,200));


    sf::RectangleShape selectedCandy(sf::Vector2f(tileSize,tileSize));
    selectedCandy.setFillColor(sf::Color::Transparent);
    selectedCandy.setOutlineThickness(4);
    selectedCandy.setOutlineColor(sf::Color(255,255,0,200));

    sf::RectangleShape fileButton(sf::Vector2f(50,50));
    fileButton.setPosition(windowWidth-70, 20);
    fileButton.setFillColor(sf::Color(100,250,100,200));
    fileButton.setOutlineThickness(2);
    fileButton.setOutlineColor(sf::Color(255,255,255,200));

     sf::Text fileTxt("Save", font2, 15);
    sf::FloatRect fileBounds = fileTxt.getLocalBounds();
    fileTxt.setPosition(
        windowWidth - 70 + (50 - fileBounds.width) / 2.0f,
        20 + (50 - fileBounds.height) / 2.0f - 5.0f
    );
    fileTxt.setFillColor(sf::Color::White);
    
    
     sf::Text scoreMsg("Score Saved!",font2, 24);;
            scoreMsg.setFillColor(sf::Color::Green);
            scoreMsg.setPosition((windowWidth-scoreMsg.getLocalBounds().width)/2, (windowHeight+280)/2);

    
   
    
    sf::RectangleShape gameOverPopUp(sf::Vector2f(500,400));
    gameOverPopUp.setPosition((windowWidth-500)/2, (windowHeight-400)/2);
    gameOverPopUp.setFillColor(sf::Color());
    gameOverPopUp.setOutlineThickness(4);
    gameOverPopUp.setOutlineColor(sf::Color(255,60,120));

    sf::Text gameOverText("Hehe, Game Over!", font, 60);
    gameOverText.setFillColor(sf::Color(255,50,120));
    gameOverText.setOutlineThickness(2);
    gameOverText.setOutlineColor(sf::Color::White);
    sf::FloatRect textBounds= gameOverText.getLocalBounds();
    gameOverText.setPosition((windowWidth- textBounds.width)/2, (windowHeight- textBounds.height)/2-80);

    sf::Text scoreText("", font2, 40);
    scoreText.setFillColor(sf::Color::White);

    sf::RectangleShape playAgain(sf::Vector2f(300,60));
    sf::Text playAgainText;
    menu->setupButton(playAgain, playAgainText, font2, "Play Again", (windowWidth-300)/2, (windowHeight +100)/2, 28);

     sf::Clock clk;
    while (window.isOpen())
{
    
   
    float timeChange= clk.restart().asSeconds();
    animationTime+= timeChange;

    sf::Event ev;

    while (window.pollEvent(ev)){
    if(ev.type== sf::Event::Closed){window.close();}

    if(ev.type==sf::Event::MouseButtonPressed){
    sf::Vector2f mousePosition(sf::Mouse::getPosition(window));
    

      
        if(gameOver){
            if(menu->isMouseOver(playAgain, mousePosition )){
            soundFx.playSelectSound();
            return;
            }
            
            if(menu->isMouseOver(fileButton, mousePosition )){
            soundFx.playSelectSound();
            saveToFile("data.txt",player);
            savedScore= true;
           
            
        
            }
        
        }
        else{
         playGame(mousePosition, board, player);
        }

           
    }
    
        }//ev

            if(menu->totalMoves <=0 && !gameOver ){
            gameOver=true;
            scoreText.setString("Final Score: " +to_string(player.getScoreVal()));
            sf::FloatRect scoreBounds= scoreText.getLocalBounds();
            scoreText.setPosition((windowWidth-scoreBounds.width)/2, (windowHeight- scoreBounds.height)/2);

           

            

            }
            window.clear();
            window.draw(background);
            window.draw(boardOverlay);
            window.draw(bgGrid);

            loadSprites(board, selectedCandy);
            stringstream ss;
            ss<< player.getName()<< "  | Score: "<< player.getScoreVal()<< "  | Remaining Moves: "<<  menu->totalMoves;

            panelText.setString(ss.str());
            window.draw(displayPanel);
            window.draw(panelText);
            window.draw(fileButton);
            window.draw(fileTxt);

            if(gameOver){
            window.draw(gameOverPopUp);
            window.draw(gameOverText);
            window.draw(scoreText);
            window.draw(playAgain);
            window.draw(playAgainText);
            if(savedScore){
            window.draw(scoreMsg);
            }

            }

            window.display();
                

            


                }
   


}// func end



};


    
  


int main() {
    

    sf::RenderWindow window(sf::VideoMode(1024,800), "M & I Candy Pop!");
    window.setFramerateLimit(60);
    
    // Load font
    sf::Font font;
    if (!font.loadFromFile("Fonts/candy_font.ttf")) {
        cout << "Failed to load font, using system default."<<endl;
    }
    sf::Font font2;
    if(!font2.loadFromFile("Fonts/Candy Beans.otf")){
    cout<<"Failed to load font!"<< endl;
    }

    SoundEffects soundFx;

    if (!initializeMusic()) {
        cout << "Failed to initialize music system."<<endl;
      
    }
    
    
    while ( window.isOpen()) {
            string playerName;
        MainMenu menu(window, font, font2, soundFx, playerName);
        menu.loadMenuGraphics();
        int totalMoves = menu.getMoves();
        
        if (totalMoves ==-1){  
            
            break;
        }  
      
        if(totalMoves>0){
        Board board;
        Player player(playerName);
        
    
        board.matchAndClearAll();  
        player.updateScore(0); 
        
        
        MainGame game(window, 85, 8, 120, soundFx, &menu);
        game.loadGameGraphics(font, font2, board, player);


    }
}

    backgroundMusic.stop();
    
    return 0;
}