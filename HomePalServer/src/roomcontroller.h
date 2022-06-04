#pragma once

#include <QObject>
#include <iostream>
#include <vector>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QTimer>

class RoomGroup;

class RoomController : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(QList<RoomGroup*> rooms READ rooms NOTIFY roomsChanged)
    Q_PROPERTY(int selectedRoom READ selectedRoom WRITE setSelectedRoom NOTIFY selectedRoomChanged)

public:
    explicit RoomController(QObject* parent = nullptr);

public:
    int selectedRoom() {return m_selectedRoom;}
    void setSelectedRoom(int roomIndex);


    Q_INVOKABLE void addRoom(QString name);

    QList<RoomGroup*> rooms() {return m_rooms;}
signals:
    void roomsChanged();
    void selectedRoomChanged();
private:
    void testRooms();

    int m_selectedRoom = 0;

    QList<RoomGroup*> m_rooms;


};
