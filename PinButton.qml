import QtQuick 2.0
import QtQuick.Controls 2.3


Button  {
    id: pinButton
    width: 75
    height: 80
    flat:true
    font.pointSize: 16
    font.family: "arial"

    property color colorDefault: "#b3b2b1"
    property color colorMouseOver: "#bfbdbb"
    property color colorPressed: "#d9d7d4"
    property bool borderButton: false

    QtObject{
        id: internal
        property var dynamicColor: if(pinButton.down){
                                       pinButton.down ? colorPressed : colorDefault;

                                   }else{
                                       pinButton.hovered ? colorMouseOver : colorDefault;
                                   }
    }
    text: qsTr("Button")
    contentItem: Item{
        Text {
            id: name
            text: pinButton.text
            font: pinButton.font
            color: borderButton ? "grey" : "#ffffff"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    background: Rectangle{
        anchors.horizontalCenter: parent.horizontalCenter
        color: internal.dynamicColor
        radius: 10
        border.color: "#bfbdbb"
        border.width: borderButton ? 2 : 0
        width: 75
        height: 80
    }
}
