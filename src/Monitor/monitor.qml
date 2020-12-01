import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2

import QtScxml 5.8

import scope.monitor.MonitorReader 1.0

Window {
    id: root
    width: 640
    height: 480
    minimumWidth: 400
    minimumHeight: 260
    visible: true

    title: "SCOPE Monitor"

    ListModel {
        id: monitorModel

        ListElement {
            property_sm: "qrc:///BatteryLevelMonitor.scxml"
        }
        ListElement {
            property_sm: "qrc:///DestinationMonitor.scxml"
        }
        ListElement {
            property_sm: "qrc:///TargetMonitor.scxml"
        }
        ListElement {
            property_sm: "qrc:///GraspMonitor.scxml"
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 2


        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: monitorModel

            delegate: MonitorDelegate {
                source: property_sm
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.minimumHeight: tick_label.height + 4
            color: "light grey"
            Row {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                spacing: 10

                Text {
                    id: tick_label
                    text: "[Tick number] (<b>" + MonitorReader.tickNumber + "</b>)"
                }

                Text {
                    text: "[Tick received] (<b>" + MonitorReader.tickReceived + "</b>)"
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.minimumHeight: battery_label.height + 4
            color: "light grey"
            Text {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                id: battery_label
                text: "[Battery level] (<b>" + MonitorReader.batteryLevel + "</b>)"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.minimumHeight: destination_label.height + 4
            color: "light grey"
            Text {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                id: destination_label
                text: "[Destination] (<b>" + MonitorReader.destination + "</b>)"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.minimumHeight: arm_label.height + 4
            color: "light grey"
            Row {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                spacing: 10

                Text {
                    id: arm_label
                    text: "[Arm extracted] (<b>" + MonitorReader.isArmExtracted + "</b>)"
                }
                Text {
                    text: "[Hand open] (<b>" + MonitorReader.isHandOpen + "</b>)"
                }
                Text {
                    text: "[Grasping] (<b>" + MonitorReader.isGrasping + "</b>)"
                }
            }
        }
    }
}
