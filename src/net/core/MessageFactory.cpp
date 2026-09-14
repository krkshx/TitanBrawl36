#pragma once
#include "../security/ClientHelloMessage.cpp"
#include "../security/ServerHelloMessage.cpp"
#include "../account/LoginMessage.cpp"
#include "../account/LoginFailedMessage.cpp"
#include "../account/LoginOkMessage.cpp"
#include "../account/CreateAccountOkMessage.cpp"
#include "../account/ClientCapabilitiesMessage.cpp"
#include "../account/KeepAliveMessage.cpp"
#include "../account/KeepAliveServerMessage.cpp"
#include "../account/AnalyticEventMessage.cpp"
#include "../account/TitanDisconnectedMessage.cpp"
#include "../home/OwnHomeDataMessage.cpp"
#include "../home/AvailableServerCommandMessage.cpp"
#include "../home/ChangeAvatarNameMessage.cpp"
#include "GenericMessage.cpp"

class MessageFactory {
public:
    static PiranhaMessage *create(std::int32_t type) {
        switch (type) {
            case 10100: return new ClientHelloMessage();
            case 10101: return new LoginMessage();
            case 10107: return new ClientCapabilitiesMessage();
            case 10108: return new KeepAliveMessage();
            case 10110: return new AnalyticEventMessage();
            case 10212: return new ChangeAvatarNameMessage();
            case 20100: return new ServerHelloMessage();
            case 20103: return new LoginFailedMessage();
            case 20104: return new LoginOkMessage();
            case 20108: return new KeepAliveServerMessage();
            case 24101: return new OwnHomeDataMessage();
            case 24111: return new AvailableServerCommandMessage();
            case 26007: return new CreateAccountOkMessage();
            case 25892: return new TitanDisconnectedMessage();
            default: return new GenericMessage(type);
        }
    }
};
