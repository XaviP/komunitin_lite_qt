import QtQuick
import QtQuick.Controls
//import QtQuick.Controls.Material


Window {
    id: appWindow
    width: 640
    height: 480
    visible: true
    property string greenLight: "#a2cc3a"
    property string greenDark: "#78a719"
    property string greenVeryDark: "#386900"
    property string greyLight: "#eeeeee"
    title: qsTr("Komunitin Lite")

//    Material.theme: Material.Light
//    Material.accent: Material.Green

    Column {
        id: columnMain
        anchors.fill: parent


        Rectangle {
            id: rectangleHeader
            width: 600
            height: 50
            color: appWindow.greenLight
            anchors.horizontalCenter: parent.horizontalCenter
            property int widthRectangles: parseInt(width/4)

            Row {
                id: rowHeader
                x: 0
                y: 0
                width: 600
                height: 50

                Rectangle {
                    id: rectangle1
                    width: rectangleHeader.widthRectangles
                    height: 50
                    color: "#00000000"
                    Button {
                        id: button
                        width: 30
                        height: 30
                        text: "\u2630"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        flat: false
                        highlighted: false
                        anchors.leftMargin: 10
                        onClicked: {
                            backend.test_qml()
                            console.log("from qml to console")
                        }
                    }
                }
                Rectangle {
                    id: rectangle2
                    x: rectangleHeader.widthRectangles
                    width: rectangleHeader.widthRectangles
                    height: 50
                    color: "#00000000"
                    ComboBox {
                        id: comboBox
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        flat: false
                        anchors.rightMargin: 10
                    }
                }
                Rectangle {
                    id: rectangle3
                    x: 2*rectangleHeader.widthRectangles
                    width: rectangleHeader.widthRectangles
                    height: 50
                    color: "#00000000"
                    clip: false
                    Label {
                        id: label
                        text: qsTr("Name")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.verticalCenterOffset: 0
                        anchors.leftMargin: 10
                        color: appWindow.greenVeryDark
                    }
                }
                Rectangle {
                    id: rectangle4
                    x: 3*rectangleHeader.widthRectangles
                    width: rectangleHeader.widthRectangles
                    height: 50
                    color: "#00000000"
                    Label {
                        id: label1
                        text: qsTr("Balance")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        color: appWindow.greenVeryDark
                    }
                }
            }
        }
        Rectangle {
            id: rectangleTable
            width: 600
            height: 400
            color: appWindow.greyLight
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
