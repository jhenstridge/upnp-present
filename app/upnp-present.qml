import QtQuick 2.0
import Ubuntu.Components 1.1
import upnp 0.1 as UPnP

MainView {
    id: main
    objectName: "mainView"
    applicationName: "upnp-present.jamesh"
    useDeprecatedToolbar: false

    width: units.gu(40)
    height: units.gu(70)

    PageStack {
        id: pageStack

        Component.onCompleted: pageStack.push(root)

        Page {
            id: root
            visible: false
            title: i18n.tr("Send Picture")

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
                            source: picker.item ? picker.item.url : ""
                        }
                    }
                    Button {
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n.tr("Pick")
                        onClicked: pageStack.push(picker)
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
                        text: i18n.tr("Send")
                        onClicked: {
                            var renderer = renderers.get(rendererSelector.selectedIndex);
                            if (!renderer) {
                                console.log("No renderer selected");
                                return;
                            }
                            var contentType = UPnP.Utils.getMimeType(resource.contentUri);
                            var protocolInfo, contentFeatures;
                            for (var i = 0; i < renderer.protocolInfo.length; i++) {
                                protocolInfo = renderer.protocolInfo[i];
                                var parts = protocolInfo.split(":");
                                contentFeatures = parts[3];
                                if (parts[0] === "http-get" || parts[2] === contentType) {
                                    break;
                                }
                            }
                            if (i >= renderer.protocolInfo.length) {
                                console.log("No matching protocol");
                                return;
                            }
                            resource.clearHeaders();
                            resource.addHeader("Content-Type", contentType);
                            resource.addHeader("contentFeatures.dlna.org", contentFeatures);
                            var didl = UPnP.Utils.makeDIDL(resource.uri, "object.item.imageItem.photo", protocolInfo);
                            renderer.setAVTransportURI(resource.uri, didl);
                            renderer.play(1);
                        }
                    }

                    Button {
                        text: i18n.tr("Clear")
                        onClicked: {
                            var renderer = renderers.get(rendererSelector.selectedIndex);
                            renderer.stop();
                        }
                    }
                }
            }
        }

        ContentPickerPage {
            id: picker
            visible: false

            onDone: pageStack.pop()
        }
    }
}
