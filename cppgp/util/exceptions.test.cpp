#include <cppgp/util/exceptions.hpp>
#include <gtest/gtest.h>


TEST(util_exceptions, test_Error){
    try{
        throw util::exceptions::Error("My Message");
        FAIL();
    }
    catch(util::exceptions::Error& e){
        EXPECT_STREQ("My Message", e.getCustomMessage().c_str())
            << "Function 'getCustomMessage' did not return as expected.";

        EXPECT_STREQ("Unknown error: My Message", e.message().c_str())
            << "Function 'message' did not return as expected.";

        return; // End test at end of catch block.
    }
    FAIL(); // Fail if not finished the catch block.
}


TEST(util_exceptions, test_InconsistentInputError){
    try{
        throw util::exceptions::InconsistentInputError("My Message");
        FAIL();
    }
    catch(util::exceptions::InconsistentInputError& e){
        EXPECT_STREQ("My Message", e.getCustomMessage().c_str())
            << "Function 'getCustomMessage' did not return as expected.";

        EXPECT_STREQ("Inconsistent input: My Message", e.message().c_str())
            << "Function 'message' did not return as expected.";

        return; // End test at end of catch block.
    }
    FAIL(); // Fail if not finished the catch block.
}


TEST(util_exceptions, test_throwException){
    try{
        util::exceptions::throwException<util::exceptions::InconsistentInputError>("Some error message");
        FAIL();
    }
    catch(util::exceptions::InconsistentInputError& e){
        EXPECT_STREQ("Inconsistent input: Some error message", e.message().c_str())
            << "Function 'message' did not return as expected.";

        return; // End test at end of catch block.
    }
    FAIL(); // Fail if not finished the catch block.
}
