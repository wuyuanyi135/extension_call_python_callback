#include <atomic>
#include <fmt/ostream.h>
#include <iostream>
#include <pybind11/pybind11.h>
#include <thread>
#include <utility>

namespace py = pybind11;
using namespace std::chrono_literals;
class extension_class {
 public:
    explicit extension_class(py::function &callback) {
        fmt::print("Extension class initialized.\n");

        py_callback = std::move(callback);
    }
    virtual ~extension_class() {
        fmt::print("Finalizing the extension class\n");
        if (background_thread.joinable()) {
            finalizing = true;
            background_thread.join();
            fmt::print("Background thread joined\n");
        }
        fmt::print("Bye\n");
    }

    void start_background_thread() {
        background_thread = std::thread(&extension_class::background_worker, this);
        auto id = std::this_thread::get_id();
        fmt::print(
            "Background thread is now running. The spawning thread id is {}. The GIL status is {}\n",
            id, PyGILState_Check());
    }

    void background_worker() {
        while (!finalizing) {
            auto id = std::this_thread::get_id();
            fmt::print("I am thread {}. Reporting from background. The GIL status is {}\n.", id, PyGILState_Check());

            fmt::print("Invoking callback.\n");
            {
                py::gil_scoped_acquire acquire;
                py_callback(invoke_count++);
            }
            fmt::print("Done invoking callback.\n");

            std::this_thread::sleep_for(1s);
        }
        fmt::print("Leaving background worker due to finalizing\n");
    }

 private:
    std::thread background_thread;
    std::atomic<bool> finalizing{false};
    py::function py_callback;
    int invoke_count = 0;
};

PYBIND11_MODULE(extension_call_python_callback, m) {
    py::class_<extension_class>(m, "ExtensionClass")
        .def(py::init<py::function &>())
        .def("start_background_thread", &extension_class::start_background_thread);
}