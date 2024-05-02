import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import "qrc:/components"

import com.mycompany.bookmodel 1.0

Item {
    signal editFocusLoader()
    signal changePopupPolice()
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
                        let columNumber = model.column;

                        if(headerText.width + offsetCell >columnWidths.get(columNumber).width){
                            columnWidths.set(columNumber, {"width": headerText.width + offsetCell});
                            tableView.model.changeColumnWidth(columNumber);
                        }
                    }
                    Component.onCompleted: {
                        console.log("AddBooks.tableView.horizontalHeader.onCompleted");
                        let columNumber = model.column;
                        let currentWidth = headerWidths.get(columNumber) ? headerWidths.get(columNumber).width : 0;

                        if (headerText.width >currentWidth){
                            console.log("columNumber:",columNumber)
                            headerWidths.set(columNumber, {"width": headerText.width + offsetCell});
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
            property bool comboActive: false

            Rectangle {
                id: tableRec
                width: addBookItem.width - 30
                height: addBookItem.height - 30
                anchors.left: verticalHeader.right
                anchors.top: horizontalHeader.bottom
                anchors.topMargin: 5
                anchors.leftMargin: 5
                color: "transparent"

                MouseArea {
                    id: mouseTableView
                    anchors.fill: parent
                    enabled: tableView.comboActive
                    onClicked: {
                        console.log("mouseTableView.onClicked")
                        editFocusLoader()
                    }
                }
            }

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
                property int columnDelegate: -1
                property int rowDelegate: -1

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

                Loader {
                        id: editorLoader
                        anchors.fill: parent
                        active: model.roleName=== BookModel.ContractorRole
                        sourceComponent:  comboBoxComponent

                        property bool visibleEditor: false
                        property bool focusLoader: false

                    }

                Component {
                    id: comboBoxComponent

                    ComboBox {
                        id: subcontractorCombo
                        height: 50
                        width: editorLoader.width
                        model: viewService.sessionManager.subcontractorModel
                        textRole: "shortName"
                        visible: editorLoader.visibleEditor
                        currentIndex: -1
                        focus: editorLoader.focusLoader

                        Connections {
                            target: tableView.parent.parent

                            function onEditFocusLoader() {
                                console.log("editorLoader.onEditFocusLoader")
                                hideCombo();
                            }
                            function onChangePopupPolice(){
                                console.log("editorLoader.onChangePopupPolice")
                                unlockPopupClose()
                            }
                        }

                        property bool isPopupOpened: false

                        delegate: ItemDelegate {
                            width: subcontractorCombo.width -10
                            text: model.shortName
                            highlighted: subcontractorCombo.highlightedIndex === index
                        }

                        contentItem: Text {
                            text: subcontractorCombo.displayText
                            font: subcontractorCombo.font
                            color: subcontractorCombo.pressed ? "#17a81a" : "#21be2b"
                        }

                        popup: Popup {
                            id: subcontractorPopup
                            width: subcontractorCombo.width
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
                            onClosed:{
                                console.log("subcontractorPopup.onClosed")
                                subcontractorCombo.isPopupOpened = false
                                subcontractorCombo.hideCombo()
                            }

                            onOpened:{
                                console.log("subcontractorPopup.onOponed")
                                subcontractorCombo.isPopupOpened = true
                            }
                            Column {
                                width: subcontractorCombo.width -10

                                ListView {
                                    id: subcontractorlistView
                                    width: subcontractorCombo.width - 10
                                    model: subcontractorCombo.model
                                    clip: true
                                    implicitHeight: contentHeight

                                    delegate: ItemDelegate {
                                        width: subcontractorCombo.width
                                        text: model.shortName
                                        background: Rectangle {
                                            color: {
                                                subcontractorlistView.currentIndex === index ? "#e7e7e7" : "white"
                                            }
                                        }
                                        onClicked: {
                                            subcontractorCombo.currentIndex = index
                                            subcontractorCombo.displayText = model.shortName
                                        }
                                    }
                                }

                                CustomButton {
                                    id: btnAddSubcontractor
                                    width: 120
                                    height: 30
                                    opacity: 1
                                    text: "Add"
                                    colorPressed: "#d9d7d4"
                                    font.family: "Segoe UI"
                                    colorMouseOver: "#bfbdbb"
                                    colorDefault: "#b3b2b1"
                                    font.pointSize: 16
                                    onClicked: {
                                        console.log("Added subcontractor")
                                        var component = Qt.createComponent("qrc:/SubcontractorWindow.qml");
                                        if (component.status === Component.Ready) {
                                            var subcontractorWindow = component.createObject(companyWindow);

                                            companyWindow.isSubcontractorWindow = true;
                                            companyWindow.subcontractorWindow = subcontractorWindow;

                                            companyLoader.item.freezeComponents(true)
                                            subcontractorCombo.lockPopupClose();
                                            companyWindow.subcontractorWindow.show();
                                            companyWindow.subcontractorWindow.changeCompany()

                                        } else {
                                            console.error("Cannot load Company.qml file:", component.errorString());
                                        }
                                    }
                                }
                            }
                        }

                        onVisibleChanged: {
                            console.log("onVisibleChanged.subcontractorCombo")
                            if (visible) {
                                editorLoader.focusLoader = true
                                editorLoader.z =1
                                subcontractorCombo.z = 2;
                                let newIndex = find(textItem.text,Qt.MatchContains)
                                subcontractorlistView.currentIndex = newIndex

                            }else{
                                editorLoader.z = 0
                                subcontractorCombo.z = 0;
                                subcontractorlistView.currentIndex = -1;
                            }
                        }
                        onCurrentIndexChanged: {

                            if (currentIndex !== -1 ) {
                                let subcontractorId = model.get(currentIndex).id;
                                visible = false
                                tableView.model.setData(tableView.model.index(recDell.rowDelegate, recDell.columnDelegate), subcontractorId, Qt.EditRole);
                                recDell.checkWidth();
                            }
                        }
                        onAccepted: {
                            console.log("onAccepted")
                        }

                        onFocusChanged: {
                            console.log("subcontractorCombo.onFocusChanged")

                            if (!focus && subcontractorCombo.isPopupOpened) {
                                hideCombo();
                            }
                        }
                        Keys.onReturnPressed: {
                            console.log("subcontractorCombo.Keys.onReturnPressed")
                            subcontractorCombo.popup.close();
                            hideCombo()
                        }
                        Component.onCompleted: {
                            console.log("subcontractorCombo.onCompleted")

                        }
                        function hideCombo(){
                            console.log("subcontractorCombo.hideCombo")
                            editorLoader.visibleEditor = false;
                            editorLoader.focusLoader = false;
                            tableView.comboActive = false;
                        }

                        function lockPopupClose() {
                            console.log("subcontractorCombo.lockPopupClose")
                            subcontractorPopup.closePolicy = Popup.NoAutoClose;
                        }

                        function unlockPopupClose() {
                            console.log("subcontractorCombo.unlockPopupClose")
                            subcontractorPopup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside;
                        }
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
                        recDell.checkWidth();
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

                        recDell.columnDelegate = model.column
                        if( !tableView.model.checkIfID(recDell.columnDelegate)){
                            if(model.roleName=== BookModel.ContractorRole){
                                editorLoader.visibleEditor = true;
                                editorLoader.width = columnWidths.get(recDell.columnDelegate).width

                                recDell.rowDelegate = model.row;
                                tableView.comboActive = true

                            }else{
                                editor.showEditor();
                            }
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

                function checkWidth()
                {
                    console.log("recDell.checkWidth")
                    if (textItem.implicitWidth + offsetCell > textItem.width ){
                        console.log("change column");
                        columnWidths.set(column, {"width": textItem.implicitWidth + offsetCell});
                        recDell.implicitWidth = columnWidths.get(model.column).width;
                        Qt.callLater(tableView.model.layoutChanged);
                    }

                    if(headerWidths.get(column).width > textItem.implicitWidth + offsetCell ){
                        console.log("change header");
                        columnWidths.set(column, {"width": textItem.implicitWidth + offsetCell});

                        recDell.implicitWidth = columnWidths.get(model.column).width;
                        Qt.callLater(tableView.model.layoutChanged);
                    }
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

