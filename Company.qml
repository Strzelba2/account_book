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

    Component.onCompleted: {
        if (loginWindowRef) {
            loginWindowRef.visible = false;
            loginWindowRef.changeStaysOnTopHint()
        }
        stackLayout.currentIndex = -1;
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
        anchors.topMargin:60
        height: 50

        Row {
            anchors.fill: parent
            CustomButton {
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
                    }
                }
            }
        }
    }

    Rectangle {
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
            property int draggingColumn: -1
            property int pressedX: -1
            property int targetColumn: -1
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
            height: parent.height - 20
            anchors.left: verticalHeader.right
            anchors.top: horizontalHeader.bottom
//            interactive: false
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
                    anchors.fill: parent
                    text: model.display
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

                        if(headerWidths.get(column).width > textItem.implicitWidth + 30 /*&& headerWidths.get(model.column).width > columnWidths.get(model.column).width*/){
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

                    console.log("oldColumnWidth: " , oldColumnWidth);
                    console.log("newColumnWidth: " , newColumnWidth);


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

    Connections {
        target: viewService.sessionManager.bookModel
        function onBookDataError (error) {
            console.log("viewService.sessionManage.bookModel.onSessionError")
            loginWindowRef.showPopup(error,false);
        }
    }

}
