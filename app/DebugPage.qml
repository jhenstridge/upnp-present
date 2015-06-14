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
import Ubuntu.Components.ListItems 1.0 as ListItem
import upnp 0.1

Page {
    id: root
    title: i18n.tr("Media Renderer Info")
    flickable: scrollarea

    property var renderer: null

    Flickable {
        id: scrollarea
        anchors.fill: parent
        contentHeight: contentItem.childrenRect.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            anchors {
                left: parent.left
                right: parent.right
            }

            ListItem.Header {
                text: i18n.tr("Renderer information")
            }

            ListItem.SingleValue {
                text: i18n.tr("Model")
                value: renderer ? renderer.modelName : ""
                visible: value
            }

            ListItem.SingleValue {
                text: i18n.tr("Model number")
                value: renderer ? renderer.modelNumber : ""
                visible: value
            }

            ListItem.SingleValue {
                text: i18n.tr("Manufacturer")
                value: renderer ? renderer.manufacturer : ""
                visible: value
            }

            ListItem.SingleValue {
                text: i18n.tr("State")
                value: renderer ? renderer.transportState : ""
                visible: value
            }

            ListItem.Header {
                text: i18n.tr("Supported formats")
            }

            Repeater {
                model: renderer ? renderer.protocolInfo : []
                delegate: ListItem.MultiValue {
                    text: modelData.mimeType
                    values: [
                        modelData.protocol,
                        modelData.dlnaProfile
                    ]
                }
            }
        }
    }
}
