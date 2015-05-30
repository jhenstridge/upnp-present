/* -*- mode: c++ -*- */

#pragma once

#include "gobj_memory.h"
#include "media-renderer.h"

#include <libgupnp/gupnp-control-point.h>
#include <QAbstractListModel>
#include <QString>

#include <vector>

class ControlPointModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(Roles)
public:
    explicit ControlPointModel(QObject *parent=nullptr);
    virtual ~ControlPointModel();

    ControlPointModel(const ControlPointModel&) = delete;
    ControlPointModel &operator=(const ControlPointModel&) = delete;

    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE MediaRenderer *get(int index) const;

    enum Roles {
        RoleDevice,
    };

protected:
    QHash<int,QByteArray> roleNames() const override;

private:
    QHash<int, QByteArray> roles;

    gobj_ptr<GUPnPContext> context;
    gobj_ptr<GUPnPControlPoint> cp;
    std::vector<MediaRenderer*> devices;

    static void device_proxy_available_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data);
    static void device_proxy_unavailable_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data);
};
