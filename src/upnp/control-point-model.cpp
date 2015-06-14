/*
 * upnp-present: Send photos to a UPnP media renderer
 * Copyright (C) 2015  James Henstridge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "control-point-model.h"

#include <stdexcept>
#include <string>

namespace {
const char MEDIA_RENDERER[] = "urn:schemas-upnp-org:device:MediaRenderer:1";
}

namespace upnp {

ControlPointModel::ControlPointModel(QObject *parent)
    : QAbstractListModel(parent) {
    roles[RoleDevice] = "device";
}

ControlPointModel::~ControlPointModel() {
    shutdownControlPoint();
}

void ControlPointModel::shutdownControlPoint() {
    beginResetModel();
    for (auto &renderer : devices) {
        renderer->deleteLater();
    }
    devices.clear();
    endResetModel();
    if (device_available_id != 0) {
        g_signal_handler_disconnect(cp.get(), device_available_id);
        device_available_id = 0;
    }
    if (device_unavailable_id != 0) {
        g_signal_handler_disconnect(cp.get(), device_unavailable_id);
        device_unavailable_id = 0;
    }
    cp.reset();
}


Context *ControlPointModel::getContext() const {
    return context;
}

void ControlPointModel::setContext(Context *new_context) {
    if (new_context == context) return;

    shutdownControlPoint();
    context = new_context;
    if (context) {
        cp.reset(gupnp_control_point_new(context->context.get(), MEDIA_RENDERER));
        device_available_id = g_signal_connect(
            cp.get(), "device-proxy-available",
            G_CALLBACK(&ControlPointModel::device_proxy_available_cb), this);
        device_unavailable_id = g_signal_connect(
            cp.get(), "device-proxy-unavailable",
            G_CALLBACK(&ControlPointModel::device_proxy_unavailable_cb), this);
        gssdp_resource_browser_set_active(GSSDP_RESOURCE_BROWSER(cp.get()), true);
    }

    Q_EMIT contextChanged();
}

QQmlListProperty<MediaRenderer> ControlPointModel::getDevices() const {
    return QQmlListProperty<MediaRenderer>(
        const_cast<ControlPointModel*>(this), nullptr,
        [](QQmlListProperty<MediaRenderer> *prop) -> int {
            auto cp = static_cast<ControlPointModel*>(prop->object);
            return cp->devices.size();
        },
        [](QQmlListProperty<MediaRenderer> *prop, int index) -> MediaRenderer* {
            auto cp = static_cast<ControlPointModel*>(prop->object);
            if (index < 0 || index >= (ptrdiff_t)cp->devices.size()) {
                return nullptr;
            }
            return cp->devices.at(index);
        });
}

void ControlPointModel::device_proxy_available_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data) {
    auto model = reinterpret_cast<ControlPointModel*>(user_data);
    g_object_ref(proxy);
    gobj_ptr<GUPnPDeviceProxy> device(proxy);

    // FIXME: we should try to sort the model by UDN or friendly name
    model->beginInsertRows(QModelIndex(), model->devices.size(), model->devices.size());
    model->devices.emplace_back(new MediaRenderer(device, model));
    model->endInsertRows();
    Q_EMIT model->devicesChanged();
}

void ControlPointModel::device_proxy_unavailable_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data) {
    auto model = reinterpret_cast<ControlPointModel*>(user_data);
    g_object_ref(proxy);
    gobj_ptr<GUPnPDeviceProxy> device(proxy);

    int i;
    decltype(model->devices)::iterator it;
    for (i = 0, it = model->devices.begin(); it != model->devices.end(); i++, it++) {
        MediaRenderer *renderer = *it;
        if (renderer->device.get() == proxy) {
            model->beginRemoveRows(QModelIndex(), i, i);
            model->devices.erase(it);
            model->endRemoveRows();
            Q_EMIT model->devicesChanged();
            renderer->deleteLater();
            break;
        }
    }
}


QHash<int, QByteArray> ControlPointModel::roleNames() const {
    return roles;
}

int ControlPointModel::rowCount(const QModelIndex&) const {
    return devices.size();
}

QVariant ControlPointModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= (ptrdiff_t)devices.size()) {
        return QVariant();
    }
    switch (role) {
    case RoleDevice:
        return QVariant::fromValue(devices[index.row()]);
    default:
        return QVariant();
    }
}

}
