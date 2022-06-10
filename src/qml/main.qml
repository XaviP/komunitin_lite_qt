import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


ApplicationWindow {
    visible: true

    Material.theme: Material.Light
    Material.accent: Material.Green

    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Menu")
            Action { text: qsTr("&Reload transfers") }
            Action { text: qsTr("&New transfer...") }
            Action { text: qsTr("&New user...") }
            MenuSeparator { }
            Action { text: qsTr("&Exit") }
        }
    }

    Column {
        anchors.centerIn: parent

        RadioButton { text: qsTr("Small") }
        RadioButton { text: qsTr("Medium");  checked: true }
        RadioButton {
            text: qsTr("Large")
            onToggled: backend.test_qml()
        }
    }
}
