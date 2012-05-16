#include "main.hpp"
#include <iostream>
#include <cstdlib>
#include "tests.hpp"


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "you need to tell the program which test it should run." << std::endl;
        std::cout << "call \"" << argv[0] << " testname\"" << std::endl;
        return EXIT_FAILURE;
    }
    std::string testname(argv[1]);
    if (testname == "basename")
        return tests::testBasename();
    else
    {
        std::cout << "test \"" << testname << "\" is unknown." << std::endl;
        return EXIT_FAILURE;
    }
	return 0;
}
