#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class SportsInstitution {
    private:
        string type; 
        string ownership;
    public:
        SportsInstitution(string t, string o) : type(t), ownership(o) {}

        string getType() const { return type; }
        string getOwnership() const { return ownership; }

};

bool compareByType(const SportsInstitution& a, const SportsInstitution& b) {
    return a.getType() < b.getType();
}

void printVector(const vector<SportsInstitution>& vector){
    for(int i = 0; i < vector.size(); i++) {
        cout << vector[i].getType() << " " << vector[i].getOwnership() << endl;
    }
}

int main(){
    int size1, size2;

    cout << "Введите размер вектора: ";
    cin >> size1;

    vector <SportsInstitution> Sports;

    cout << "Заполнение вектора" << endl;
    for(int i = 0; i < size1; i++){
        string type, ownership;
        cout << "Объект " << i + 1 << ":\n" << "  Тип учреждения: ";
        cin >> type;
        cout << "  Тип собственности: ";
        cin >> ownership;
        Sports.push_back(SportsInstitution(type, ownership));
    }
    cout << "\nПервый вектор" << endl;
    printVector(Sports);
    vector <SportsInstitution> Sports2;

    for(auto& current : Sports){
        if(current.getOwnership() == "частная") Sports2.push_back(current);
    }

    cout << "\n Второй вектор (ДО сортировки)" << endl;
    printVector(Sports2);

    if(!Sports2.empty()) sort(Sports2.begin(), Sports2.end(), compareByType);
    else return 0;

    cout << "\n Второй вектор (ПОСЛЕ сортировки)" << endl;
    printVector(Sports2);

    cout << "Введите количество полей: ";
    cin >> size2;

    cout << "\nДозаполнение вектора" << endl;
    for(int i = 0; i < size2; i++){
        string type;
        cout << "Объект " << i + 1 << ":\n" << "  Тип учреждения: ";
        cin >> type;
        SportsInstitution newInstance(type, "частная");
        auto position = lower_bound(Sports2.begin(), Sports2.end(), newInstance, compareByType);
        Sports2.insert(position, newInstance);
    }

    cout << "\nВторой вектор (после добавления)" << endl;
    printVector(Sports2);
}
