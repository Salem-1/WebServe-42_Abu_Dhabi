#include <iostream>
#include <string>
#include <exception>

int main()
{
    std::string a = "hi";
    try
    {
        if (a == "hi")
        {
            a = "hello";
                throw (std::runtime_error("change hi to hello"));
        }
                // throw (std::runtime_error("change hi to hello"));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        // std::cout << "inside exception" << a << std::endl; 
    }
    std::cout << "a = " << a << std::endl; 
    
}