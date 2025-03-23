import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Basic

Item{
    ColumnLayout{

        anchors{
            top: parent.top
            left: parent.left
            right: parent.right

            
        }
        Text{
            font.pointSize: 48
            text: "settings"
            font.bold: true
            color: "white"
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.topMargin: 10

            Text{
                text: "dolphin path:"
                color: "white"
                font.bold: true
                font.pointSize: 16
            }

            TextField {
                id: textfield
                Layout.fillWidth: true
                text: GlobalModels.presetParser.dolphPath
                //Layout.topMargin: 10
                Layout.preferredHeight: 40
                Layout.rightMargin: 5
                color: "white"


                background: Rectangle{
                    border.color: "#343434"
                    border.width: 2
                    color: "#232425"
                    radius: 10
                }

            }

            CButton{
                onButtonClicked: dolphinDialog.open()    
                buttonText: "select path"
            }

            FileDialog {
                id: dolphinDialog
                title: "select path to dolphin executable"
                onAccepted: {
                    GlobalModels.presetParser.setExecutablePath(file)
            }
    }

        }
    }
}