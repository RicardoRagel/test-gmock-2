#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"

using ::testing::Sequence;
using ::testing::Expectation;
using ::testing::InSequence;
using ::testing::_;
using ::testing::Return;

//--------------------------------------------------------------------------------------------------
// If we don't specify anything, all the EXPECT_CALL expectations can be accomplished in any order.
// 
// Enforcing the ORDER of Calls: 
//
// Using the combination of Sequence and EXPECT_CALL().InSequence
//
// Example: 
// We want to call for example A, B, C, D as:
//
// A should be called before B and before C
// C should be called before D
//
// In that case we should add:
//
// TEST
// (
//     ...
//     Sequence s1, s2;
//     EXPECT_CALL(mock_bankserver, A()).InSequence(s1,s2); // It will be called the first one in seq s1 and s2
//     EXPECT_CALL(mock_bankserver, B()).InSequence(s1);    // It will be called the second one in seq s1

//     EXPECT_CALL(mock_bankserver, C()).InSequence(s2);   //  It will be called the second one in the seq s2
//     EXPECT_CALL(mock_bankserver, D()).InSequence(s2);   //  It will be called the third one in the seq s2
// )
//
// Let's do a simple example: Check that Connect() is called before Disconnect() although no balance
TEST(AtmMachine, TestInSequenceMethod)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    Sequence s1;
    EXPECT_CALL(mock_bankserver, Connect()).InSequence(s1);
    EXPECT_CALL(mock_bankserver, Disconnect()).InSequence(s1);

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, value);
}

//--------------------------------------------------------------------------------------------------
// Enforcing the ORDER of Calls: 
//
// Using the EXPECT_CALL().After()
TEST(AtmMachine, TestAfterMethod)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    Sequence s1;
    Expectation a = EXPECT_CALL(mock_bankserver, Connect());
    Expectation b = EXPECT_CALL(mock_bankserver, GetBalance(_)).After(a)
        .WillOnce(Return(1000));
    Expectation c = EXPECT_CALL(mock_bankserver, Debit(_,_)).After(b);
    Expectation d = EXPECT_CALL(mock_bankserver, Disconnect()).After(b, c); // actually it is enough with c in this case

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, value);
}

//--------------------------------------------------------------------------------------------------
// Enforcing the ORDER of Calls: 
//
// Using the InSequence object. It ensures that the EXPECT_CALLs in its SCOPE 
// are called in the correct order
TEST(AtmMachine, TestInSequence)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations (in 2 scopes)
    {
        InSequence seq1;
        EXPECT_CALL(mock_bankserver, Connect());
        EXPECT_CALL(mock_bankserver, Disconnect());
    }
    {
        InSequence seq2;
        EXPECT_CALL(mock_bankserver, GetBalance(_))
            .WillOnce(Return(1000));
        EXPECT_CALL(mock_bankserver, Debit(_,_));
    }

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, value);
}

