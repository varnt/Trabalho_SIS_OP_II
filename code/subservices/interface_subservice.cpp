#include "interface_subservice.hpp"

InterfaceSubservice::InterfaceSubservice(participante *&tabelaParticipantes){
    this->tabelaParticipantes = tabelaParticipantes;
};
InterfaceSubservice::~InterfaceSubservice(){};

bool InterfaceSubservice::isActive()
{
    return this->currentState;
};

void InterfaceSubservice::setActive()
{
    this->currentState = true;
};

void InterfaceSubservice::setNotActive()
{
    this->currentState = false;
};

void InterfaceSubservice::gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
};

int InterfaceSubservice::updateServerScreen()
{
    while (this->isActive() == true)
    {
        // Clear the screen
        system("clear");
        // set cursor to top left

        this->gotoxy(0, 3);
        printList(tabelaParticipantes);

        this->gotoxy(0, 0);
        cout << "Insert Command > ";
        string userCommand;
        getline(cin, userCommand);

        if (userCommand == "help")
        {
            system("clear");
            cout << "  -  help - show this help" << endl;
            cout << "  -  quit - exit the program" << endl;
            cout << endl <<"Press anything to quit help tab" << endl;
            cin.get();
        }
        else if(userCommand == "quit"){
            return 0;
        }
    }
    return 0;
};