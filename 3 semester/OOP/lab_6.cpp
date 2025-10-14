#include <iostream>
#include <queue>

using namespace std;

class questionary{
    private:
        int age;
        string gender, education, answer;
    public:
        questionary() : age(0), gender(""), education(""), answer("") {}
        questionary(int _age, string _gender, string _education, string _answer) 
        : age(_age), gender(_gender), education(_education), answer(_answer) {}

        int getAge() const { return age; }
    string getGender() const { return gender; }
    string getEducation() const { return education; }
    string getAnswer() const { return answer; }
    
    void outputTo() const {
        cout << "\n" << "Возраст: " << age << endl;
        cout << "Пол: " << gender << endl;
        cout << "Образование: " << education << endl;
        cout << "Ответ: " << answer << endl;
    }
};
int main(){
    int count, count40 = 0, count30 = 0, count25 = 0;
    queue <questionary> anketa;

    cout << "Введите количество анкет" << endl;
    cin >> count;

    for(int i = 0; i < count; i++){
        int age;
        string gender, education, answer;

        cout << " Анкета №" << i + 1 << endl;
        cout << "Возраст ";
        cin >> age;
        cout << "Пол (м, ж) ";
        cin >> gender;
        cout << "Образование  (начальное, среднее, высшее) ";
        cin >> education;
        cout << "Ответ на вопрос анкеты (да, нет) ";
        cin >> answer;
        questionary human(age,gender,education,answer);
        anketa.push(human);
    }
    
    cout << "Все анкеты" << endl;
    while (!anketa.empty()){
        anketa.front().outputTo();

        if(anketa.front().getAge() > 40 && anketa.front().getGender() == "м" && anketa.front().getEducation() == "высшее" &&
         anketa.front().getAnswer() == "да") count40++;
        if(anketa.front().getAge() < 30 && anketa.front().getGender() == "ж" && anketa.front().getEducation() == "среднее" &&
         anketa.front().getAnswer() == "нет") count30++;
        if(anketa.front().getAge() < 25 && anketa.front().getGender() == "м" && anketa.front().getEducation() == "начальное" &&
         anketa.front().getAnswer() == "да") count25++;
        
        anketa.pop();
    }
    cout << "\n" << "Мужчин старше 40 лет с высшим образованием, ответивших да: " << count40 << endl;
    cout << "Женщин моложе 30 лет со средним образованием, ответивших нет: " << count30 << endl;
    cout << "Мужчин моложе 25 лет с начальным образованием, ответивших да: " << count25 << endl;

}
