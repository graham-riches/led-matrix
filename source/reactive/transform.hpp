/**
 * \file io_transform.hpp 
 * \brief defines transformation operators on reactive IO streams
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
namespace reactive
{
namespace internals
{
/**
 * \brief internal implementation of the transformation actor
 * 
 * \tparam Sender sender type
 * \tparam Transformation the transformation function type
 * \tparam SourceMessageType type of message the source is sending
 * \tparam MessageType type of message the transform is emitting
 */
template <typename Sender,
          typename Transformation,
          typename SourceMessageType = typename Sender::value_type,
          typename MessageType = decltype(std::declval<Transformation>()(std::declval<SourceMessageType>()))>
class transform_impl {
  public:
    using value_type = MessageType;

    /**
     * \brief Construct a new transform helper object
     * 
     * \param sender sender/source message
     * \param transformation transformation function
     */
    transform_impl(Sender&& sender, Transformation transformation)
        : _sender(std::move(sender))
        , _transformation(transformation) { }

    /**
     * \brief Set the message emitter to pass on the received messages
     *      
     * \param emitter function to emit messages
     */
    template <typename EmitFunction>
    void set_message_emit_handler(EmitFunction emitter) {
        _emitter = emitter;
        // create a lambda to register as the emitter for the previous connection in the chain
        _sender.set_message_emit_handler([this](SourceMessageType&& message) { process_message(std::move(message)); });
    }

    /**
     * \brief process a message of one type through the supplied transformation and emit it as a new message type
     * 
     * \param message the source message before transformation
     */
    void process_message(SourceMessageType&& message) const {
        _emitter(std::invoke(_transformation, std::move(message)));
    }

  private:
    Sender _sender;
    Transformation _transformation;
    std::function<void(MessageType&&)> _emitter;
};

/**
 * \brief helper struct to allow partial application of transforms
 * 
 * \tparam Transformation transformation function
 */
template <typename Transformation>
struct transform_helper {
    Transformation transformation;
};
};  // namespace internals

/**
 * \brief helper function to apply a transformation to a reactive stream
 * 
 * \param sender sender/source message object
 * \param transformation transformation function to apply
 * \retval new message stream transformed
 */
template <typename Sender, typename Transformation>
auto transform(Sender&& sender, Transformation&& transformation) {
    return reactive::internals::transform_impl(std::forward<Sender>(sender), std::forward<Transformation>(transformation));
}

namespace operators
{
/**
 * \brief helper to partially apply a transformation
 * 
 * \param transformation the transformation that is partially applied/bound
 * \retval helper struct that will later get converted to a complete transform actor
 */
template <typename Transformation>
auto transform(Transformation&& transformation) {
    return reactive::internals::transform_helper<Transformation>{std::forward<Transformation>(transformation)};
}

/**
 * \brief operator | to allow for range-style pipe syntax for reactive streams
 * 
 * \param sender sender/source message object
 * \param transformation transformation function to apply
 * \retval transformation actor
 */
template <typename Sender, typename Transformation>
auto operator|(Sender&& sender, reactive::internals::transform_helper<Transformation> transformation) {
    return reactive::internals::transform_impl<Sender, Transformation>(std::forward<Sender>(sender), transformation.transformation);
}

};  // namespace operators
};  // namespace reactive
