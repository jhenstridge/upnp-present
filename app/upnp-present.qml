import QtQuick 2.0
import Ubuntu.Components 1.1

MainView {
    id: root
    objectName: "mainView"
    applicationName: "upnp-present.jamesh"
    useDeprecatedToolbar: false

    width: units.gu(40)
    height: units.gu(70)

    PageStack {
        id: pageStack

        Component.onCompleted: pageStack.push(main)

        MainPage {
            id: main
            visible: false

            contentItem: picker.item
            onShowContentPicker: pageStack.push(picker)
        }

        ContentPickerPage {
            id: picker
            visible: false

            onDone: pageStack.pop()
        }
    }
}
