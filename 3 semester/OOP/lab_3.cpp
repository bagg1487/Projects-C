#include <iostream>
#include <fstream>
using namespace std;

class Employee {
    private:
        string surname, position;
        int birthday, experience, salary, bonus;

    public:
        Employee() : surname(""), position(""), birthday(0), experience(0), salary(0), bonus(0) {}
        Employee(string _surname, string _position, int _birthday, int _experience, int _salary) : 
            surname(_surname), position(_position), birthday(_birthday), experience(_experience), salary(_salary), bonus(0) {}
        
        string getSurname() const { return surname; }
        string getPosition() const { return position; }
        int getBirthday() const { return birthday; }
        int getExperience() const { return experience; }
        int getSalary() const { return salary; }
        int getBonus() const { return bonus; }

        void setSurname(string sur) { surname = sur; }
        void setPosition(string pos) { position = pos; }
        void setBirthday(int birth) { birthday = birth; }
        void setExperience(int exp) { experience = exp; }
        void setSalary(int sal) { salary = sal; }
        void setBonus(int bon) { bonus = bon; }
        void outputTo(){
            cout << "ФИО: " << surname << " Должность: " << position << " Др: " << birthday 
            << " Стаж: " << experience << " ЗП: " << salary << " ЗП с набавкой(если есть) " << bonus << endl;
        }

        void calculateBonus() {
            if (experience > 10) bonus = 1.1 * salary;
            else if (experience > 20) bonus = 1.15 * salary;
            else bonus = salary;
        }
        
        void writeToBinary(ofstream& binFile) {
            int size = surname.size();
            binFile.write((char*)&size, sizeof(size));
            binFile.write(surname.c_str(), size);
            
            size = position.size();
            binFile.write((char*)&size, sizeof(size));
            binFile.write(position.c_str(), size);
            
            binFile.write((char*)&birthday, sizeof(birthday));
            binFile.write((char*)&experience, sizeof(experience));
            binFile.write((char*)&salary, sizeof(salary));
            binFile.write((char*)&bonus, sizeof(bonus));
        }

        void readFromBinary(ifstream& binFile) {
            int size;
            
            binFile.read((char*)&size, sizeof(size));
            surname.resize(size);
            binFile.read(&surname[0], size);
            
            binFile.read((char*)&size, sizeof(size));
            position.resize(size);
            binFile.read(&position[0], size);
            
            binFile.read((char*)&birthday, sizeof(birthday));
            binFile.read((char*)&experience, sizeof(experience));
            binFile.read((char*)&salary, sizeof(salary));
            binFile.read((char*)&bonus, sizeof(bonus));
        }
};

int main(){
    ifstream textFile("employers.txt");
    ofstream binFile("employers.bin", ios::binary);

    int count;
    textFile >> count;
    textFile.ignore(); 
    binFile.write((char*)&count, sizeof(count));
    
    cout << "------------ До записи в бинарник ------------" << endl;

    for(int i = 0; i < count; i++){
        string surname, position;
        int birthday, experience, salary;
        getline(textFile, surname);
        getline(textFile,position);
        textFile >> birthday;
        textFile.ignore(); 
        textFile >> experience;
        textFile.ignore(); 
        textFile >> salary;
        textFile.ignore(); 

        Employee Human(surname, position, birthday, experience, salary);

        Human.outputTo();
        Human.writeToBinary(binFile);
    }
    textFile.close();
    binFile.close();

    cout << "------------ После записи в бинарник ------------" << endl;

    ifstream bin2("employers.bin", ios::binary);
    bin2.read((char*)&count, sizeof(count));
    for(int i = 0; i < count; i++) {
        Employee Human;
        Human.readFromBinary(bin2);
        Human.calculateBonus(); 
        Human.outputTo();
    }
    bin2.close();
}
