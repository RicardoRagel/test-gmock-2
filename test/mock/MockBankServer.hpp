#include "BankServer.hpp"
#include <gmock/gmock.h>

/*
    Mock BankServer class: 
    
    The Mock of our AtmMachine dependency: the BankServer

    Mandatory: it must override all the bankserver functions, otherwise it will be not possible
    to create a MockBankServer object due to BankServer is Abstract.
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
