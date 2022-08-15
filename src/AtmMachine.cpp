#include "AtmMachine.hpp"

AtmMachine::AtmMachine(BankServer* bankserver) : m_bankserver ( bankserver)
{
};

bool AtmMachine::withdraw(int account_number, int value)
{
    bool result = false;

    m_bankserver->Connect();

    auto available_balance = m_bankserver->GetBalance(account_number);

    if(available_balance >= value)
    {
        m_bankserver->Debit(account_number, value);
        result = true;
    }

    m_bankserver->Disconnect();

    return result;
}