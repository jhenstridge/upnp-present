/* -*- mode: c++ -*- */
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

#pragma once

#include "gobj_memory.h"
#include "context.h"
#include "media-renderer.h"

#include <libgupnp/gupnp-control-point.h>
#include <QAbstractListModel>
#include <QPointer>
#include <QQmlListProperty>
#include <QString>

#include <vector>

namespace upnp {

class ControlPointModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(Roles)
    Q_PROPERTY(upnp::Context* context READ getContext WRITE setContext NOTIFY contextChanged)
    Q_PROPERTY(QQmlListProperty<upnp::MediaRenderer> devices READ getDevices NOTIFY devicesChanged)

public:
    explicit ControlPointModel(QObject *parent=nullptr);
    virtual ~ControlPointModel();

    ControlPointModel(const ControlPointModel&) = delete;
    ControlPointModel &operator=(const ControlPointModel&) = delete;

    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    enum Roles {
        RoleDevice,
    };

Q_SIGNALS:
    void contextChanged();
    void devicesChanged();

protected:
    QHash<int,QByteArray> roleNames() const override;

    Context *getContext() const;
    void setContext(Context *new_context);
    QQmlListProperty<MediaRenderer> getDevices() const;

private:
    QHash<int, QByteArray> roles;

    QPointer<Context> context;
    gobj_ptr<GUPnPControlPoint> cp;
    std::vector<MediaRenderer*> devices;

    unsigned int device_available_id = 0;
    unsigned int device_unavailable_id = 0;

    void shutdownControlPoint();

    static void device_proxy_available_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data);
    static void device_proxy_unavailable_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data);
};

}
