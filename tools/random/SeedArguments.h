#pragma once

#include <charconv>
#include <cstdint>
#include <string_view>

namespace carrot_random_validation
{
    inline bool parse_seed(std::string_view text, std::uint64_t& result) noexcept
    {
        int base = 10;
        if (text.starts_with("0x") || text.starts_with("0X"))
        {
            base = 16;
            text.remove_prefix(2);
        }

        if (text.empty())
        {
            return false;
        }

        const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), result, base);
        return error == std::errc{} && end == text.data() + text.size();
    }
} // namespace carrot_random_validation
