#include "main.hpp"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cctype>

#include "tests.hpp"
#include "testframework.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "you need to tell the program which test it should run." << std::endl;
        std::cout << "call \"" << argv[0] << " testname\"" << std::endl;
        return EXIT_FAILURE;
    }
    std::string testname(argv[1]);
    std::transform(testname.begin(), testname.end(), testname.begin(), ::tolower);
    
    std::cout << "running test \"" << testname << "\"..." << std::flush << std::endl;
    if (testname == "basename")
        return tests::testBasename();
    else if (testname == "endswith")
        return tests::testEndsWith();
    else if (testname == "soundfile")
        return tests::testSoundFile();
    else if (testname == "iirfilter")
        return tests::testIIRFilter();
    else if (testname == "sortingiirfilter")
        return tests::testSortingIIRFilter();
    else if (testname == "resampling")
        return tests::testResampling();
    else if (testname == "showmetadata")
    {
        if (argc < 3)
        {
            std::cout << "this test needs extra parameters:" << std::endl;
            std::cout << "call \"" << argv[0] << " showmetadata filename\"" << std::endl;
            return EXIT_FAILURE;
        }
        return tests::showMetadata(argv[2]);
    }
    else
    {
        std::cout << "test \"" << testname << "\" is unknown." << std::endl;
        return EXIT_FAILURE;
    }
    
	return EXIT_SUCCESS;
}


