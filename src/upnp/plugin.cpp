#include "plugin.h"
#include "control-point-model.h"
#include "media-renderer.h"

namespace upnp {

void UpnpPlugin::registerTypes(const char *uri) {
    qmlRegisterType<ControlPointModel>(uri, 0, 1, "ControlPointModel");
    qmlRegisterUncreatableType<MediaRenderer>(uri, 0, 1, "MediaRenderer",
        "Use ControlPointModel to access MediaRenderers");
}

}
