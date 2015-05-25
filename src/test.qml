import QtQuick 2.0
import Ubuntu.Components 1.1
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
                iconSource: device.iconUrl
                constrainImage: true
                text: device.friendlyName
                subText: device.modelName
            }
        }
    }
}
