import QtQuick 2.0
import MyComponents 1.0

import "qrc:/components"


Item {

    TOTPManager {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 35
        id: qrCodeItem
        width: 200
        height: 200
        qrImage: viewService.sessionManager.totpManager.qrImage
    }

    CustomButton {
        id: btnScaned
        width: 320
        height: 50
        opacity: 1
        text: "SCANED"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.topMargin: 390
        font.pointSize: 16
        font.family: "arial"
        colorPressed: "#d9d7d4"
        colorMouseOver: "#bfbdbb"
        colorDefault: "#b3b2b1"

        MouseArea {
            id: mauselogin
            anchors.fill: parent
            onClicked: viewService.saveSecret()
        }
    }

    Component.onCompleted: {
            loginWindow.changeImageOpacity(0.5);
        }

    Connections {
        target: viewService.sessionManager
        function onSecredSaved (success)
        {
            console.log(" registration successful!" + success);
            if (success)
            {
                myLoader.source = "Login.qml";
            }
        }
    }
}
