A C++ 17 implementation of [qntm's base65536](https://github.com/qntm/base65536) that runs at compile time.
With alternatives for C++ 11 and C++ 14 that runs at runtime.

Useage:

At compile time:

```cpp
#include "base65536/base65536.h"

static constexpr char testInput[] = "Hello World";
constexpr auto test = Base65536::encode(testInput);
constexpr auto data = Base65536::decode(test);
static_assert(data.success);
```

runtime time:

```cpp
#include "base65536/base65536.h"

const std::string testInput = "Hello World";
constexpr auto test = Base65536::encode(testInput);
bool fail = false; // use this to check for errors
constexpr auto data = Base65536::decode(test, fail);
if (fail) {}
```