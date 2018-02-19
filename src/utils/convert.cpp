#include <string>

int str_to_int(std::string value)
{
    if(value[0] == '0' && value[1] == 'x')
        return std::stoi(value, nullptr, 16);
    return std::stoi(value);
}

char str_to_byte(std::string value)
{
    return (char)str_to_int(value);
}

