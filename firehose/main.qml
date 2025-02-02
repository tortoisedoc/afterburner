import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import FireEngine 1.0


ApplicationWindow{

    width:640
    height:500
    visible:true

    Column {
        anchors.fill: parent
        spacing:0
        FireEngine {
            id: myFire
            width:parent.width
            height:480
        }
        Button{
            onClicked: {
                myFire.update()
            }
        }
    }
}
