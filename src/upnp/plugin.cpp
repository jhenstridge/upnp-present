#include "plugin.h"
#include "context.h"
#include "control-point-model.h"
#include "media-renderer.h"
#include "web-resource.h"

namespace upnp {

void UpnpPlugin::registerTypes(const char *uri) {
    qmlRegisterType<Context>(uri, 0, 1, "Context");
    qmlRegisterType<ControlPointModel>(uri, 0, 1, "ControlPointModel");
    qmlRegisterUncreatableType<MediaRenderer>(uri, 0, 1, "MediaRenderer",
        "Use ControlPointModel to access MediaRenderers");
    qmlRegisterType<WebResource>(uri, 0, 1, "WebResource");
}

}
