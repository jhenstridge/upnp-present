/* -*- mode: c++ -*- */

#pragma once

#include "gobj_memory.h"

#include <libgupnp/gupnp-control-point.h>
#include <QAbstractListModel>
#include <QString>

class ControlPointModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(Roles)
public:
    ControlPointModel(const QString &target, QObject *parent=nullptr);
    virtual ~ControlPointModel();

    ControlPointModel(const ControlPointModel&) = delete;
    ControlPointModel &operator=(const ControlPointModel&) = delete;

    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    enum Roles {
        RoleUDN,
        RoleUrlBase,
        RoleLocation,
    };
private:
    gobj_ptr<GUPnPContext> context;
    gobj_ptr<GUPnPControlPoint> cp;
};
