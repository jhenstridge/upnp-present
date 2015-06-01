/* -*- mode: c++ -*- */

#pragma once

#include "gobj_memory.h"
#include "context.h"
#include "media-renderer.h"

#include <libgupnp/gupnp-control-point.h>
#include <QAbstractListModel>
#include <QPointer>
#include <QString>

#include <vector>

namespace upnp {

class ControlPointModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(Roles)
    Q_PROPERTY(upnp::Context* context READ getContext WRITE setContext NOTIFY contextChanged)
public:
    explicit ControlPointModel(QObject *parent=nullptr);
    virtual ~ControlPointModel();

    ControlPointModel(const ControlPointModel&) = delete;
    ControlPointModel &operator=(const ControlPointModel&) = delete;

    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE upnp::MediaRenderer *get(int index) const;

    enum Roles {
        RoleDevice,
    };

Q_SIGNALS:
    void contextChanged();

protected:
    QHash<int,QByteArray> roleNames() const override;

    Context *getContext() const;
    void setContext(Context *new_context);

private:
    QHash<int, QByteArray> roles;

    QPointer<Context> context;
    gobj_ptr<GUPnPControlPoint> cp;
    std::vector<MediaRenderer*> devices;

    static void device_proxy_available_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data);
    static void device_proxy_unavailable_cb(GUPnPControlPoint *cp, GUPnPDeviceProxy *proxy, void *user_data);
};

}
