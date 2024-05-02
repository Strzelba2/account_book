import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import "qrc:/components"

Item {
    id:subcontractor
    property bool company: true
    Rectangle {
        anchors.top: parent.top
        anchors.topMargin:company ? companyWindow.barTopMargin + companyWindow.barHeight + companyWindow.offsetBar : 70
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
        height: company ? parent.height - 20 - 2*bar.height : parent.height - 80
        color : "#DFDCDC"
        Rectangle{
            id: subcontractorRec
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: btnCreate.width + 60
            height: 7*textShortName.height + 7*textShortName.anchors.bottomMargin + 2*btnCreate.anchors.bottomMargin
            color : "white"

            Text {
                id: subcontractorLabel
                text: "Add subcontractor"
                font.pixelSize: 24
                font.family: "arial"
                anchors.bottom: textShortName.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#cf953e"
            }

            CustomTextField {
                id: textShortName
                x: 50
                y: 195
                opacity: 1
                anchors.bottom: textName.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "ShortName"
                enabled: true
            }

            CustomTextField {
                id: textName
                x: 50
                y: 195
                opacity: 1
                anchors.bottom: textNIP.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "Name"
                enabled: true
            }

            CustomTextField {
                id: textNIP
                x: 50
                y: 195
                opacity: 1
                anchors.bottom: textZip.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "NIP"
                enabled: true
            }

            CustomTextField {
                id: textZip
                x: 50
                y: 195
                opacity: 1
                anchors.bottom: textCity.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "Zip-code"
                enabled: true
            }

            CustomTextField {
                id: textCity
                x: 50
                y: 195
                opacity: 1
                anchors.bottom: textStreet.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "City"
                enabled: true
            }

            CustomTextField {
                id: textStreet
                x: 50
                y: 195
                opacity: 1
                anchors.bottom: btnCreate.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "Street"
                enabled: true
            }

            CustomButton {
                id: btnCreate
                width: 320
                height: 50
                opacity: 1
                text: "Create"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 60
                anchors.horizontalCenter: parent.horizontalCenter
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
                        console.log("create");
                        createSubcontractor();
                    }
                }
            }
        }
    }

    function freezeComponents(freeze)  {
        console.log("subcontractor.freezeComponents!" + !freeze)
        btnCreate.enabled = !freeze;
        textStreet.enabled = !freeze;
        textCity.enabled = !freeze;
        textZip.enabled = !freeze;
        textNIP.enabled = !freeze;
        textName.enabled = !freeze;
        textShortName.enabled = !freeze;
    }

    function createSubcontractor(){
        console.log("createSubcontractor!");
        viewService.sessionManager.subcontractorModel.addSubcontractor(textShortName.text, textName.text, textNIP.text,
                                                                       textZip.text, textCity.text, textStreet.text)
    }

    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            createSubcontractor()
        }
    }
}



