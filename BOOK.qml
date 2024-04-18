import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import "qrc:/components"

Item {
    Rectangle {
        id:bookItem
        anchors.top: parent.top
        anchors.topMargin:bar.anchors.topMargin + bar.height + companyWindow.offsetBar
        anchors.horizontalCenter: parent.horizontalCenter
        width: companyWindow.width - 20
        height: companyWindow.height - 20 - 2*bar.height
        color : "#DFDCDC"

        Row {
            id: months
            height: 40
            spacing: 1
            anchors.top: parent.top
            anchors.left: parent.left
            Repeater {
                model: ListModel {
                    ListElement { roman: "I"; number: 1 }
                    ListElement { roman: "II"; number: 2 }
                    ListElement { roman: "III"; number: 3 }
                    ListElement { roman: "IV"; number: 4 }
                    ListElement { roman: "V"; number: 5 }
                    ListElement { roman: "VI"; number: 6 }
                    ListElement { roman: "VII"; number: 7 }
                    ListElement { roman: "VIII"; number: 8 }
                    ListElement { roman: "IX"; number: 9 }
                    ListElement { roman: "X"; number: 10 }
                    ListElement { roman: "XI"; number: 11 }
                    ListElement { roman: "XII"; number: 12 }
                }

                CustomButton {
                    text: model.roman
                    width: 60
                    height: 40
                    font.pointSize: 16
                    font.family: "arial"
                    colorPressed: "#d9d7d4"
                    colorMouseOver: "#bfbdbb"
                    colorDefault: "#b3b2b1"

                    MouseArea {
                        id: month
                        anchors.fill: parent
                        onClicked:{
                            console.log("Kliknięto miesiąc: " + model.roman + ", Numer: " + model.number);
                            tableView.model.setMonth(model.number);

                        }
                    }
                }
            }
        }

        HorizontalHeaderView {
            id: horizontalHeader
            anchors.left: tableView.left
            anchors.top: parent.top
            anchors.topMargin: months.height
            syncView: tableView
            height: Math.max(defaultHeight, delegateHeight)
            clip: true
            property int draggingColumn: -1
            property int pressedX: -1
            property int defaultHeight: 30
            property int delegateHeight: 0
            property var table: null
            property double activeColumnOffset: 0

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

                MouseArea {
                    anchors.fill: parent
                    drag.target: horizontalHeader
                    property point lastMousePos: Qt.point(0, 0)

                    onPressed: function(event) {
                        console.log("onPressed delegate");
                        let index = model.column;
                        if (index !== -1) {
                            horizontalHeader.draggingColumn = index;
                            lastMousePos = Qt.point(mouseX, mouseY);
                            horizontalHeader.pressedX = firstPosition(index,event.x);
                            horizontalHeader.activeColumnOffset = horizontalHeader.pressedX;
                        }
                    }

                    onPositionChanged: {
                        console.log("onPositionChanged");
                        if (drag.active) {
                            horizontalHeader.activeColumnOffset += (mouseX - lastMousePos.x);
                            headerDelegate.x += (mouseX - lastMousePos.x);

                            if(horizontalHeader.table) {
                                horizontalHeader.table.updateDelegatesPosition((mouseX - lastMousePos.x), horizontalHeader.draggingColumn);
                            }
                        }
                    }
                    onReleased:   {
                        if (horizontalHeader.draggingColumn !== -1) {
                            console.log("onReleased" );
                            var newIndex = calculateColumnIndex(horizontalHeader.activeColumnOffset);

                            if (newIndex !== -1 && newIndex !== horizontalHeader.draggingColumn) {

                                console.log("Column change" )
                                tableView.swupColumnWidth(horizontalHeader.draggingColumn, newIndex);
                                viewService.sessionManager.bookModel.updateColumnOrder(horizontalHeader.draggingColumn, newIndex);
                            }
                            else{
                                console.log("else" )
                                tableView.model.layoutChanged();
                            }

                            horizontalHeader.draggingColumn = -1;
                        }
                    }
                    function firstPosition (column , mouseX){
                        var totalWidth = 0;
                        for (let i = 0; i < column; i++) {
                            totalWidth += tableView.columnWidthProvider(i);
                        }
                        return totalWidth + mouseX;
                    }

                    function calculateColumnIndex(mouseX) {
                        var totalWidth = 0;
                        var columnCount = columnWidths.count;

                        for (let i = 0; i < columnCount; i++) {
                            totalWidth += tableView.columnWidthProvider(i);
                            console.log("totalWidth : " + totalWidth);
                            if (mouseX < totalWidth) {
                                return i;
                            }
                        }
                        return -1;
                    }
                }
                Button {
                    id: sortButton
                    text: "▲▼"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    background: Rectangle {
                        color: sortButton.down ? "#D3D3D3" : headerBack.color
                        border.width: 0
                    }
                    onClicked: {
                        console.log("sortButton Clicked");
                        tableView.model.sort(model.column, Qt.AscendingOrder);
                    }
                    hoverEnabled: false
                }
            }

            function init(tableView) {
                    this.table = tableView;
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
            height: parent.height - 20 - months.height
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
                    visible: true
                    font.pointSize: 12
                    font.family: "arial"
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

            function updateDelegatesPosition(newX, columnIndex) {
                console.log("companyWindow.tableView.updateDelegatesPosition",newX,",",columnIndex );
                for (let row = 0; row < this.model.rowCount(); row++) {
                    let cellDelegate = this.itemAtCell(Qt.point(columnIndex, row));
                    if (cellDelegate !== null) {
                        cellDelegate.x += newX;
                        cellDelegate.z = 2;
                    }
                }
            }
            function swupColumnWidth (oldColumn , newColumn){
                console.log("companyWindow.tableView.swupColumnWidth");
                if (oldColumn !== newColumn){
                    let oldColumnWidth = columnWidths.get(oldColumn).width;
                    let newColumnWidth = columnWidths.get(newColumn).width;
                    let oldHederWidth = headerWidths.get(oldColumn).width;
                    let newHederWidth = headerWidths.get(newColumn).width;

                    columnWidths.set(oldColumn, {"width": newColumnWidth});
                    columnWidths.set(newColumn, {"width": oldColumnWidth});

                    headerWidths.set(oldColumn, {"width": newHederWidth});
                    headerWidths.set(newColumn, {"width": oldHederWidth});
                }
            }

            Component.onCompleted: {
                console.log("companyWindow.tableView.onCompleted");
                horizontalHeader.init(tableView);
            }
        }
     }
    function freezeComponents(freeze)  {
        console.log("bookItem.freezeComponents!" + !freeze)
        bookItem.enabled = !freeze;
    }
}
