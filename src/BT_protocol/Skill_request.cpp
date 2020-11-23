/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

// Autogenerated by Thrift Compiler (0.12.0-yarped)
//
// This is an automatically generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <Skill_request.h>

#include <yarp/os/idl/WireTypes.h>

class Skill_request_request_ack_helper :
        public yarp::os::Portable
{
public:
    explicit Skill_request_request_ack_helper();
    bool write(yarp::os::ConnectionWriter& connection) const override;
    bool read(yarp::os::ConnectionReader& connection) override;

    thread_local static SkillAck s_return_helper;
};

thread_local SkillAck Skill_request_request_ack_helper::s_return_helper = {};

Skill_request_request_ack_helper::Skill_request_request_ack_helper()
{
    s_return_helper = {};
}

bool Skill_request_request_ack_helper::write(yarp::os::ConnectionWriter& connection) const
{
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) {
        return false;
    }
    if (!writer.writeTag("request_ack", 1, 2)) {
        return false;
    }
    return true;
}

bool Skill_request_request_ack_helper::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) {
        return false;
    }
    int32_t ecast0;
    SkillAckVocab cvrt1;
    if (!reader.readEnum(ecast0, cvrt1)) {
        reader.fail();
        return false;
    } else {
        s_return_helper = static_cast<SkillAck>(ecast0);
    }
    return true;
}

class Skill_request_send_start_helper :
        public yarp::os::Portable
{
public:
    explicit Skill_request_send_start_helper();
    bool write(yarp::os::ConnectionWriter& connection) const override;
    bool read(yarp::os::ConnectionReader& connection) override;
};

Skill_request_send_start_helper::Skill_request_send_start_helper()
{
}

bool Skill_request_send_start_helper::write(yarp::os::ConnectionWriter& connection) const
{
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) {
        return false;
    }
    if (!writer.writeTag("send_start", 1, 2)) {
        return false;
    }
    return true;
}

bool Skill_request_send_start_helper::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) {
        return false;
    }
    return true;
}

class Skill_request_send_stop_helper :
        public yarp::os::Portable
{
public:
    explicit Skill_request_send_stop_helper();
    bool write(yarp::os::ConnectionWriter& connection) const override;
    bool read(yarp::os::ConnectionReader& connection) override;
};

Skill_request_send_stop_helper::Skill_request_send_stop_helper()
{
}

bool Skill_request_send_stop_helper::write(yarp::os::ConnectionWriter& connection) const
{
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) {
        return false;
    }
    if (!writer.writeTag("send_stop", 1, 2)) {
        return false;
    }
    return true;
}

bool Skill_request_send_stop_helper::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) {
        return false;
    }
    return true;
}

class Skill_request_send_ok_helper :
        public yarp::os::Portable
{
public:
    explicit Skill_request_send_ok_helper();
    bool write(yarp::os::ConnectionWriter& connection) const override;
    bool read(yarp::os::ConnectionReader& connection) override;
};

Skill_request_send_ok_helper::Skill_request_send_ok_helper()
{
}

bool Skill_request_send_ok_helper::write(yarp::os::ConnectionWriter& connection) const
{
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) {
        return false;
    }
    if (!writer.writeTag("send_ok", 1, 2)) {
        return false;
    }
    return true;
}

bool Skill_request_send_ok_helper::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) {
        return false;
    }
    return true;
}

// Constructor
Skill_request::Skill_request()
{
    yarp().setOwner(*this);
}

SkillAck Skill_request::request_ack()
{
    Skill_request_request_ack_helper helper{};
    if (!yarp().canWrite()) {
        yError("Missing server method '%s'?", "SkillAck Skill_request::request_ack()");
    }
    bool ok = yarp().write(helper, helper);
    return ok ? Skill_request_request_ack_helper::s_return_helper : SkillAck{};
}

void Skill_request::send_start()
{
    Skill_request_send_start_helper helper{};
    if (!yarp().canWrite()) {
        yError("Missing server method '%s'?", "void Skill_request::send_start()");
    }
    yarp().write(helper, helper);
}

void Skill_request::send_stop()
{
    Skill_request_send_stop_helper helper{};
    if (!yarp().canWrite()) {
        yError("Missing server method '%s'?", "void Skill_request::send_stop()");
    }
    yarp().write(helper, helper);
}

void Skill_request::send_ok()
{
    Skill_request_send_ok_helper helper{};
    if (!yarp().canWrite()) {
        yError("Missing server method '%s'?", "void Skill_request::send_ok()");
    }
    yarp().write(helper, helper);
}

