/**
 * \file io_service.h
 * \author Graham Riches (graham.riches@live.com)
 * \brief classes for managing io as an asynchronous service
 * \version 0.1
 * \date 2021-01-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <boost/asio.hpp>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

/********************************** Types *******************************************/
/**
 * \brief asynchronous session handling class that will continue to receive messages on a session object
 *        until otherwise notified.
 * 
 * \tparam EmitFunction message handler function to emit received messages to another actor
 */
template <typename EmitFunction>
class io_session : public std::enable_shared_from_this<io_session<EmitFunction>> {
    using tcp = boost::asio::ip::tcp;

  public:
    /**
     * \brief Construct a new io_session object
     * 
     * \param socket rvalue reference to a socket object
     * \param emitter emit function which passes received messages to a handler function
     */
    io_session(tcp::socket&& socket, EmitFunction emitter)
        : _socket(std::move(socket))
        , _emitter(emitter) { }

    /**
     * \brief start the session service
     */
    void start() {
        async_read();
    }

  private:
    //!< alias to remove some boilerplate while using std::enable_shared_from_this below
    using shared_session = std::enable_shared_from_this<io_session<EmitFunction>>;

    /**
     * \brief asynchronously read from a session until a new line is received. The new line is then passed
     *        to any message handlers registered to the session and the session is preserved by re-starting the 
     *        read function, which creates a new shared_ptr reference count to the current object.
     * 
     */
    void async_read() {
        auto self = shared_session::shared_from_this();  //!< creates a new std::shared_ptr reference to the current object
        boost::asio::async_read_until(_socket, _data, '\n', [this, self](auto& error, auto size) {
            std::istream in_stream(&_data);
            std::string line;
            std::getline(in_stream, line);
            _emitter(std::move(line));

            async_read();  //!< re-start the read to keep the session alive
        });
    }

    //!< private members
    boost::asio::streambuf _data;
    tcp::socket _socket;
    EmitFunction _emitter;
};

/**
 * \brief helper function to create a shared pointer instance to a session that owns a socket and
 *        emitter function.
 * 
 * \param socket universal reference to the socket object
 * \param emitter universal reference to the emitter function
 * \retval auto shared_ptr to the session
 */
template <typename Socket, typename EmitFunction>
auto make_shared_session(Socket&& socket, EmitFunction&& emitter) {
    return std::make_shared<io_session<EmitFunction>>(std::forward<Socket>(socket), std::forward<EmitFunction>(emitter));
}

/**
 * \brief IO service object that will create a TCP endpoint, receive messages asynchronously, and will 
 *        call whatever emit function is registered to the session
 */
class io_service {
    using tcp = boost::asio::ip::tcp;

  public:
    //!< convenience alias to let other classes pluck off the message type of the session. TODO: could be templated to be more generic
    using value_type = std::string;

    /**
     * \brief Construct a new io_service object
     * 
     * \param service reference to a boost io_service object
     * \param port connection port the server will listen on
     */
    explicit io_service(boost::asio::io_service& service, uint16_t port = 1234)
        : _acceptor(service, tcp::endpoint{tcp::v4(), port})
        , _socket(service) { }

    //!< disable copy construction and allow moves
    io_service(const io_service& other) = delete;
    io_service(io_service&& other) = default;

    /**
     * \brief Set the emit handler function, which will pass any received messages to a user-specified function
     * 
     * \tparam EmitFunction type of the function
     * \param emitter message handler function
     */
    template <typename EmitFunction>
    void set_message_emit_handler(EmitFunction emitter) {
        //!< once a handler is register, start accepting connections as the messages have somewhere to go!
        _emitter = emitter;
        accept_connections();
    }

  private:
    /**
     * \brief accept connections over TCP and create a new session object for each client
     */
    void accept_connections() {
        //!< wait for connections, and pass a lambda as the connection handler function
        _acceptor.async_accept(_socket, [this](const boost::system::error_code& error) {
            if ( !error ) {
                //!< move the ownership of the socket into the session and start the session
                auto session = make_shared_session(std::move(_socket), _emitter);
                session->start();
            } else {
                std::cerr << error.message() << std::endl;
            }

            accept_connections();  //!< listen for new connections
        });
    }

    //!< private members
    tcp::acceptor _acceptor;
    tcp::socket _socket;
    std::function<void(std::string&&)> _emitter;
};