#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockBankServer.hpp"
#include "AtmMachine.hpp"
#include <exception>

using ::testing::Expectation;
using ::testing::DoDefault;
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
// In these series of test we will use the different MOCK ACTIONS that we can use
// with the EXPECT_CALL members .Will*()
//
// The first one is DoDefault(). It means that it will do what you have specified in a 
// previous ON_CALL() function
class TestFixture : public ::testing::Test
{
    public:
        void SetUp() override
        {
            // Here we are defining what is the default behavior fro DoDefault()
            ON_CALL(m_mock_bankserver, GetBalance).WillByDefault(Return(1000));
        }

        MockBankServer m_mock_bankserver;
};
TEST_F(TestFixture, TestDoDefault)
{
    // Expectations
    EXPECT_CALL(m_mock_bankserver, GetBalance(_))
        .WillOnce(DoDefault());

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&m_mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(1234, 1000);
}
TEST_F(TestFixture, TestDoDefaultOverwritten)
{
    // Expectations
    EXPECT_CALL(m_mock_bankserver, GetBalance(_))
        .WillOnce(Return(1000));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&m_mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(1234, 1000);
}

//--------------------------------------------------------------------------------------------------
// Another Mock Actions are the called "SIDE EFFECTS"
//
// For example, in the same way that we have used DoDefault() and Return(),
// we can use:
//
// * Assign(&variable, value) to assign a value to a target variable in our test
// 
// For example, let's check if the function Disconnect() is been called. Like this function
// doesn't return any value, we can assign one to a variable, that could be for example in 
// our FixtureClass.
class TestFixture2 : public ::testing::Test
{
    public:
    void SetUp() override
    {
        m_disconnected = false;
    }

    MockBankServer m_mock_bankserver;
    bool m_disconnected;
    const int m_account_number = 1234;
    const int m_withdraw_value = 1000;
};
TEST_F(TestFixture2, TestAssign)
{
    // Expectations
    EXPECT_CALL(m_mock_bankserver, Disconnect())
        .WillOnce(Assign(&m_disconnected, true));

    // Actuate (including in this case the construction)
    AtmMachine atm_machine(&m_mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(1234, 1000);

    // Asserts
    if(m_disconnected)
    {
        std::cout << "-- Disconnected successfully --" << std::endl;

        // Do some other stuff that need that it is disconnected
    }
}

// Let's do a more complex example test using DoAll() and SaveArg<>():
TEST_F(TestFixture2, TestSaveArgs)
{
    // Expectations: Let's expect the function Debit is called and let's save the values
    // which the ones this function is called in 2 variables.
    EXPECT_CALL(m_mock_bankserver, GetBalance(_))
        .WillOnce(Return(1000));
    
    int debit_account_number = -1;
    int debit_withdraw_value = -1;
    EXPECT_CALL(m_mock_bankserver, Debit(_,_))
        .WillOnce(DoAll(SaveArg<0>(&debit_account_number), SaveArg<1>(&debit_withdraw_value)));

    // Note:    DoAll() simply executes multiple input actions.
    //          SaveArg<n>() save the arg n which the mock function was called

    // Actuate: Let's call the withdraw function with the expected values
    AtmMachine atm_machine(&m_mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(m_account_number, m_withdraw_value);

    // Asserts: Let's check the ones that arrives to Debit() are correctly the same
    // that the ones we send to withdraw()
    EXPECT_EQ(m_account_number, debit_account_number);    
    EXPECT_EQ(m_withdraw_value, debit_withdraw_value);    
}
// Notice that we are also setting a mock calling to GetBalance -> Return(1000)
// Why?
// If no GetBalance mock expectations with a Return value > 0, the default returned 
// value will be 0, that means that the Debit function will never be called.

// Finally, let's check how we can also use exceptions throws:
class InvalidValueException : public std::exception 
{
    public:
        const char * what () const throw () 
        {
            return "My InvalidValueException message";
        }
};
TEST(AtmMachine, TestThrowException)
{
    // Arrange mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, Connect())
        .Times(1)
        .WillRepeatedly(Throw(InvalidValueException()));
    
    // Acts AND ASSERTS
    AtmMachine atm_machine(&mock_bankserver);

    // WE CAN'T DO THIS DIRECTLY, THE EXCEPTION WILL MAKE THE TEST/PROGRAM FAILs
    // bool withdraw_success = atm_machine.withdraw(account_number, withdraw_value);

    // So, we use the new assert function: EXPECT_THROW, to act and assert at the same time. 
    EXPECT_THROW(
                    atm_machine.withdraw(account_number, withdraw_value),
                    InvalidValueException
                );
}
// Another way to do it directly with EXPECT_THAT()
TEST(AtmMachine, TestThrowException2)
{
    // Arrange mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, Connect())
        .Times(1)
        .WillRepeatedly(Throw(InvalidValueException()));
    
    // Acts AND ASSERTS
    AtmMachine atm_machine(&mock_bankserver);

    // WE CAN'T DO THIS DIRECTLY, THE EXCEPTION WILL MAKE THE TEST/PROGRAM FAILs
    // bool withdraw_success = atm_machine.withdraw(account_number, withdraw_value);

    // Another way to act and catch the exception is using EXPECT_THAT of the lambda function
    // to act on, and using the matcher ThrowsMessage<>
    EXPECT_THAT (
                    [&]() { atm_machine.withdraw(account_number, withdraw_value); },
                    ThrowsMessage<InvalidValueException>(StrEq("My InvalidValueException message"))
                );
}

//--------------------------------------------------------------------------------------------------
// Another Mock Actions to Invoke Functions
// Let's suppose I want to return the argument of GetBalance and passing through another function:
double square(double x)
{
    std::cout << "Computing square of " << x << std::endl; 
    return ( x * x );
}
TEST(AtmMachine, TestInvoke)
{
    // Arrange mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(_))
        .Times(1)
        .WillOnce(Invoke(square));  // Notice that this is using the same arg that we passed to GetBalance,
                                    // in this case, 1234 (withdraw -> GetBalance -> Invoke)
        
        // In any case, the return value of the mock function will be what Invoke is returning
        
        // Actually, it is the same that to do .WillOnce(square)
    
    // Acts
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, withdraw_value);
}
// But... what if I want to specify the arg to square()??
// Easy, don't use Invoke, use directly Return():
TEST(AtmMachine, TestInvoke2)
{
    // Arrange mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(_))
        .Times(1)
        .WillOnce(Return(square(33)));
    
    // Acts
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, withdraw_value);
}
// But... what if I want to directly return the value of GetBalance?
// Easy, don't use Invoke, use directly ReturnArg<n>():
TEST(AtmMachine, TestInvoke3)
{
    // Arrange mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(_))
        .Times(1)
        .WillOnce(ReturnArg<0>());
    
    // Acts
    AtmMachine atm_machine(&mock_bankserver);
    bool withdraw_success = atm_machine.withdraw(account_number, withdraw_value);
}

