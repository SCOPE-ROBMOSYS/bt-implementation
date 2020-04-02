/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "@KEY_SKILL_NAME@Skill.h"

#include <QCoreApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("@KEY_SKILL_NAME@Skill");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("@KEY_SKILL_NAME@ Skill Description");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({{"n", "skill-name"}, "The <name> of the skill (implies the port opened).", "name", "@KEY_SKILL_NAME@"});

@KEY_ADDITIONAL_OPTION_PARSE@

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    QString skillName = parser.value("skill-name");

@KEY_ADDITIONAL_OPTION_ASSIGN@

    QCoreApplication::setApplicationName(skillName);

    @KEY_SKILL_NAME@Skill bt(skillName.toStdString() @KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS@);
    if (!bt.start()) {
        return 1;
    }

    return app.exec();
}
