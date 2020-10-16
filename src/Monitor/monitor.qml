import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtScxml 5.8
// import MonitorStateMachine 1.0
import scope.monitor.MonitorReader 1.0

Window {
    id: root
    width: 640
    height: 480
    minimumWidth: 120
    minimumHeight: 50
    visible: true

    StateMachineLoader {
        id: loader
        source: "qrc:///BatteryLevelMonitor.scxml"
    }
//     MonitorReader.batteryLevelChanged: visible = false/*stateMachine.submitEvent("batteryLevelChanged", level)*/;
//     MonitorReader.batteryLevelChanged: {
//         console.log("batteryLevelChanged: "/* + level*/);
//     }

    Connections {
        target: MonitorReader
        onBatteryLevelChanged: {
            console.log("battery level changed: " + level);
            loader.stateMachine.submitEvent("batteryLevelChanged", level);
        }
    }
    ColumnLayout {
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 100
            Layout.margins: 2
            Layout.preferredWidth: root.width - 2 * Layout.margins
            Layout.preferredHeight: root.height - 2 * Layout.margins

            border.color: "black"
            border.width: 1

            RowLayout {
                id: layout
                anchors.fill: parent
                spacing: 2

                Rectangle {
                    Layout.fillWidth: true
                    Layout.minimumWidth: 50
                    Layout.preferredWidth: 300
                    Layout.minimumHeight: 30
                    Layout.margins: 2
                    border.color: "black"
                    border.width: 1
                    Text {
                        id: label
                        anchors.centerIn: parent
                        text: "[Battery level > 20] (" + MonitorReader.batteryLevel + ")"
                    }
                }

                Rectangle {
                    id: light
                    Layout.minimumWidth: 30
                    Layout.preferredWidth: 30
                    Layout.maximumWidth: 30
                    Layout.minimumHeight: width
                    Layout.maximumHeight: width
                    Layout.preferredHeight: width
                    Layout.margins: 2
                    color: "green"
                    border.color: "black"
                    border.width: 1
                    radius: width * 0.5

                    states: [
                        State {
                            name: "Ok"
                            when: loader.stateMachine.idle

                            PropertyChanges {
                                target: light
                                color: "green"
                            }
                        },
                        State {
                            name: "Warning"
                            when: loader.stateMachine.getLevel

                            PropertyChanges {
                                target: light
                                color: "yellow"
                            }
                        },
                        State {
                            name: "Error"
                            when: loader.stateMachine.failure

                            PropertyChanges {
                                target: light
                                color: "red"
                            }
                        }
                    ]

                }

            } // RowLayout
        } // Rectangle
    } // ColumnLayout
}
