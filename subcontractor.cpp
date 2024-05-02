#include "subcontractor.hpp"
#include <QDebug>

Subcontractor::Subcontractor(QObject *parent)
    : QObject{parent}
{

}

int Subcontractor::id() const
{
    qDebug() << "Subcontractor::id()";
    return m_id;
}

void Subcontractor::setId(int newId)
{
    qDebug() << "Subcontractor::setId";
    if (m_id == newId)
        return;
    m_id = newId;
    emit idSubcontractorChanged();
}

const QString &Subcontractor::shortname() const
{
    qDebug() << "Subcontractor::shortname";
    return m_shortname;
}

void Subcontractor::setShortname(const QString &newShortname)
{
    qDebug() << "Subcontractor::setShortname";
    if (m_shortname == newShortname)
        return;
    m_shortname = newShortname;
    emit shortnameSubcontractorChanged();
}

const QString &Subcontractor::name() const
{
    qDebug() << "Subcontractor::name";
    return m_name;
}

void Subcontractor::setName(const QString &newName)
{
    qDebug() << "Subcontractor::setName";
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameSubcontractorChanged();
}

const QString &Subcontractor::nip() const
{
    qDebug() << "Subcontractor::nip()";
    return m_nip;
}

void Subcontractor::setNip(const QString &newNip)
{
    qDebug() << "Subcontractor::setNip";
    if (m_nip == newNip)
        return;
    m_nip = newNip;
    emit nipSubcontractorChanged();
}

const QString &Subcontractor::zip() const
{
    qDebug() << "Subcontractor::zip()";
    return m_zip;
}

void Subcontractor::setZip(const QString &newZip)
{
    qDebug() << "Subcontractor::setZip";
    if (m_zip == newZip)
        return;
    m_zip = newZip;
    emit zipSubcontractorChanged();
}

const QString &Subcontractor::city() const
{
    qDebug() << "Subcontractor::city()";
    return m_city;
}

void Subcontractor::setCity(const QString &newCity)
{
    qDebug() << "Subcontractor::setCity";
    if (m_city == newCity)
        return;
    m_city = newCity;
    emit citySubcontractorChanged();
}

const QString &Subcontractor::street() const
{
    qDebug() << "Subcontractor::street()";
    return m_street;
}

void Subcontractor::setStreet(const QString &newStreet)
{
    qDebug() << "Subcontractor::setStreet";
    if (m_street == newStreet)
        return;
    m_street = newStreet;
    emit streetSubcontractorChanged();
}
