#include "SeedArguments.h"

#include <chlm/Random.h>

#include <array>
#include <cstdint>
#include <cstdio>

namespace
{
    constexpr std::uint64_t k_default_state_seed{ 0x4d595df4d0f33173ULL };
    constexpr std::uint64_t k_default_stream_seed{ 0x14057b7ef767814fULL };

    void print_usage(const char* executable) noexcept
    {
        std::fprintf(stderr, "Usage: %s [state-seed [stream-seed]]\n", executable);
    }
}

int main(const int argc, char** argv)
{
    if (argc > 3)
    {
        print_usage(argv[0]);
        return 2;
    }

    std::uint64_t state_seed = k_default_state_seed;
    std::uint64_t stream_seed = k_default_stream_seed;
    if ((argc >= 2 && !carrot_random_validation::parse_seed(argv[1], state_seed))
        || (argc == 3 && !carrot_random_validation::parse_seed(argv[2], stream_seed)))
    {
        std::fputs("Seeds must be unsigned decimal or 0x-prefixed hexadecimal integers.\n", stderr);
        print_usage(argv[0]);
        return 2;
    }

    chlm::rng32 rng{ state_seed, stream_seed };
    std::array<std::uint32_t, 16 * 1024> buffer{};

    for (;;)
    {
        for (std::uint32_t& word : buffer)
        {
            word = rng.next_u32();
        }

        if (std::fwrite(buffer.data(), sizeof(buffer.front()), buffer.size(), stdout) != buffer.size())
        {
            return std::ferror(stdout) ? 1 : 0;
        }
    }
}
