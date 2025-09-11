#include <iostream>
#include <cmath>
using namespace std;

typedef struct parallelogram {
    float a, b, alpha;
} pll;

void isSquare(pll figures[],int &K1,int &K2,int &K){
    for(int i = 0; i < K;i++){
        if(figures[i].a == figures[i].b && figures[i].alpha == 90.0)
            K1++;
        if(figures[i].a != figures[i].b && figures[i].alpha == 90.0)
            K2++;
    }
}
        
int main(){
    int K, K1 = 0, K2 = 0;
    cout << "количество фигур K\n";
    cin >> K;
    pll* figures = new pll[K];
    
    for(int i = 0; i < K;i++){ 
        cout << " длина " << i+1 << " параллелограммма\n";
        cin >> figures[i].a;
        cout << "ширина " << i+1 << " параллелограммма\n";
        cin >> figures[i].b;
        cout << "угол " << i+1 << " параллелограммма\n";
        cin >> figures[i].alpha;
        
    }
    isSquare(figures,K1,K2,K);
    cout << K1 << " квадратов\n";
    cout << K2 << " прямоугольников\n";
}
