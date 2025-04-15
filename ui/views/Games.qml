pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: gamesView

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Header
        Text {
            text: "games"
            font.pointSize: 48
            font.bold: true
            color: "grey"
            Layout.alignment: Qt.AlignLeft
            Layout.bottomMargin: 10
        }

        // Input row to add new game entry
        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            TextField {
                id: fileNameField
                placeholderText: "File name"
                placeholderTextColor: "grey"
                Layout.preferredWidth: 200
                Layout.preferredHeight: 40
                color: "white"

                background: Rectangle{
                    border.color: "#343434"
                    border.width: 2
                    color: "#232425"
                    radius: 10
                }
            }
            TextField {
                id: filePathField
                placeholderText: "File path"
                placeholderTextColor: "grey"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                color: "white"
                cursorPosition: 0
                onFocusChanged: {
                            if (focus) {
                                cursorPosition = 0
                            }
                        }

                background: Rectangle{
                    border.color: "#343434"
                    border.width: 2
                    color: "#232425"
                    radius: 10
                }
            }

            CButton{
                buttonText: "select\ngame file"
                buttonTextSize: 8
                onButtonClicked: gameDialog.open()

            }
            CButton {
                buttonText: "add"
                font.bold: true
                onButtonClicked: {
                    // Ensure fileName and filePath are not empty
                    if(fileNameField.text !== "" && filePathField.text !== "") {
                        // 'gameModel' must be exposed as a context property from C++ (an instance of your FileListModel)
                        GlobalModels.gamesModel.addFile(fileNameField.text, filePathField.text)
                        fileNameField.text = ""
                        filePathField.text = ""
                    }
                }
            }

            FileDialog{
                id: gameDialog
                title: "select a game file"

                property bool forDelegate: false
                property int delegateIndex

                function newPath(index){
                    delegateIndex = index
                    forDelegate = true
                    gameDialog.open()
                }

                onAccepted :{

                    if(forDelegate){
                        var item = fileListView.itemAtIndex(delegateIndex)
                        item.filePath = GlobalModels.getLocalPath(selectedFile)
                    }
                    else{
                        filePathField.text = GlobalModels.getLocalPath(selectedFile)
                    }
                }


            }
        }

        // List view of game entries
        ListView {
            id: fileListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rightMargin: 5
            model: GlobalModels.gamesModel   // 'gameModel' is your FileListModel instance
            clip: true
            delegate: Rectangle {
                id: fileListDelegate
                width: fileListView.width
                height: 60
                color: "#222222"
                border.color: "#555555"
                border.width: 1
                radius: 4

                required property string fileName
                required property string filePath
                required property int index

                property bool isEditing: false

                RowLayout {
                    anchors.fill: parent
                    spacing: 20

                    TextField {
                        id: editFilename
                        placeholderText: "File name"
                        placeholderTextColor: "grey"
                        text: fileListDelegate.fileName
                        cursorPosition: 0
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 40
                        Layout.leftMargin: 5
                        color: "white"
                        visible: fileListDelegate.isEditing

                        background: Rectangle{
                            border.color: "#343434"
                            border.width: 2
                            color: "#232425"
                            radius: 10
                        }
                    }

                    TextField {
                        id: editFilePath
                        visible: fileListDelegate.isEditing
                        text: fileListDelegate.filePath
                        placeholderText: "File path"
                        placeholderTextColor: "grey"
                        Layout.fillWidth: true
                        cursorPosition: 0
                        Layout.preferredHeight: 40
                        color: "white"
                        onFocusChanged: {
                            if (focus) {
                                cursorPosition = 0
                            }
                        }

                        background: Rectangle{
                            border.color: "#343434"
                            border.width: 2
                            color: "#232425"
                            radius: 10
                        }
                    }

                    Text {
                        id: fileNameText
                        text: fileListDelegate.fileName
                        color: "white"
                        font.pixelSize: 16
                        Layout.preferredWidth: 200
                        Layout.alignment: Qt.AlignVCenter
                        elide: Text.ElideRight
                        visible: !fileListDelegate.isEditing
                    }
                    Text {
                        id: filePathText
                        text: fileListDelegate.filePath
                        color: "lightgray"
                        font.pixelSize: 16
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        elide: Text.ElideRight
                        visible: !fileListDelegate.isEditing
                    }

                    CButton{
                        buttonText: "select\nfile"
                        buttonTextSize: 8
                        Layout.alignment: Qt.AlignVCenter
                        visible: fileListDelegate.isEditing

                        onButtonClicked : {
                            gameDialog.newPath(fileListDelegate.index)
                        }
                    }

                    CButton{
                        buttonText: "save"
                        buttonTextSize: 8
                        Layout.alignment: Qt.AlignVCenter
                        visible: fileListDelegate.isEditing

                        onButtonClicked : {

                            if(fileListDelegate.isEditing){
                                fileListDelegate.isEditing = !fileListDelegate.isEditing
                            }
                            GlobalModels.gamesModel.updateFile(fileListDelegate.index, 
                                editFilename.text, editFilePath.text)
                        }

                        
                    }

                    CButton{
                        buttonText: "edit"
                        buttonTextSize: 8
                        Layout.alignment: Qt.AlignVCenter
                        onClicked: {
                            fileListDelegate.isEditing = !fileListDelegate.isEditing
                            
                        }
                    }

                    CButton {
                        buttonText: "remove"
                        Layout.alignment: Qt.AlignVCenter
                        buttonTextSize: 8
                    }
                }
            }
        }
    }
}