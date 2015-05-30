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
            spacing: units.gu(1)
            anchors {
                margins: units.gu(2)
                fill: parent
            }
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

            Button {
                text: i18n.tr("Send")
                onClicked: {
                    var renderer = renderers.get(rendererSelector.selectedIndex);
                    var didl = '<?xml version="1.0"?>
<DIDL-Lite xmlns="urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/" xmlns:upnp="urn:schemas-upnp-org:metadata-1-0/upnp/" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:sec="http://www.sec.co.kr/">
<item id="f-0" parentID="0" restricted="0">
    <dc:title>Picture</dc:title>
    <dc:creator>Anonymous</dc:creator>
    <upnp:class>object.item.imageItem.photo</upnp:class>
    <res protocolInfo="http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG;DLNA.ORG_FLAGS=8cf00000000000000000000000000000">{0}</res>
</item>
</DIDL-Lite>';
                    //var uri = "http://192.168.0.12:1234/";
                    var uri = "http://jamesh.id.au/james.jpeg";
                    renderer.setAVTransportURI(uri, didl.replace("{0}", uri));
                    renderer.play(1);
                }
            }
        }
    }
}
