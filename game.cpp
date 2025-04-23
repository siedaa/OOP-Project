#include<iostream>
#include<time.h>
using namespace std;

class Candy{

    protected:
    char symbol;

    public:
    Candy(char symbol): symbol(symbol){}
    virtual char getSymbol()const=0;

    virtual ~Candy(){}

};

class JellyBean:public Candy{

    public:
    JellyBean(): Candy('R'){}
    char getSymbol()const{
        return symbol;
    }
};
class Lozenge:public Candy{

    public:
    Lozenge(): Candy('O'){}
    char getSymbol()const{
        return symbol;
    }
};
class LemonDrop:public Candy{

    public:
    LemonDrop(): Candy('Y'){}
    char getSymbol()const{
        return symbol;
    }
};
class GumSquare:public Candy{
    public:
GumSquare(): Candy('G'){}
    char getSymbol()const{
        return symbol;
    }
};
class LollipopHead:public Candy{

    public:
    LollipopHead(): Candy('B'){}
    char getSymbol()const{
        return symbol;
    }
};
class JujubeCluster:public Candy{

    public:
    JujubeCluster(): Candy('P'){}
    char getSymbol()const{
        return symbol;
    }
};
class Board{

    Candy* board[8][8];

    public:
    Board(){
        srand(time(0));
        for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
        board[i][j]= randomCandies();
        
        }
    }
}

    Candy* randomCandies(){
    
        int random= rand()%6;
        if(random== 0){
            return new JellyBean();
        }
        else if(random== 1){
            return new Lozenge();
        }
        else if(random== 2){
            return new LemonDrop();
        }
       else if(random== 3){
            return new GumSquare();
        }
        else if(random== 4){
            return new LollipopHead();
        }
        else{
            return new JujubeCluster();
        }
    }

    void displayBoard()const{
        cout<<endl <<"      0  1  2  3  4  5  6  7 "<< endl;
        for(int i=0; i<8; i++){
            cout<<" "<< i<< " | ";
        for(int j=0;j<8;j++){
           
            cout<<" "<< board[i][j]->getSymbol()<< " ";

        }
        cout<< endl;
    
    }
    }

    int matchingCandiesAtOnce(bool&found){
        bool matchBoard[8][8];
        for(int i=0; i<8;i++){
        for(int j=0; j<8; j++){
            matchBoard[i][j]= false;
        }
        }

    found= false;
    int score= 0;  
    int countH;

    // Horizontal Match
    for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
        char symbol= board[i][j]->getSymbol();
        countH=1;
        for(int k=j+1; k<8; k++){
        if( board[i][k]->getSymbol()== symbol){
        countH++;
    }
    else{
    break;
}
        }

        if(countH>=3){
            for(int k=0; k<countH; k++){
            matchBoard[i][j+k]= true;
        }

        if(countH==3){score+=60;}
        else if(countH==4){score+=120;}
        else if(countH==5){score+=200;}
        else if(countH>=6){score+=250;}

        found= true;
        }

        j+=countH-1; // TO AVOID MULTIPLE CHECKINGS

    }

    }

    // Vertical Match
 int countV;

 for(int j=0;j<8;j++){
for(int i=0; i<8; i++){
    char symbol= board[i][j]->getSymbol();
    countV=1;

    for(int k=i+1; k<8; k++){
        if(board[k][j]->getSymbol()== symbol){
        countV++;
    }
    else{
    break;
}
    }

    if(countV>=3){
    
    for(int k=0; k<countV; k++){
    matchBoard[i+k][j]=true;
    }
    if(countV==3){score+=60;}
    else if(countV==4){score+=120;}
    else if(countV==5){score+=200;}
    else if(countV>=6){score+=250;}

    found= true;
}

    i+= countV-1;
}

}
// Refill.
for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
    if(matchBoard[i][j]){
        delete board[i][j];
        board[i][j]= randomCandies();
    }
    }
    }

return score;
}


    int matchAndClearAll(){
        bool found;
        int totalScore=0;
        do{
         int score= matchingCandiesAtOnce(found);   
            totalScore+= score;
        }
        while(found);

        return totalScore;

    }
    void swappingCandies(int r1, int c1, int r2, int c2){
        Candy *temp= board[r1][c1];
        board[r1][c1]= board[r2][c2];
        board[r2][c2]= temp;



    }

    bool validatePosition(int r, int c){
        if(r>=0 && r<8 && c>=0 && c<8){
        return true;
    }
    else{return false;}
    }

    bool adjacentCandies(int r1, int c1, int r2, int c2){
        if(r1==r2){
        if(abs(c1-c2)==1){return true;}
        }
        else if(c1==c2){
        if(abs(r1-r2)==1){return true;}
    }

        return false;
    
    }


    ~Board() {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                delete board[i][j];
            }
        }
    }
    
    };

class Player{

    protected:
    string playerName;
    int score;

    public:
    Player() :score(0){
        cout<<  "Enter your name: "<< endl;
        cin>> playerName;
    }

    void displayInfo(int moves){
        cout<< "Name: "<< playerName<< " || Moves Remaining:  "<< moves<< "  || Score: "<< score<< " "<< endl;

    }

    void updateScore(int score){
    this->score+= score;
}


void getScore(){
    cout<< "Dear, "<<playerName<< " Your Score is : "<< score<< " "<< endl;
}

};    

