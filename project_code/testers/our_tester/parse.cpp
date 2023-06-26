#include "../../srcs/parsing/parsing_lib.hpp"


void runSplitTests();

int main()
{
    runSplitTests();

    // return (0);
}
void runSplitTests()
{
    std::vector<std::pair<std::string, std::string> > testCases;
    testCases.push_back(std::make_pair("Accept Accept-Charset Accept-Encoding Accept-Language Authorization Cache-Control Connection Cookie Date Expect Host If-Match If-Modified-Since If-None-Match If-Range If-Unmodified-Since Max-Forwards Pragma Range Referer TE User-Agent", " "));

    // testCases.push_back(std::make_pair("Hello World", " "));
    // testCases.push_back(std::make_pair("This is a test", " "));
    // testCases.push_back(std::make_pair("Another example", " "));
    // testCases.push_back(std::make_pair("", " "));
    // testCases.push_back(std::make_pair("Single", " "));
    // testCases.push_back(std::make_pair("One|Two|Three", "|"));
    // testCases.push_back(std::make_pair("Lorem|ipsum|dolor|sit|amet|consectetur|adipiscing|elit", "|"));
    // testCases.push_back(std::make_pair("Lorem ipsum dolor sit amet, consectetur adipiscing elit", " "));
    // testCases.push_back(std::make_pair("|||", "|"));
    // testCases.push_back(std::make_pair("|Hello|World|", "|"));
    // testCases.push_back(std::make_pair("Hello||World", "|"));
    // testCases.push_back(std::make_pair("Hello|World|", "|"));
    // testCases.push_back(std::make_pair("Hello|World||", "|"));
    // testCases.push_back(std::make_pair("||Hello|World", "|"));
    // testCases.push_back(std::make_pair("Hello||||World", "|"));
    // testCases.push_back(std::make_pair("Hello|World||||", "|"));
    // testCases.push_back(std::make_pair("||||Hello|World||||", "|"));
    for (std::vector<std::pair<std::string, std::string> >::const_iterator it = testCases.begin(); it != testCases.end(); ++it)
    {
        const std::string& input = it->first;
        const std::string& delimiter = it->second;

        std::cout << "Input: \"" << input << "\", Delimiter: \"" << delimiter << "\"" << std::endl;

        std::vector<std::string> result = split(input, delimiter);

        std::cout << "Result: [\n";
        for (std::vector<std::string>::const_iterator it = result.begin(); it != result.end(); ++it)
        {
            std::cout << "\"" << *it << ":\", " << std::endl;
        }
        std::cout << "]" << std::endl;

        std::cout << "---------------------" << std::endl;
    }
}
