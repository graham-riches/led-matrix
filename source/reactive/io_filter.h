/**
 * \file io_filter.h
 * \author Graham Riches (graham.riches@live.com)
 * \brief defines filtering operations for reactive IO streams
 * \version 0.1
 * \date 2021-01-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <functional>
#include <utility>


/********************************** Types *******************************************/
namespace IOInternals {

/**
 * \brief filter a reactive stream by a predicate function. Returns only messages that satisfy the predicate.
 * 
 * \tparam Sender sender source object
 * \tparam Predicate filtering function
 * \tparam Sender::value_type type of the passed messages
 */
template <typename Sender, typename Predicate, typename MessageType = typename Sender::value_type>
class FilterImpl {
    public:
    using value_type = MessageType;

    /**
     * \brief Construct a new Filter Impl object
     * 
     * \param sender the sender object
     * \param predicate filtering predicate
     */
    FilterImpl(Sender&& sender, Predicate predicate)
    : _sender(std::move(sender))
    , _predicate(std::move(predicate)) {}

    /**
     * \brief Set the message emitter to pass on the received messages
     *      
     * \param emitter function to emit messages
     */
    template <typename EmitFunction>
    void set_message_emit_handler(EmitFunction emitter) {
        _emitter = emitter;
        //!< create a lambda to register as the emitter for the previous connection in the chain
        _sender.set_message_emit_handler( [this](MessageType&& message){
            process_message(std::move(message));
        });
    }
    
    /**
     * \brief processes a received message by checking the supplied predicate and only forwarding if valid
     * 
     * \param message the received message
     */
    void process_message(MessageType&& message) const {        
        if (std::invoke(_predicate, message)) {
            _emitter(std::move(message));
        }
    }

    private:
    Sender _sender;
    Predicate _predicate;
    std::function<void(MessageType&&) _emitter;

};


/**
 * \brief helper class template to allow partial application of filter implementation
 * 
 * \tparam Predicate the predicate function to bind
 */
template <typename Predicate>
struct FilterHelper {
    Predicate predicate;
};
};


/**
 * \brief helper function to apply a filter to a reactive stream
 * 
 * \param sender source message object
 * \param predicate filtering function
 * \retval filtered stream source
 */
template <typename Sender, typename Predicate>
auto filter(Sender&& sender, Predicate&& predicate) {
    return IOInternals::FilterImpl<Sender, Predicate>(std::forward<Sender>(sender), std::forward<Predicate>(predicate));
}


/**
 * \brief operator | to allow for range-style pipe syntax
 * 
 * \param sender sender message source
 * \param predicate helper struct containing a predicate function
 * \retval auto 
 */
template <typename Sender, typename Predicate>
auto operator |(Sender&& sender, IOInternals::FilterHelper<Predicate> predicate) {
    return IOInternals::FilterImpl<Sender, Predicate>(std::forward<Sender>(sender), std::forward<Predicate>(predicate.predicate));
}
