pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt.labs.platform

import hex_launcher

Window {
    width: 1000
    height: 800
    visible: true
    title: qsTr("hex launcher")
    color: "#171717"

    Connections{
        target: GlobalModels.presetParser

        function onSendError(error_msg) {
            errorPopUp.openPopup(error_msg)
        }
    }

    Sidebar{
        id: sidebar

        onSettingsClicked: content.push("qrc:/qt/qml/hex_launcher/ui/views/Settings.qml")
        onPresetsClicked: content.push("qrc:/qt/qml/hex_launcher/ui/views/Presets.qml")

        anchors {
            left: parent.left
            top:parent.top
            bottom: parent.bottom
        }
    }

    StackView{
        id: content
        clip: true
        anchors{
            left: sidebar.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom

            leftMargin: 5
            topMargin: 5
        }
        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }

        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }

        initialItem: "qrc:/qt/qml/hex_launcher/ui/views/Presets.qml"

    }
    

    
    

    // ColumnLayout {
    //     anchors.fill: parent

    //     Text{
    //         Layout.alignment: Qt.AlignHCenter
    //         text: "mkwii game launcher"
    //         font.bold: true
    //         font.pointSize: 20
    //         color: "white"
    //     }




    //     CButton {
    //         buttonTextBold: true
    //         buttonTextSize: 9
    //         buttonText: "select dolphin executable"
    //         Layout.alignment: Qt.AlignCenter

    //         onButtonClicked: {
    //             dolphinDialog.open()
    //         }
    //     }


    //     Text{
    //         id: dolphinPathText
    //         Layout.alignment: Qt.AlignHCenter
    //         Layout.preferredHeight: dolphinTextMetrics.height
    //         text: GlobalModels.presetParser.dolphPath
    //         font.bold: true
    //         color: "white"

    //         TextMetrics {
    //             id: dolphinTextMetrics
    //             text: dolphinPathText.text
    //         }

    //     }


    //     CButton {
    //         Layout.alignment: Qt.AlignHCenter
    //         buttonText: "select mkwii file"
    //         buttonTextSize: 9
    //         buttonTextBold: true
    //         onButtonClicked: {
    //             mkwiiDialog.open()
    //         }
    //     }

    //     Text{
    //         id: mkwiiPathText
    //         Layout.alignment: Qt.AlignHCenter
    //         text: GlobalModels.presetParser.mkwiiPath
    //         font.bold: true
    //         Layout.preferredHeight: mkwiiTextMetrics.height
    //         color: "white"

    //         TextMetrics{
    //             id: mkwiiTextMetrics
    //             text: mkwiiPathText.text
    //         }
    //     }

    //     CButton {
    //         buttonText: "install RR"
    //         onButtonClicked: GlobalModels.updater.checkAndUpdate()
            
    //     }
    //     CButton{
    //         buttonText: "add game preset"
    //         buttonTextSize: 8
    //         buttonTextColor: "white"

    //         onButtonClicked: {
    //             addPresetPopUp.openPopup()
    //         }
    //     }


        

    //     // Item {
    //     //     Layout.fillHeight: true
    //     // }
    // }


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
