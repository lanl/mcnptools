#include "mcnptools/McnpTools.hpp"
#include "mcnptools/McnpToolsException.hpp"
#include "gtest/gtest.h"

TEST(Exception, HasCertainMessage)
{
    // Test that the expected exception is thrown.
    EXPECT_THROW(
        {
            try {
                mcnptools::Mctal mctal_malformed("test_mctal_malformed");
            }

            catch (const mcnptools::McnpToolsException& e) {
                // Test that the exception has the expected message.
                EXPECT_STREQ(
                    "file test_mctal_malformed does not look like an MCNP mctal file",
                    e.what());
                throw;
            }
        },
        mcnptools::McnpToolsException);
}
