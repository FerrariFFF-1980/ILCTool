#pragma once

#include <string>
#include <cctype>

namespace StringUtils
{
    // Removes leading and trailing whitespace and returns a new string
    inline std::string TrimCopy(const std::string &text)
    {
        auto left = text.begin();
        auto right = text.end();

        while (left != right && std::isspace(static_cast<unsigned char>(*left)))
        {
            ++left;
        }

        while (right != left && std::isspace(static_cast<unsigned char>(*(right - 1))))
        {
            --right;
        }

        return std::string(left, right);
    }
}
