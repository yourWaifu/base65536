A C++ 17 implementation of qntm's base65536 that runs at compile time.
With alternatives for C++ 11 and C++ 14 that runs at runtime.

Useage:

At compile time:

```cpp
#include "base65536/base65536.h"

static constexpr char testInput[] = "Hello World";
constexpr auto test = encode(testInput);
constexpr auto data = decode(test);
static_assert(std::string_view{testInput}.compare(std::string_view{data}) == 0);
```

runtime time:

```cpp
#include "base65536/base65536.h"

const std::string testInput = "Hello World";
constexpr auto test = encode(testInput);
constexpr auto data = decode(test);
```