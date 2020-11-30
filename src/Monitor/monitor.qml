import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2

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

        MonitorWidget {
            source: "qrc:///BatteryLevelMonitor.scxml"
            text: "[Battery level > 20] (" + MonitorReader.batteryLevel + ")"
        }

        MonitorWidget {
            source: "qrc:///DestinationMonitor.scxml"
            text: "[Destination] (" + MonitorReader.destination + ")"
        }

        MonitorWidget {
            source: "qrc:///TargetMonitor.scxml"
            text: "[Destination] (" + MonitorReader.destination + ")"
        }

        MonitorWidget {
            source: "qrc:///GraspMonitor.scxml"
            text: "[Grasping] (" + MonitorReader.isGrasping + ")"
        }

    } // ColumnLayout
}
