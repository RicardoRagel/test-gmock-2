#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Define some matchers
using ::testing::StartsWith;
using ::testing::MatchesRegex;
using ::testing::StrNe;
using ::testing::Eq;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::IsTrue;
using ::testing::FloatEq;
using ::testing::FloatNear;
using ::testing::IsNull;
using ::testing::IsNan;
using ::testing::Ref;
using ::testing::AllOf;
using ::testing::Each;
using ::testing::Conditional;
using ::testing::HasSubstr;
using ::testing::Field;
using ::testing::Property;
using ::testing::Truly;

