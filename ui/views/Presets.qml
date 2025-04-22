pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt.labs.platform

import hex_launcher

Item{

    AddPresetPopUp{
            id: addPresetPopUp
        }

    ColumnLayout{
        anchors.fill: parent
        spacing: 0
        Text{
            Layout.alignment: Qt.AlignLeft
            text: "presets"
            font.pointSize: 48
            font.bold: true
            color: "grey"
        }

        RowLayout{
            CButton{
                Layout.alignment: Qt.AlignLeft
                buttonText: "+"
                font.pointSize: 16
                font.bold: true

                onButtonClicked: addPresetPopUp.openPopup()
            }

            CButton{
                Layout.alignment: Qt.AlignLeft
                buttonText: "install retro rewind"
                font.pointSize: 16
                font.bold: true

                onButtonClicked: GlobalModels.downloadRR()  

            }
        }
        

        GridView{
            id: presetGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 5
            model: GlobalModels.gamePresetModel

            cellWidth: 300
            cellHeight: 350
            clip: true



            ScrollBar.vertical: ScrollBar {
                id:presetGridScrollbar
                policy: ScrollBar.AlwaysOn

                width: 10

                contentItem: Rectangle {
                    implicitWidth: 10
                    radius: 3
                    color: "#606060"
                }


            }
            delegate: Component{
                Rectangle{
                    id: presetDelegate
                    color: "#222222"
                    width: presetGrid.cellWidth-8

                    height: presetGrid.cellHeight-8

                    required property string display_name
                    required property var option_model
                    required property string json_path
                    required property int index
                    required property int gameIndex

                    property bool hovering: false
                    signal hoverChanged()

                    radius: 5
                    border.color: "#606060"
                    border.width: 2
                    ColumnLayout{
                        anchors.fill: parent

                        Text{
                            id: optionText
                            text: presetDelegate.display_name
                            Layout.topMargin: 10
                            Layout.alignment: Qt.AlignHCenter
                            Layout.bottomMargin: 10
                            color: "white"
                            font.bold: true
                            font.pointSize: 14

                        }
                        
                        ListView{
                            id: optionList
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.margins: 5
                            model: presetDelegate.option_model
                            clip: true
                            spacing: 5

                            ScrollBar.vertical: ScrollBar {
                                id:optionScrollbar
                                policy: ScrollBar.AlwaysOn

                                width: 10

                                contentItem: Rectangle {
                                    implicitWidth: 10
                                    radius: 3
                                    color: "#2e2e2e"
                                }

                                background: Rectangle {
                                    implicitWidth: 10
                                    color: "#7a7a7a"
                                    radius: 3
                                }

                            }

                            delegate: Component{
                                Item{
                                    id: optionDelegate
                                    required property string choiceName
                                    required property list<string> choicesList
                                    required property int choiceIndex
                                    required property string presetName

                                    height: 40
                                    width: optionList.width
                                    RowLayout{
                                        anchors.fill: parent
                                        Text{
                                            id: comboLabel
                                            Layout.preferredWidth: 100
                                            text: optionDelegate.choiceName + ":"
                                            color: "white"
                                        }
                                        ComboBox{
                                            id: comboOptions
                                            Layout.preferredWidth: 100
                                            model: optionDelegate.choicesList

                                            delegate : ItemDelegate{
                                                id: comboDelegate
                                                width: comboOptions.width

                                                required property var model
                                                required property int index

                                                contentItem: Text {
                                                    text: comboDelegate.model[comboOptions.textRole]
                                                    font: comboOptions.font
                                                    elide: Text.ElideRight
                                                    color: "white"
                                                    verticalAlignment: Text.AlignVCenter
                                                }

                                                background: Rectangle{
                                                    anchors.fill: comboDelegate
                                                    color: comboDelegate.highlighted ? "#4e4e4e" : "transparent"

                                                }

                                                highlighted: comboOptions.highlightedIndex === index

                                            }

                                            indicator: Canvas {
                                                id: canvas
                                                x: comboOptions.width - width - comboOptions.rightPadding
                                                y: comboOptions.topPadding + (comboOptions.availableHeight - height) / 2
                                                width: 12
                                                height: 8
                                                contextType: "2d"

                                                Connections {
                                                    target: comboOptions
                                                    function onPressedChanged() {canvas.requestPaint();}

                                                }

                                                onPaint: {
                                                    context.reset();
                                                    context.moveTo(0, 0);
                                                    context.lineTo(width, 0);
                                                    context.lineTo(width / 2, height);
                                                    context.closePath();
                                                    context.fillStyle = "#606060";
                                                    context.fill();
                                                }

                                            }

                                            contentItem: Text {
                                                leftPadding: 5
                                                rightPadding: comboOptions.indicator.width + comboOptions.spacing
                                                text: comboOptions.displayText
                                                font.bold: true
                                                color: "white"
                                                verticalAlignment: Text.AlignVCenter
                                                elide: Text.ElideRight
                                            }

                                            background: Rectangle {
                                                implicitWidth: 100
                                                implicitHeight: 40
                                                border.color: "#606060"
                                                border.width: comboOptions.visualFocus ? 2 : 1
                                                color: "transparent"
                                                radius: 2
                                            }

                                            popup: Popup {
                                                y: comboOptions.height - 1
                                                width: comboOptions.width
                                                implicitHeight: contentItem.implicitHeight
                                                padding: 1
                                                contentItem: ListView{
                                                    clip: true
                                                    interactive: false
                                                    implicitHeight: contentHeight
                                                    model: comboOptions.popup.visible ? comboOptions.delegateModel: null
                                                    currentIndex: comboOptions.highlightedIndex
                                                }

                                                background: Rectangle {
                                                    border.color: "#606060"
                                                    radius: 2
                                                    color: "#2e2e2e"

                                                }
                                            }

                                            onActivated: (index) => {
                                                GlobalModels.gamePresetModel.setOptionChoice(optionDelegate.choiceIndex, optionDelegate.presetName, index)
                                            }

                                        }

                                        

                                        Connections{
                                            target: GlobalModels.presetParser

                                            function onGetOptions(){
                                                GlobalModels.presetParser.addOption(comboLabel.text, comboOptions.indexOfValue(comboOptions.currentText))
                                            }
                                        }
                                    }
                                }

                            }
                        }

                        Text{
                            visible: GlobalModels.gamesModel.rowCount() == 0
                            Layout.alignment: Qt.AlignHCenter
                            text: "add game from sidebar first"
                            color: "white"
                            font.bold: true
                        }
                        RowLayout {
                            Layout.alignment: Qt.AlignHCenter

                            Text {
                                text: "base game"
                                color: "white"
                            }

                            CCombobox{
                                id: gameSelector
                                comboModel: GlobalModels.gamesModel
                                clip: true
                                comboTextRole: "fileName"
                                comboIndex: {
                                    if(presetDelegate.gameIndex >= GlobalModels.gamesModel.rowCount()){
                                        return -1
                                    }
                                    else{
                                        return presetDelegate.gameIndex
                                    }
                                }

                                Layout.preferredWidth: 100

                                property string selectedItem

                                onActivated : (index) => {
                                    GlobalModels.gamePresetModel.setGamePath(presetDelegate.index, index)
                                }
                            }
                        }

                        RowLayout{
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredHeight: childrenRect.height
                            Layout.fillWidth: true

                            CButton{
                                id: runBtn
                                buttonText: "run game"
                                buttonTextSize: 10
                                Layout.alignment: Qt.AlignLeft

                                onButtonClicked : {
                                    var status = GlobalModels.presetParser.runGame(presetDelegate.json_path, presetDelegate.display_name, presetDelegate.gameIndex)
                                    if(status == 1){
                                        saveDialog.openDialog(presetDelegate.display_name)
                                    }
                                }
                            }

                            CButton{
                                id: saveBtn
                                buttonText: "save settings"
                                buttonTextSize: 10
                                Layout.alignment: Qt.AlignRight

                                onButtonClicked: {
                                //emit cpp signal from here, then add connection
                                //GlobalModels.presetParser.savePreset(display_name)

                                if(presetDelegate.json_path != ""){
                                    console.log(presetDelegate.json_path)
                                    var status = GlobalModels.presetParser.updatePreset(presetDelegate.display_name, presetDelegate.json_path, presetDelegate.gameIndex)

                                    if(status == 1){
                                        saveDialog.openDialog(presetDelegate.display_name)
                                    }
                                }
                                else{
                                    saveDialog.openDialog(presetDelegate.display_name)
                                }

                                }
                            }

                            CButton{
                                id: saveLocation
                                buttonText: "change save\nlocation"
                                buttonTextSize: 10
                                Layout.preferredWidth: 90

                                onButtonClicked : {
                                    saveDialog.openDialog(presetDelegate.display_name, true)
                                }
                            }


                        }


                        FolderDialog{
                            id: saveDialog
                            title: "select save path"
                            property string presetName
                            property bool onlyPath: false

                            onAccepted : {
                                if(onlyPath){
                                    GlobalModels.gamePresetModel.setJsonPath(presetName, folder)
                                    GlobalModels.presetParser.savePreset(presetName, folder, presetDelegate.gameIndex)
                                }
                                else{
                                    var status = GlobalModels.presetParser.savePreset(presetName, folder, presetDelegate.gameIndex)
                                }
                            }

                            function openDialog(preset_name: string, saveOnlyPath=false){
                                if(saveOnlyPath){
                                    onlyPath = true
                                }
                                else{
                                    onlyPath = false
                                }

                                presetName = preset_name
                                saveDialog.open()

                            }
                        }
                    }

                    Canvas {
                        id: xButtonCanvas
                        width: 20
                        height: 20
                        //x: presetDelegate.x + 5 // Position relative to the delegate
                        //y: presetDelegate.y + 5 // Position relative to the delegate
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.topMargin: 3
                        anchors.leftMargin: 3
                        visible: delegateHover.hovered // Show only when the delegate is hovered
                        contextType: "2d"

                        Connections {
                            target: presetDelegate
                            function onHoverChanged() { xButtonCanvas.requestPaint(); }
                        }

                        onPaint: {
                            var ctx = getContext("2d");
                            ctx.clearRect(0, 0, width, height); // Clear previous drawings

                                    // Draw an "X"
                            ctx.beginPath();
                            ctx.moveTo(4, 4); // Start at top-left
                            ctx.lineTo(width - 4, height - 4); // Draw to bottom-right
                            ctx.moveTo(width - 4, 4); // Start at top-right
                            ctx.lineTo(4, height - 4); // Draw to bottom-left
                            ctx.lineWidth = 2;
                            ctx.strokeStyle = "#7a7a7a"; // Red color for the "X"
                            ctx.stroke();
                        }

                        TapHandler{
                            gesturePolicy: TapHandler.ReleaseWithinBounds
                            onTapped: GlobalModels.gamePresetModel.deletePreset(presetDelegate.display_name)
                        }

                    }

                    HoverHandler{
                        id: delegateHover
                        onHoveredChanged: presetDelegate.hoverChanged()
                    }

                }

            }
        }

        
    }
    
}
