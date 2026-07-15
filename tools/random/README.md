# Random Validation Tools

These opt-in executables expose the raw output of `chlm::rng32` to PractRand and
TestU01. They are development tools and do not add dependencies to CarrotHLM.

Configure and build against a TestU01 installation prefix:

```sh
cmake -S . -B build/random-validation \
  -DBUILD_TESTING=OFF \
  -DCARROTHLM_BUILD_RANDOM_VALIDATION_TOOLS=ON \
  -DCARROTHLM_TESTU01_ROOT="$HOME/.local/testu01"
cmake --build build/random-validation --parallel
```

Run a bounded PractRand test using native 32-bit words:

```sh
build/random-validation/tools/random/CarrotHLM_PractRandStream \
  | "$HOME/.local/bin/RNG_test" stdin32 -tlmin 1MB -tlmax 64MB
```

Run TestU01's batteries (`small` is the default):

```sh
build/random-validation/tools/random/CarrotHLM_TestU01 small
build/random-validation/tools/random/CarrotHLM_TestU01 crush
build/random-validation/tools/random/CarrotHLM_TestU01 big
```

Both adapters accept optional state and stream seeds. Seeds may be unsigned
decimal or `0x`-prefixed hexadecimal values. Keep the reported seeds with test
results so the exact stream can be reproduced.
