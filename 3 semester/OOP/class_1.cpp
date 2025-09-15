#include<iostream>
#include<string>
using namespace std;

class SportsInstitution {
private:
    string type; 
    string ownership;
public:
    SportsInstitution(string t, string o) : type(t), ownership(o) {}

    void outputTo() {
        cout << "Тип учреждения: " << type << endl;
        cout << "Тип собственности: " << ownership << endl;
    }

    string get_Type() {
        return type;
    }
    string get_Ownership() {
        return ownership;
    }

    void set_Type(string t) {
        type = t;
    }
    void set_Ownership(string o) {
        ownership = o;
    }
};

int main() {
    SportsInstitution gym("спортзал", "частная");
    SportsInstitution *club = new SportsInstitution("спортклуб", "муниципальная");
    
    cout << "Исходные данные:" << endl;
    club->outputTo();
    gym.outputTo();
    

    club->set_Ownership("государственная");
    gym.set_Type("спорткомплекс");
    
    cout << "\nПосле изменений:" << endl;
    cout << "Тип собственности клуба: " << club->get_Ownership() << "\n";
    cout << "Тип учреждения зала: " << gym.get_Type() << endl;
    
    cout << "\nПолная информация:" << endl;
    club->outputTo();
    gym.outputTo();
    
    delete club;
    return 0;
}