// help method
std::vector<std::string> Skill_request::help(const std::string& functionName)
{
    bool showAll = (functionName == "--all");
    std::vector<std::string> helpString;
    if (showAll) {
        helpString.emplace_back("*** Available commands:");
        helpString.emplace_back("request_ack");
        helpString.emplace_back("send_start");
        helpString.emplace_back("send_stop");
        helpString.emplace_back("send_ok");
        helpString.emplace_back("help");
    } else {
        if (functionName == "request_ack") {
            helpString.emplace_back("SkillAck request_ack() ");
            helpString.emplace_back("request_ack  Get the ack of the skill. ");
            helpString.emplace_back("return              The enum indicating the ack of the skill.. ");
        }
        if (functionName == "send_start") {
            helpString.emplace_back("void send_start() ");
            helpString.emplace_back("request_stop  Send a cmd_start skill. ");
            helpString.emplace_back("return               void. ");
        }
        if (functionName == "send_stop") {
            helpString.emplace_back("void send_stop() ");
            helpString.emplace_back("send_stop  Send a CMD_STOP request to the skill. ");
            helpString.emplace_back("return              void. ");
        }
        if (functionName == "send_ok") {
            helpString.emplace_back("void send_ok() ");
            helpString.emplace_back("send_stop  Send a CMD_OK tell the skill that a succes/failure state was read. ");
            helpString.emplace_back("return              void. ");
        }
        if (functionName == "help") {
            helpString.emplace_back("std::vector<std::string> help(const std::string& functionName = \"--all\")");
            helpString.emplace_back("Return list of available commands, or help message for a specific function");
            helpString.emplace_back("@param functionName name of command for which to get a detailed description. If none or '--all' is provided, print list of available commands");
            helpString.emplace_back("@return list of strings (one string per line)");
        }
    }
    if (helpString.empty()) {
        helpString.emplace_back("Command not found");
    }
    return helpString;
}

// read from ConnectionReader
bool Skill_request::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::idl::WireReader reader(connection);
    reader.expectAccept();
    if (!reader.readListHeader()) {
        reader.fail();
        return false;
    }

    std::string tag = reader.readTag();
    bool direct = (tag == "__direct__");
    if (direct) {
        tag = reader.readTag();
    }
    while (!reader.isError()) {
        if (tag == "request_ack") {
            Skill_request_request_ack_helper::s_return_helper = request_ack();
            yarp::os::idl::WireWriter writer(reader);
            if (!writer.isNull()) {
                if (!writer.writeListHeader(1)) {
                    return false;
                }
                if (!writer.writeI32(static_cast<int32_t>(Skill_request_request_ack_helper::s_return_helper))) {
                    return false;
                }
            }
            reader.accept();
            return true;
        }
        if (tag == "send_start") {
            send_start();
            yarp::os::idl::WireWriter writer(reader);
            if (!writer.isNull()) {
                if (!writer.writeListHeader(0)) {
                    return false;
                }
            }
            reader.accept();
            return true;
        }
        if (tag == "send_stop") {
            send_stop();
            yarp::os::idl::WireWriter writer(reader);
            if (!writer.isNull()) {
                if (!writer.writeListHeader(0)) {
                    return false;
                }
            }
            reader.accept();
            return true;
        }
        if (tag == "send_ok") {
            send_ok();
            yarp::os::idl::WireWriter writer(reader);
            if (!writer.isNull()) {
                if (!writer.writeListHeader(0)) {
                    return false;
                }
            }
            reader.accept();
            return true;
        }
        if (tag == "help") {
            std::string functionName;
            if (!reader.readString(functionName)) {
                functionName = "--all";
            }
            auto help_strings = help(functionName);
            yarp::os::idl::WireWriter writer(reader);
            if (!writer.isNull()) {
                if (!writer.writeListHeader(2)) {
                    return false;
                }
                if (!writer.writeTag("many", 1, 0)) {
                    return false;
                }
                if (!writer.writeListBegin(BOTTLE_TAG_INT32, static_cast<uint32_t>(help_strings.size()))) {
                    return false;
                }
                for (const auto& help_string : help_strings) {
                    if (!writer.writeString(help_string)) {
                        return false;
                    }
                }
                if (!writer.writeListEnd()) {
                    return false;
                }
            }
            reader.accept();
            return true;
        }
        if (reader.noMore()) {
            reader.fail();
            return false;
        }
        std::string next_tag = reader.readTag();
        if (next_tag == "") {
            break;
        }
        tag.append("_").append(next_tag);
    }
    return false;
}
