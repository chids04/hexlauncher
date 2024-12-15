pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt.labs.platform

import "./components"

//import com.rr_launcher.globalModel
import hex_launcher


Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("hex launcher")
    color: "#242424"

    Connections{
        target: GlobalModels.presetParser

        function onSendError(error_msg) {
            errorPopUp.openPopup(error_msg)
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Text{
            Layout.alignment: Qt.AlignHCenter
            text: "mkwii game launcher"
            font.bold: true
            font.pointSize: 20
            color: "white"
        }

        CButton {
            buttonTextBold: true
            buttonTextSize: 9
            buttonText: "select dolphin executable"
            Layout.alignment: Qt.AlignHCenter

            onButtonClicked: {
                dolphinDialog.open()
            }
        }

        Text{
            id: dolphinPathText
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: dolphinTextMetrics.height
            text: GlobalModels.presetParser.dolphPath
            font.bold: true
            color: "white"

            TextMetrics {
                id: dolphinTextMetrics
                text: dolphinPathText.text
            }

        }


        CButton {
            Layout.alignment: Qt.AlignHCenter
            buttonText: "select mkwii file"
            buttonTextSize: 9
            buttonTextBold: true
            onButtonClicked: {
                mkwiiDialog.open()
            }
        }

        Text{
            id: mkwiiPathText
            Layout.alignment: Qt.AlignHCenter
            text: GlobalModels.presetParser.mkwiiPath
            font.bold: true
            Layout.preferredHeight: mkwiiTextMetrics.height
            color: "white"

            TextMetrics{
                id: mkwiiTextMetrics
                text: mkwiiPathText.text
            }
        }



        CButton{
            buttonText: "add game preset"
            buttonTextSize: 8
            buttonTextColor: "white"

            onButtonClicked: {
                addPresetPopUp.openPopup()
            }
        }


        GridView{
            id: presetGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 5
            model: GlobalModels.gamePresetModel

            cellWidth: 250
            cellHeight: 300
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
                    color: "#2e2e2e"
                    width: presetGrid.cellWidth-8

                    height: presetGrid.cellHeight-8

                    required property string display_name
                    required property var option_model
                    required property string json_path
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
                                    width: 200
                                    RowLayout{
                                        anchors.fill: parent
                                        Text{
                                            id: comboLabel
                                            Layout.preferredWidth: 90
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

                        RowLayout{
                            Layout.fillWidth: true
                            Layout.preferredHeight: childrenRect.height

                            CButton{
                                id: runBtn
                                buttonText: "run game"
                                buttonTextSize: 8
                                Layout.alignment: Qt.AlignLeft

                                onButtonClicked : {
                                    GlobalModels.presetParser.runGame(presetDelegate.json_path)
                                }
                            }

                            CButton{
                                id: saveBtn
                                buttonText: "save settings"
                                buttonTextSize: 8
                                Layout.alignment: Qt.AlignRight

                                onButtonClicked: {
                                //emit cpp signal from here, then add connection
                                //GlobalModels.presetParser.savePreset(display_name)
                                saveDialog.openDialog(presetDelegate.display_name)
                                }
                            }


                        }


                        FolderDialog{
                            id: saveDialog
                            title: "select save path"
                            property string presetName
                            onAccepted : {
                                GlobalModels.presetParser.savePreset(presetName, folder)
                            }

                            function openDialog(preset_name){
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

        // Item {
        //     Layout.fillHeight: true
        // }
    }


    FileDialog {
        id: dolphinDialog
        title: "select path to dolphin executable"
        onAccepted: {
            GlobalModels.presetParser.setExecutablePath(file)
        }
    }




    FileDialog {
        id: mkwiiDialog
        property bool fileAdded: false
        title: "select mkwii game file"

        onAccepted: {
            GlobalModels.presetParser.setGamePath(file)
        }

    }
    
    FileDialog{
        id: savePresetDialog
        title: "select save location"
    	property string presetName;
        
        onAccepted: {
            if(GlobalModels.presetParser.mkwiiPath == ""){
                errorPopUp.openPopup("please select mkwii game file")
            }
            else{
                GlobalModels.presetParser.writeToJson(GlobalModels.presetParser.mkwiiPath, savePresetDialog.file, presetName)
            }

        }

        function savePreset(preset_name){
            savePresetDialog.presetName = preset_name
        }
    }

    ErrorPopUp{
        id: errorPopUp
    }

    AddPresetPopUp{
        id: addPresetPopUp
    }

}
