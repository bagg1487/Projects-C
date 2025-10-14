#include <iostream>
#include <list>

using namespace std;

class repairHouse {
    private:
        int rooms;           
        double area;       
        string address, repairRequestDate; 
        

    public:
        repairHouse() : rooms(0), area(0.0), address(""), repairRequestDate("") {}
        repairHouse(int _rooms, double _area, string _address, string _repairRequestDate)
            : rooms(_rooms), area(_area), address(_address), repairRequestDate(_repairRequestDate) {}
        
        int getRooms() const { return rooms; }
        double getArea() const { return area; }
        string getAddress() const { return address; }
        string getRepairRequestDate() const { return repairRequestDate; }

        void getRepairCompletionDate(int& day, int& month, int& year) const {
            int ddmmgg[3] = {0}, i = 0, repairingTime = 6;
            string temp;

            for(char c : repairRequestDate){
                if (c != '.') temp += c; 
                else {
                    ddmmgg[i] = stoi(temp);
                    
                    i++;
                    temp = "";
                } 
            }
            ddmmgg[2] = stoi(temp);

            day = ddmmgg[0]; 
            month = ddmmgg[1]; 
            year = ddmmgg[2];

            if (month <= 12 - repairingTime) month += repairingTime;
            else {
                month = month - repairingTime;
                year++;
            }
        }

        string repairCompletionDate() const {
            int day, month, year;
            getRepairCompletionDate(day, month, year);
            return to_string(day) + '.' + to_string(month) + '.' + to_string(year);
        }
        

        static bool compareByCompletionDate(const repairHouse &a, const repairHouse &b){
            int a_day, a_month, a_year, b_day, b_month, b_year;

            a.getRepairCompletionDate(a_day, a_month, a_year);
            b.getRepairCompletionDate(b_day, b_month, b_year);

            if (a_year != b_year) return a_year < b_year;
            if (a_month != b_month) return a_month < b_month;
            return a_day < b_day;
        }

        void outputTo() const {
            cout << "\n" <<  "Информация о квартире:\n";
            cout << "Количество комнат: " << rooms << endl;
            cout << "Площадь: " << area << " кв.м" << endl;
            cout << "Адрес: " << address << endl;
            cout << "Дата подачи заявления на ремонт: " << repairRequestDate << endl;
            cout << "Дата окончания ремонта: " << repairCompletionDate() << endl;
        }
};

int main(){
    int count;
    list <repairHouse> house;

    cout << "Введите количество карточек" << endl;
    cin >> count;
    for (int i = 0; i < count; i++) {
        cout << "\nВвод данных для квартиры " << i + 1 << ":\n";
        int rooms;
        double area;
        string address, repairRequestDate; 
        
        cout << "Количество комнат: ";
        cin >> rooms;
        cout << "Площадь (кв.м): ";
        cin >> area;
        cout << "Адрес: ";
        cin.ignore();
        getline(cin, address);
        cout << "Дата подачи заявления на ремонт (ДД.ММ.ГГ): ";
        cin >> repairRequestDate;

        repairHouse newHouse(rooms, area, address, repairRequestDate);

        house.push_back(newHouse);
    }

    cout << "\n" << "До сортировки: " << endl;
    for(auto el : house){
        el.outputTo();
    }

    list <repairHouse> sortedHouse = house;
    sortedHouse.sort(repairHouse::compareByCompletionDate);

    cout << "\n" << "После сортировки: " << endl;
    for(auto el : sortedHouse){
        el.outputTo();
    }
}
    
