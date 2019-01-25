#include "binaryserializer.h"

#include <QMetaType>
#include <QDataStream>
#include <QJsonDocument>
#include <QDebug>

QDataStream &operator<<(QDataStream &stream, const QJsonDocument &doc)
{
    qDebug() << "<<" << doc;
    stream << doc.toJson();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QJsonDocument &doc)
{
    qDebug() << ">>" << doc;
    QByteArray buffer;
    stream << buffer;
    doc = QJsonDocument::fromJson(buffer);
    return stream;
}

BinarySerializer::BinarySerializer() : AbstractSerializer ()
{

}

QVariant BinarySerializer::fromString(const QString &value, const QMetaType::Type &type) const
{
    QVariant copy;
    QVariant v;
    QByteArray data = QByteArray::fromBase64(value.toLocal8Bit());
    QDataStream ds(&data,QIODevice::ReadOnly);
    ds >> v;
    switch (type) {
    case QMetaType::QJsonDocument:
        v = QVariant(QJsonDocument::fromJson(v.toByteArray()));
        break;
    }
    return v;
}

QString BinarySerializer::toString(const QVariant &value) const
{
    QVariant copy;
    QMetaType::Type type = static_cast<QMetaType::Type>(value.type());
    switch (type) {
    case QMetaType::QJsonDocument:
        copy = value.value<QJsonDocument>().toJson();
        break;

    default:
        copy = value;
    }

    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    ds << copy;
    return QString(data.toBase64());
}
