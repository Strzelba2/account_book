import QtQuick
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.15
import "qrc:/components"


Rectangle {
    color: "transparent"
    property string pinCode: ""

    function updatePinCode(number)
    {
        if (pinCode.length < 4)
        {
            pinCode += number;
        }
        if (pinCode.length === 4)
        {
            checkPin(pinCode);
        }
    }
    function removeLastDigit() {
        if (pinCode.length > 0) {
            pinCode = pinCode.substring(0, pinCode.length - 1);
        }
    }
    function checkPin(pin) {
        if (viewService.sessionManager.loginState.isRegister)
        {
            console.log("Register: " + pin);
            UsbUser.processAndEncrypt(pin,loginWindow.path);
        }
        else
        {
            UsbUser.processAndDecrypt(pin,loginWindow.path);
        }
    }

    function spacedStars(length) {
       var spacedText = "";
       for (var i = 0; i < length; i++) {
           spacedText += "*";
           if (i < length - 1) {
               spacedText += " ";
           }
       }
       return spacedText;
    }

    Text {
        id: pinLabel
        text: "Unlock with pin code"
        font.pixelSize: 24
        font.family: "arial"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 35
        color: "#cf953e"
    }
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
                console.log("close PIN panel");
                loginWindow.usbConnected = false;
                myLoader.source = "LoginAdmin.qml";
            }
        }
    }

    Text {
        text: spacedStars(pinCode.length)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        font.pixelSize: 40
        anchors.topMargin: 75
     }

    Item {
        id: pinPanel
        width: 280
        height: 280
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 105
        anchors.leftMargin: 45

        Column {
            id: column2
            anchors.horizontalCenter: pinPanel.horizontalCenter
            anchors.bottom: pinPanel.bottom
            spacing: 2

            PinButton{
                id: pinButton2
                text: "2"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton2.text);
                    }
                }
            }

            PinButton{
                id: pinButton5
                text: "5"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton5.text);
                    }
                }
            }

            PinButton{
                id: pinButton8
                text: "8"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton8.text);
                    }
                }
            }
        }

        Column {
            id: column1
            anchors.right: column2.left
            anchors.rightMargin: 2
            anchors.bottom: pinPanel.bottom
            spacing: 2
            PinButton{
                id: pinButton1
                text: "1"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton1.text);
                    }
                }
            }
            PinButton{
                id: pinButton4
                text: "4"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton4.text);
                    }
                }
            }
            PinButton{
                id: pinButton7
                text: "7"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton7.text);
                    }
                }
            }
        }
        Column {
            id: column3
            anchors.left: column2.right
            anchors.leftMargin: 2
            anchors.bottom: pinPanel.bottom
            spacing: 2
            PinButton{
                id: pinButton3
                text: "3"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton3.text);
                    }
                }
            }
            PinButton{
                id: pinButton6
                text: "6"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton6.text);
                    }
                }
            }
            PinButton{
                id: pinButton9
                text: "9"
                enabled: true
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        activityTimer.restart();
                        updatePinCode(pinButton9.text);
                    }
                }
            }
        }
    }
    Item {
        id: row1
        width: 280
        height: 300
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: pinPanel.bottom
        anchors.topMargin: 2

        PinButton{
            id: pinButton0
            text: "0"
            enabled: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            MouseArea {
                anchors.fill: parent
                onClicked:{
                    activityTimer.restart();
                    updatePinCode(pinButton0.text);
                }
            }
        }
    }
    Text {
        id: pinForgot
        text: "Forgot your Pin Code?"
        font.pixelSize: 20
        font.family: "arial"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 35
        color: "#cf953e"
        enabled: true

        MouseArea {
            anchors.fill: parent
            onClicked: console.log("forgoten password")
        }
    }

    Timer {
        id: activityTimer
        interval: 20000
        repeat: false
        onTriggered: {
            console.log( "activityTimer onTriggered");
            loginWindow.usbConnected = false;
            myLoader.source = "LoginAdmin.qml";
        }
    }

    function freezeComponents(freeze)  {
        console.log("onFreezeComponents!" + !freeze)
        pinButton0.enabled = !freeze;
        pinButton1.enabled = !freeze;
        pinButton2.enabled = !freeze;
        pinButton3.enabled = !freeze;
        pinButton4.enabled = !freeze;
        pinButton5.enabled = !freeze;
        pinButton6.enabled = !freeze;
        pinButton7.enabled = !freeze;
        pinButton8.enabled = !freeze;
        pinButton9.enabled = !freeze;
        pinForgot.enabled = !freeze;
    }

    Component.onCompleted: {
        console.log("Pin.qml onCompleted");
        activityTimer.start();
    }
}


