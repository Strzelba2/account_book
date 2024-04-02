import QtQuick 2.0
import QtQuick.Controls 2.3
import "qrc:/components"

Popup {
    id: popup
    width: 400
    height: 200
//        anchors.centerIn: parent
    modal: true
    focus: true

    Rectangle {

        property int customWidthMargin: 30;
        property int customHeightMargin: 20;
        id: popupRec
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.Top
        anchors.topMargin: 2
        width: popup.width - customWidthMargin
        height: popup.height - popupButton.height - customHeightMargin

        color: "red"
        border.color: "black"
        radius: 10


        TextArea {
            id: popupText
            text: "default"
            anchors.fill: parent
            anchors.margins: 5
            wrapMode: TextArea.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            readOnly: true

            font.pixelSize: 16
        }

    }

    CustomButton {
        id: popupButton
        text: "OK"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.5
        height: parent.height * 0.15
        font.pointSize: 14
        font.family: "arial"
        colorPressed: "#d9d7d4"
        colorMouseOver: "#bfbdbb"
        colorDefault: "#b3b2b1"
        onClicked: popup.buttonClickedFunction()

    }
    onWidthChanged: adjustSize()
    onHeightChanged: adjustSize()

    function adjustSize() {
        console.log("adjustmet")
        var maxWidth = 500;
        var maxHeight = 600;
        var currentFontSize = popupText.font.pixelSize;
        var minWidth = Math.min(popupText.contentWidth + 30, maxWidth) + popupRec.customWidthMargin + popupText.anchors.margins;
        var minHeight = Math.min(popupText.contentHeight, maxHeight)+popupButton.height + popupRec.customHeightMargin + popupText.anchors.margins + popupRec.anchors.topMargin;
        console.log("popup.width: " + popup.width)
        console.log("minWidth: " + minWidth)
        console.log("minHeight: " + minHeight)

        if (popup.width < minWidth){

            console.log(minWidth)
            popup.width = maxWidth;
        }
        if(popup.height < minHeight){
            console.log(minHeight)
            popup.height = minHeight
        }


        while ((popupText.contentWidth > maxWidth || popupText.contentHeight > maxHeight)) {
                   currentFontSize--;
                   popupText.font.pixelSize = currentFontSize;
               }

    }

    property var buttonClickedFunction: function() {
            // Domyślna akcja
            loginWindow.changeImageOpacity(1)
            popup.close();
    }

}
