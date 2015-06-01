/* -*- mode: c++ -*- */

#pragma once

#include "gobj_memory.h"

#include <libgupnp/gupnp-context.h>
#include <QObject>
#include <QString>

namespace upnp {

class Context : public QObject {
    Q_OBJECT
public:
    explicit Context(QObject *parent=nullptr);
    virtual ~Context();

    Context(const Context&) = delete;
    Context &operator=(const Context&) = delete;

    gobj_ptr<GUPnPContext> context;
};

}
