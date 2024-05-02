import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import "qrc:/components"

Item {
    id:subcontractorList
    property bool company: true
    Row {
        id: headerSubcontractor
        height: 40
        spacing: 1
        anchors.top: parent.top
        anchors.topMargin:companyWindow.barTopMargin + companyWindow.barHeight + companyWindow.offsetBar
        anchors.left: parent.left
        anchors.leftMargin: 10
        Repeater {
            model: ListModel {
                ListElement { name: "Id"; width: 70 }
                ListElement { name: "ShortName"; width: 220 }
                ListElement { name: "Name"; width: 337 }
                ListElement { name: "NIP"; width: 157 }
                ListElement { name: "ZIP-CODE"; width: 137 }
                ListElement { name: "City"; width: 207 }
                ListElement { name: "Street"; width: 292 }
            }

            CustomButton {
                text: model.name
                width: model.width
                height: 40
                font.pointSize: 16
                font.family: "arial"
                colorPressed: "#d9d7d4"
                colorMouseOver: "#bfbdbb"
                colorDefault: "#b3b2b1"
            }
        }
    }
    Rectangle {
        id:subcontractorRec
        anchors.top: headerSubcontractor.top
        anchors.topMargin:headerSubcontractor.height + 1
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
        height: parent.height - 20 - 2*bar.height
        color : "#DFDCDC"

        ListView {
            anchors.fill: parent
            model: viewService.sessionManager.subcontractorModel
            ScrollBar.horizontal: ScrollBar{}
            ScrollBar.vertical: ScrollBar{}
            delegate: Rectangle {
                height: 50
                radius: 10
                color : "gray"
                border.color: "cyan"
                width: parent.width
                Row{
                    id: rowSubcontractor
                    spacing: 1
                    anchors.top: parent.top
                    anchors.topMargin: 1
                    anchors.left: parent.left

                    CustomTextField {
                        id:subcontractorId
                        text : id
                        implicitWidth: 70
                        readOnly: true
                    }
                    CustomTextField {
                        id:subcontractorShortName
                        text : shortName
                        implicitWidth: 220
                        onEditingFinished: {
                            model.shortName = text
                        }
                    }
                    CustomTextField {
                        id:subcontractorName
                        text : name
                        implicitWidth: 337
                        onEditingFinished: {
                            model.name = text
                        }
                    }
                    CustomTextField {
                        id:subcontractorNIP
                        text : nip
                        implicitWidth: 157
                        onEditingFinished: {
                            model.nip = text
                        }
                    }
                    CustomTextField {
                        id:subcontractorZIP
                        text : zip
                        implicitWidth: 137
                        onEditingFinished: {
                            model.zip = text
                        }
                    }
                    CustomTextField {
                        id:subcontractorCity
                        text : city
                        implicitWidth: 207
                        onEditingFinished: {
                            model.city = text
                        }
                    }
                    CustomTextField {
                        id:subcontractorStreet
                        text : street
                        implicitWidth: 292
                        onEditingFinished: {
                            model.street = text
                        }
                    }
                    CustomButton {
                        id:removeSubcontractor
                        text: "Delete"
                        width: 88
                        height: 50
                        font.pointSize: 13
                        font.family: "arial"
                        colorPressed: "#d9d7d4"
                        colorMouseOver: "#bfbdbb"
                        colorDefault: "#b3b2b1"
                        visible: true
                        clip: false
                        onClicked:{
                            viewService.sessionManager.subcontractorModel.removeSubcontractor(index)
                        }
                    }
                }
            }
        }
    }
    function freezeComponents(freeze)  {
        console.log("subcontractor.freezeComponents!" + !freeze)
        subcontractorRec.enabled = !freeze;
    }

}
