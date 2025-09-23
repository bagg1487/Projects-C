#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class Square {
    private:
        double side;
    public:
        Square() : side(0.0) {}
        Square(double s) : side(s){}

        double getPerimeter() const {return side * 4; }
        virtual double getArea() const {return side * side; }
        double getDiagonal() const {return side * sqrt(2); }
        double getSide() const {return side; }

        virtual void outputTo() const {
            cout << "Сторона " << side << endl;
            cout << "Периметр " << getPerimeter() << endl;
            cout << "Площадь " << getArea() << endl;
            cout << "Диагональ " << getDiagonal() << "\n"<< endl;
        }
};
class Cube: public Square{
    public:
        
        Cube() : Square(0.0) {}
        Cube(double s) : Square(s) {} 

        double getVolume() const {return getSide() * getSide() * getSide(); } 
        double getArea() const override { return getSide() * getSide() * 6; }
        void outputTo() const override{
            cout << "Сторона " << getSide() << endl;
            cout << "Объем " << getVolume() << endl;
            cout << "Площадь " << getArea() << "\n" << endl;
        }  
};

int main(){
    int N1, N2;

    cout << "Введите количество квадратов" << endl;
    cin >> N1;
    cout << "Введите количество кубов" << endl;
    cin >> N2;
    
    Square* Squares = new Square[N1];
    double averageArea;
    Cube* Cubes = new Cube[N2];

    for(int i = 0; i < N1; i++){
         double side;
         cout << "Введите сторону квадрата" << endl;
         cin >> side;
         Squares[i] = Square(side);
         averageArea += Squares[i].getArea();
    }
    for(int i = 0; i < N2; i++){
         double side;
         cout << "Введите сторону куба" << endl;
         cin >> side;
         Cubes[i] = Cube(side);
    }

    for(int i = 0; i < N1; i++){
         cout << "Квадрат " << i + 1 << endl;
         Squares[i].outputTo();
    }
    for(int i = 0; i < N2; i++){
         cout << "Куб " << i + 1 << endl;
         Cubes[i].outputTo();
    }

    double maxValue = Cubes[0].getArea();
    int maxValueCount = 0;

    for (int i = 1; i < N2; i++){
        if (Cubes[i].getArea() > maxValue){
            maxValue = Cubes[i].getArea();
        } 
    }
    for (int i = 0; i < N2; i++){
        if (Cubes[i].getArea() == maxValue){
            maxValueCount++;
        } 
    }

    cout << "Средняя длина всех квадратов " << averageArea / N1 << endl;
    cout << "Количество кубов с наибольшей площадью: " << maxValueCount << " Площадь: " << maxValue << endl;
}
