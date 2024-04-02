import QtQuick 2.12
import QtQuick.Controls 2.3

import "qrc:/components"

Item {
    CustomButton {
        id: btnClose
        width: 30
        height: 30
        opacity: 1
        visible: true
        text: "X"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 365
        clip: false
        anchors.topMargin: 22
        colorPressed: "#d9d7d4"
        font.family: "Segoe UI"
        colorMouseOver: "#bfbdbb"
        colorDefault: "#b3b2b1"
        font.pointSize: 16

        MouseArea {
            id: myMouseId
            anchors.fill: parent
            onClicked:{
                loginWindow.close();
            }
        }
    }

    CustomTextField {
        id: textUsername
        x: 50
        y: 185
        opacity: 1
        anchors.bottom: textPassword.top
        anchors.bottomMargin: 20
        anchors.left: loginWindow.left
        anchors.leftMargin: 50
        placeholderText: "Username"
        onAccepted:
        {
            attemptLogin();
        }
    }

    CustomTextField {
        id: textPassword
        x: 50
        y: 195
        opacity: 1
        anchors.bottom: btnLogin.top
        anchors.bottomMargin: 145
        anchors.left: loginWindow.left
        anchors.leftMargin: 90
        placeholderText: "Password"
        echoMode: TextInput.Password
        onAccepted:
        {
            attemptLogin();
        }
    }

    Switch {
        id: rememberMe
        checked: true

        Text {
            width: 151
            height: 36
            text: qsTr("Remember me")
            font.family: "Helvetica"
            font.pointSize: 10
            color: "grey"
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 65
        }

        indicator: Rectangle{
            implicitWidth: 40
            implicitHeight: 20
            x: rememberMe.width - width - rememberMe.rightPadding
            y: parent.height / 2 - height / 2
            radius: 10
            color:  rememberMe.hovered ? "#c6c8cc" : "#d6d8dc"
            border.color: "grey"

            Rectangle {
               x: rememberMe.checked ? parent.width - width : 0
               width: 20
               height: 20
               radius: 10
               border.color: "grey"
           }

        }

        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.bottom: textPassword.bottom
        font.styleName: "Normalny"
        font.pointSize: 10
        anchors.bottomMargin: -125
        onToggled: {
            if(checked) {
                console.log("switch on");
            } else {
                console.log("switch off");
            }
        }
    }

    Text {
        y: 340
        width: 150
        height: 35
        text: qsTr("Forgotten password?")
        font.family: "Helvetica"
        font.pointSize: 10
        color: "#cf953e"
        anchors.left: rememberMe.left
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 183

        MouseArea {
            anchors.fill: parent
            onClicked: console.log("forgoten password")
        }
    }

    CustomButton {
        id: btnLogin
        width: 320
        height: 50
        opacity: 1
        text: "LOGIN"
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
            onClicked:{
                attemptLogin();
            }
        }
    }
    function freezeComponents(freeze)  {
        console.log("onFreezeComponents!" + !freeze);
        textPassword.enabled = !freeze;
        btnLogin.enabled = !freeze;
        textUsername.enabled = !freeze;
    }

    function attemptLogin() {
        if(textPassword.text.trim() === "" && textUsername.text.trim() !== ""){
            showPopup("please enter a password",false);
            textPassword.forceActiveFocus();
        }
        if(textUsername.text.trim() === "" && textPassword.text.trim() !== ""){
            showPopup("please enter a username",false);
            textUsername.forceActiveFocus();
        }
        if (textPassword.text.trim() !== "" && textUsername.text.trim() !== "") {
            viewService.login(textUsername.text , textPassword.text);
            textUsername.text = "";
            textPassword.text = "";
        }
    }
}
