import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects


// ErrorPopup.qml

Popup {
    id: popup
    //width: errorPopUpDelegate.width + 20
    //height: errorPopUpDelegate.height + 20
    modal: true
    focus: true

   // anchors.centerIn: Overlay.overlay
    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: parent.height - height - 20
    dim: true

    padding: 10

    property string errorText

    function openPopup(text){
        errorText = text
        popup.open()
    }

    Timer{
        id: autoClose
        interval: 2000
        running: popup.visible
        onTriggered: popup.close()
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
            id: popUpBackground
            anchors.fill: parent
            color: "#202020"
            border.color: "#2f3137"
            border.width: 2
            radius: 20

        }



    contentItem: Text{
        text: "<b><u>Error:</u></b> " + popup.errorText
        font.pointSize: 10
        color: "white"

    }



}

