import QtQuick 2.15
import "qrc:/components"

Window {
    id: subcontractorWindow
    width: 400
    height: 610
    visible: true
    title: qsTr("subcontractor")

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    property bool freezeWindow: false

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
            subcontractorWindow.x += (mouseX - lastMousePos.x);
            }
        }
        onMouseYChanged: {
            if(!freezeWindow)
            {
            subcontractorWindow.y += (mouseY - lastMousePos.y);
            }
        }
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
        anchors.leftMargin: 350
        clip: false
        anchors.topMargin: 20
        colorPressed: "#d9d7d4"
        font.family: "Segoe UI"
        colorMouseOver: "#bfbdbb"
        colorDefault: "#b3b2b1"
        font.pointSize: 16
        z:1

        MouseArea {
            id: myMouseId
            anchors.fill: parent
            onClicked:{
                viewService.sessionManager.subcontractorModel.subcontractorCloseWindow()
                subcontractorWindow.destroy();
            }
        }
    }
    Loader {
        id: subcontractorLoader
        anchors.fill: parent
        source: "Subcontractor.qml"
        onLoaded: {
            console.log("subcontractorWindow.Loader.onLoaded");

        }
    }

    function changeCompany(){
        console.log("subcontractorWindow.changeCompany")
        subcontractorLoader.item.company = false
    }

    function freezeComponents(status){
        subcontractorLoader.item.freezeComponents(status)
    }

}
