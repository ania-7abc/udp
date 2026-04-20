// udp.hpp
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <stop_token>
#include <string>
#include <thread>

#include <boost/asio.hpp>

class UDP
{
  public:
    using Callback =
        std::function<void(const std::string &message, const std::string &sender_ip, uint16_t sender_port)>;

    explicit UDP(uint16_t port);

    ~UDP();
    UDP(const UDP &) = default;
    UDP(UDP &&) = default;
    UDP &operator=(const UDP &) = default;
    UDP &operator=(UDP &&) = default;

    void set_callback(Callback callback);

    void send(const std::string &msg, const std::string &recipient_ip, uint16_t recipient_port);

    void join_multicast_group(const std::string &multicast_ip);
    void leave_multicast_group(const std::string &multicast_ip);
    void set_multicast_interface(const std::string &local_ip);

    [[nodiscard]] auto port() const -> uint16_t;

  private:
    void receive();

    boost::asio::io_context io_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_;
    std::array<char, 1 << 16> buffer_{};
    Callback callback_;
    std::stop_source stop_source_;
    uint16_t port_;
    std::jthread thread_;
};
