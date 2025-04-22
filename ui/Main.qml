pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt.labs.platform

import hex_launcher

Window {
    id: mainWindow
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

    onClosing : {
        GlobalModels.handleShutdown()
    }

    

    Sidebar{
        id: sidebar

        onSettingsClicked: content.push("qrc:/qt/qml/hex_launcher/ui/views/Settings.qml")
        onPresetsClicked: content.push("qrc:/qt/qml/hex_launcher/ui/views/Presets.qml")
        onGamesClicked: content.push("qrc:/qt/qml/hex_launcher/ui/views/Games.qml")

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


    Rectangle {
        id: slidingContainer
        height: 70
        
        y: mainWindow.height - height
        width: mainWindow.width

        x: 0

        color: "#434343"

        topLeftRadius: 10
        topRightRadius: 10

        Connections{
            target: GlobalModels

            function onShowDlProgress(){
                slidingContainer.state = "visible"
                slidingLoader.setSource("qrc:/qt/qml/hex_launcher/ui/components/DownloadProgress.qml")
                
            }

        }

        Connections{
            target: GlobalModels.updater
            
            function onModInstallFinished(){
                closeTimer.start()
            }
        }

        Timer{
            id: closeTimer
            interval: 2000
            running: false
            repeat: false
            onTriggered: {
                slidingContainer.state = "hidden"
            }
        }
        
        
        Loader {
            id: slidingLoader
            anchors.fill: parent
            source: ""  // Initially empty
        }
        
        states: [
            State {
                name: "visible"
                PropertyChanges {
                    slidingContainer{
                        y: mainWindow.height - slidingContainer.height
                    }
                }
            },
            State {
                name: "hidden"
                PropertyChanges {
                    slidingContainer{
                        y: mainWindow.height
                    }
                }
            }
        ]
        
        // Transitions for smooth animation
        transitions: [
            Transition {
                from: "*"; to: "visible"
                NumberAnimation { 
                    property: "y"
                    duration: 300
                    easing.type: Easing.OutQuad
                }
            },
            Transition {
                from: "*"; to: "hidden"
                NumberAnimation { 
                    property: "y"
                    duration: 300
                    easing.type: Easing.InQuad
                }
            }
        ]
        
        // Initialize to hidden state
        state: "hidden"
    }

}
