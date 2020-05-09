
#include <cctype>
#include <string>
#include  <iostream>

void strip_leading_spaces(std::string& str) {
    int i = 0;
    while (std::isspace(str[i]) && i < str.length()) ++i;
    str = str.substr(i, str.length() - i + 1);
}

void strip_trailing_spaces(std::string& str) {
    while (std::isspace(str.back())) str.pop_back();
}

void strip_spaces(std::string& str) {
    strip_leading_spaces(str);
    strip_trailing_spaces(str);
}


void test_str_utils() {
    {
        std::cout << "test strip_leading_spaces" << std::endl;
        std::string tmp = "       123     ";
        strip_leading_spaces(tmp);
        std::cout << tmp << std::endl;

        tmp = "123     ";
        strip_leading_spaces(tmp);
        std::cout << tmp << std::endl;

        tmp = "123";
        strip_leading_spaces(tmp);
        std::cout << tmp << std::endl;

        tmp = "1";
        strip_leading_spaces(tmp);
        std::cout << tmp << std::endl;

        tmp = " 1";
        strip_leading_spaces(tmp);
        std::cout << tmp << std::endl;

        tmp = "";
        strip_leading_spaces(tmp);
        std::cout << tmp << std::endl;
    }

    {
        std::cout << "test strip_trailing_spaces" << std::endl;
        std::string tmp = "       123     ";
        strip_trailing_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = "123     ";
        strip_trailing_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = "123";
        strip_trailing_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = "1";
        strip_trailing_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = " 1";
        strip_trailing_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = "";
        strip_trailing_spaces(tmp);
        std::cout << tmp << "!" << std::endl;
    }
    {
        std::cout << "test strip_spaces" << std::endl;
        std::string tmp = "       123     ";
        strip_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = "123     ";
        strip_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = "123";
        strip_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = "1";
        strip_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = " 1";
        strip_spaces(tmp);
        std::cout << tmp << "!" << std::endl;

        tmp = "";
        strip_spaces(tmp);
        std::cout << tmp << "!" << std::endl;
    }
}