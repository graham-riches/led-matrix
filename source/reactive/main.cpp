/*! \file main.cpp
*
*  \brief main application entry point
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "canvas.h"
#include "led-matrix.h"
#include "config_parser.h"
#include "nlohmann/json.hpp"
#include "reactive.h"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>


/********************************** Types *******************************************/


/****************************** Function Definitions ***********************************/
/**
 * \brief main application entry point.
 * 
 * \param argc number of command line arguments
 * \param argv space delimited command line arguments
 * \retval int process return value
 */
int main(int argc, char* argv[]) {
    /* open the json configuration file and parse it into an expected ADT which contains either valid JSON or an error message */
    using json = nlohmann::json;    
    json config = json::parse(std::ifstream{"config.json"});    

    auto maybe_options = create_options_from_json(config);
    if (!maybe_options) {
        std::cout << "Error: matrix configuration is invalid\n";
        std::cout << maybe_options.get_error() << std::endl;
        return -1;
    }

    /* create the RGB Matrix object from the validated options. Note: wrap the raw pointer returned from the Matrix factory method in a 
       unique pointer for memory management.
       TODO: matrix has an internal thread that can't be accessed directly, which is a minor inconvenience as it can't share same threading structure
             as other application components
       */
    auto options = maybe_options.get_value();
    auto matrix = std::unique_ptr<rgb_matrix::RGBMatrix>(rgb_matrix::CreateMatrixFromOptions(options.options, options.runtime_options));    
    matrix->StartRefresh();    

    /* create a vector of threads to hold the threads that will do the other application work */
    std::vector<std::thread> threads;

    /* start up a TCP server to receive control messages */
    boost::asio::io_service service;
    auto io_pipeline = IOService(service) |                        
    sink([](const auto& message){ std::cout << message << std::endl; });    

    /* start the IO service */
    threads.push_back(std::thread{ [&service](){service.run();} });        

    /* wait for threads to be done their work */
    for (const auto& thread : threads) {
        thread.join();
    }

    return 0;
}