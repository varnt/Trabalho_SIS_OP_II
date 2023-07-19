#include "interface_subservice.hpp"

InterfaceSubservice::InterfaceSubservice(participante *&tabelaParticipantes, bool *tabelaParticipantesUpdate)
{
    this->tabelaParticipantes = tabelaParticipantes;
    this->tabelaEstaAtualizada = tabelaParticipantesUpdate;
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

void InterfaceSubservice::printTable(bool &tabelaEstaAtualizada, participante *&tabelaParticipantes)
{
    system("clear");
    while (true)
    {
        if (tabelaEstaAtualizada == false && this->enablePrinting == true)
        {
            this->gotoxy(1, 3);
            printList(tabelaParticipantes);
            this->gotoxy(0, 0);
            cout << "Insert Command > ";
            tabelaEstaAtualizada = true;

            //sleep(3);
        }
    }
}

int InterfaceSubservice::updateServerScreen()
{
    this->setActive();
    this->enablePrinting = true;
    *tabelaEstaAtualizada = false;

    thread printTable_thr([&]()
                          {
                              InterfaceSubservice::printTable(*tabelaEstaAtualizada, this->tabelaParticipantes); // thread
                          });


    while (this->isActive() == true)
    {

        // set cursor to top left'
        this->gotoxy(0, 0);
        cout << "Insert Command > ";
        string userCommand;
        getline(cin, userCommand);
        this->enablePrinting = false;
        getline(cin, userCommand);
        if (userCommand == "HELP")
        {
            this->enablePrinting = false;
            system("clear");
            cout << "  -  HELP - show this help" << endl;
            cout << "  -  EXIT - exit the program" << endl;
            cout << endl
                 << "Press anything to quit help tab" << endl;
            cin.get();
            system("clear");
            this->enablePrinting = true;
        }
        else if (userCommand == "EXIT")
        {
            exit(0);
        }
        else{
            this->enablePrinting = true;
        }
    }
    printTable_thr.join();
    return 0;
};

int InterfaceSubservice::updateClientScreen()
{
    while (this->isActive() == true)
    {
        // Clear the screen
        system("clear");
        // set cursor to top left
        this->gotoxy(0, 0);
        cout << "Insert Command > ";
        string userCommand;
        getline(cin, userCommand);

        if (userCommand == "HELP")
        {
            system("clear");
            cout << "  -  HELP - show this help" << endl;
            cout << "  -  QUIT - exit the program" << endl;
            cout << endl
                 << "Press anything to quit help tab" << endl;
            cin.get();
        }
        else if (userCommand == "EXIT")
        {
            // make function
            system("clear");
            cout << "Exiting..." << endl;
            exit(0);
        }
    }
    return 0;
}
