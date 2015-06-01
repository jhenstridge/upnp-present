/* -*- mode: c++ -*- */

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
