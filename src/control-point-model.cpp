#include "control-point-model.h"

#include <stdexcept>
#include <string>

namespace {

const char MEDIA_RENDERER[] = "urn:schemas-upnp-org:device:MediaRenderer:1";

}

ControlPointModel::ControlPointModel(QObject *parent)
    : QAbstractListModel(parent) {
    roles[RoleDevice] = "device";

    GError *error = nullptr;
    context.reset(gupnp_context_new(nullptr, nullptr, 0, &error));
    if (!context) {
        std::string message("Could not create UPnP context: ");
        message += error->message;
        g_error_free(error);
        throw std::runtime_error(message);
    }

    cp.reset(gupnp_control_point_new(context.get(), MEDIA_RENDERER));
    g_signal_connect(
        cp.get(), "device-proxy-available",
        G_CALLBACK(&ControlPointModel::device_proxy_available_cb), this);
    g_signal_connect(
        cp.get(), "device-proxy-unavailable",
        G_CALLBACK(&ControlPointModel::device_proxy_unavailable_cb), this);
    gssdp_resource_browser_set_active(GSSDP_RESOURCE_BROWSER(cp.get()), true);
}

ControlPointModel::~ControlPointModel() = default;

void ControlPointModel::device_proxy_available_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data) {
    auto model = reinterpret_cast<ControlPointModel*>(user_data);
    g_object_ref(proxy);
    gobj_ptr<GUPnPDeviceProxy> device(proxy);

    // FIXME: we should try to sort the model by UDN or friendly name
    model->beginInsertRows(QModelIndex(), model->devices.size(), model->devices.size());
    model->devices.emplace_back(new MediaRenderer(device, model));
    model->endInsertRows();
}

void ControlPointModel::device_proxy_unavailable_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data) {
    auto model = reinterpret_cast<ControlPointModel*>(user_data);
    g_object_ref(proxy);
    gobj_ptr<GUPnPDeviceProxy> device(proxy);

    const char *udn = gupnp_device_info_get_udn(GUPNP_DEVICE_INFO(proxy));
    int i;
    decltype(model->devices)::iterator it;
    for (i = 0, it = model->devices.begin(); it != model->devices.end(); i++, it++) {
        MediaRenderer *renderer = *it;
        if (renderer->udn() == udn) {
            model->beginRemoveRows(QModelIndex(), i, i);
            model->devices.erase(it);
            model->endRemoveRows();
            renderer->deleteLater();
            break;
        }
    }
}


QHash<int, QByteArray> ControlPointModel::roleNames() const {
    return roles;
}

int ControlPointModel::rowCount(const QModelIndex&) const {
    return 0;
}

QVariant ControlPointModel::data(const QModelIndex &index, int role) const {
    return QVariant();
}
