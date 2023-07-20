#include "interface_subservice.hpp"

InterfaceSubservice::InterfaceSubservice(participante *&tabelaParticipantes,
                                         bool *tabelaParticipantesUpdate) {
  this->tabelaParticipantes = tabelaParticipantes;
  this->tabelaEstaAtualizada = tabelaParticipantesUpdate;
};
InterfaceSubservice::~InterfaceSubservice(){};

bool InterfaceSubservice::isActive() { return this->currentState; };

void InterfaceSubservice::setActive() { this->currentState = true; };

void InterfaceSubservice::setNotActive() { this->currentState = false; };

void InterfaceSubservice::gotoxy(int x, int y) {
  printf("%c[%d;%df", 0x1B, y, x);
};

void InterfaceSubservice::printTable(bool &tabelaEstaAtualizada,
                                     participante *&tabelaParticipantes) {
  system("clear");
  while (true) {
    if (tabelaEstaAtualizada == false && this->enablePrinting == true) {
      this->gotoxy(1, 3);
      printList(tabelaParticipantes);
      this->gotoxy(0, 0);
      cout << "Insert Command > ";
      table_mtx.lock();
      tabelaEstaAtualizada = true;
      table_mtx.unlock();

      // sleep(3);
    }
  }
}

void InterfaceSubservice::wakeOnLan(participante *&tabelaParticipantes,
                                    string hostname) {

  participante *participanteAtual = this->tabelaParticipantes;

  while (participanteAtual != nullptr) {
    if (participanteAtual->hostname == hostname) {
      string cmd = "wakeonlan -i " + participanteAtual->ip_address + " " +
                   participanteAtual->mac_address;
      system(cmd.c_str());
      return;
    }
    participanteAtual = participanteAtual->next;
  }

  cout << "host not found";
}

int InterfaceSubservice::updateServerScreen() {
  this->setActive();
  this->enablePrinting = true;
  table_mtx.lock();
  *tabelaEstaAtualizada = false;
  table_mtx.unlock();

  thread printTable_thr([&]() {
    InterfaceSubservice::printTable(*tabelaEstaAtualizada,
                                    this->tabelaParticipantes); // thread
  });

  while (this->isActive() == true) {

    // set cursor to top left'
    this->gotoxy(0, 0);
    cout << "Insert Command > ";
    string userCommand;
    getline(cin, userCommand);
    this->enablePrinting = false;
    getline(cin, userCommand);
    string command = userCommand.substr(0, userCommand.find(" "));
    string argument = userCommand.substr(userCommand.find(" ") + 1);
    if (command == "HELP") {
      this->enablePrinting = false;
      system("clear");
      cout << "  -  HELP - show this help" << endl;
      cout << "  -  EXIT - exit the program" << endl;
      cout << endl << "Press anything to quit help tab" << endl;
      cin.get();
      system("clear");
      this->enablePrinting = true;
    } else if (command == "EXIT") {
      exit(0);
    } else if (command == "WAKE") {
        wakeOnLan(this->tabelaParticipantes, argument);
    } else {
      this->enablePrinting = true;
    }
  }
  printTable_thr.join();
  return 0;
};

int InterfaceSubservice::updateClientScreen() {
  while (this->isActive() == true) {
    // Clear the screen
    system("clear");
    // set cursor to top left
    this->gotoxy(0, 0);
    cout << "Insert Command > ";
    string userCommand;
    getline(cin, userCommand);

    if (userCommand == "HELP") {
      system("clear");
      cout << "  -  HELP - show this help" << endl;
      cout << "  -  QUIT - exit the program" << endl;
      cout << endl << "Press anything to quit help tab" << endl;
      cin.get();
    } else if (userCommand == "EXIT") {
      // make function
      system("clear");
      cout << "Exiting..." << endl;
      exit(0);
    }
  }
  return 0;
}
