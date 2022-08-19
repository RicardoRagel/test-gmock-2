#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"

using ::testing::NiceMock;
using ::testing::MockFunction;
using ::testing::InSequence;
using ::testing::Return;

//--------------------------------------------------------------------------------------------------
// CHECKPOINTS
//
// Basically, checkpoints allow us to check if the test has passed through a specific way.
//
// We need to define another Mock function to be called for these checkpoints. To do that
// we can create it by ourself (similar to MockBankServer, no BankServer is necessary) OR
// create it using the provided by GMock: 
//
//      MockFunction<return_type(arg_type arg_name)> checkpoint;
// 
// Then, we can call to checkpoint.Call(arg_value) to emit this checkpoint where we want.
//
// To caught (expect_call) them, we need to specify the expected calls for these Call() 
// mock function and, obviously, in a inSequence scope!!!

TEST(AtmMachine, TestCheckpoints1)
{
    // Arrange
    NiceMock<MockBankServer> mock_bankserver;
    MockFunction<void(std::string msg)> checkpoint;
    const int available_balance = 5000;

    // Expectations
    {
        InSequence seq; // Notice that it retires the satisfied expectations, as we can check using this checkpoint

        EXPECT_CALL(mock_bankserver, GetBalance(1234))          // It must be reached the first
            .WillOnce(Return(available_balance));
        EXPECT_CALL(mock_bankserver, Debit(1234, 1000));        // It must be reached the second    

        EXPECT_CALL(checkpoint, Call("1"));                     // It must be reached the third

        EXPECT_CALL(mock_bankserver, GetBalance(1234))          // It must be reached the forth
            .WillOnce(Return(available_balance));
        EXPECT_CALL(mock_bankserver, Debit(1234, 1000));        // It must be reached the fifth
    }

    // Acts
    AtmMachine atm_machine(&mock_bankserver);
    bool result1 = atm_machine.withdraw(1234, 1000);
    checkpoint.Call("1");
    bool result2 = atm_machine.withdraw(1234, 1000);
}


// Another example to try to understand it better: Let's suppose that we will call
// 3 times to withdraw, and the second one with a bigger withdraw value than the
// available, Debit() should not be called. Let's check adding some checkpoints
// to confirm it easily:
TEST(AtmMachine, TestCheckpoints2)
{
    // Arrange
    NiceMock<MockBankServer> mock_bankserver;
    MockFunction<void(std::string msg)> checkpoint;
    const int available_balance = 5000;

    // Expectations
    {
        InSequence seq; // Notice that it retires the satisfied expectations, as we can check using this checkpoint

        EXPECT_CALL(mock_bankserver, GetBalance(1234))          
            .WillOnce(Return(available_balance));
        EXPECT_CALL(mock_bankserver, Debit(1234, 1000));        

        EXPECT_CALL(checkpoint, Call("1"));

        EXPECT_CALL(mock_bankserver, GetBalance(1234))          
            .WillOnce(Return(available_balance));
        
        EXPECT_CALL(checkpoint, Call("2"));

        EXPECT_CALL(mock_bankserver, GetBalance(1234))          
            .WillOnce(Return(available_balance));
        EXPECT_CALL(mock_bankserver, Debit(1234, 1000));        
    }

    // Acts
    AtmMachine atm_machine(&mock_bankserver);
    bool result1 = atm_machine.withdraw(1234, 1000);
    checkpoint.Call("1");
    bool result2 = atm_machine.withdraw(1234, 1000000);
    checkpoint.Call("2");
    bool result3 = atm_machine.withdraw(1234, 1000);
}
