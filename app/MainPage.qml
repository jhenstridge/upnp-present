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

import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0
import upnp 0.1 as UPnP

Page {
    id: root
    title: i18n.tr("Send Picture")

    readonly property var renderer: renderers.devices[rendererSelector.selectedIndex]
    // Data from the content hub
    property var contentItem

    signal showContentPicker
    signal showDebugPage

    function showError(message) {
        PopupUtils.open(Qt.resolvedUrl("ErrorDialog.qml"), root, {"text": message});
    }

    UPnP.Context {
        id: upnpContext
    }

    UPnP.ControlPointModel {
        id: renderers
        context: upnpContext
    }

    UPnP.WebResource {
        id: resource
        context: upnpContext
        path: "/resource/"
        contentUri: picker.item ? picker.item.url : ""
    }

    UPnP.ProtocolInfo {
        id: resourceInfo
        protocol: "http-get";
        dlnaConversion: UPnP.ProtocolInfo.None
        dlnaFlags: UPnP.ProtocolInfo.BackgroundTransferMode | UPnP.ProtocolInfo.DlnaV15
    }

    Column {
        spacing: units.gu(1)
        anchors {
            margins: units.gu(2)
            fill: parent
        }
        Label {
            text: i18n.tr("Pick picture")
            fontSize: "large"
        }
        Row {
            spacing: units.gu(1)
            UbuntuShape {
                width: units.gu(20)
                height: units.gu(20)
                Image {
                    width: parent.width
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source: contentItem ? contentItem.url : ""
                }
            }
            Button {
                anchors.verticalCenter: parent.verticalCenter
                text: i18n.tr("Pick")
                onClicked: root.showContentPicker()
            }
        }

        Label {
            text: i18n.tr("Pick output")
            fontSize: "large"
        }
        OptionSelector {
            id: rendererSelector
            anchors {
                left: parent.left
                right: parent.right
            }
            model: renderers
            expanded: true
            delegate: OptionSelectorDelegate {
                iconSource: device.iconUrl
                constrainImage: true
                text: device.friendlyName
                subText: device.manufacturer
            }
        }

        Row {
            spacing: units.gu(1)

            Button {
                id: sendButton
                text: i18n.tr("Send")
                onClicked: {
                    if (!renderer) {
                        showError("No renderer selected");
                        return;
                    }
                    resourceInfo.mimeType = UPnP.Utils.getMimeType(resource.contentUri);
                    resourceInfo.dlnaProfile = "";
                    for (var i = 0; i < renderer.protocolInfo.length; i++) {
                        var info = renderer.protocolInfo[i];
                        if (resourceInfo.isCompatible(info)) {
                            resourceInfo.dlnaProfile = info.dlnaProfile;
                            break;
                        }
                    }
                    resource.clearHeaders();
                    resource.addHeader("Content-Type", resourceInfo.mimeType);
                    resource.addHeader("contentFeatures.dlna.org", resourceInfo.asString().split(":")[3]);
                    var didl = UPnP.Utils.makeDIDL(resource.uri, "object.item.imageItem.photo", resourceInfo.asString());
                    if (!renderer.setAVTransportURI(resource.uri, didl)) {
                        showError("Failed to SetAVTransportURI");
                        return;
                    }
                    renderer.play(1);
                }
            }

            Button {
                text: i18n.tr("Clear")
                onClicked: {
                    renderer.stop();
                }
            }
        }
    }

    Label {
        id: pi
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: units.gu(1)
        }
        text: "\u03C0"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.showDebugPage()
            }
        }
    }
}
