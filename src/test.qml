import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.ListItems 0.1
import Ubuntu.Components.Popups 0.1
import upnp 0.1

MainView {
    id: root
    objectName: "mainView"
    applicationName: "foo"
    useDeprecatedToolbar: false

    Page {
        title: i18n.tr("Send Picture")

        ControlPointModel {
            id: renderers
        }

        OptionSelector {
            id: rendererSelector
            anchors.fill: parent
            model: renderers
            expanded: true
            delegate: OptionSelectorDelegate {
                text: device.friendlyName
            }
        }
    }
}
