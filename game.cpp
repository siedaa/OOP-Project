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
