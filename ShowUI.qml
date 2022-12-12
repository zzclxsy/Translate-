import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml 2.12
import QtGraphicalEffects 1.12
import QtQuick.Window 2.12
Rectangle {
    id:root
    color: "#ffffff"
    state: "hide"
    border.width: 0

    Connections{
        target: hookData
        onMessage:{
            testShow.text =mess
            window.setHeight(testShow.contentHeight)
            testShow.focus = true
        }
    }

    TextEdit{
        id:testShow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleRect.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 5
        font.family: "黑体"
        color: "#1E90FF"
        readOnly: true
        selectByMouse: true
        wrapMode: Text.WrapAnywhere
        font {
            pixelSize: 20
            bold: true
        }

        Component.onCompleted: {
            console.log(hookData.registerKeyBoardHook())
        }
    }

    Rectangle {
        id: titleRect
        height: 32
        color: "#50CCCCCC"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        Rectangle {
            id: closeRect
            x: 163
            width: 38
            color: "#00d60303"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            anchors.rightMargin: 0

            Image {
                id: closeimage
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                source: "image/close.svg"
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                fillMode: Image.PreserveAspectFit
                layer.enabled: true
                layer.effect:ColorOverlay{
                    id:colorOverlay
                    anchors.fill: closeimage
                    source: closeimage
                    color: "#638cba"

                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            parent.color="#e1453c"
                        }
                        onExited: {
                            parent.color="transparent"
                        }
                        onPressed: {
                            parent.color = "#e58a84"
                        }

                        onClicked: {
                            close()
                        }
                    }
                }

            }

        }

        Rectangle {
            id: topRect
            x: 568
            width: 38
            color: "#00a2cc6f"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            anchors.rightMargin: 34

            MouseArea{
                anchors.fill: parent
                property bool topHint: true
                onClicked: {
                    if (!topHint)
                    {
                        image.source = "qrc:/image/Top.svg"
                        window.flags |= Qt.WindowStaysOnTopHint
                    }
                    else
                    {
                        image.source = "qrc:/image/NoTop.svg"
                        window.flags =  Qt.Window |Qt.FramelessWindowHint
                    }

                    topHint = !topHint
                }

                Image {
                    id: image
                    anchors.fill: parent
                    source: "image/NoTop.svg"
                    fillMode: Image.PreserveAspectCrop
                }
            }
            Component.onCompleted: {
                image.source = "qrc:/image/Top.svg"
                window.flags |= Qt.WindowStaysOnTopHint
            }
        }

        Rectangle {
            id: minRect
            x: 163
            width: 38
            color: "transparent"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0

            Image {
                id: minimage
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                source: "qrc:/image/min.svg"
                anchors.bottomMargin: 2
                anchors.topMargin: 2
                fillMode: Image.PreserveAspectFit
                layer.enabled: true
                layer.effect:ColorOverlay{
                    anchors.fill: minimage
                    source: minimage
                    color: "transparent"

                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            parent.color="#50CCCCCC"
                        }
                        onExited: {
                            parent.color="transparent"
                        }
                        onPressed: {
                            parent.color = "#50CCCCCC"
                        }

                        onClicked: {
                            window.visibility = Window.Minimized;
                        }
                    }
                }

            }

        }

    }

    //设置动画
    states: [
        State {                         //状态1
            name: "hide"              //设置状态名称
            PropertyChanges {           //性质改变
                target: titleRect
                height:0
            }
        },
        State {                         //状态2
            name: "show"
            PropertyChanges {           //性质改变
                target: titleRect
                height:32
            }
        }
    ]

    transitions: [
        Transition {
            from: "hide"
            to: "show"
            PropertyAnimation{
                properties: "height"  //基于哪些属性变化（属性名称）
                duration: 150                    //过程时间
            }
        },
        Transition {
            from: "show"
            to: "hide"
            PropertyAnimation{
                properties: "height"  //基于哪些属性变化（属性名称）
                duration: 150                    //过程时间
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:6}D{i:9}
}
##^##*/
