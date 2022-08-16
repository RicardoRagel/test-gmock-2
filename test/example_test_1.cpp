#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"

using ::testing::Return;
using ::testing::AtLeast;

//--------------------------------------------------------------------------------------------------
// CHECKING THE BEHAVIOUR FOR UNSPECIFIED EXPECTATIONS
// that basically means, that we are not specifying the returned value (like using ON_CALL)
TEST(AtmMachine, TestWithdrawUnspecified)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;

    // Set the expectations for my mock's behaviour (my AtmMachine using that "dummy" bankserver)
    // i.e. What should the mock do?
    // Here we can use "Unspecified", "ON_CALL" or "EXPECT_CALL"

    /* NO SPECIF ANY EXPECTATION */

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(1234, 1000);

    // Asserts (Optional, because we can just check the Expectations)
    std::cout << "(Unspecified) withdraw_success: " << withdraw_success << std::endl;
    EXPECT_FALSE(withdraw_success); // OK. Why does it passed? 

    // It passed because withdraw_success will be the return value of withdraw()
    // that is equal to return value by the MOCK of GetBalance()
    // Due to it is UNSPECIFIED, it will return 0, that is FALSE.
    // The default of the MOCK functions are:
    // void --> void
    // bool --> false
    // others built-in --> 0
    // user defined types --> the default constructor
}
// Notice also that this test will pass but it will appear a lot of GMOCK WARNING:
// Uninteresting mock function call - returning directly
// This means that we haven't specified the behaviour of some mocks, but it is intentional in this case,
// so it is not important

//--------------------------------------------------------------------------------------------------
// CHECKING THE BEHAVIOUR FOR SPECIFIED EXPECTATIONS USING ON_CALL()
// They are specifications about the RETURNED VALUE
TEST(AtmMachine, TestWithdrawOnCall)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    ON_CALL(mock_bankserver, GetBalance(account_number))
        .WillByDefault(Return(2000));
    // Here, we are defining that the GetBalance mock function will return 2000
    // for that account_number.
    // Instead of this account number, GMock matchers can be also used there, for example "_" (= any)

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, value);

    // Asserts (Optional, because we can just check the Expectations)
    std::cout << "(ON_CALL) withdraw_success: " << withdraw_success << std::endl;
    EXPECT_TRUE(withdraw_success); // OK. Why does it passed? 

    // It passed because the GetBalance() returned 2000, so the withdraw() returned true
}

// WE CAN DO THE SAME BUT USING A TEST FIXTURE CLASS TO GROUP ALL OUR SPECIFICATIONS
class AtmMachineTest : public ::testing::Test
{
    public:
        void SetUp() override
        {
            ON_CALL(m_mock_bankserver, GetBalance(m_account_number))
                .WillByDefault(Return(2000));
        }
    
        MockBankServer m_mock_bankserver;
        const int m_account_number = 1234;
        const int m_value = 1000;
};
TEST_F(AtmMachineTest, TestWithdrawOnCall)
{
    // Expectations
    /* They are already on AtmMachineTest fixture, even the mock object */
    
    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&m_mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(m_account_number, m_value);

    // Asserts (Optional, because we can just check the Expectations)
    std::cout << "(ON_CALL) withdraw_success: " << withdraw_success << std::endl;
    EXPECT_TRUE(withdraw_success);
}

//--------------------------------------------------------------------------------------------------
// CHECKING THE BEHAVIOUR FOR SPECIFIED EXPECTATIONS USING EXPECTED_CALL()
// They are specifications about the RETURNED VALUE but also
// They are VERIFICATIONS about the function is ACTUALLY CALLED, otherwise it fails
TEST(AtmMachine, TestWithdrawExpectedCall)
{
    // Arrange dependency mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(account_number))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(2000));

    // Here, we are defining that the GetBalance mock function will be called (before the TEST ends)
    // at least once, and it will return 2000 N times (although this times 1 is enough)
    // Notice that here we are doing the same than using ON_CALL, we are setting the returned value for this
    // mock function but, in addition, we are also checking that is is actually called.

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, value);

    // Asserts (Optional, because we can just check the Expectations)
    std::cout << "(ON_EXPECTED) withdraw_success: " << withdraw_success << std::endl;
    EXPECT_TRUE(withdraw_success); // OK. Why does it passed? 
    // It passed because the GetBalance() returned 2000, so the withdraw() returned true
}

