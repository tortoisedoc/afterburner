import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import FireEngine 1.0


ApplicationWindow {

    width: 640
    height: 800
    visible: true

    Column {

        anchors.fill: parent
        spacing: 10

        LabeledSlider {
            id: fireIntensity
            width: parent.width
            value: 255.0
            label: "Fire intensity"
        }

        LabeledSlider {
            id: redChannel
            width: parent.width
            value : 255.0
            label: "Red Channel"
        }

        LabeledSlider {
            id: greenChannel
            width: parent.width
            value : 0.0
            label: "Green Channel"
        }

        LabeledSlider {
            id: blueChannel
            width: parent.width
            value : 0.0
            label: "Blue Channel"
        }

        Rectangle {
            width: parent.width
            height: 480
            color: "black"

            FireEngine {
                id: myFire
                anchors.fill: parent
                redChannel: redChannel.value
                greenChannel: greenChannel.value
                blueChannel: blueChannel.value
                intensity: fireIntensity.value
            }
        }
        //Apparently having the other QQuickItems here breaks the rendering (Qt 5.15, intel CPU / Integrated graphics)
    }
}
