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
