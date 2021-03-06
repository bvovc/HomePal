#include "connectioncontroller.h"
#include "Connection/tcpclient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

ConnectionController::ConnectionController(QObject *parent) : QObject(parent)
{
    qDebug() << "ConnectionController::ConnectionController is initialized";
}

void ConnectionController::startClient()
{
    m_client = new ClientTCP(QStringLiteral("ws://") + m_defaultAdress + ":" + QString::number(m_defaultPort));
    connect(m_client, &IClient::receivedDataChanged, this, &ConnectionController::onDataReceived);
    connect(m_client, &IClient::connectionLost, this, &ConnectionController::connectionLost);
    connect(m_client, &IClient::connected, this, &ConnectionController::connected);

    m_defaultAdress = "127.0.0.1";
    m_defaultPort = 1488;
}

void ConnectionController::startClient(QString adress, int port)
{
    m_defaultAdress = adress;
    m_defaultPort = port;
}

void ConnectionController::stopClient()
{
    m_client->disconnect(this);
}

void ConnectionController::requestComponent()
{
    QJsonDocument doc;
    QJsonObject d = doc.object();
    d["type"] = ConnType::RequestComponents;
    doc = QJsonDocument(d);
    m_client->sendData(doc);
}

void ConnectionController::onDataReceived()
{
    qDebug() << "ConnectionController::onDataReceived :: signal emmited";
    QJsonDocument receivedDoc = m_client->receiveData();
    processData(receivedDoc);
}

void ConnectionController::processData(QJsonDocument &doc)
{
    QJsonObject received = doc.object();
    ConnType dataType = static_cast<ConnType>(received.value("type").toInt());
    switch(dataType){
    case ConnType::Init:

    case ConnType::ComponentList:
        Q_EMIT componentListReceived(doc);
        return;
    case ConnType::CreateComponent:
    case ConnType::CreateRoom:
    case ConnType::EditRoom:
    case ConnType::EditComponent:
        qDebug() << "ConnectionController::processData :: nothing to do with this type of data on client side" << dataType;
        return;
    case ConnType::Journal:
        Q_EMIT journalReceived(doc);
        return;
    case ConnType::LastPage:
        Q_EMIT lastPageReceived(doc);
        return;
    case ConnType::Disconnect:
        qDebug() << "ConnectionController::processData :: client preparing for disconnect";
        m_client->disconnect(this);
        return;

    default:
        return;
    }
}

void ConnectionController::addComponent(Component &comp)
{
    QJsonDocument doc = Component::toDoc(comp);
    QJsonObject d = doc.object();
    d["type"] = ConnType::CreateComponent;
    doc = QJsonDocument(d);
    m_client->sendData(doc);
}

void ConnectionController::addRoom(RoomGroup &room)
{
    QJsonDocument doc = RoomGroup::toDoc(room);
    QJsonObject d = doc.object();
    d["type"] = ConnType::CreateRoom;
    doc = QJsonDocument(d);
    m_client->sendData(doc);
}

void ConnectionController::editComponent(Component &comp)
{
    QJsonDocument doc = Component::toDoc(comp);
    QJsonObject d = doc.object();
    d["type"] = ConnType::EditComponent;
    doc = QJsonDocument(d);
    m_client->sendData(doc);
}

void ConnectionController::editRoom(RoomGroup &room)
{
    QJsonDocument doc = RoomGroup::toDoc(room);
    qDebug() << "DOC DOC DOC" << doc;
    QJsonObject d = doc.object();
    d["type"] = ConnType::EditRoom;
    doc = QJsonDocument(d);
    m_client->sendData(doc);
}

void ConnectionController::deleteRoom(RoomGroup &room)
{
    QJsonDocument doc = RoomGroup::toDoc(room);
    QJsonObject d = doc.object();
    d["type"] = ConnType::DeleteRoom;
    doc = QJsonDocument(d);
    m_client->sendData(doc);
}

void ConnectionController::deleteComponent(Component &comp)
{
    QJsonDocument doc = Component::toDoc(comp);
    QJsonObject d = doc.object();
    d["type"] = ConnType::DeleteComponent;
    doc = QJsonDocument(d);
    m_client->sendData(doc);
}
