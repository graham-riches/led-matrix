/**
 * \file io_server.h
 * \author Graham Riches (graham.riches@live.com)
 * \brief 
 * \version 0.1
 * \date 2021-01-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <boost/asio.hpp>


class IOServer {
    using tcp = boost::asio::ip::tcp;
    using context = boost::asio::io_context;

    public:
        void run() {
            try {
                context io_context;
                tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));

                while ( true ) {
                    tcp::socket socket(io_context);
                    acceptor.accept(socket);
                    std::string hello_message{"hello from server"};
                    boost::system::error_code ignored_error;
                    boost::asio::write(socket, boost::asio::buffer(hello_message), ignored_error );
                }
            }
            catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
};
