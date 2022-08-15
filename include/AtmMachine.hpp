#ifndef ATMMACHINE_HPP
#define ATMMACHINE_HPP

#include "BankServer.hpp"

/*
    AtmMachine class:
    
    The class that is under test.

    It uses BankServer as an injected dependency, in Dependency Injection 
    (https://en.wikipedia.org/wiki/Dependency_injection) terminology, AtmMachine 
    is a client of the BankServer service.
*/

class AtmMachine
{
  public:
    
    // Constructor receiving the injected dependency bankserver.
    // Notice that: It is receiving a pointer to the base abstract class BankServer. 
    // When the real class (the implementation) exists and it is inheriting from BankServer, 
    // I will can use (receive) it in my AtmMachine, because I will be receiving a Child Class
    // object in a Base class pointer and, due to all the functions are (pure) virtual in the base 
    // class, the AtmMachine will be calling to the child member functions.
    AtmMachine(BankServer* bankserver);

    // Function to get money using the ATM. 
    // It returns true if it was OK. Otherwise return false
    // Notice that: It is using multiple function of BankServer and in a specific order 
    bool withdraw(int account_number, int value);

  private:

    BankServer* m_bankserver;
};


#endif