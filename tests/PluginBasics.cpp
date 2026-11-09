#include "helpers/test_helpers.h"
#include <PluginProcessor.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE ("one is equal to one", "[dummy]")
{
    REQUIRE (1 == 1);
}

TEST_CASE ("Plugin instance", "[instance]")
{
    JX11AudioProcessor testPlugin;

    SECTION ("name")
    {
        CHECK_THAT (testPlugin.getName().toStdString(),
            Catch::Matchers::Equals ("JX11"));
    }

    SECTION ("program name")
    {
        // Steinberg's VST3 validator fails plugins whose programs have no name
        CHECK (testPlugin.getProgramName (0).isNotEmpty());
    }
}


#ifdef PAMPLEJUCE_IPP
    #include <ipp.h>

TEST_CASE ("IPP version", "[ipp]")
{
    // Compare against the headers rather than a hardcoded version,
    // since CI's apt install picks up each new IPP release on Linux
    CHECK_THAT (ippsGetLibVersion()->Version, Catch::Matchers::StartsWith (IPP_VERSION_STR));
}
#endif
