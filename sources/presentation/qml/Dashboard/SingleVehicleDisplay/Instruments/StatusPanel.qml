import QtQuick 2.6
import QtQuick.Layouts 1.3
import JAGCS 1.0

import Industrial.JS 1.0 as JS
import Industrial.Controls 1.0 as Controls
import Industrial.Indicators 1.0 as Indicators
import "../../DashboardControls" as DashboardControls

BaseInstrument {
    id: root

    property bool dmsFormat: settings.boolValue("Gui/coordinatesDms")

    RowLayout {
        anchors.centerIn: parent
        width: parent.width
        spacing: controlSize.spacing

        Controls.ColoredIcon {
            id: snsIcon
            color: {
                switch (vehicle.satellite.fix) {
                case -1:
                case 0: return customPalette.sunkenColor;
                case 1: return customPalette.dangerColor;
                case 2: return customPalette.cautionColor;
                case 3:
                default: return customPalette.positiveColor;
                }
            }
            source: "qrc:/icons/gps.svg"
            height: controlSize.baseSize
            width: height
            Layout.alignment: Qt.AlignRight

            Text {
                text: vehicle.satellite.satellitesVisible
                font.pixelSize: parent.height / 4
                font.bold: true
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                color: parent.color
            }
        }

        ColumnLayout {
            id: column
            Layout.fillWidth: true
            spacing: controlSize.spacing

            DashboardControls.Label {
                color: snsIcon.color
                text: qsTr("Lat.: ") + (dmsFormat ?
                           JS.Helper.degreesToDmsString(vehicle.satellite.coordinate.latitude, false, 2) :
                           JS.Helper.degreesToString(vehicle.satellite.coordinate.latitude, 6))
                font.pixelSize: controlSize.fontSize * 0.6
            }

            DashboardControls.Label {
                color: snsIcon.color
                text: qsTr("Lon.: ") + (dmsFormat ?
                           JS.Helper.degreesToDmsString(vehicle.satellite.coordinate.longitude, true, 2) :
                           JS.Helper.degreesToString(vehicle.satellite.coordinate.longitude, 6))
                font.pixelSize: controlSize.fontSize * 0.6
            }
        }

        Item {
            Layout.fillWidth: true
        }

        Indicators.BatteryIndicator {
            id: battery
            percentage: vehicle.battery.percentage
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: itemMenuButton.width

            DashboardControls.Label {
                color: vehicle.battery.voltage > 0.01 ? customPalette.textColor :
                                                        customPalette.sunkenColor
                text: vehicle.battery.voltage.toFixed(2) + " " + qsTr("V")
                font.pixelSize: controlSize.fontSize * 0.6
            }

            DashboardControls.Label {
                color: {
                    if (vehicle.battery.current < -0.01) return customPalette.positiveColor;
                    if (vehicle.battery.current > 0.0) return customPalette.textColor;
                    if (vehicle.battery.current > 5.0) return customPalette.cautionColor;
                    if (vehicle.battery.current > 10.0) return customPalette.dangerColor;

                    return customPalette.sunkenColor;
                }
                text: vehicle.battery.current.toFixed(2) + " " + qsTr("A")
                font.pixelSize: controlSize.fontSize * 0.6
            }
        }
    }
}
