#include "BatteryNotChargingSkillDataModel.h"
#include <QDebug>
#include <QScxmlStateMachine>

BatteryNotChargingSkillDataModel::BatteryNotChargingSkillDataModel()
{
    qDebug() << "BatteryNotChargingSkillDataModel::BatteryNotChargingSkillDataModel() called";
}

bool BatteryNotChargingSkillDataModel::setup(const QVariantMap &initialDataValues)
{
    Q_UNUSED(initialDataValues)

    if (!yarp::os::NetworkBase::checkNetwork()) {
        qWarning("Error! YARP Network is not initialized");
        return false;
    }

    if (!client_port.open("/BatteryNotChargingClient")) {
        qWarning("Error! Cannot open YARP port");
        return false;
    }

    if(!batteryReader.yarp().attachAsClient(client_port)) {
        qWarning("Error! Could not attach as client");
        return false;
    }

    return true;
}
