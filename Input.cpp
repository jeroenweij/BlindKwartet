#include <iostream>
#include <string>

#include "Input.h"

int Input::GetInt(const int max)
{
    std::string ans;
    int         i = -100;

    while (i < 0 || i > max)
    {
        try
        {
            std::cin >> ans;
            i = std::stoi(ans);
        }
        catch (const std::exception& e)
        {
            std::cout << "Exception " << e.what() << std::endl;
        }
    }
    return i;
}

std::string Input::GetString()
{
    std::string ans;
    std::cin >> ans;

    return ans;
}
