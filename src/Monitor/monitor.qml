import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtScxml 5.8

import scope.monitor.MonitorReader 1.0

Window {
    id: root
    width: 640
    height: 480
    minimumWidth: 120
    minimumHeight: 50
    visible: true

    title: "SCOPE Monitor"

    ColumnLayout {
        spacing: 2

        // BatteryLevelMonitor
        Rectangle {
            Layout.minimumWidth: 100
            Layout.margins: 2
            Layout.preferredWidth: root.width - 2 * Layout.margins
            Layout.preferredHeight: 40

            StateMachineLoader {
                id: loader_01
                source: "qrc:///BatteryLevelMonitor.scxml"
            }

            Connections {
                target: MonitorReader
                onTick: loader_01.stateMachine.submitEvent("tick")
            }

            Connections {
                target: MonitorReader
                onBatteryLevelChanged: loader_01.stateMachine.submitEvent("batteryLevelChanged", { "level": level })
            }

            Connections {
                target: MonitorReader
                onDestinationChangeCommandSent: loader_01.stateMachine.submitEvent("destinationChangeCommandSent", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_01.stateMachine.submitEvent("destinationChanged", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onIsGraspingChanged: loader_01.stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
            }

            Connections {
                target: MonitorReader
                onPreGraspCommandSent: loader_01.stateMachine.submitEvent("preGraspCommandSent")
            }

            Connections {
                target: MonitorReader
                onExtractHandCommandSent: loader_01.stateMachine.submitEvent("extractHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onRetractHandCommandSent: loader_01.stateMachine.submitEvent("retractHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onCloseHandCommandSent: loader_01.stateMachine.submitEvent("closeHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onOpenHandCommandSent: loader_01.stateMachine.submitEvent("openHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onHasGraspedCommandSent: loader_01.stateMachine.submitEvent("hasGraspedCommandSent")
            }

            Connections {
                target: MonitorReader
                onPreGraspReplyReceived: loader_01.stateMachine.submitEvent("preGraspReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onExtractHandReplyReceived: loader_01.stateMachine.submitEvent("extractHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onRetractHandReplyReceived: loader_01.stateMachine.submitEvent("retractHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onCloseHandReplyReceived: loader_01.stateMachine.submitEvent("closeHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onOpenHandReplyReceived: loader_01.stateMachine.submitEvent("openHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onHasGraspedReplyReceived: loader_01.stateMachine.submitEvent("hasGraspedReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onHomeReplyReceived: loader_01.stateMachine.submitEvent("homeReplyReceived", { "ret": ret })
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
                        anchors.centerIn: parent
                        text: "[Battery level > 20] (" + MonitorReader.batteryLevel + ")"
                    }
                }

                Rectangle {
                    id: light_01
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
                            when: loader_01.stateMachine.idle

                            PropertyChanges {
                                target: light_01
                                color: "green"
                            }
                        },
//                         State {
//                             name: "Warning"
//                             when: loader_01.stateMachine.warning
//
//                             PropertyChanges {
//                                 target: light_01
//                                 color: "yellow"
//                             }
//                         },
                        State {
                            name: "Error"
                            when: loader_01.stateMachine.failure

                            PropertyChanges {
                                target: light_01
                                color: "red"
                            }
                        }
                    ]

                }
            } // RowLayout
        } // Rectangle

        // DestinationMonitor
        Rectangle {
            Layout.minimumWidth: 100
            Layout.margins: 2
            Layout.preferredWidth: root.width - 2 * Layout.margins
            Layout.preferredHeight: 40

            StateMachineLoader {
                id: loader_02
                source: "qrc:///DestinationMonitor.scxml"
            }

            Connections {
                target: MonitorReader
                onTick: loader_02.stateMachine.submitEvent("tick")
            }

            Connections {
                target: MonitorReader
                onBatteryLevelChanged: loader_02.stateMachine.submitEvent("batteryLevelChanged", { "level": level })
            }

            Connections {
                target: MonitorReader
                onDestinationChangeCommandSent: loader_02.stateMachine.submitEvent("destinationChangeCommandSent", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_02.stateMachine.submitEvent("destinationChanged", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onPreGraspCommandSent: loader_02.stateMachine.submitEvent("preGraspCommandSent")
            }

            Connections {
                target: MonitorReader
                onExtractHandCommandSent: loader_02.stateMachine.submitEvent("extractHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onRetractHandCommandSent: loader_02.stateMachine.submitEvent("retractHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onCloseHandCommandSent: loader_02.stateMachine.submitEvent("closeHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onOpenHandCommandSent: loader_02.stateMachine.submitEvent("openHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onHasGraspedCommandSent: loader_02.stateMachine.submitEvent("hasGraspedCommandSent")
            }

            Connections {
                target: MonitorReader
                onHomeCommandSent: loader_02.stateMachine.submitEvent("homeCommandSent")
            }

            Connections {
                target: MonitorReader
                onPreGraspReplyReceived: loader_02.stateMachine.submitEvent("preGraspReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onExtractHandReplyReceived: loader_02.stateMachine.submitEvent("extractHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onRetractHandReplyReceived: loader_02.stateMachine.submitEvent("retractHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onCloseHandReplyReceived: loader_02.stateMachine.submitEvent("closeHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onOpenHandReplyReceived: loader_02.stateMachine.submitEvent("openHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onHasGraspedReplyReceived: loader_02.stateMachine.submitEvent("hasGraspedReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onHomeReplyReceived: loader_02.stateMachine.submitEvent("homeReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onIsGraspingChanged: loader_02.stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
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
                        anchors.centerIn: parent
                        text: "[Destination] (" + MonitorReader.destination + ")"
                    }
                }

                Rectangle {
                    id: light_02
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
                            when: loader_02.stateMachine.idleState

                            PropertyChanges {
                                target: light_02
                                color: "green"
                            }
                        },
                        State {
                            name: "Warning"
                            when: loader_02.stateMachine.warning

                            PropertyChanges {
                                target: light_02
                                color: "yellow"
                            }
                        },
                        State {
                            name: "Error"
                            when: loader_02.stateMachine.failure

                            PropertyChanges {
                                target: light_02
                                color: "red"
                            }
                        }
                    ]

                }

            } // RowLayout
        } // Rectangle

        // TargetMonitor
        Rectangle {
            Layout.minimumWidth: 100
            Layout.margins: 2
            Layout.preferredWidth: root.width - 2 * Layout.margins
            Layout.preferredHeight: 40

            StateMachineLoader {
                id: loader_03
                source: "qrc:///TargetMonitor.scxml"
            }

            Connections {
                target: MonitorReader
                onTick: loader_03.stateMachine.submitEvent("tick")
            }

            Connections {
                target: MonitorReader
                onBatteryLevelChanged: loader_03.stateMachine.submitEvent("batteryLevelChanged", { "level": level })
            }

            Connections {
                target: MonitorReader
                onDestinationChangeCommandSent: loader_03.stateMachine.submitEvent("destinationChangeCommandSent", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_03.stateMachine.submitEvent("destinationChanged", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onIsGraspingChanged: loader_03.stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
            }

            Connections {
                target: MonitorReader
                onPreGraspCommandSent: loader_03.stateMachine.submitEvent("preGraspCommandSent")
            }

            Connections {
                target: MonitorReader
                onExtractHandCommandSent: loader_03.stateMachine.submitEvent("extractHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onRetractHandCommandSent: loader_03.stateMachine.submitEvent("retractHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onCloseHandCommandSent: loader_03.stateMachine.submitEvent("closeHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onOpenHandCommandSent: loader_03.stateMachine.submitEvent("openHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onHasGraspedCommandSent: loader_03.stateMachine.submitEvent("hasGraspedCommandSent")
            }

            Connections {
                target: MonitorReader
                onHomeCommandSent: loader_03.stateMachine.submitEvent("homeCommandSent")
            }

            Connections {
                target: MonitorReader
                onPreGraspReplyReceived: loader_03.stateMachine.submitEvent("preGraspReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onExtractHandReplyReceived: loader_03.stateMachine.submitEvent("extractHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onRetractHandReplyReceived: loader_03.stateMachine.submitEvent("retractHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onCloseHandReplyReceived: loader_03.stateMachine.submitEvent("closeHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onOpenHandReplyReceived: loader_03.stateMachine.submitEvent("openHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onHasGraspedReplyReceived: loader_03.stateMachine.submitEvent("hasGraspedReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onHomeReplyReceived: loader_03.stateMachine.submitEvent("homeReplyReceived", { "ret": ret })
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
                        anchors.centerIn: parent
                        text: "[Destination] (" + MonitorReader.destination + ")"
                    }
                }

                Rectangle {
                    id: light_03
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
                            when: loader_03.stateMachine.idleState

                            PropertyChanges {
                                target: light_03
                                color: "green"
                            }
                        },
                        State {
                            name: "Warning"
                            when: loader_03.stateMachine.warning

                            PropertyChanges {
                                target: light_03
                                color: "yellow"
                            }
                        },
                        State {
                            name: "Error"
                            when: loader_03.stateMachine.failure

                            PropertyChanges {
                                target: light_03
                                color: "red"
                            }
                        }
                    ]

                }

            } // RowLayout
        } // Rectangle

        // GraspMonitor
        Rectangle {
            Layout.minimumWidth: 100
            Layout.margins: 2
            Layout.preferredWidth: root.width - 2 * Layout.margins
            Layout.preferredHeight: 40

            StateMachineLoader {
                id: loader_04
                source: "qrc:///GraspMonitor.scxml"
            }

            Connections {
                target: MonitorReader
                onTick: loader_04.stateMachine.submitEvent("tick")
            }

            Connections {
                target: MonitorReader
                onBatteryLevelChanged: loader_04.stateMachine.submitEvent("batteryLevelChanged", { "level": level })
            }

            Connections {
                target: MonitorReader
                onDestinationChangeCommandSent: loader_04.stateMachine.submitEvent("destinationChangeCommandSent", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_04.stateMachine.submitEvent("destinationChanged", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onPreGraspCommandSent: loader_04.stateMachine.submitEvent("preGraspCommandSent")
            }

            Connections {
                target: MonitorReader
                onExtractHandCommandSent: loader_04.stateMachine.submitEvent("extractHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onRetractHandCommandSent: loader_04.stateMachine.submitEvent("retractHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onCloseHandCommandSent: loader_04.stateMachine.submitEvent("closeHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onOpenHandCommandSent: loader_04.stateMachine.submitEvent("openHandCommandSent")
            }

            Connections {
                target: MonitorReader
                onHasGraspedCommandSent: loader_04.stateMachine.submitEvent("hasGraspedCommandSent")
            }

            Connections {
                target: MonitorReader
                onHomeCommandSent: loader_04.stateMachine.submitEvent("homeCommandSent")
            }

            Connections {
                target: MonitorReader
                onPreGraspReplyReceived: loader_04.stateMachine.submitEvent("preGraspReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onExtractHandReplyReceived: loader_04.stateMachine.submitEvent("extractHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onRetractHandReplyReceived: loader_04.stateMachine.submitEvent("retractHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onCloseHandReplyReceived: loader_04.stateMachine.submitEvent("closeHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onOpenHandReplyReceived: loader_04.stateMachine.submitEvent("openHandReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onHasGraspedReplyReceived: loader_04.stateMachine.submitEvent("hasGraspedReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onHomeReplyReceived: loader_04.stateMachine.submitEvent("homeReplyReceived", { "ret": ret })
            }

            Connections {
                target: MonitorReader
                onIsGraspingChanged: loader_04.stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
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
                        anchors.centerIn: parent
                        text: "[Grasping] (" + MonitorReader.isGrasping + ")"
                    }
                }

                Rectangle {
                    id: light_04
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
                            when: loader_04.stateMachine.idleState

                            PropertyChanges {
                                target: light_04
                                color: "green"
                            }
                        },
                        State {
                            name: "Warning"
                            when: loader_04.stateMachine.warning

                            PropertyChanges {
                                target: light_04
                                color: "yellow"
                            }
                        },
                        State {
                            name: "Error"
                            when: loader_04.stateMachine.failure

                            PropertyChanges {
                                target: light_04
                                color: "red"
                            }
                        }
                    ]

                }

            } // RowLayout
        } // Rectangle

    } // ColumnLayout
}
