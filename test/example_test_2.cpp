#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;         // "any"

//--------------------------------------------------------------------------------------------------
// FULL CALLING CHECK USING EXPECTED_CALL() OF ALL THE MOCK FUNCTIONS CALLED BY AtmMachine::withdraw
TEST(AtmMachine, TestWithdrawExpectedCalls)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;

    // Expectations
    EXPECT_CALL(mock_bankserver, Connect()) 
        .Times(1);
        // It does't return any value, so it is not necessary any .Will*

    EXPECT_CALL(mock_bankserver, GetBalance(_))
        .Times(AtLeast(1))
        .WillOnce(Return(2000));
    
    EXPECT_CALL(mock_bankserver, Debit(_,_))
        .Times(AtLeast(1));
    
    EXPECT_CALL(mock_bankserver, Disconnect())
        .Times(1);

    // Actually all of them call be called without any (_), because that is the meaning
    // if you write something like EXPECT_CALL(mock_bankserver, Debit).Times(AtLeast(1));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(1234, 1000);

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_TRUE(withdraw_success);
}

// Notice that we are setting that the GetBalance function will return 2000, so the balance
// in the account is 2000. Then we call to get 1000 dollars, and we check that everything work correctly
// Let's check the following example where we set that the balance is 999, and we try to ge again 1000 dollars
TEST(AtmMachine, TestWithdrawExpectedCallsFAIL)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;

    // Expectations
    EXPECT_CALL(mock_bankserver, Connect()) // It does't return any value, so:
        .Times(1);

    EXPECT_CALL(mock_bankserver, GetBalance(_))
        .Times(AtLeast(1))
        .WillOnce(Return(999));
    
    EXPECT_CALL(mock_bankserver, Debit(_,_))
        .Times(AtLeast(1));
    
    EXPECT_CALL(mock_bankserver, Disconnect())
        .Times(1);

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(1234, 1000);

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_TRUE(withdraw_success);
}
// Obviously it fails due to the Debit function was not called. Why? If you check the 
// withdraw function, the Debit() function is only call if the balance is enough (bigger
// than the money to get from the ATM). In this case, it is not enough, so the function is not called
// and our TEST is expecting that this function is called.

