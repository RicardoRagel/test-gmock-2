#include "BankServer.hpp"
#include <gmock/gmock.h>

/*
    Mock BankServer class: 
    
    The Mock of our AtmMachine dependency: the BankServer

    Mandatory: 
    
        * It must override all the bankserver functions, otherwise it will be not possible
          to create a MockBankServer object due to BankServer is Abstract.
        * All the mocked method must be public in any case, although they are private in the
          base class (BankServer)
        * In case the base class has overloaded functions, simply mock all of them adding a 
          mock function for each one with tits own args.
        * In case the base class is templated, simply template also the mock class in the 
          same way
*/

class MockBankServer : public BankServer 
{
    public:
        MOCK_METHOD(void, Connect, (), (override));
        MOCK_METHOD(void, Disconnect, (), (override));
        MOCK_METHOD(void, Credit, (int, int), (override));
        MOCK_METHOD(void, Debit, (int, int), (override));
        MOCK_METHOD(int,  DoubleTransaction, (int, int, int), (override));
        MOCK_METHOD(int,  GetBalance, (int), (const, override));
};
