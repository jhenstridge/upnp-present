import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0

Dialog {
    id: dialog

    property alias text: label.text

    Label {
        id: label
    }

    Button {
        text: i18n.tr("OK")
        onClicked: PopupUtils.close(dialog)
    }
}
