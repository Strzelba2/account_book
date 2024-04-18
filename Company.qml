import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import "qrc:/components"

Window {
    id: companyWindow
    width: Screen.width
    height: Screen.height
    visible: true
    title: qsTr("book")

    flags: Qt.FramelessWindowHint

    property var loginWindowRef: null
    property int offsetCell: 60
    property int offsetBar: 0
    property int lastClickedButtonIndex: -1
    property int barHeight: 50
    property int barTopMargin: 60
    property bool popupOpen: false
    property bool isSubcontractorWindow: false
    property var subcontractorWindow: null

    property var customButtonClickedFunction: function(popup) {
        console.log("companyWindow.customButtonClickedFunction");
        companyWindow.popupOpen = false;
        companyWindow.freezeComponents(false)
        companyLoader.item.freezeComponents(false);
        popup.destroy();
    }

    Component.onCompleted: {
        if (loginWindowRef) {
            loginWindowRef.visible = false;
            loginWindowRef.changeStaysOnTopHint()
        }
        stackLayout.currentIndex = -1;
        companyLoader.source = "BOOK.qml"
        console.log("companyWindow.onCompleted");
    }
    onClosing: {
        console.log("companyWindow: onClosing");
    }
    onVisibleChanged: {
        if (!visible) {
            console.log("companyWindow has become invisible");
        }
    }

    CustomButton {
        id: btnClose
        width: 30
        height: 30
        opacity: 1
        visible: true
        text: "X"
        anchors.right:  parent.right
        anchors.top: parent.top
        anchors.rightMargin: 30
        clip: false
        anchors.topMargin: 22
        colorPressed: "#d9d7d4"
        font.family: "Segoe UI"
        colorMouseOver: "#bfbdbb"
        colorDefault: "#b3b2b1"
        font.pointSize: 16

        onParentChanged: {
            if (!parent) {
                console.log("btnClose: parent has been cleared");
            }
        }
        MouseArea {
            id: myMouseId
            anchors.fill: parent
            onClicked:{
                companyWindow.closeAndShowLogin()
            }
        }
    }
    ListModel {
        id: columnWidths
    }

    ListModel {
        id: headerWidths
    }

    Rectangle {
        id: bar
        width: parent.width - 20
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin:barTopMargin
        height: barHeight

        Row {
            anchors.fill: parent
            CustomButton {
                id:profileButton
                text: "Profile"
                width: 150
                height: 50
                font.pointSize: 16
                font.family: "arial"
                colorPressed: "#d9d7d4"
                colorMouseOver: "#bfbdbb"
                colorDefault: "#b3b2b1"
                visible: true
                clip: false
                onClicked: handleButtonClick(0)
            }
            CustomButton {
                id:subcontractorButton
                text: "Subcontractor"
                width: 200
                height: 50
                font.pointSize: 16
                font.family: "arial"
                colorPressed: "#d9d7d4"
                colorMouseOver: "#bfbdbb"
                colorDefault: "#b3b2b1"
                visible: true
                clip: false
                onClicked: handleButtonClick(1)
            }
            CustomButton {
                id:assetsButton
                text: "Fixed assets"
                width: 200
                height: 50
                font.pointSize: 16
                font.family: "arial"
                colorPressed: "#d9d7d4"
                colorMouseOver: "#bfbdbb"
                colorDefault: "#b3b2b1"
                visible: true
                clip: false
                onClicked: handleButtonClick(2)
            }
            CustomButton {
                id:booksButton
                text: "Books"
                width: 120
                height: 50
                font.pointSize: 16
                font.family: "arial"
                colorPressed: "#d9d7d4"
                colorMouseOver: "#bfbdbb"
                colorDefault: "#b3b2b1"
                visible: true
                clip: false
                onClicked: handleButtonClick(3)
            }
            CustomButton {
                id:jpkButton
                text: "JPK generation"
                width: 200
                height: 50
                font.pointSize: 16
                font.family: "arial"
                colorPressed: "#d9d7d4"
                colorMouseOver: "#bfbdbb"
                colorDefault: "#b3b2b1"
                visible: true
                clip: false
                onClicked: console.log("JPK generation");
            }
        }
    }

    StackLayout {
        id: stackLayout
        anchors.top: bar.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: -1
        width: companyWindow.width - 20

        Item {
            Rectangle {
                id: bar1
                height: 30
                width: companyWindow.width - 20
                Row {
                    CustomButton {
                        width: 200
                        height: bar1.height
                        text: "Change the password"
                        font.pointSize: 14
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false
                    }
                    CustomButton {
                        width: 120
                        height: bar1.height
                        text: "Edit User"
                        font.pointSize: 16
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false
                    }
                }
            }
        }

        Item {
            Rectangle {
                id: bar2
                height: 30
                width: companyWindow.width - 20
                Row {
                    CustomButton {
                        width: 180
                        height: bar1.height
                        text: "Add subcontractor"
                        font.pointSize: 14
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false

                        MouseArea {
                            id: addSubcontractor
                            anchors.fill: parent
                            onClicked:{
                                hideLayout();
                                companyLoader.source = "Subcontractor.qml";
                            }
                        }
                    }
                    CustomButton {
                        width: 230
                        height: bar1.height
                        text: "List of subcontractors"
                        font.pointSize: 16
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false

                        MouseArea {
                            id: subcontractorList
                            anchors.fill: parent
                            onClicked:{
                                hideLayout();
                                companyLoader.source = "SubcontractorList.qml";
                            }
                        }
                    }
                }
            }
        }

        Item {
            Rectangle {
                id: bar3
                height: 30
                width: companyWindow.width - 20
                Row {
                    CustomButton {
                        width: 170
                        height: bar1.height
                        text: "add fixed asset"
                        font.pointSize: 14
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false
                    }
                    CustomButton {

                        width: 200
                        height: bar1.height
                        text: "List of fixed asset"
                        font.pointSize: 16
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false
                    }
                    CustomButton {

                        width: 210
                        height: bar1.height
                        text: "Account fixed asset"
                        font.pointSize: 16
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false
                    }

                }
            }
        }

        Item {
            Rectangle {
                id: bar4
                height: 30
                width: companyWindow.width - 20
                Row {
                    CustomButton {
                        width: 160
                        height: bar1.height
                        text: "Accounting"
                        font.pointSize: 14
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false
                        MouseArea {
                            id: accounting
                            anchors.fill: parent
                            onClicked:{
                                viewService.sessionManager.bookModel.setMonth(0);
                                viewService.sessionManager.bookModel.changeSorte(0);
                                viewService.sessionManager.bookModel.sort(0, Qt.AscendingOrder);
                                hideLayout()
                                companyLoader.source = "AddBooks.qml"
                            }
                        }
                    }
                    CustomButton {

                        width: 180
                        height: bar1.height
                        text: "Tax Accountant"
                        font.pointSize: 16
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false

                        MouseArea {
                            id: taxAccountant
                            anchors.fill: parent
                            onClicked:{
                                viewService.sessionManager.bookModel.setMonth(1);
                                viewService.sessionManager.bookModel.sort(0, Qt.AscendingOrder);
                                hideLayout()
                                companyLoader.source = "BOOK.qml"
                            }
                        }
                    }
                }
            }
        }
    }

    Loader {
        id: companyLoader
        anchors.fill: parent
        source: "LoginAdmin.qml"
        onLoaded: {
            console.log("CompanyWindow.Loader.onLoaded");
        }
    }

    function freezeComponents(freeze)  {
        console.log("company.freezeComponents!" + !freeze)
        jpkButton.enabled = !freeze;
        booksButton.enabled = !freeze;
        assetsButton.enabled = !freeze;
        subcontractorButton.enabled = !freeze;
        profileButton.enabled = !freeze;
    }

    function closeAndShowLogin()
    {

        console.log("companyWindow.closeAndShowLogin");
        viewService.dbClose();
        if (loginWindowRef) {
            loginWindowRef.showWindow();
        }
        destroy();
    }
    function handleButtonClick(buttonIndex) {
        console.log("companyWindow.handleButtonClick")
        if (lastClickedButtonIndex === buttonIndex) {
            stackLayout.currentIndex = -1;
            lastClickedButtonIndex = -1;
            companyWindow.offsetBar = 0;
        } else {
            stackLayout.currentIndex = buttonIndex;
            lastClickedButtonIndex = buttonIndex;
            companyWindow.offsetBar = bar1.height;
        }
    }
    function hideLayout(){
        stackLayout.currentIndex = -1;
        lastClickedButtonIndex = -1;
        companyWindow.offsetBar = 0;
    }

    function showPopup(errorMessage) {
        console.log("companyWindow.showPopup")

        if(loginWindow.popupOpen){
            console.log("Popup window is already open.");
            return;
        }
        companyWindow.freezeComponents(true)

        if(isSubcontractorWindow){
            subcontractorWindow.freezeComponents(true);
            companyLoader.item.freezeComponents(true);
        }else{
            companyLoader.item.freezeComponents(true);
        }

        var component = Qt.createComponent("qrc:/PopupWindow.qml");
        if (component.status === Component.Ready) {
            var popup = component.createObject(companyWindow, {
                "buttonClickedFunction": function() {
                    companyWindow.customButtonClickedFunction(popup);
                }
            });
            if (popup) {
                popup.message = errorMessage;
                popup.dynamicColor = "red";
                popup.adjustSize();
                popup.show();
                companyWindow.popupOpen = true;
            }else {
                console.error("Error: Popup object was not created.");
            }

        } else if (component.status === Component.Error) {
            console.error("Error loading component:", component.errorString());
        }
    }

    Connections {
        target: viewService.sessionManager.bookModel
        function onBookDataError (error) {
            console.log("viewService.sessionManage.bookModel.onSessionError")
            companyWindow.showPopup(error);
        }
    }

    Connections {
        target: viewService.sessionManager.subcontractorModel
        function onSubcontractorDataError (error) {
            console.log("viewService.sessionManage.subcontractorModel.onSubcontractorDataError")
            companyWindow.showPopup(error);
        }
        function onSubcontractorDataChanged () {
            console.log("viewService.sessionManage.subcontractorModel.onSubcontractorDataChanged")
            if(companyLoader.source.toString() === "Subcontractor.qml"){
                companyLoader.source = "SubcontractorList.qml"
            }
        }
    }
}
