import QtQuick.Controls

Dialog {
    width: 300
    height: 200
    title: "Login"
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: console.log("Ok clicked")
    onRejected: console.log("Cancel clicked")

    Text {
        id: emailTextEdit
    }
    Text {
        id: passwordTextEdit
    }
    }

