# UDP

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Boost](https://img.shields.io/badge/Boost-Asio-green.svg)](https://www.boost.org/)

A lightweight C++17 UDP socket wrapper using Boost.Asio with support for unicast and multicast communication.

## Description

This library provides a `UDP` class that simplifies UDP networking operations using Boost.Asio. It supports:
- Sending and receiving UDP messages asynchronously
- Setting up callback-based message handling
- Joining and leaving multicast groups
- Configuring multicast interface binding

All operations are thread-safe and use modern C++ features.

## Requirements

- C++17 compiler
- [Boost](https://www.boost.org/) (components: system, asio)

## Installation

### Using CMake FetchContent

Add the following to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    udp
    GIT_REPOSITORY https://github.com/ania-7abc/udp.git
    GIT_TAG v1.0.0
)

# Optionally, control whether to build shared libraries (default OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

FetchContent_MakeAvailable(udp)

# Link against the library
target_link_libraries(your_target PRIVATE udp)
```

The library exports the target `udp`. Dependencies on Boost are handled automatically.

## Usage

Include the header:

```cpp
#include <udp/udp.hpp>
```

### Creating a UDP Socket

```cpp
// Create a UDP socket bound to port 12345
UDP udp(12345);
```

### Setting Up a Message Callback

```cpp
// Set up a callback to receive messages
udp.set_callback([](const std::string& message, const std::string& sender_ip, uint16_t sender_port) {
    std::cout << "Received from " << sender_ip << ":" << sender_port 
              << " -> " << message << std::endl;
});
```

### Sending Messages

```cpp
// Send a message to a specific IP and port
udp.send("Hello, World!", "192.168.1.100", 54321);
```

### Multicast

```cpp
// Join a multicast group
udp.join_multicast_group("239.255.0.1");

// Set the local interface for multicast
udp.set_multicast_interface("192.168.1.50");

// Leave a multicast group
udp.leave_multicast_group("239.255.0.1");
```

### Stopping the Socket

```cpp
// Clean shutdown
udp.stop();
```

> **Note:** The `UDP` class is non-copyable and non-movable. It manages its own background thread for async I/O.

## API Reference

| Method | Description |
|--------|-------------|
| `UDP(uint16_t port)` | Constructs a UDP socket bound to the given port. |
| `~UDP()` | Destructor that automatically stops the socket. |
| `set_callback(Callback callback)` | Sets the callback for received messages. |
| `stop()` | Stops the socket and joins the background thread. |
| `send(const std::string& msg, const std::string& recipient_ip, uint16_t recipient_port)` | Sends a message to the specified address. |
| `join_multicast_group(const std::string& multicast_ip)` | Joins a multicast group. |
| `leave_multicast_group(const std::string& multicast_ip)` | Leaves a multicast group. |
| `set_multicast_interface(const std::string& local_ip)` | Sets the interface for outgoing multicast. |
| `port() const` | Returns the bound port number. |

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
