#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"
#include <exception>

using ::testing::NiceMock;
using ::testing::AnyNumber;
using ::testing::Return;
using ::testing::Assign;
using ::testing::DoAll;
using ::testing::SaveArg;
using ::testing::Throw;
using ::testing::ThrowsMessage;
using ::testing::StrEq;
using ::testing::Invoke;
using ::testing::ReturnArg;
using ::testing::WithArg;
using ::testing::_;

//--------------------------------------------------------------------------------------------------
// STICKY RULES: All the Expectations are Sticky (unless we use the method RetiresOnSturation())
//
// That means:
//      * They remains active even after they have reached the upper bound of invocations.
//      * They will be matched from the BOTTOM to the TOP.
TEST(AtmMachine, TestSticky1)
{
    // Arrange
    NiceMock<MockBankServer> mock_bankserver;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(5678))
        .Times(AnyNumber())
        .WillRepeatedly(Return(1000));

    EXPECT_CALL(mock_bankserver, GetBalance(1234))
        .Times(1)
        .WillOnce(Return(1000));

    // Act
    AtmMachine atm_machine(&mock_bankserver);
    bool result1 = atm_machine.withdraw(1234, withdraw_value); // This will be catched by the expectation 1234
    bool result2 = atm_machine.withdraw(5678, withdraw_value); // This will be catched by the expectation 5678
    
    // Asserts
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
}
// That works, but check what happens if we set also withdraw 1234 the second time:
//
// Mock function called more times than expected - returning default value.
//     Function call: GetBalance(1234)
//           Returns: 0
//          Expected: to be called once
//            Actual: called twice - over-saturated and active
// /home/ricardo/Libraries/test-gmock-2/test/example_test_7.cpp:49: Failure
// Value of: result2
//   Actual: false
// Expected: true
//
// In other words, the second expectation (1234) is catching both calls, that it is an error,
// because we specified it will be called only 1. It also makes that the second one doesn't
// is received.
// 
// The same would happen if we use 1234 in all the expectations and all the withdraw.
// Let's solve it:
TEST(AtmMachine, TestSticky2)
{
    // Arrange
    NiceMock<MockBankServer> mock_bankserver;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(1234))
        .Times(AnyNumber())
        .WillRepeatedly(Return(1000));

    EXPECT_CALL(mock_bankserver, GetBalance(1234))
        .Times(1)
        .WillOnce(Return(1000))
        .RetiresOnSaturation(); // This will remove this expectation as soon as it reaches the upper bound (1)

    // Act
    AtmMachine atm_machine(&mock_bankserver);
    bool result1 = atm_machine.withdraw(1234, withdraw_value); // This will be caught by the expectation 1234
    bool result2 = atm_machine.withdraw(1234, withdraw_value); // This will be caught by the expectation 5678
    
    // Asserts
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
}







