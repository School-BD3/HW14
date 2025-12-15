#include <iostream>
#include <string>
#include <conio.h> 

using namespace std;

class Person {
    string _name;
public:
    Person(const string& name = "") : _name(name) {}
    string name() const { return _name; }
};

class Position {
    string _title;
public:
    Position(const string& title = "") : _title(title) {}
    string title() const { return _title; }
};

class Employee {
    Person _person;
    Position _position;
public:
    Employee(const Person& p, const Position& pos)
        : _person(p), _position(pos) {}

    string info() const {
        return _person.name() + " - " + _position.title();
    }
};

class Employer {
    struct Office {
        bool _free;
        Employee* _pemployee;
    };

    Office* _office;
    size_t _volume;

public:
    Employer(size_t volume) : _volume(volume) {
        _office = new Office[_volume];
        for (size_t i = 0; i < _volume; ++i) {
            _office[i]._free = true;
            _office[i]._pemployee = nullptr;
        }
    }

    ~Employer() {
        for (size_t i = 0; i < _volume; ++i)
            delete _office[i]._pemployee;
        delete[] _office;
    }

    void hire(const Person& person, const Position& position) {
        size_t i = 0;
        while (i < _volume && !_office[i]._free)
            i++;

        if (i == _volume) {
            cout << "All vacancies are occupied. Fire someone first.\n";
            return;
        }

        _office[i]._pemployee = new Employee(person, position);
        _office[i]._free = false;
        cout << "Employee hired successfully.\n";
    }

    void fire(const size_t i) {
        if (i >= _volume) {
            cout << "Invalid position number!\n";
            return;
        }

        if (_office[i]._free) {
            cout << "This position is already free.\n";
            return;
        }

        delete _office[i]._pemployee;
        _office[i]._pemployee = nullptr;
        _office[i]._free = true;

        cout << "Employee fired.\n";
    }

    void print() const {
        cout << "\nOffice state:\n";
        for (size_t i = 0; i < _volume; ++i) {
            cout << i << ": ";
            if (_office[i]._free)
                cout << "free\n";
            else
                cout << _office[i]._pemployee->info() << endl;
        }
    }
};

struct Calc {
    double _a;
    double _x;

    double add() { return _a += _x; }
    double sub() { return _a -= _x; }
    double mul() { return _a *= _x; }
    double div() {
        if (_x == 0) {
            cout << "Division by zero!\n";
            return _a;
        }
        return _a /= _x;
    }
};

typedef double (Calc::* Operation)();

double exe(Calc& c, Operation op) {
    return (c.*op)();
}

int main() {
    Employer company(3);

    while (true) {
        cout << "\nMenu:\n";
        cout << "1 - Hire employee\n";
        cout << "2 - Fire employee\n";
        cout << "3 - Show office state\n";
        cout << "4 - Calculator demo\n";
        cout << "ESC - Exit\n";

        int key = _getch();

        if (key == 27) { // ESC
            cout << "\nProgram finished.\n";
            break;
        }

        if (key == '1') {
            string name, position;
            cout << "\nEnter name: ";
            cin >> name;
            cout << "Enter position: ";
            cin >> position;
            company.hire(Person(name), Position(position));
        }
        else if (key == '2') {
            size_t index;
            cout << "\nEnter position number to fire: ";
            cin >> index;
            company.fire(index);
        }
        else if (key == '3') {
            company.print();
        }
        else if (key == '4') {
            Calc c;
            cout << "\nEnter initial value: ";
            cin >> c._a;

            while (true) {
                cout << "\nEnter number (or 'q' to quit calculator): ";
                string input;
                cin >> input;
                if (input == "q") break;
                c._x = stod(input);

                cout << "Enter operation (+ - * /): ";
                char op;
                cin >> op;

                switch (op) {
                case '+': cout << "Result: " << exe(c, &Calc::add) << endl; break;
                case '-': cout << "Result: " << exe(c, &Calc::sub) << endl; break;
                case '*': cout << "Result: " << exe(c, &Calc::mul) << endl; break;
                case '/': cout << "Result: " << exe(c, &Calc::div) << endl; break;
                default: cout << "Invalid operation!\n"; break;
                }
            }
        }
    }

    return 0;
}
