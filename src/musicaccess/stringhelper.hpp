#ifndef STRINGHELPER_HPP
#define STRINGHELPER_HPP

inline bool endsWith(const std::string& str, const std::string& ending)
{
    if (str.length() >= ending.length())
    {
        return (0 == str.compare(str.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}


#endif //STRINGHELPER_HPP 
