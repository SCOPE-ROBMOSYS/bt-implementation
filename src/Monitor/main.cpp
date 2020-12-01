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
#include <yarp/os/Vocab.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScxmlStateMachine>
#include <QTextStream>
#include <QIcon>

#define NO_LOCATION "N/A"

namespace {
constexpr yarp::conf::vocab32_t VOCAB_OK = yarp::os::createVocab('o', 'k');
constexpr yarp::conf::vocab32_t VOCAB_FAIL = yarp::os::createVocab('f', 'a', 'i', 'l');
}

enum class EventType
{
    Unknown,
    Tick,
    Command,
    Reply
};

std::string eventTypeToString(EventType t)
{
    switch (t) {
    case EventType::Tick:
        return "tick";
    case EventType::Command:
        return "command";
    case EventType::Reply:
        return "reply";
    case EventType::Unknown:
    default:
        return "[unknown]";
    }
}

EventType stringToEventType(const std::string& s)
{
    if (s == "tick") {
        return EventType::Tick;
    }
    if (s == "command") {
        return EventType::Command;
    }
    if (s == "reply") {
        return EventType::Reply;
    }
    return EventType::Unknown;
}


// FIXME copied from GoToStatus.h
enum GoToStatus
{
    NOT_STARTED = 0,
    RUNNING = 1,
    SUCCESS = 2,
    ABORT = 3
};


// FIXME Maybe use thrift...
struct MonitorEvent
{
    double timestamp {0.0};
    std::string source;
    std::string destination;
    EventType type;
    std::string protocol;
    std::string command;
    std::string arguments;
    std::string reply;
};

