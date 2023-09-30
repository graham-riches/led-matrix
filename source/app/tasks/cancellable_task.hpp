#pragma once

#include <atomic>
#include <functional>
#include <thread>

namespace tasks
{

class cancellable_task {
  public:
    // Should return true to keep the task running, false to stop the task
    using TaskFunction = std::function<bool()>;

    cancellable_task(TaskFunction f)
        : task_function(f)
        , cancellation_token(false) { }

    void cancel() {
        try {
            cancellation_token = true;
            task_thread.join();
        } catch ( ... ) {
            // TODO: Empty exception handler
        }
    }

    void start() {
        auto task = [&]() {
            bool continue_task = true;
            while ( continue_task && !cancellation_token ) {
                continue_task = task_function();
            }
        };
        task_thread = std::thread(task);
    }

    void await_complete() {
        task_thread.join();
    }

  private:
    TaskFunction task_function;
    std::atomic<bool> cancellation_token;
    std::thread task_thread;
};

};  // namespace tasks