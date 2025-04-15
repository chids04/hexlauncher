pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ComboBox{
    id: comboOptions
    Layout.preferredWidth: 100

    property alias comboTextRole: comboOptions.textRole
    property alias comboModel: comboOptions.model
    property alias comboIndex: comboOptions.currentIndex

    signal choiceSelected(int index)


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
        comboOptions.choiceSelected(index)
    }

}