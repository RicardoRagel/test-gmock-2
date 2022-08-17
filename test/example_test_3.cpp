#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::AtMost;
using ::testing::_;         // "any"

//--------------------------------------------------------------------------------------------------
// EXPECT_CALL with the expectations of different number of callings
// Let's suppose we will withdraw 1000 dollars 3 times and we will simulate the return value
// as we expect it having only 2000 dollars at the beginning.
TEST(AtmMachine, TestWithdrawExpectedCalls1)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number))
        .Times(3)
        .WillOnce(Return(2000))
        .WillOnce(Return(1000))
        .WillOnce(Return(0));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success_1 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_2 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_3 = atm_machine.withdraw(account_number, value);

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_TRUE(withdraw_success_1);
    EXPECT_TRUE(withdraw_success_2);
    EXPECT_FALSE(withdraw_success_3);
}

// Another example but setting a maximum of expected calls
TEST(AtmMachine, TestWithdrawExpectedCalls2)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number))
        .Times(AtMost(10))
        .WillOnce(Return(5000))
        .WillOnce(Return(4000));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success_1 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_2 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_3 = atm_machine.withdraw(account_number, value);

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_TRUE(withdraw_success_1);
    EXPECT_TRUE(withdraw_success_2);
    EXPECT_FALSE(withdraw_success_3); // now it is false due to getBalance is unspecified
}
// In this case, the actual callings are 3, so Times(AtMost(5)) is OK
// We are setting the return value only for 2, so the third one will return 
// the default value, that is 0. So the assertion will be as they are in the TEST

//--------------------------------------------------------------------------------------------------
// Another example but setting a the expected values for multiple mock callings
TEST(AtmMachine, TestWithdrawExpectedCalls3)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number))
        .Times(AtMost(10))
        .WillOnce(Return(5000))
        .WillRepeatedly(Return(4000));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success_1 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_2 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_3 = atm_machine.withdraw(account_number, value);

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_TRUE(withdraw_success_1);
    EXPECT_TRUE(withdraw_success_2);
    EXPECT_TRUE(withdraw_success_3); // now this is again true
}

// What happens if we don't specify the .Times() ?
TEST(AtmMachine, TestWithdrawExpectedCalls4FAIL)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number));
    // Neither .Times() nor .Will*() specified -> Times(1) and default returned values

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success_1 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_2 = atm_machine.withdraw(account_number, value);
}
// IT WILL FAIL, due to it will be called 2 times, and we are only setting Times(1)

// Solution 1
TEST(AtmMachine, TestWithdrawExpectedCalls4_sol1)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number))
        .WillOnce(Return(2000))
        .WillOnce(Return(1000));
    // No .Times() but 2 .WillOnce() specified -> Times(2)

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success_1 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_2 = atm_machine.withdraw(account_number, value);
}

// Solution 2
TEST(AtmMachine, TestWithdrawExpectedCalls4_sol2)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number))
        .WillRepeatedly(Return(1000));
    // No .Times() but .WillRepeatedly() specified -> Times(AtLeast(0))

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success_1 = atm_machine.withdraw(account_number, value);
    bool withdraw_success_2 = atm_machine.withdraw(account_number, value);
}

//--------------------------------------------------------------------------------------------------
// DISALLOWING A CALL --> .Times(0) means exactly that, we expect that this function is not called 
TEST(AtmMachine, TestWithdrawExpectedCalls5)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number))
        .Times(1)
        .WillOnce(Return(0)); // So Debit should not be called!!

    EXPECT_CALL(mock_bankserver, Debit(_,_)).Times(0); // Setting this function should not be called

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, value);

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_FALSE(withdraw_success); // GetBalance returns 0 --> withdraw returns false
}