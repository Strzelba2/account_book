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
        placeholderText: "Admin Login"
        enabled: true
        onTextChanged: loginWindow.restartPopupTimer()
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
        anchors.bottomMargin: 215
        anchors.left: loginWindow.left
        anchors.leftMargin: 90
        placeholderText: "Password"
        echoMode: TextInput.Password
        enabled: true
        onTextChanged: loginWindow.restartPopupTimer()
        onAccepted:
        {
            attemptLogin();
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
        anchors.topMargin: 460
        font.pointSize: 16
        font.family: "arial"
        colorPressed: "#d9d7d4"
        colorMouseOver: "#bfbdbb"
        colorDefault: "#b3b2b1"
        enabled: true

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
                viewService.loginAdmin(textUsername.text , textPassword.text);
                textUsername.text = "";
                textPassword.text = "";
            }
        }

    Connections {
        target: viewService.sessionManager
        function onLoginAdminFinished (success) {
            console.log("Admin is logged successfully!" + success);
            if (success)
            {
                loginWindow.usbBlock = true;
                myLoader.source = "Register.qml";
            }
        }
    }

}
