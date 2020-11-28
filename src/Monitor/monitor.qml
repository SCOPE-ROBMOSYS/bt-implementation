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
                onDestinationChangeRequested: loader_01.stateMachine.submitEvent("destinationChangeRequested", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_01.stateMachine.submitEvent("destinationChanged", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_01.stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
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
                onDestinationChangeRequested: loader_02.stateMachine.submitEvent("destinationChangeRequested", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_02.stateMachine.submitEvent("destinationChanged", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_02.stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
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

        // ChargingStationMonitor
        Rectangle {
            Layout.minimumWidth: 100
            Layout.margins: 2
            Layout.preferredWidth: root.width - 2 * Layout.margins
            Layout.preferredHeight: 40

            StateMachineLoader {
                id: loader_03
                source: "qrc:///ChargingStationMonitor.scxml"
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
                onDestinationChangeRequested: loader_03.stateMachine.submitEvent("destinationChangeRequested", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_03.stateMachine.submitEvent("destinationChanged", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_03.stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
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
                onDestinationChangeRequested: loader_04.stateMachine.submitEvent("destinationChangeRequested", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_04.stateMachine.submitEvent("destinationChanged", { "destination": destination })
            }

            Connections {
                target: MonitorReader
                onDestinationChanged: loader_04.stateMachine.submitEvent("isGraspingChanged", { "isGrasping": isGrasping })
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
