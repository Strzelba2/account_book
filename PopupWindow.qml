import QtQuick 2.0
import QtQuick.Controls 2.3
import "qrc:/components"

Window {
    id: popupMain
    width: 400
    height: 200
    visible: true
    title: qsTr("Login")

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    property string message: "default"
    property color dynamicColor: "red"
    property color backgraundColor: "#DFDCDC"

    color: "#F2F1F1"

    Rectangle {

        property int customWidthMargin: 30;
        property int customHeightMargin: 20;
        id: popupRec
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: (popupMain.width - width)/2
        width: popupMain.width - customWidthMargin
        height: popupMain.height - popupButton.height - customHeightMargin*2

        color: backgraundColor
        border.color: "black"
        radius: 10

        TextArea {
            id: popupText
            text: popupMain.message
            anchors.fill: parent
            anchors.margins: 5
            wrapMode: TextArea.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            readOnly: true
            color: dynamicColor
            background: Rectangle {
                color: backgraundColor
            }

            font.pixelSize: 16
        }
    }

    Rectangle {
        id: moveMouse
        color: "#00000033"
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
            property point lastMousePos: Qt.point(0, 0)
            onPressed: { lastMousePos = Qt.point(mouseX, mouseY); }
            onMouseXChanged: popupMain.x += (mouseX - lastMousePos.x)
            onMouseYChanged: popupMain.y += (mouseY - lastMousePos.y)
        }
    }

    CustomButton {
        id: popupButton
        text: "OK"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.5
        height: parent.height * 0.15
        font.pointSize: 14
        font.family: "arial"
        colorPressed: "#d9d7d4"
        colorMouseOver: "#bfbdbb"
        colorDefault: "#b3b2b1"
        onClicked: popupMain.buttonClickedFunction()

    }
    onWidthChanged: adjustSize()
    onHeightChanged: adjustSize()

    function adjustSize() {
        console.log("adjustmet")
        var maxWidth = 600;
        var maxHeight = 400;
        var minHeightConst = 200;
        var currentFontSize = popupText.font.pixelSize;
        var minWidth = Math.min(popupText.contentWidth + 30, maxWidth) + popupRec.customWidthMargin + popupText.anchors.margins;

        if (popupMain.width < minWidth){
            popupMain.width = maxWidth;
        }

        var minHeight = Math.min(popupText.contentHeight, maxHeight)+popupButton.height + popupRec.customHeightMargin*2 + popupRec.anchors.topMargin;

        if((minHeight < minHeightConst) && ((minHeight-minHeightConst)>2) ){
            popupMain.height = minHeightConst;
        }

        if((minHeight < popupMain.height )&&(minHeight > minHeightConst)&&((popupMain.height-minHeight)>2)){
            popupMain.height = minHeight;
        }
        if(( popupMain.height < minHeight)&&(minHeight > minHeightConst)&&((minHeight-popupMain.height>2))){
            popupMain.height = minHeight;
        }
        if (( popupText.contentHeight > maxHeight)) {
           popupText.font.pixelSize = currentFontSize - 1;
        }
    }

    property var buttonClickedFunction: function() {

        loginWindow.changeImageOpacity(1);
        loginWindow.popupOpen = false;
        popupMain.close();
    }

}
