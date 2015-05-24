#include "control-point-model.h"

#include <stdexcept>
#include <string>

ControlPointModel::ControlPointModel(const QString &target, QObject *parent) {
    GError *error = nullptr;

    context.reset(gupnp_context_new(nullptr, nullptr, 0, &error));
    if (!context) {
        std::string message("Could not create UPnP context: ");
        message += error->message;
        g_error_free(error);
        throw std::runtime_error(message);
    }

    cp.reset(gupnp_control_point_new(context.get(), target.toUtf8().constData()));
}

ControlPointModel::~ControlPointModel() = default;

int ControlPointModel::rowCount(const QModelIndex&) const {
    return 0;
}

QVariant ControlPointModel::data(const QModelIndex &index, int role) const {
    return QVariant();
}
