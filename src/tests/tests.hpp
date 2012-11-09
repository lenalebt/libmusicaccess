#ifndef TESTS_HPP
#define TESTS_HPP

#include <string>

namespace tests
{
    int testEndsWith();
    int testSoundFile();
    int testResampling();
    int testIIRFilter();
    /**
     * @todo missing: test for float-sortingiirfilter
     */
    int testSortingIIRFilter();
    int showMetadata(std::string filename);
}

#endif //TESTS_HPP
