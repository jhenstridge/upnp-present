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
import Ubuntu.Content 1.1

Page {
    id: root

    property ContentItem item
    property var activeTransfer

    signal done

    function _setTransfer(transfer) {
        if (root.activeTransfer) {
            root.item = null
            root.activeTransfer.finalize();
        }
        root.activeTransfer = transfer;
        if (root.activeTransfer.state == ContentTransfer.Charged) {
            root.item = root.activeTransfer.items[0];
        }
    }

    Connections {
        target: root.activeTransfer

        onStateChanged: {
            if (root.activeTransfer.state == ContentTransfer.Charged) {
                root.item = root.activeTransfer.items[0];
            }
        }
    }

    Connections {
        target: ContentHub

        onImportRequested: {
            root._setTransfer(transfer);
        }

        onShareRequested: {
            root._setTransfer(transfer);
        }
    }

    ContentPeerPicker {
        id: picker
        contentType: ContentType.Pictures
        handler: ContentHandler.Source

        onPeerSelected: {
            peer.selectionType = ContentTransfer.Single;
            root._setTransfer(peer.request());
            done();
        }

        onCancelPressed: {
            done();
        }
    }
}
