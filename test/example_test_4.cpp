#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"

using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;

//--------------------------------------------------------------------------------------------------
// Previous tests throw a lot of warnings about "Uninteresting mock functions call":
//
// GMOCK WARNING:
// Uninteresting mock function call - returning directly.
//     Function call: Disconnect()
// NOTE: You can safely ignore the above warning unless this call should not happen. 
//
// They are due to they have been called in our "Acts", we have not specified any EXPECT_CALL
// for them. 
//
// To suppress these GMock Warnings, we can use the template class 
// 
// NiceMock<YourMockClass> 
//
// instead of to use directly our Mock Class
TEST(AtmMachine, UninterestingCalls_IGNORING)
{
    // Arrange dependency mock
    NiceMock<MockBankServer> mock_bankserver; // Suppressing uninteresting warnings
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number)).WillOnce(Return(2000));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, value);

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_TRUE(withdraw_success);
}

//--------------------------------------------------------------------------------------------------
// In the other hand, you could be interested in the opposite: Makes these 
// uninteresting warning = FAILURES
//
// In that case we can use the template class 
//
// StrictMock<YourMockClass> 
// 
// instead of to use directly our Mock Class
TEST(AtmMachine, UninterestingCalls_MakeThemFailures)
{
    // Arrange dependency mock
    StrictMock<MockBankServer> mock_bankserver; // Make uninteresting warnings Failures
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number)).WillOnce(Return(2000));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, value);

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_TRUE(withdraw_success);
}

//--------------------------------------------------------------------------------------------------
// Finally, we have to take into account that the previous were UNINTERESTING CALLs, the ones that 
// could be called, but we are not interested on them.
//
// There are also UNEXPECTED CALLs. They are callings to a mock function with a specific form 
// (for example, with some specific arg values) that we are expecting (EXPECT_CALL()) but we are
// calling without any other form (for example, with another arg values)
// StrictMock<YourMockClass> 
// 
// Let's check an example
TEST(AtmMachine, UnexpectedCalls)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number)).WillOnce(Return(2000));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(5678, value); // Notice that we are calling it with a different account_number

    // Asserts (Optional, because we can just check the Expectations)
    EXPECT_FALSE(withdraw_success);
}
// In this case, the test FAILS with:
// Unexpected mock function call - returning default value.
//     Function call: GetBalance(5678)
//           Returns: 0
// Google Mock tried the following 1 expectation, but it didn't match:

// /home/ricardo/Libraries/test-gmock-2/test/example_test_4.cpp:89: EXPECT_CALL(mock_bankserver, GetBalance(account_number))...
//   Expected arg #0: is equal to 1234
//            Actual: 5678
//          Expected: to be called once
//            Actual: never called - unsatisfied and active