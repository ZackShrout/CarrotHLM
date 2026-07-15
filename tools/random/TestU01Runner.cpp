#include "SeedArguments.h"

#include <chlm/Random.h>

extern "C"
{
#include <bbattery.h>
#include <unif01.h>
}

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>

namespace
{
    constexpr std::uint64_t k_default_state_seed{ 0x4d595df4d0f33173ULL };
    constexpr std::uint64_t k_default_stream_seed{ 0x14057b7ef767814fULL };

    chlm::rng32 generator{};

    unsigned int next_bits() noexcept
    {
        static_assert(std::numeric_limits<unsigned int>::digits == 32);
        return generator.next_u32();
    }

    void print_usage(const char* executable) noexcept
    {
        std::fprintf(stderr, "Usage: %s [small|crush|big] [state-seed [stream-seed]]\n", executable);
    }
}

int main(const int argc, char** argv)
{
    if (argc > 4)
    {
        print_usage(argv[0]);
        return 2;
    }

    const char* battery = argc >= 2 ? argv[1] : "small";
    if (std::strcmp(battery, "small") != 0
        && std::strcmp(battery, "crush") != 0
        && std::strcmp(battery, "big") != 0)
    {
        print_usage(argv[0]);
        return 2;
    }

    std::uint64_t state_seed = k_default_state_seed;
    std::uint64_t stream_seed = k_default_stream_seed;
    if ((argc >= 3 && !carrot_random_validation::parse_seed(argv[2], state_seed))
        || (argc == 4 && !carrot_random_validation::parse_seed(argv[3], stream_seed)))
    {
        std::fputs("Seeds must be unsigned decimal or 0x-prefixed hexadecimal integers.\n", stderr);
        print_usage(argv[0]);
        return 2;
    }

    generator.seed(state_seed, stream_seed);
    char name[] = "CarrotHLM rng32 (PCG-XSH-RR)";
    unif01_Gen* test_generator = unif01_CreateExternGenBits(name, &next_bits);
    if (test_generator == nullptr)
    {
        std::fputs("TestU01 failed to create the external generator adapter.\n", stderr);
        return 1;
    }

    if (std::strcmp(battery, "small") == 0)
    {
        bbattery_SmallCrush(test_generator);
    }
    else if (std::strcmp(battery, "crush") == 0)
    {
        bbattery_Crush(test_generator);
    }
    else
    {
        bbattery_BigCrush(test_generator);
    }

    unif01_DeleteExternGenBits(test_generator);
    return 0;
}
