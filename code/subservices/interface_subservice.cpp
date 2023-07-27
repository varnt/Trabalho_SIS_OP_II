#include "interface_subservice.hpp"
#include "../common/packet.hpp"
#include "../common/socketAPI.hpp"

InterfaceSubservice::InterfaceSubservice(participante *&tabelaParticipantes,
                                         bool *tabelaParticipantesUpdate)
{
  this->tabelaParticipantes = tabelaParticipantes;
  this->tabelaEstaAtualizada = tabelaParticipantesUpdate;
};
InterfaceSubservice::~InterfaceSubservice(){};

bool InterfaceSubservice::isActive() { return this->currentState; };

void InterfaceSubservice::setActive() { this->currentState = true; };

void InterfaceSubservice::setNotActive() { this->currentState = false; };

void InterfaceSubservice::gotoxy(int x, int y)
{
  printf("%c[%d;%df", 0x1B, y, x);
};

void InterfaceSubservice::printTable(bool &tabelaEstaAtualizada,
                                     participante *&tabelaParticipantes)
{
  system("clear");
  while (true)
  {
    if (tabelaEstaAtualizada == false && this->enablePrinting == true)
    {
      gotoxy(0, 0);
      cout << "Press Enter to insert command";
      this->gotoxy(1, 5);
      system("tput ed");
      printList(tabelaParticipantes);
      table_mtx.lock();
      tabelaEstaAtualizada = true;
      table_mtx.unlock();

      // sleep(3);
    }
  }
}

void InterfaceSubservice::wakeOnLan(participante *&tabelaParticipantes,
                                    string hostname)
{

  participante *participanteAtual = this->tabelaParticipantes;

  while (participanteAtual != nullptr)
  {
    if (participanteAtual->hostname == hostname)
    {
      string cmd = "wakeonlan -i " + participanteAtual->ip_address + " " +
                   participanteAtual->mac_address;
      system(cmd.c_str());
      return;
    }
    participanteAtual = participanteAtual->next;
  }

  cout << "host not found";
}

int InterfaceSubservice::updateServerScreen()
{
  this->setActive();
  this->enablePrinting = true;
  table_mtx.lock();
  *tabelaEstaAtualizada = false;
  table_mtx.unlock();

  thread printTable_thr([&]()
                        {
                          InterfaceSubservice::printTable(*tabelaEstaAtualizada,
                                                          this->tabelaParticipantes); // thread
                        });

  while (this->isActive() == true)
  {

    // set cursor to top left'
    string userCommand;
    getline(cin, userCommand);
    this->enablePrinting = false;
    system("clear");
    this->gotoxy(1, 5);
    printList(tabelaParticipantes);
    system("tput ed");
    this->gotoxy(0, 0);
    cout << "Insert Command > ";

    getline(cin, userCommand);
    string command = userCommand.substr(0, userCommand.find(" "));
    string argument = userCommand.substr(userCommand.find(" ") + 1);
    if (command == "HELP")
    {
      this->enablePrinting = false;
      system("clear");
      cout << "  -  HELP - show this help" << endl;
      cout << "  -  WAKEUP <hostname>- wake computer with the hostname argument" << endl;
      cout << "  -  EXIT - exit the program" << endl;
      cout << endl
           << "Press anything to quit help tab" << endl;
      cin.get();
      system("clear");
    }
    else if (command == "EXIT")
    {
      exit(0);
    }
    else if (command == "WAKEUP")
    {
      wakeOnLan(this->tabelaParticipantes, argument);
    }
    this->enablePrinting = true;
    gotoxy(0, 0);
    cout << "Press Enter to insert command";
    this->gotoxy(1, 5);
    system("tput ed");
    if (*tabelaEstaAtualizada == false)
    {
      printList(tabelaParticipantes);
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
      SocketAPI clientSocket(PORTA_EXIT, "client");
      packet_struct exitPacket =
          createPacket(0, PORTA_DESCOBERTA, PORTA_EXIT,
                       GLOBAL_BROADCAST_ADD, getLocalIpAddress(), gethostname(),
                       getMacAddress(), "awaken", EXIT_MSG);
      int n = clientSocket.sendPacket(&exitPacket, GLOBAL_BROADCAST_ADD,
                                      PORTA_DESCOBERTA);
      if (n < 0)
      {
        cerr << "DiscoverySubservice>clientDiscoverySubservice> error on "
                "sending "
                "EXIT_MSG = "
             << strerror(errno) << endl;
      }
      exit(0);
    }
  }
  return 0;
}
