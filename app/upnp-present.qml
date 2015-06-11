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
            onShowDebugPage: pageStack.push(debug)
        }

        ContentPickerPage {
            id: picker
            visible: false

            onDone: pageStack.pop()
        }

        DebugPage {
            id: debug
            visible: false
            renderer: main.renderer
        }
    }
}
