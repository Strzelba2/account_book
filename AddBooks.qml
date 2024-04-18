import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import "qrc:/components"

Item {
    Rectangle {
        id: addBookItem
        anchors.top: parent.top
        anchors.topMargin:bar.anchors.topMargin + bar.height + companyWindow.offsetBar
        anchors.horizontalCenter: parent.horizontalCenter
        width: companyWindow.width - 20
        height: companyWindow.height - 20 - 2*bar.height
        color : "#DFDCDC"

        HorizontalHeaderView {
            id: horizontalHeader
            anchors.left: tableView.left
            anchors.top: parent.top
            syncView: tableView
            height: Math.max(defaultHeight, delegateHeight)
            clip: true
            property int defaultHeight: 30
            property int delegateHeight: 0

            delegate: ItemDelegate {
                id: headerDelegate
                Text {
                    id: headerText
                    text: tableView.model.headerData(index, Qt.Horizontal)
                    font.family: "Arial"
                    font.pointSize: 14
                    color: "blue"
                    onHeightChanged: {
                        console.log("companyWindow.tableView.horizontalHeader.onHeightChanged");
                        if (headerText.height > horizontalHeader.delegateHeight && headerText.height > horizontalHeader.defaultHeight) {
                            horizontalHeader.delegateHeight = headerText.height;
                        }else{
                            horizontalHeader.delegateHeight = horizontalHeader.defaultHeight;
                        }
                    }
                    onWidthChanged: {
                        console.log("companyWindow.tableView.horizontalHeader.onWidthChanged");
                        if(headerText.width + offsetCell >columnWidths.get(model.column).width){
                            columnWidths.set(model.column, {"width": headerText.width + offsetCell});
                            tableView.model.changeColumnWidth(model.column);
                        }
                    }
                    Component.onCompleted: {
                        console.log("companyWindow.tableView.horizontalHeader.onCompleted");
                        let currentWidth = headerWidths.get(model.column) ? headerWidths.get(model.column).width : 0;
                        if (headerText.width >currentWidth){
                            headerWidths.set(model.column, {"width": headerText.width + offsetCell});
                        }
                    }
                }

                background: Rectangle {
                    id: headerBack
                    color: "lightgray"
                    height: horizontalHeader.height
                }
            }
        }

        VerticalHeaderView {
            id: verticalHeader
            anchors.top: tableView.top
            anchors.left: parent.left
            syncView: tableView
            clip: true

        }

        TableView {
            id: tableView
            width: parent.width - 20
            height: parent.height - 20
            anchors.left: verticalHeader.right
            anchors.top: horizontalHeader.bottom
            anchors.topMargin: 5
            anchors.leftMargin: 5
            columnSpacing: 1
            rowSpacing: 1
            ScrollBar.horizontal: ScrollBar{}
            ScrollBar.vertical: ScrollBar{}
            model: viewService.sessionManager.bookModel
            property int defaultWidth: 30

            columnWidthProvider: function(column)
            {
                let currentWidth = columnWidths.get(column) ? columnWidths.get(column).width : defaultWidth;
                console.log("columnWidthProvide:" , column,":",currentWidth);
                return currentWidth;
            }

            delegate: Rectangle {

                id: recDell
                implicitWidth: tableView.defaultWidth
                implicitHeight: 50
                border.width: 1
                radius: 10
                color : "gray"
                border.color: "cyan"

                Text {
                    id: textItem
                    anchors.verticalCenter:  parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: model.display === 0  ? "" : model.display
                    font.pointSize: 12
                    font.family: "arial"
                    visible: !editor.visible
                    Component.onCompleted: {
                        console.log("companyWindow.tableView.recDell.textItem.onCompleted");
                    }
                }

                TextField {
                    id: editor
                    text: model.edit
                    anchors.fill: parent
                    visible: false
                    property bool isProcessing: false

                    onEditingFinished: {
                        console.log("companyWindow.tableView.recDell.editor.onEditingFinished");
                        if (isProcessing) {
                            return;
                        }
                        isProcessing = true;
                        hideEditor();
                        model.edit = text
                        let column = model.column;
                        console.log("onEditingFinished");
                        if (textItem.implicitWidth + 30 > textItem.width ){
                            console.log("change column");
                            columnWidths.set(column, {"width": textItem.implicitWidth + 30});
                            recDell.implicitWidth = columnWidths.get(model.column).width;
                            Qt.callLater(tableView.model.layoutChanged);
                        }

                        if(headerWidths.get(column).width > textItem.implicitWidth + 30 ){
                            console.log("change header");
                            columnWidths.set(column, {"width": textItem.implicitWidth + 30});

                            recDell.implicitWidth = columnWidths.get(model.column).width;
                            Qt.callLater(tableView.model.layoutChanged);
                        }
                        isProcessing = false;
                    }
                    function showEditor() {
                        console.log("companyWindow.tableView.recDell.editor.showEditor");
                        if (!visible) {
                            visible = true;
                            forceActiveFocus();
                            selectAll();
                        }
                    }
                    function hideEditor() {
                        console.log("companyWindow.tableView.recDell.editor.hideEditor");
                        if (visible) {
                            visible = false;
                            parent.focus = true;
                        }
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onDoubleClicked: {
                        console.log("companyWindow.tableView.recDell.onDoubleClicked");
                        if( !tableView.model.checkIfID(model.column)){
                           editor.showEditor();
                        }
                    }
                }

                onWidthChanged: {
                    console.log("companyWindow.tableView.recDell.onWidthChanged");
                    let column = model.column;
                    let currentheaderWidth = headerWidths.get(column) ? headerWidths.get(column).width : 0;
                    let currentWidth = columnWidths.get(column) ? columnWidths.get(column).width : 0;
                    if (textItem.implicitWidth + offsetCell > currentWidth)
                    {
                        console.log("onWidthChanged");
                        columnWidths.set(column, {"width": textItem.implicitWidth + offsetCell});
                    }
                    else if (textItem.implicitWidth + offsetCell < currentheaderWidth && currentheaderWidth > currentWidth)
                    {
                        console.log("onWidthChanged");
                        columnWidths.set(column, {"width": currentheaderWidth});
                    }
                }

                Connections {
                    target: viewService.sessionManager.bookModel
                    function onColumnWidtChanged (column) {
                        console.log("viewService.sessionManager.bookModel.onColumnWidtChanged");
                        let currentWidth = recDell.implicitWidth;

                        if (column ===model.column){
                            console.log("change of column: ", column);
                            if(currentWidth < columnWidths.get(column).width){
                                console.log("column width smaller");
                                Qt.callLater(tableView.requestForceLayout);
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    console.log("companyWindow.tableView.recDell.onCompleted");

                    let column = model.column;
                    let currentWidth = columnWidths.get(column) ? columnWidths.get(column).width : 0;
                    if (textItem.implicitWidth + offsetCell > currentWidth) {
                        columnWidths.set(column, {"width": textItem.implicitWidth + offsetCell});
                    }
                    recDell.implicitWidth = columnWidths.get(column).width;
                }
            }

            function requestForceLayout()
            {
                console.log("companyWindow.tableView.requestForceLayout");
                tableView.forceLayout();
            }
        }
     }
    function freezeComponents(freeze)  {
        console.log("addBookItem.freezeComponents!" + !freeze)
        addBookItem.enabled = !freeze;
    }
}

