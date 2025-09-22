#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class Square {
    private:
        double side;
    public:
        Square(double s): side(s){}

        double getPerimeter() {return side * 4;}
        double getArea() {return side * side;}
        double getDiagonal() {return side * sqrt(2);}
        double getSide() {return side;}

        void outputTo(){
            cout << "Квадрат" << endl;
            cout << "Сторона " << side << endl;
            cout << "Периметр " << getPerimeter() << endl;
            cout << "Площадь " << getArea() << endl;
            cout << "Диагональ " << getDiagonal() << endl;
        }
};
class Cube: public Square{
    public:
        Cube(double s) : Square(s) {} 

        double getVolume() { return getSide() * getArea(); }
        double getArea() { return 6 * getArea(); }
        double getDiagonal() { return getSide() * sqrt(3); }
        
        void outputTo() { 
            cout << "Куб" << endl;
            cout << "Сторона " << getSide() << endl;
            cout << "Площадь " << getArea() << endl;
            cout << "Диагональ " << getDiagonal() << endl;
             cout << "Объем " << getVolume() << endl;
        }
        

};

int main(){
    Square a = Square(6);
    Cube b = Cube(4);
    a.outputTo();
    b.outputTo();
}