class MonitorReader :
        public QObject,
        public yarp::os::Bottle
{
    Q_OBJECT
    Q_DISABLE_COPY(MonitorReader)
    Q_PROPERTY(quint64 tickNumber READ tickNumber NOTIFY tick)
    Q_PROPERTY(quint64 tickReceived READ tickReceived NOTIFY tickReceivedChanged)
    Q_PROPERTY(double batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(QString destination READ destination NOTIFY destinationChanged)
    Q_PROPERTY(bool isArmExtracted READ isArmExtracted NOTIFY isArmExtractedChanged)
    Q_PROPERTY(bool isHandOpen READ isHandOpen NOTIFY isHandOpenChanged)
    Q_PROPERTY(bool isGrasping READ isGrasping NOTIFY isGraspingChanged)
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

    quint64 tickNumber() const { return m_tickNumber; }
    quint64 tickReceived() const { return m_tickReceived; }

    double batteryLevel() const { return m_batteryLevel; }
    QString destination() const { return m_destination;  }
    bool isArmExtracted() const { return m_isArmExtracted;  }
    bool isHandOpen() const { return m_isHandOpen;  }
    bool isGrasping() const { return m_isGrasping;  }

Q_SIGNALS:
    void tick(quint64 tickNumber);
    void tickReceivedChanged(quint64 tickReceived);

    void send_startCommandSent(const QString& destination);
    void destinationChanged(const QString& destination);
    void destinationReached(const QString& destination);

    void batteryLevelChanged(double level);

    void preGraspCommandSent();
    void extractHandCommandSent();
    void retractHandCommandSent();
    void closeHandCommandSent();
    void openHandCommandSent();
    void hasGraspedCommandSent();
    void homeCommandSent();

    void preGraspReplyReceived(bool ret);
    void extractHandReplyReceived(bool ret);
    void retractHandReplyReceived(bool ret);
    void closeHandReplyReceived(bool ret);
    void openHandReplyReceived(bool ret);
    void hasGraspedReplyReceived(bool ret);
    void homeReplyReceived(bool ret);

    void isArmExtractedChanged(bool isArmExtracted);
    void isHandOpenChanged(bool isHandOpen);
    void isGraspingChanged(bool isGrasping);

private:
    static QObject* m_instance;

    quint64 m_tickNumber {0};
    quint64 m_tickReceived {0};
    double m_batteryLevel {100.0};
    QString m_destination {NO_LOCATION};
    bool m_isArmExtracted {false};
    bool m_isHandOpen {false};
    bool m_isGrasping {false};
};

QObject* MonitorReader::m_instance = nullptr;

bool MonitorReader::read(yarp::os::ConnectionReader &reader)
{
    bool read_ret = yarp::os::Bottle::read(reader);

    yInfo("%s", toString().c_str());

    MonitorEvent event {
        /*.timestamp =*/ get(0).asDouble(),
        /*.source =*/ get(1).asString(),
        /*.destination =*/ get(2).asString(),
        /*.type =*/ stringToEventType(get(3).asString()),
        /*.protocol =*/ get(4).asString(),
        /*.command =*/ (get(5).asList()->size() > 0 ? get(5).asList()->get(0).asString() : ""),
        /*.arguments =*/ (get(6).asList()->size() > 0 ? get(6).asList()->get(0).toString() : ""),
        /*.reply =*/ (get(7).asList()->size() > 0 ? get(7).asList()->get(0).toString() : ""),
    };

    yDebugExternalTime(event.timestamp, "%s intercepted:\n"
                                        "  From      : %s\n"
                                        "  To        : %s\n"
                                        "  Protocol  : %s\n"
                                        "  Command   : %s\n"
                                        "  Arguments : %s\n"
                                        "  Reply     : %s",
        (event.type == EventType::Reply ? "Reply" : (
         event.type == EventType::Command ? "Command" : (
         event.type == EventType::Tick ? "Tick" : "Unknown"))),
        event.type == EventType::Reply ? event.destination.c_str() : event.source.c_str(),
        event.type == EventType::Reply ? event.source.c_str() : event.destination.c_str(),
        event.protocol.c_str(),
        event.command.c_str(),
        event.arguments.c_str(),
        event.reply.c_str()
    );

    if (event.type == EventType::Tick) {
        auto tickNumber = static_cast<uint64_t>(get(6).asList()->get(0).asInt64());
        m_tickNumber = tickNumber;
        ++m_tickReceived;
        Q_EMIT tick(m_tickNumber);
        Q_EMIT tickReceivedChanged(m_tickReceived);
    }

    if (event.type == EventType::Reply
          && event.source == "/BatteryReaderBatteryLevelClient"
          && event.destination == "/BatteryComponent"
          && event.command == "level") {
        double batteryLevel = get(7).asList()->get(0).asDouble();
        if (m_batteryLevel != batteryLevel) {
            m_batteryLevel = batteryLevel;
            Q_EMIT batteryLevelChanged(m_batteryLevel);
        }
    }

    if (event.type == EventType::Command
          && (
            (event.source == "/GoToDestination/BT_rpc/client" &&
             event.destination == "/GoToDestination/BT_rpc/server") ||
            (event.source == "/GoToUser/BT_rpc/client" &&
             event.destination == "/GoToUser/BT_rpc/server") ||
            (event.source == "/GoToChargingStation/BT_rpc/client" &&
             event.destination == "/GoToChargingStation/BT_rpc/server")
          )
          && event.command == "send_start") {
        QString destination = get(6).asList()->get(0).asString().c_str();
        Q_EMIT send_startCommandSent(destination);
    }

    if (event.type == EventType::Reply
          && event.destination == "/GoToComponent"
          && event.command == "getStatus") {
        QString destination = get(6).asList()->get(0).asString().c_str();
        GoToStatus status = static_cast<GoToStatus>(get(7).asList()->get(0).asInt32());
        if (status == RUNNING) {
            if (m_destination != destination) {
                m_destination = destination;
                Q_EMIT destinationChanged(m_destination);
            }
        } else if (status == SUCCESS) {
            if (m_destination == destination) {
                Q_EMIT destinationReached(m_destination);
                m_destination = NO_LOCATION;
                Q_EMIT destinationChanged(m_destination);
            }
        } else { // NOT_STARTED or ABORT
            if (m_destination != NO_LOCATION) {
                m_destination = NO_LOCATION;
                Q_EMIT destinationChanged(m_destination);
            }
        }
    }

    if (event.type == EventType::Reply
          && event.destination == "/GoToComponent"
          && event.command == "isAtLocation") {
        QString destination = get(6).asList()->get(0).asString().c_str();
        bool ret = get(7).asList()->get(0).asVocab() == VOCAB_OK;
        if (ret && m_destination == destination) {
            Q_EMIT destinationReached(m_destination);
            m_destination = NO_LOCATION;
            Q_EMIT destinationChanged(m_destination);
        }
    }

    if (event.type == EventType::Command
          && event.destination == "/ArmComponent") {
        if (event.command == "preGrasp") {
            Q_EMIT preGraspCommandSent();
        } else if (event.command == "extractHand") {
            Q_EMIT extractHandCommandSent();
        } else if (event.command == "retractHand") {
            Q_EMIT retractHandCommandSent();
        } else if (event.command == "closeHand") {
            Q_EMIT closeHandCommandSent();
        } else if (event.command == "openHand") {
            Q_EMIT openHandCommandSent();
        } else if (event.command == "hasGrasped") {
            Q_EMIT hasGraspedCommandSent();
        } else if (event.command == "home") {
            Q_EMIT homeCommandSent();
        }
    }

    if (event.type == EventType::Reply
          && event.destination == "/ArmComponent") {
        if (event.command == "preGrasp") {
            bool ret = get(7).asList()->get(0).asVocab() == VOCAB_OK;
            Q_EMIT preGraspReplyReceived(ret);
        } else if (event.command == "extractHand") {
            bool ret = get(7).asList()->get(0).asVocab() == VOCAB_OK;
            Q_EMIT extractHandReplyReceived(ret);
            if (ret && m_isArmExtracted != true) {
                m_isArmExtracted = true;
                Q_EMIT isArmExtractedChanged(m_isArmExtracted);
            }
        } else if (event.command == "retractHand") {
            bool ret = get(7).asList()->get(0).asVocab() == VOCAB_OK;
            Q_EMIT retractHandReplyReceived(ret);
            if (ret && m_isArmExtracted != false) {
                m_isArmExtracted = false;
                Q_EMIT isArmExtractedChanged(m_isArmExtracted);
            }
        } else if (event.command == "closeHand") {
            bool ret = get(7).asList()->get(0).asVocab() == VOCAB_OK;
            Q_EMIT closeHandReplyReceived(ret);
            if (ret && m_isHandOpen != false) {
                m_isHandOpen = false;
                Q_EMIT isHandOpenChanged(m_isHandOpen);
            }
        } else if (event.command == "openHand") {
            bool ret = get(7).asList()->get(0).asVocab() == VOCAB_OK;
            Q_EMIT openHandReplyReceived(ret);
            if (ret && m_isHandOpen != true) {
                m_isHandOpen = true;
                Q_EMIT isHandOpenChanged(m_isHandOpen);
            }
        } else if (event.command == "hasGrasped") {
            bool ret = get(7).asList()->get(0).asVocab() == VOCAB_OK;
            Q_EMIT hasGraspedReplyReceived(ret);
            if (m_isGrasping != ret) {
                m_isGrasping = ret;
                Q_EMIT isGraspingChanged(m_isGrasping);
            }
        } else if (event.command == "home") {
            bool ret = get(7).asList()->get(0).asVocab() == VOCAB_OK;
            Q_EMIT homeReplyReceived(ret);
            if (ret) {
                if (m_isArmExtracted) {
                    m_isArmExtracted = false;
                    Q_EMIT isHandOpenChanged(m_isArmExtracted);
                }
                if (m_isHandOpen) {
                    m_isHandOpen = false;
                    Q_EMIT isHandOpenChanged(m_isHandOpen);
                }
            }
        }
    }

    return read_ret;
}



int main (int argc, char *argv[])
{
    yarp::os::ResourceFinder &rf = yarp::os::ResourceFinder::getResourceFinderSingleton();
//     rf.setDefaultConfigFile("Monitor.ini");  //overridden by --from parameter
//     rf.setDefaultContext("Monitor");         //overridden by --context parameter
    rf.configure(argc, argv);

    yarp::os::Network yarp;

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("gnome-power-manager"));
    app.setApplicationVersion("0.1");
    app.setApplicationName("Monitor");

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<MonitorReader>("scope.monitor.MonitorReader", 1, 0,
                                            "MonitorReader",
                                            &MonitorReader::qmlInstance);


    engine.load(QUrl(QStringLiteral("qrc:///monitor.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    yarp::os::Port port;
    auto* monitorReader = dynamic_cast<MonitorReader*>(MonitorReader::qmlInstance(&engine, nullptr));
    port.setReader(*monitorReader);
    port.setCallbackLock();
    port.open("/monitor");

    // Just for testing that signals work
    QObject::connect(monitorReader, &MonitorReader::tick, [](quint64 tickNumber){ yWarning("SIGNAL RECEIVED: tick(tickNumber = %llu)", tickNumber); });
    QObject::connect(monitorReader, &MonitorReader::tickReceivedChanged, [](quint64 tickReceived){ yWarning("SIGNAL RECEIVED: tickReceivedChanged(tickReceived = %llu)", tickReceived); });
    QObject::connect(monitorReader, &MonitorReader::batteryLevelChanged, [](double level){ yWarning("SIGNAL RECEIVED: batteryLevelChanged(level = %f)", level); });
    QObject::connect(monitorReader, &MonitorReader::send_startCommandSent, [](const QString& destination){ yWarning("SIGNAL RECEIVED: send_startCommandSent(destination = %s)", destination.toStdString().c_str()); });
    QObject::connect(monitorReader, &MonitorReader::destinationChanged, [](const QString& destination){ yWarning("SIGNAL RECEIVED: destinationChanged(destination = %s)", destination.toStdString().c_str()); });
    QObject::connect(monitorReader, &MonitorReader::destinationReached, [](const QString& destination){ yWarning("SIGNAL RECEIVED: destinationReached(destination = %s)", destination.toStdString().c_str()); });

    QObject::connect(monitorReader, &MonitorReader::preGraspCommandSent, [](){ yWarning("SIGNAL RECEIVED: preGraspCommandSent()"); });
    QObject::connect(monitorReader, &MonitorReader::extractHandCommandSent, [](){ yWarning("SIGNAL RECEIVED: extractHandCommandSent()"); });
    QObject::connect(monitorReader, &MonitorReader::retractHandCommandSent, [](){ yWarning("SIGNAL RECEIVED: retractHandCommandSent()"); });
    QObject::connect(monitorReader, &MonitorReader::closeHandCommandSent, [](){ yWarning("SIGNAL RECEIVED: closeHandCommandSent()"); });
    QObject::connect(monitorReader, &MonitorReader::openHandCommandSent, [](){ yWarning("SIGNAL RECEIVED: openHandCommandSent()"); });
    QObject::connect(monitorReader, &MonitorReader::hasGraspedCommandSent, [](){ yWarning("SIGNAL RECEIVED: hasGraspedCommandSent()"); });
    QObject::connect(monitorReader, &MonitorReader::homeCommandSent, [](){ yWarning("SIGNAL RECEIVED: homeCommandSent()"); });

    QObject::connect(monitorReader, &MonitorReader::preGraspReplyReceived, [](bool ret){ yWarning("SIGNAL RECEIVED: preGraspReplyReceived(ret = %s)", (ret ? "true" : "false")); });
    QObject::connect(monitorReader, &MonitorReader::extractHandReplyReceived, [](bool ret){ yWarning("SIGNAL RECEIVED: extractHandReplyReceived(ret = %s)", (ret ? "true" : "false")); });
    QObject::connect(monitorReader, &MonitorReader::retractHandReplyReceived, [](bool ret){ yWarning("SIGNAL RECEIVED: retractHandReplyReceived(ret = %s)", (ret ? "true" : "false")); });
    QObject::connect(monitorReader, &MonitorReader::closeHandReplyReceived, [](bool ret){ yWarning("SIGNAL RECEIVED: closeHandReplyReceived(ret = %s)", (ret ? "true" : "false")); });
    QObject::connect(monitorReader, &MonitorReader::openHandReplyReceived, [](bool ret){ yWarning("SIGNAL RECEIVED: openHandReplyReceived(ret = %s)", (ret ? "true" : "false")); });
    QObject::connect(monitorReader, &MonitorReader::hasGraspedReplyReceived, [](bool ret){ yWarning("SIGNAL RECEIVED: hasGraspedReplyReceived(ret = %s)", (ret ? "true" : "false")); });
    QObject::connect(monitorReader, &MonitorReader::homeReplyReceived, [](bool ret){ yWarning("SIGNAL RECEIVED: homeReplyReceived(ret = %s)", (ret ? "true" : "false")); });

    QObject::connect(monitorReader, &MonitorReader::isArmExtractedChanged, [](bool isArmExtracted){ yWarning("SIGNAL RECEIVED: isArmExtractedChanged(isArmExtracted = %s)", isArmExtracted ? "true" : "false"); });
    QObject::connect(monitorReader, &MonitorReader::isHandOpenChanged, [](bool isHandOpen){ yWarning("SIGNAL RECEIVED: isHandOpenChanged(isHandOpen = %s)", isHandOpen ? "true" : "false"); });
    QObject::connect(monitorReader, &MonitorReader::isGraspingChanged, [](bool isGrasping){ yWarning("SIGNAL RECEIVED: isGraspingChanged(isGrasping = %s)", isGrasping ? "true" : "false"); });

    return app.exec();
}

#include "main.moc"