//--------------------------------------------------------------------------------------------------
// Composite Actions
// Basically, some Actions that allow us to apply multiple actions one by one
// Actually, WE HAVE ALREADY SEE ONE, search for the previous TEST using DoAll(),
// but let's check how to use the action WithArg<n>() in the following test:
TEST(AtmMachine, TestWithArg)
{
    // Arrange mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, DoubleTransaction(_,_,_))
        .Times(1)
        .WillOnce( DoAll(
                            WithArg<0>(Invoke(square)),
                            WithArg<1>(Invoke(square)),
                            WithArg<2>(Invoke(square))
                        )
                    );
    
    // Acts (Let's act DIRECTLY ON THE MOCK)
    EXPECT_EQ(mock_bankserver.DoubleTransaction(2, 4, 6), 36);

    // 36 ?????????
    // Sure thing, DoAll() execute them sequentially, and the last one is returning 6 * 6
}

//--------------------------------------------------------------------------------------------------
// Custom Actions
//
// It is necessary define them as functors, if it is possible, templated.
class Double
{
    public:
        template <typename T>
        T operator()(T arg)
        {
            return (2 * arg);
        }
};
TEST(AtmMachine, TestCustomAction)
{
    // Arrange mock
    MockBankServer mock_bankserver;
    const int account_number = 1234;
    const int withdraw_value = 1000;

    // Expectations
    EXPECT_CALL(mock_bankserver, GetBalance(_))
        .Times(1)
        .WillOnce(Double{}); // {} ? Because the functor object has to be constructed, it is a class
                             // No args? Then, the same arg than the mock function
    
    // Acts (Let's act DIRECTLY ON THE MOCK)
    EXPECT_EQ(mock_bankserver.GetBalance(33), 66);
}





