#include "context.h"
#include <stdexcept>
#include <string>

namespace upnp {

Context::Context(QObject *parent)
    : QObject(parent) {
    GError *error = nullptr;
    context.reset(gupnp_context_new(nullptr, nullptr, 0, &error));
    if (!context) {
        std::string message("Could not create UPnP context: ");
        message += error->message;
        g_error_free(error);
        throw std::runtime_error(message);
    }
}

Context::~Context() = default;

}
