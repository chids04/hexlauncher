import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt.labs.platform

import hex_launcher
//import com.rr_launcher.globalModel


Popup {

    id: popup
    //width: errorPopUpDelegate.width + 20
    //height: errorPopUpDelegate.height + 20
    modal: true
    focus: true

    anchors.centerIn: Overlay.overlay
    dim: true

    function openPopup(){
        popup.open()
    }

    onClosed: {
        inputField.text = ""
    }

    enter: Transition {
                ParallelAnimation {
                    id: popIn
                    PropertyAnimation {
                        target: popup
                        property: "scale"
                        from: 0.9
                        to: 1
                        duration: 50
                    }
                    PropertyAnimation {
                        target: popup
                        property: "opacity"
                        from: 0.9
                        to: 1
                        duration: 50
                    }
                }
            }
            exit: Transition {
                ParallelAnimation {
                    id: popOut
                    PropertyAnimation {
                        target: popup
                        property: "scale"
                        from: 1
                        to: 0.9
                        duration: 50
                    }
                    PropertyAnimation {
                        target: popup
                        property: "opacity"
                        from: 1
                        to: 0.9
                        duration: 50
                    }
                }
            }

        background: Rectangle{
            anchors.fill: parent
            color: "#202020"
            border.color: "#2f3137"
            border.width: 2
            radius: 20
        }


    contentItem: ColumnLayout{

        CButton{
            buttonText: "select riivolution xml"
            buttonTextSize: 8
            Layout.alignment: Qt.AlignHCenter
            buttonTextBold: true

            onButtonClicked: {
                riivoDialog.open()
            }
        }

        Text{
            id: xmlPath
            font.bold: true
            color: "white"
        }

        TextField{
            id: inputField
            Layout.topMargin: 7
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 200

            placeholderText: "enter preset name"
            placeholderTextColor: "#6a6f76"
            color: "white"

            background: Rectangle{
                color: "#2f3137"
                radius: 10

            }

            TextMetrics{
                id: metrics
                elideWidth: inputField.width
            }
        }

        CButton{
            Layout.topMargin: 20
            buttonText: "confirm"
            buttonTextSize: 6
            Layout.alignment: Qt.AlignRight

            onButtonClicked: {
                if(inputField.text == ""){
                    errorPopup.openPopup("game preset must have a name")
                }
                else if(riivoDialog.foundXml == false){
                    errorPopup.openPopup("select a valid riivolution xml")
                }
                else{
                    if(GlobalModels.gamePresetModel.isDuplicate(inputField.text)){
                        errorPopup.openPopup("presets must have unqiue names")
                    }
                    else{
                        GlobalModels.presetParser.parseRiivoXML(riivoDialog.file, inputField.text)
                        inputField.text = ""
                        xmlPath.text = ""
                        riivoDialog.foundXml = false
                        popup.close()
                    }

                }

            }
        }
    }

    FileDialog {
        id: riivoDialog
        property bool foundXml: false
        title: "select riivolution xml"
        nameFilters: ["xml files (*.xml)", "All Files (*)"]

        onAccepted: {
            //add selected mkwii game path to next to mkwii name
            foundXml = true
            xmlPath.text = basename(file.toString())
            console.log(xmlPath.text)
        }

        function basename(str){
            return(str.slice(str.lastIndexOf("/")+1))
        }
    }

    ErrorPopUp{
        id: errorPopup
    }
}

