import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtScxml 5.8

import scope.monitor.MonitorReader 1.0


Rectangle {
    property alias source: loader.source
    property alias stateMachine: loader.stateMachine
    property alias text: label.text

    Layout.minimumWidth: 100
    Layout.margins: 2
    Layout.preferredWidth: root.width - 2 * Layout.margins
    Layout.preferredHeight: 40

    StateMachineLoader {
        id: loader
    }

    Connections {
        target: MonitorReader
        onTick: stateMachine.submitEvent("tick")
    }

    Connections {
        target: MonitorReader
        onBatteryLevelChanged: stateMachine.submitEvent("batteryLevelChanged", { "level": level })
    }

    Connections {
        target: MonitorReader
        onDestinationChangeCommandSent: stateMachine.submitEvent("destinationChangeCommandSent", { "destination": destination })
    }

    Connections {
        target: MonitorReader
        onDestinationChanged: stateMachine.submitEvent("destinationChanged", { "destination": destination })
    }

    Connections {
        target: MonitorReader
        onPreGraspCommandSent: stateMachine.submitEvent("preGraspCommandSent")
    }

    Connections {
        target: MonitorReader
        onExtractHandCommandSent: stateMachine.submitEvent("extractHandCommandSent")
    }

    Connections {
        target: MonitorReader
        onRetractHandCommandSent: stateMachine.submitEvent("retractHandCommandSent")
    }

    Connections {
        target: MonitorReader
        onCloseHandCommandSent: stateMachine.submitEvent("closeHandCommandSent")
    }

    Connections {
        target: MonitorReader
        onOpenHandCommandSent: stateMachine.submitEvent("openHandCommandSent")
    }

    Connections {
        target: MonitorReader
        onHasGraspedCommandSent: stateMachine.submitEvent("hasGraspedCommandSent")
    }

    Connections {
        target: MonitorReader
        onHomeCommandSent: stateMachine.submitEvent("homeCommandSent")
    }

    Connections {
        target: MonitorReader
        onPreGraspReplyReceived: stateMachine.submitEvent("preGraspReplyReceived", { "ret": ret })
    }

    Connections {
        target: MonitorReader
        onExtractHandReplyReceived: stateMachine.submitEvent("extractHandReplyReceived", { "ret": ret })
    }

    Connections {
        target: MonitorReader
        onRetractHandReplyReceived: stateMachine.submitEvent("retractHandReplyReceived", { "ret": ret })
    }

    Connections {
        target: MonitorReader
        onCloseHandReplyReceived: stateMachine.submitEvent("closeHandReplyReceived", { "ret": ret })
    }

    Connections {
        target: MonitorReader
        onOpenHandReplyReceived: stateMachine.submitEvent("openHandReplyReceived", { "ret": ret })
    }

    Connections {
        target: MonitorReader
        onHasGraspedReplyReceived: stateMachine.submitEvent("hasGraspedReplyReceived", { "ret": ret })
    }

    Connections {
        target: MonitorReader
        onHomeReplyReceived: stateMachine.submitEvent("homeReplyReceived", { "ret": ret })
    }

    Connections {
        target: MonitorReader
        onIsGraspingChanged: stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
    }

    RowLayout {
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
                    when: stateMachine.idleState

                    PropertyChanges {
                        target: light
                        color: "green"
                    }
                },
                State {
                    name: "Warning"
                    when: stateMachine.warning

                    PropertyChanges {
                        target: light
                        color: "yellow"
                    }
                },
                State {
                    name: "Error"
                    when: stateMachine.failure

                    PropertyChanges {
                        target: light
                        color: "red"
                    }
                }
            ]

        }

    } // RowLayout
} // Rectangle
