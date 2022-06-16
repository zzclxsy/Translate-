import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml 2.12
import QtGraphicalEffects 1.12
Rectangle {
    id:root
    color: "#00000000"
    state: "hide"
    border.width: 0
    function distinguish(data)
    {
        let pattern = /[^\u4E00-\u9FA5]/;
        var countCN = 0
        var countEN = 0
        for (var i = 0; i < data.length; i++)
        {
            if (!pattern.test(data[i])) {
                countCN++
                console.log("中文:",data[i])
            } else {
                countEN++
                console.log("英文:",data[i])
            }
        }
        var width = countCN*textarea.font.pixelSize + (textarea.font.pixelSize/2)*countEN;
        var row = parseInt(width/300) + 1

        window.setWidth(300 + textarea.font.pixelSize)
        window.setHeight(row * textarea.font.pixelSize + 38*2);
    }

    Connections{
        target: hookData
        onMessage:{
            textarea.text =mess
            distinguish(mess)
        }
    }

    Text{
        id:textarea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: rectangle.bottom
        anchors.bottom: parent.bottom
        font.family: "黑体"
        anchors.topMargin: 0
        wrapMode: Text.WrapAnywhere
        font {
            pixelSize: 25
            bold: true
        }
        Component.onCompleted: {
            console.log(hookData.registerKeyBoardHook())
        }
    }

    Rectangle {
        id: rectangle
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
                ColorOverlay{
                    id:colorOverlay
                    anchors.fill: closeimage
                    source: closeimage
                    color: "#638cba"
                }
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        colorOverlay.color="#e1453c"
                    }
                    onExited: {
                        colorOverlay.color="transparent"
                    }
                    onPressed: {
                        colorOverlay.color = "#e58a84"
                    }

                    onClicked: {
                        close()
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
                property bool topHint: false
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
        }
    }

    //设置动画
    states: [
        State {                         //状态1
            name: "hide"              //设置状态名称
            PropertyChanges {           //性质改变
                target: rectangle
                height:0
            }
        },
        State {                         //状态2
            name: "show"
            PropertyChanges {           //性质改变
                target: rectangle
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
