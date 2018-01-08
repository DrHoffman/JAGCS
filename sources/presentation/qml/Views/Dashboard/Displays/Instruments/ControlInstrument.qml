import QtQuick 2.6
import QtQuick.Layouts 1.3
import JAGCS 1.0

import "qrc:/Controls" as Controls
import "../CommandControls" as CommandControls

Controls.Pane {
    id: controlDisplay

    enabled: online

    function updateCommandStatus(command, status) {
        switch (command) {
        case Command.ArmDisarm:
            armDisarm.status = status;
            break;
        case Command.SetMode:
            modeBox.status = status;
            break;
        case Command.Return:
            rtl.status = status;
            break;
        default:
            break;
        }
    }

    RowLayout {
        anchors.centerIn: parent
        width: parent.width
        spacing: sizings.spacing

        CommandControls.DelayButton {
            id: armDisarm
            text: armed ? qsTr("DISARM") : qsTr("ARM")
            args: [ !armed ]
            command: Command.ArmDisarm
            Layout.preferredWidth: sizings.controlBaseSize * 4
            Layout.fillWidth: true
        }

        CommandControls.ModeBox {
            id: modeBox
            mode: vehicleDisplay.mode
            model: availableModes
            tipText: qsTr("Select mode")
            Layout.fillWidth: true
        }

        CommandControls.Button {
            id: rtl
            tipText: qsTr("Return to launch")
            iconSource: "qrc:/icons/home.svg"
            command: Command.Return
            Layout.alignment: Qt.AlignRight
        }
    }
}
