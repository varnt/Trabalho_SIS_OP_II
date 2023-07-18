#ifndef INTERFACE_SUBSERVICE_HPP
#define INTERFACE_SUBSERVICE_HPP

#include "../global.hpp"

class InterfaceSubservice
{
public:
    InterfaceSubservice(participante*& tabelaParticipantes);
    ~InterfaceSubservice();
    int updateServerScreen();
    //void updateTable();

    bool isActive();
    void setActive();
    void setNotActive();

    void gotoxy(int x, int y);

private:
    bool currentState;
    participante* tabelaParticipantes;
};

#endif