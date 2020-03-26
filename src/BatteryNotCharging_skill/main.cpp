/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "BatteryNotChargingSkill.h"

#include <QCoreApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("BatteryNotChargingSkill");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("BatteryNotCharging Skill Description");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({{"n", "skill-name"}, "The <name> of the skill (implies the port opened).", "name", "BatteryNotCharging"});

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    QString skillName = parser.value("skill-name");

    

    BatteryNotChargingSkill bt(skillName.toStdString() );
    if (!bt.start()) {
        return 1;
    }

    return app.exec();
}
