#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 1. Паттерн Команда

class Command {
public:
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual ~Command() {}
};

class Light {
public:
    void On() { cout << "Свет включен.\n"; }
    void Off() { cout << "Свет выключен.\n"; }
};

class TV {
public:
    void On() { cout << "Телевизор включен.\n"; }
    void Off() { cout << "Телевизор выключен.\n"; }
};

class LightOnCommand : public Command {
    Light* light;
public:
    LightOnCommand(Light* l) : light(l) {}
    void Execute() override { light->On(); }
    void Undo() override { light->Off(); }
};

class LightOffCommand : public Command {
    Light* light;
public:
    LightOffCommand(Light* l) : light(l) {}
    void Execute() override { light->Off(); }
    void Undo() override { light->On(); }
};

class TVOnCommand : public Command {
    TV* tv;
public:
    TVOnCommand(TV* t) : tv(t) {}
    void Execute() override { tv->On(); }
    void Undo() override { tv->Off(); }
};

class TVOffCommand : public Command {
    TV* tv;
public:
    TVOffCommand(TV* t) : tv(t) {}
    void Execute() override { tv->Off(); }
    void Undo() override { tv->On(); }
};

class Remote {
    Command* onCommand;
    Command* offCommand;
public:
    void SetCommands(Command* onCmd, Command* offCmd) {
        onCommand = onCmd;
        offCommand = offCmd;
    }
    void PressOn() { onCommand->Execute(); }
    void PressOff() { offCommand->Execute(); }
    void PressUndo() { onCommand->Undo(); }
};

// 2. Паттерн Шаблонный метод

class Beverage {
public:
    void PrepareRecipe() {
        BoilWater();
        Brew();
        PourInCup();
        AddCondiments();
    }
protected:
    void BoilWater() { cout << "Кипячение воды...\n"; }
    void PourInCup() { cout << "Наливание в чашку...\n"; }

    virtual void Brew() = 0;
    virtual void AddCondiments() = 0;
};

class Tea : public Beverage {
    void Brew() override { cout << "Заваривание чая...\n"; }
    void AddCondiments() override { cout << "Добавление лимона...\n"; }
};

class Coffee : public Beverage {
    void Brew() override { cout << "Заваривание кофе...\n"; }
    void AddCondiments() override { cout << "Добавление сахара и молока...\n"; }
};

// 3. Паттерн Посредник

class Mediator;

class Colleague {
protected:
    Mediator* mediator;
public:
    Colleague(Mediator* m) : mediator(m) {}
    virtual void Receive(const string& message) = 0;
};

class Mediator {
    vector<Colleague*> colleagues;
public:
    void Register(Colleague* colleague) { colleagues.push_back(colleague); }

    void SendMessage(const string& message, Colleague* sender) {
        for (auto* colleague : colleagues) {
            if (colleague != sender) {
                colleague->Receive(message);
            }
        }
    }
};

class User : public Colleague {
    string name;
public:
    User(Mediator* m, const string& n) : Colleague(m), name(n) {}

    void Send(const string& message) {
        cout << name << " отправляет: " << message << endl;
        mediator->SendMessage(message, this);
    }

    void Receive(const string& message) override {
        cout << name << " получает: " << message << endl;
    }
};

// Главняа функция
int main() {
    // 1. Команда
    Light light;
    TV tv;

    LightOnCommand lightOn(&light);
    LightOffCommand lightOff(&light);
    TVOnCommand tvOn(&tv);
    TVOffCommand tvOff(&tv);

    Remote remote;
    cout << "Управление светом:\n";
    remote.SetCommands(&lightOn, &lightOff);
    remote.PressOn();
    remote.PressOff();
    remote.PressUndo();

    cout << "\nУправление телоевизором:\n";
    remote.SetCommands(&tvOn, &tvOff);
    remote.PressOn();
    remote.PressOff();

    // 2. Шаблонный метод
    cout << "\nПриготовление напитков:\n";
    Tea tea;
    Coffee coffee;
    tea.PrepareRecipe();
    coffee.PrepareRecipe();

    // 3. Посредник
    cout << "\nЧат-система:\n";
    Mediator chat;
    User user1(&chat, "Алиса");
    User user2(&chat, "Боб");
    User user3(&chat, "Чарли");

    chat.Register(&user1);
    chat.Register(&user2);
    chat.Register(&user3);

    user1.Send("Привет, Боб!");
    user2.Send("Привет, Алиса!");
    user3.Send("Всем привет!");

    return 0;
}
