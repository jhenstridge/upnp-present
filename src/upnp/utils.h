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

#include <QObject>
#include <QString>
#include <QUrl>

namespace upnp {

class Utils : public QObject {
    Q_OBJECT
public:
    Utils(QObject *parent=nullptr);
    virtual ~Utils();

    Utils(const Utils&) = delete;
    Utils &operator=(const Utils&) = delete;

    Q_INVOKABLE QString makeDIDL(const QString &url,
                                 const QString &upnpClass,
                                 const QString &protocolInfo);
    Q_INVOKABLE QString getMimeType(const QUrl &url);
};

}
