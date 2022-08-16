#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;         // "any"

//--------------------------------------------------------------------------------------------------
// EXPECT_CALL with the expectations of different number of callings
// TEST(AtmMachine, TestWithdrawExpectedCalls)
// {
//     // Arrange dependency mock
//     MockBankServer mock_bankserver;

//     // Expectations
//     EXPECT_CALL(mock_bankserver, Connect()) 
//         .Times(1);
//         // It does't return any value, so it is not necessary any .Will*

//     EXPECT_CALL(mock_bankserver, GetBalance(_))
//         .Times(AtLeast(1))
//         .WillOnce(Return(2000));
    
//     EXPECT_CALL(mock_bankserver, Debit(_,_))
//         .Times(AtLeast(1));
    
//     EXPECT_CALL(mock_bankserver, Disconnect())
//         .Times(1);

//     // Actuate (including in this case the construction)
//     AtmMachine atm_machine(&mock_bankserver);
//     bool withdraw_success = atm_machine.withdraw(1234, 1000);

//     // Asserts (Optional, because we can just check the Expectations)
//     EXPECT_TRUE(withdraw_success);
// }
