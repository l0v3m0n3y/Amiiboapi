# Amiiboapi
api for amiiboapi.com site Amiibo database that holds all amiibo information
# main
```cpp
#include "Amiiboapi.h"
#include <iostream>

int main() {
   Amiiboapi api;

    auto characters = api.character_list().then([](json::value result) {
        std::cout << result<< std::endl;
    });
    characters.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```

