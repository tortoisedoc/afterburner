import QtQuick 2.15
import QtQuick.Controls 2.15

Row {
    height: 20
    spacing: 0

    property alias label : _label.text
    property alias value : _slider.value

    Label{
        id: _label
        width: parent.width*0.2
    }

    Slider{
        id: _slider
        from: 0.0
        to: 255.0
        width: parent.width*0.8
        height: parent.height
    }
}
