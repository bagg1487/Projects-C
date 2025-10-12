#include <iostream>
#include <stack>

using namespace std;

int main() {
    stack <string> firstStack;
    stack <string> secondStack;
    int count;
    string data;

    cout << "Введите количество элементов стека" << endl;
    cin >> count;
    for(int i = 0; i < count; i++){
        cout << "Введите " << i + 1 << " данные" << endl;
        cin >> data;
        firstStack.push(data);
    }

    stack <string> firstStackCopy = firstStack;

    while(!firstStackCopy.empty()){
        string top = firstStackCopy.top();
        string word;

        for(char c : top){
            if (islower(c)) word += c;
            }
            if (!word.empty()) secondStack.push(word); 
            firstStackCopy.pop();
    }

    cout << "Первый стек" << endl;
    while(!firstStack.empty()){
        cout << firstStack.top() << " ";
        firstStack.pop();
    }

    cout << "\n" << "Второй стек" << endl;
    while(!secondStack.empty()){
        cout <<secondStack.top() << " ";
        secondStack.pop();
    }
    cout << endl;
}
