/**
 * \file io_sinks.h
 * \author Graham Riches (graham.riches@live.com)
 * \brief 
 * \version 0.1
 * \date 2021-01-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include <functional>
#include <utility>

/********************************** Types *******************************************/

namespace IOInternals
{

/**
 * \brief implementation of the sink object. Wrapped in a namespace to be somewhat hidden
 * 
 * \tparam Sender type that conforms to the IO actor interface : TODO probably re-define that class :S
 * \tparam Function message handler function
 * \tparam Sender::value_type type of the sender messages
 */
template <typename Sender, typename Function, typename MessageType = typename Sender::value_type>
class SinkImpl {
  public:
    /**
     * \brief create a sink instance
     * 
     * \param sender message sender object
     * \param function message handler function
     */
    SinkImpl(Sender&& sender, Function&& function)
        : _sender(std::move(sender))
        , _function(function) {
        _sender.set_message_emit_handler([this](MessageType&& message) { process_message(std::move(message)); });
    }

    /**
     * \brief function to pass a receive message on to the internally registered function object
     * 
     * \param message rvalue reference to the message. Moved to function.
     */
    void process_message(MessageType&& message) const {
        std::invoke(_function, std::move(message));
    }

  private:
    Sender _sender;
    Function _function;
};

/**
 * \brief helper used in convenience pipe notation operator overload
 * 
 * \tparam Function template function
 */
template <typename Function>
struct SinkHelper {
    Function function;
};

};  // namespace IOInternals

/**
 * \brief helper function to create a new sink object
 * 
 * \param sender message sender object
 * \param function message handler function 
 * \retval new instance of a sink with the attributes passed in
 */
template <typename Sender, typename Function>
auto sink(Sender&& sender, Function&& function) {
    return IOInternals::SinkImpl(std::forward<Sender>(sender), std::forward<Function>(function));
}

/**
 * \brief partially applied version of the sink function to allow operation chaining via pipes
 *  
 * \param function function to bind
 * \retval auto 
 */
template <typename Function>
auto sink(Function&& function) {
    return IOInternals::SinkHelper<Function>{std::forward<Function>(function)};
}

/**
 * \brief operator overload to enable pipe syntax for IO operators :D
 * 
 * \param sender message sender template object
 * \param sink sink function
 * \retval auto 
 */
template <typename Sender, typename Function>
auto operator|(Sender&& sender, IOInternals::SinkHelper<Function> sink) {
    return IOInternals::SinkImpl<Sender, Function>(std::forward<Sender>(sender), std::forward<Function>(sink.function));
}

