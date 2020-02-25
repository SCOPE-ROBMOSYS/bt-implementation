#include "GoToSkill.h"


#include <QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    GoToSkill bt;
    if (!bt.start()) {
        return 1;
    }

    return app.exec();
}
