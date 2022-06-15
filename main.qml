import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
 import QtQml 2.12
Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Connections{
        target: hookData
        onMessage:{
            textarea.text =mess
            console.log("info:"+mess)
        }
    }

    TextArea{
        id:textarea
        anchors.fill: parent
        Component.onCompleted: {
            console.log(hookData.registerKeyBoardHook())
        }
    }
}
