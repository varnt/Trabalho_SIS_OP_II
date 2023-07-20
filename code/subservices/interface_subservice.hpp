#ifndef INTERFACE_SUBSERVICE_HPP
#define INTERFACE_SUBSERVICE_HPP

#include "../global.hpp"

class InterfaceSubservice {
public:
  InterfaceSubservice(participante *&tabelaParticipantes,
                      bool *tabelaParticipantesUpdate);
  ~InterfaceSubservice();
  int updateServerScreen();
  int updateClientScreen();
  void wakeOnLan(participante *&tabelaParticipantes, string hostname);

  bool isActive();
  void setActive();
  void setNotActive();

  void gotoxy(int x, int y);

private:
  bool currentState;
  participante *tabelaParticipantes;
  bool *tabelaEstaAtualizada;
  void printTable(bool &tabelaEstaAtualizada,
                  participante *&tabelaParticipantes);
  bool enablePrinting;
};

#endif
