pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: sideBar

    width: 105
    height: 700
    state: 'close'

    signal settingsClicked()
    signal presetsClicked()
    signal gamesClicked()

    states: [
        State {
            name: 'open'

            PropertyChanges {
                target: sideBar
                width: 270
            }

            PropertyChanges {
                target: timer
                index: 0
            }
        },
        State {
            name: 'close'

            PropertyChanges {
                target: sideBar
                width: 105
            }

            PropertyChanges {
                target: timer
                index: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: 'close'
            to: 'open'

            NumberAnimation {
                properties: 'width'
                duration: 300
                easing.type: Easing.OutCubic
            }

            ScriptAction {
                script: {
                    timer.start();
                }
            }
        },
        Transition {
            from: 'open'
            to: 'close'

            SequentialAnimation {

                ScriptAction {
                    script: {
                        timer.start();
                    }
                }

                PauseAnimation {
                    duration: 600
                }

                NumberAnimation {
                    properties: 'width'
                    duration: 300
                    easing.type: Easing.OutCubic
                }
            }
        }
    ]

    Timer {
        id: timer

        property int index: 0

        interval: 10

        onTriggered: {
            if (sideBar.state == 'open')
                columnItems.itemAt(index).state = 'left';
            else
                columnItems.itemAt(index).state = 'middle';

            if (++index != 9)
                timer.start();
        }
    }

    Rectangle {
        id: body

        topRightRadius: 10
        bottomRightRadius: 10

        border.color: "#3a3a3a"
        color: '#242424'
        anchors.fill: parent

        ColumnLayout {
            id: buttonColumn

            width: parent.width
            spacing: 20
            anchors { top: parent.top; topMargin: 30 }

            Repeater {
                id: columnItems

                model: ['Menu', 'Presets', 'Games', 'Settings']
                delegate: Rectangle {
                    id: button

                    required property int index
                    required property string modelData

                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 50
                    radius: 10
                    color: buttonMouseArea.containsMouse ? "#3a3a3a" : "#2e2e2e"
                    Layout.alignment: Qt.AlignLeft
                    Layout.topMargin: button.index === 1 ? 20 : 0
                    state: 'middle'

                    Behavior on color {
                        ColorAnimation {
                            duration: 200
                        }
                    }

                    states: [
                        State {
                            name: 'left'

                            PropertyChanges {
                                target: button
                                Layout.leftMargin: 10
                                Layout.preferredWidth: button.index !== 0 ? 240 : 50
                            }

                            PropertyChanges {
                                target: title
                                opacity: 1
                            }
                        },
                        State {
                            name: 'middle'

                            PropertyChanges {
                                target: button
                                Layout.leftMargin: Math.ceil((sideBar.width - 50) / 2)
                                Layout.preferredWidth: 50
                            }

                            PropertyChanges {
                                target: title
                                opacity: 0
                            }
                        }
                    ]

                    transitions: [
                        Transition {
                            from: 'middle'
                            to: 'left'

                            NumberAnimation {
                                properties: 'Layout.leftMargin, Layout.preferredWidth, opacity'
                                duration: 300
                                easing.type: Easing.InOutSine
                            }
                        },
                        Transition {
                            from: 'left'
                            to: 'middle'

                            NumberAnimation {
                                properties: 'Layout.leftMargin, Layout.preferredWidth, opacity'
                                duration: 300
                                easing.type: Easing.InOutSine
                            }
                        }
                    ]

                    MouseArea {
                        id: buttonMouseArea

                        hoverEnabled: true
                        anchors.fill: parent

                        onClicked: {
                            if (button.index === 0) {
                                if (sideBar.state == 'close')
                                    sideBar.state = 'open';
                                else
                                    sideBar.state = 'close';
                            }
                            else if(button.modelData == "Settings"){
                                sideBar.settingsClicked()
                            }

                            else if(button.modelData == "Presets"){
                                sideBar.presetsClicked()
                            }
                            else if(button.modelData == "Games"){
                                sideBar.gamesClicked()
                            }
                        }
                    }

                    Image {
                        id: icon

                        source: "qrc:/qt/qml/hex_launcher/ui/icons/" + button.modelData + '.svg'
                        sourceSize: Qt.size(30, 30)
                        anchors { verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 10 }
                        layer.enabled: true
                        layer.effect: MultiEffect{
                            source: icon
                            anchors.fill: icon
                            colorization: 1.0
                            brightness: 1.0
                            colorizationColor: "white"

                        }
                    }

                    Text {
                        id: title
                        font.pointSize: 16
                        color: "white"
                        font.capitalization: Font.AllLowercase
                        text: button.index === 0 ? '' : button.modelData
                        anchors { verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 55 }

                    }
                }
            }
        }
    }

}
