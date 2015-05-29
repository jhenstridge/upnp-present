import QtQuick 2.0
import Ubuntu.Components 1.1
import upnp 0.1

MainView {
    id: root
    objectName: "mainView"
    applicationName: "foo"
    useDeprecatedToolbar: false

    width: units.gu(40)
    height: units.gu(70)

    Page {
        title: i18n.tr("Send Picture")

        ControlPointModel {
            id: renderers
        }

        Column {
            anchors.fill: parent
            Label {
                text: i18n.tr("Pick picture")
                fontSize: "large"
            }
            Button {
                text: i18n.tr("Pick")
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
        }
    }
}
