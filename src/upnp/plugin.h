#pragma once

#include <QtQml>

namespace upnp {

class UpnpPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    virtual void registerTypes(const char *uri) override;
};

}
