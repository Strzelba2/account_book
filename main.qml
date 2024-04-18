import QtQuick 2.12
import QtQuick.Controls 2.3
import com.enum 1.0

import "qrc:/components"

Window {
    id: loginWindow
    width: 420
    height: 560
    visible: true
    title: qsTr("Login")

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    property string path: ""
    property bool freezeWindow: false
    property color popupText: "red"
    property bool popupOpen: false
    property int secondsRemaining: 30
    property bool usbConnected: false
    property bool usbBlock: false
    property var changeImageOpacity: function(newOpacity) {
            image.opacity = newOpacity;
    }
    property var customButtonClickedFunction: function(popup,reload) {
        console.log("loginWindow.customButtonClickedFunction");
        loginWindow.changeImageOpacity(1);
        var state = viewService.sessionManager.loginState.currentState;
        if(reload)
        {
            var oldSource = myLoader.source;
            myLoader.source = "";
            if(state === LoginState.Register){
                myLoader.source = "LoadWindow.qml";
                myLoader.item.restartProgressTimer();
                myLoader.item.handleUpdateCountdown("30");
                oldSource = "";
            }else if (oldSource.toString() === "Register.qml"){
                myLoader.source = "Register.qml";
            }else{
                myLoader.source = "LoginAdmin.qml";
            }
            loginWindow.restartPopupTimer();
        }
        else
        {
            myLoader.item.freezeComponents(false);
        }
        loginWindow.freezeWindow = false;
        loginWindow.popupOpen = false;

        popup.destroy();
    }

    Image {
        id: image
        width: 540
        source: "qrc:/images/Images/pelna-ksiegowosc.jpg"
        scale: 1
        clip: true
        opacity: 1.0

        MouseArea {
            anchors.fill: parent
            property point lastMousePos: Qt.point(0, 0)
            onPressed: {
                if(!freezeWindow)
                {
                lastMousePos = Qt.point(mouseX, mouseY);
                }
            }
            onMouseXChanged: {
                if(!freezeWindow)
                {
                loginWindow.x += (mouseX - lastMousePos.x);
                }
            }
            onMouseYChanged: {
                if(!freezeWindow)
                {
                loginWindow.y += (mouseY - lastMousePos.y);
                }
            }
        }
    }
    Loader {
        id: myLoader
        anchors.fill: parent
        source: "LoginAdmin.qml"
        onLoaded: {
            console.log("loginWindow.Loader.onLoaded");
            var state = viewService.sessionManager.loginState.currentState;
            handleStateChange(state);
        }
    }

    function handleStateChange(state) {
        console.log("loginWindow.handleStateChange");
        var newSource = "";
        switch (state) {
            case LoginState.LoginAdmin:
                loginWindow.stopPopupTimer();
                if(loginWindow.usbConnected)
                {
                    newSource = "Pin.qml";
                    break;
                }
                else if (myLoader.item.showRegister)
                {
                    newSource = "Register.qml";
                    break;
                }

                else
                {
                    newSource = "LoginAdmin.qml";
                    popupTimer.start();
                    break;
                }

            case LoginState.Register:
                if(usbConnected)
                {
                    loginWindow.stopPopupTimer();
                    newSource =  "Pin.qml";
                    break;
                }
                else if (!myLoader.item.showLoadWindow)
                {
                    console.log("showLoadWindow");
                    loginWindow.restartPopupTimer();
                    newSource = "LoginAdmin.qml";
                    break;
                }
                else
                {
                    console.log("Current state: Register");
                    newSource =  "LoadWindow.qml";
                    popupTimer.start();
                    break;
                }
            case LoginState.PIPUSB:
                newSource = "QRCode.qml";
                loginWindow.stopPopupTimer();
                break;
            case LoginState.QRCode:
                if (myLoader.item.showTotp)
                {
                    newSource = "Totp.qml";
                    loginWindow.stopPopupTimer();
                    break;
                }
                else
                {
                    newSource = "Login.qml";
                    loginWindow.stopPopupTimer();
                    break;
                }
            case LoginState.Login:
                if (myLoader.item.showTotp)
                {
                    newSource = "Totp.qml";
                    loginWindow.stopPopupTimer();
                    break;
                }
                else
                {
                    newSource = "Login.qml";
                    loginWindow.stopPopupTimer();
                    break;
                }
        }
        if (myLoader.source.toString() !== newSource) {
            console.log("compare false");
            myLoader.source = newSource;
        }
    }

    function showPopup(errorMessage , reload) {
        console.log("loginWindow.showPopup")
        console.log(loginWindow.popupOpen)
        loginWindow.changeImageOpacity(0.5)
        loginWindow.freezeWindow = true
        myLoader.item.freezeComponents(true);

        if(loginWindow.popupOpen){
            console.log("Popup window is already open.");
            return;
        }
        var component = Qt.createComponent("qrc:/PopupWindow.qml");
        if (component.status === Component.Ready) {
            var popup = component.createObject(loginWindow, {
                "buttonClickedFunction": function() {
                    loginWindow.customButtonClickedFunction(popup,reload);
                }
            });
            if (popup) {
                popup.message = errorMessage;
                popup.dynamicColor = loginWindow.popupText;
                popup.adjustSize();
                popup.show();
                loginWindow.popupOpen = true;
            }else {
            console.error("Error: Popup object was not created.");
            }

        } else if (component.status === Component.Error) {
            console.error("Error loading component:", component.errorString());
        }
    }

    function stopPopupTimer() {
        console.log("loginWindow.stopPopupTimer")
        popupTimer.stop();
    }

    function restartPopupTimer() {
        console.log("loginWindow.restartPopupTimer")
        loginWindow.secondsRemaining = 30
        popupTimer.restart();
    }
    function showWindow(){
        console.log("showWindow");
        loginWindow.usbConnected = false;
        visible = true;
        loginWindow.addWindowStaysOnTopHint();
        myLoader.source = "LoadWindow.qml";
    }

    function changeStaysOnTopHint(){
        console.log( " changeStaysOnTopHint " );
        flags &= ~Qt.WindowStaysOnTopHint;
    }

    function addWindowStaysOnTopHint() {
        loginWindow.flags = Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint;
    }

    function openCompanyWindow() {
        console.log("loginWindow.openCompanyWindow");
        changeStaysOnTopHint();
        var component = Qt.createComponent("qrc:/Company.qml");
        if (component.status === Component.Ready) {
            var companyWindow = component.createObject(loginWindow, {"loginWindowRef": this});
            companyWindow.show();
        } else {
            console.error("Nie można załadować Company.qml:", component.errorString());
        }
    }

    Connections {
           target: UsbUser
           function onUsbConnected (path) {
               if(!usbBlock){
                   console.log("UsbUser.onUsbConnected")
                   console.log("the flash drive has been plugged in!:  " + path);
                   loginWindow.path = path;
                   loginWindow.usbConnected = true;
                   myLoader.source = "Pin.qml";
               }
           }
           function onUsbDisconnected () {
               console.log("UsbUser.onUsbDisconnected");
               loginWindow.usbConnected = false;
               loginWindow.path = "";
           }
           function onUserConnected () {
               console.log("UsbUser.onUserConnected");
               myLoader.source = "Login.qml";
           }

           function onUserSaved(){
               console.log("UsbUser.onUserSaved");
               viewService.generateQrCode();
           }
           function onUsbError (error) {
               console.log("UsbUser.onUsbError");
               showPopup(error,true);
           }
       }

    Connections {
        target: viewService.sessionManager.totpManager
        function onQrImageChanged()
        {
            console.log("viewService.sessionManage.onQrImageChanged")
            loginWindow.usbBlock = true;
            myLoader.source = "QRCode.qml"
        }
    }

    Connections {
        target: viewService.sessionManager
        function onSessionError(error)
        {
            console.log("viewService.sessionManage.onSessionError")
            showPopup(error,true);
        }
        function onTotpAuthorization ()
        {
            console.log("viewService.sessionManage.onTotpAuthorization")
            myLoader.source = "Totp.qml"

        }
        function onVeryficationStatus (message)
        {
            console.log("viewService.sessionManage.onVeryficationStatus")
            console.log(message);
            showPopup(message,true);
        }
        function onLoginUserFinished (success)
        {
            console.log("viewService.sessionManage.onLoginUserFinished");
            if (success){
                openCompanyWindow();
            }
        }
    }

    Timer {
        id: popupTimer
        interval: 1000 // Odliczanie co sekundę
        repeat: true
        running: true
        onTriggered: {
            loginWindow.secondsRemaining -= 1;
            console.log("popupTimer.onTriggered");
            var state = viewService.sessionManager.loginState.currentState;
            if (myLoader.status === Loader.Ready) {
                if (myLoader.source.toString() === "LoadWindow.qml")
                {
                    var loadedComponent = myLoader.item;
                    loadedComponent.updateCountdown(loginWindow.secondsRemaining.toString());
                }
            }

            if (loginWindow.secondsRemaining <= 0) {

                if (state === LoginState.LoginAdmin)
                {
                    showPopup("Please insert the flash drive or Login as Admin", false);
                }
                else
                {
                   showPopup("Please reinsert the flash drive", true);
                }
                loginWindow.secondsRemaining = 30;
            }
        }
    }
}

