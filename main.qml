import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQml 2.12

Window {
    id: window
    width: 350
    height: 200
    visible: true
    title: qsTr("翻译")
    color: "transparent"
    flags: Qt.Window |Qt.FramelessWindowHint

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        property point clickPos: "0,0"  //定义一个点
        property point deltaPos: "0,0"  //定义一个点
        property bool isPress: false
        onPressed: {
            clickPos = Qt.point(mouseX, mouseY)
            isPress = true
        }
        onReleased: {
            isPress = false
        }

        onPositionChanged: {
            if (isPress)
            {
                deltaPos = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                window.setX(window.x + deltaPos.x )
                window.setY(window.y + deltaPos.y)
            }
        }

        onEntered: {
            window.height = window.height + 38
            showUI.state = "show"
            showUI.border.width= 1
        }
        onExited: {
            window.height = window.height - 38
            showUI.state = "hide"
            showUI.border.width= 0
        }

        ShowUI{
            id:showUI
            anchors.fill: parent
        }
    }


}
