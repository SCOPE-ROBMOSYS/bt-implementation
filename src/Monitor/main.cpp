/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include <yarp/os/Bottle.h>
#include <yarp/os/ConnectionReader.h>
#include <yarp/os/Log.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/ResourceFinder.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScxmlStateMachine>
#include <QTextStream>



// FIXME Maybe use thrift...
struct MonitorEvent
{
    double timestamp {0.0};
    std::string source;
    std::string destination;
    bool isReply;
    std::string command;
    std::string arguments;
};

class MonitorReader :
        public QObject,
        public yarp::os::Bottle
{
    Q_OBJECT
    Q_DISABLE_COPY(MonitorReader)
    Q_PROPERTY(double batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
//     QML_ELEMENT

    MonitorReader() {}

public:
    static QObject* qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        if(!m_instance) {
            m_instance = new MonitorReader();
        }
        return m_instance;
    }

    bool read(yarp::os::ConnectionReader &reader) override;

    double batteryLevel() const { return m_batteryLevel; }

Q_SIGNALS:
    void batteryLevelChanged(double level);

private:
    static QObject* m_instance;

    double m_batteryLevel {100.0};
};

QObject* MonitorReader::m_instance = nullptr;

bool MonitorReader::read(yarp::os::ConnectionReader &reader)
{
    bool ret = yarp::os::Bottle::read(reader);

    yInfo("%s", toString().c_str());

    MonitorEvent event {
        /*.timestamp =*/ get(0).asDouble(),
        /*.source =*/ get(1).asString(),
        /*.destination =*/ get(2).asString(),
        /*.isReply =*/ (get(3).asString() == "reply"),
        /*.command =*/ get(4).asList()->get(0).asString(),
        /*.arguments =*/ (get(4).asList()->size() > 1 ? get(4).asList()->toString().substr(get(4).asList()->get(0).asString().size() + 1) : "")
    };

    yDebugExternalTime(event.timestamp, "Message received:\n  From     : %s\n  To       : %s\n  %s: %s\n  Arguments: %s",
        event.isReply ? event.destination.c_str() : event.source.c_str(),
        event.isReply ? event.source.c_str() : event.destination.c_str(),
        event.isReply ? "Reply    " : "Command  ",
        event.command.c_str(),
        event.arguments.c_str()
    );

    if (event.isReply
          && event.source == "/BatteryReaderBatteryLevelClient"
          && event.destination == "/BatteryComponent"
          && event.command == "level") {
        yInfo("level = %s", event.arguments.c_str());
        m_batteryLevel = get(4).asList()->get(1).asDouble();
        Q_EMIT batteryLevelChanged(m_batteryLevel);
    }

    return ret;
}



int main (int argc, char *argv[])
{
    yarp::os::ResourceFinder &rf = yarp::os::ResourceFinder::getResourceFinderSingleton();
//     rf.setDefaultConfigFile("Monitor.ini");  //overridden by --from parameter
//     rf.setDefaultContext("Monitor");         //overridden by --context parameter
    rf.configure(argc, argv);

    yarp::os::Network yarp;

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<MonitorReader>("scope.monitor.MonitorReader", 1, 0,
                                            "MonitorReader",
                                            &MonitorReader::qmlInstance);


    engine.load(QUrl(QStringLiteral("qrc:///monitor.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    std::mutex mutex;
    yarp::os::Port port;
    auto* monitorReader = dynamic_cast<MonitorReader*>(MonitorReader::qmlInstance(&engine, nullptr));
    port.setReader(*monitorReader);
    port.setCallbackLock();
    port.open("/monitor");

    QObject::connect(monitorReader, &MonitorReader::batteryLevelChanged, [](double level){ yWarning("SIGNAL RECEIVED: level = %f", level); });

    return app.exec();
}

#include "main.moc"
