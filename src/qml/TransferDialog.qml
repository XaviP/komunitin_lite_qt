import QtQuick.Controls

Dialog {
    title: "New Transfer"
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    width: 300
    height: 200

    onAccepted: console.log("Ok clicked")
    onRejected: console.log("Cancel clicked")
}
