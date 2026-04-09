// udp.cpp
#include <exception>
#include <udp/udp.hpp>
#include <utility>

UDP::UDP(uint16_t port)
    : work_guard_(boost::asio::make_work_guard(io_)), thread_([this](std::stop_token st) { io_.run(); }),
      socket_(io_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)), port_(port)
{
}

void UDP::set_callback(Callback callback)
{
    callback_ = std::move(callback);
    receive();
}

void UDP::send(const std::string &msg, const std::string &recipient_ip, uint16_t recipient_port)
{
    auto endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(recipient_ip), recipient_port);
    socket_.async_send_to(boost::asio::buffer(msg), endpoint,
                          [](auto error_code, size_t) { /* игнорируем, можно логировать */ });
}

void UDP::join_multicast_group(const std::string &multicast_ip)
{
    auto address = boost::asio::ip::make_address(multicast_ip);
    socket_.set_option(boost::asio::ip::multicast::join_group(address));
}

void UDP::leave_multicast_group(const std::string &multicast_ip)
{
    auto address = boost::asio::ip::make_address(multicast_ip);
    socket_.set_option(boost::asio::ip::multicast::leave_group(address));
}

void UDP::set_multicast_interface(const std::string &local_ip)
{
    auto address = boost::asio::ip::make_address(local_ip);
    socket_.set_option(boost::asio::ip::multicast::outbound_interface(address.to_v4()));
}

auto UDP::port() const -> uint16_t
{
    return port_;
}

void UDP::receive()
{
    if (!callback_)
        return;

    socket_.async_receive_from(boost::asio::buffer(buffer_), sender_, [this](auto error_code, size_t bytes) {
        if (!error_code && bytes > 0 && callback_)
            callback_(std::string(buffer_.data(), bytes), sender_.address().to_string(), sender_.port());

        if (!stop_source_.stop_requested())
            receive();
    });
}