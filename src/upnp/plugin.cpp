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

#include "plugin.h"
#include "context.h"
#include "control-point-model.h"
#include "media-renderer.h"
#include "protocol-info.h"
#include "web-resource.h"
#include "utils.h"

namespace upnp {

void UpnpPlugin::registerTypes(const char *uri) {
    qmlRegisterType<Context>(uri, 0, 1, "Context");
    qmlRegisterType<ControlPointModel>(uri, 0, 1, "ControlPointModel");
    qmlRegisterUncreatableType<MediaRenderer>(uri, 0, 1, "MediaRenderer",
        "Use ControlPointModel to access MediaRenderers");
    qmlRegisterType<ProtocolInfo>(uri, 0, 1, "ProtocolInfo");
    qmlRegisterType<WebResource>(uri, 0, 1, "WebResource");
    qmlRegisterSingletonType<Utils>(uri, 0, 1, "Utils",
        [](QQmlEngine*, QJSEngine*) -> QObject* { return new Utils(); });
}

}
