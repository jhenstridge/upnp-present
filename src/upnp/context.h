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
