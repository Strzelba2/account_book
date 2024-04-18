import QtQuick
import QtQuick.Controls 2.3
import com.enum 1.0
import "qrc:/components"

Item {

    property bool condition: true
    property bool showRegister: true
    focus: true

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
                viewService.dbClose()
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
        enabled: true
    }

    CustomTextField {
        id: textPassword
        x: 50
        y: 195
        opacity: 1
        anchors.bottom: textEmail.top
        anchors.bottomMargin: 20
        anchors.left: loginWindow.left
        anchors.leftMargin: 90
        placeholderText: "Password"
        echoMode: TextInput.Password
        enabled: true
    }

    CustomTextField {
        id: textEmail
        x: 50
        y: 195
        opacity: 1
        anchors.bottom: textCompany.top
        anchors.bottomMargin: 20
        anchors.left: loginWindow.left
        anchors.leftMargin: 90
        placeholderText: "Email"
        enabled: true
    }

    CustomTextField {
        id: textCompany
        x: 50
        y: 195
        opacity: 1
        anchors.bottom: btnRegister.top
        anchors.bottomMargin: 90
        anchors.left: loginWindow.left
        anchors.leftMargin: 90
        placeholderText: "Company"
        enabled: true
    }

    CustomButton {
        id: btnRegister
        width: 320
        height: 50
        opacity: 1
        text: "REGISTER"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.topMargin: 460
        font.pointSize: 16
        font.family: "arial"
        colorPressed: "#d9d7d4"
        colorMouseOver: "#d1d0cf"
        colorDefault: "#dedddc"
        borderButton: true
        enabled: true

        MouseArea {
            id: mauseregister
            anchors.fill: parent
            onClicked: {
                register()
            }
        }
    }
    function freezeComponents(freeze)  {
        console.log("onFreezeComponents!" + !freeze)
        btnRegister.enabled = !freeze;
        textCompany.enabled = !freeze;
        textEmail.enabled = !freeze;
        textPassword.enabled = !freeze;
        textUsername.enabled = !freeze;
    }

    function areAllFieldsValid() {
        return textUsername.text.trim() !== "" &&
               textPassword.text.trim() !== "" &&
               textEmail.text.trim() !== "" &&
               textCompany.text.trim() !== "";
    }

    function checkEmptyFields() {
        var emptyFields = [];

        if (textUsername.text.trim() === "") {
            emptyFields.push("Username");
        }
        if (textPassword.text.trim() === "") {
            emptyFields.push("Password");
        }
        if (textEmail.text.trim() === "") {
            emptyFields.push("Email");
        }
        if (textCompany.text.trim() === "") {
            emptyFields.push("Company");
        }

        return emptyFields.join(", ");
    }

    function showEmptyFieldsPopup() {
        var emptyFields = checkEmptyFields();
        if (emptyFields !== "") {
            var message = "Please enter inputs: " + emptyFields;
            showPopup(message, false);
        } else {
            showPopup("unexpected error", false);
        }
    }

    function register(){
        if (areAllFieldsValid()){
            viewService.registerUser(textUsername.text,textPassword.text,textEmail.text,textCompany.text)
        }else{
            showEmptyFieldsPopup()
        }
    }


    Keys.onPressed: function(event) {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                register()
            }
        }




    Connections {
        target: viewService
        function onDbClosedStatus (success)
        {
            console.log("DB closed successfully!" + success)
            if (success)
            {
                showRegister = false
                loginWindow.usbBlock = false;
                myLoader.source = "LoginAdmin.qml";
            }
        }
        function onRegisterStatus (success)
        {
            console.log("Register User!" + success)
            if (success)
            {
                loginWindow.popupText = "black";
                loginWindow.usbBlock = false;
                showPopup("please put pendrive to USB and select PIN with four letters",true);
                console.log("please put pendrive to USB" + success);
            }
        }
    }
}


