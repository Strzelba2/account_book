#include "subcontractor.hpp"

Subcontractor::Subcontractor(QObject *parent)
    : QObject{parent}
{

}

int Subcontractor::id() const
{
    return m_id;
}

void Subcontractor::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idSubcontractorChanged();
}

const QString &Subcontractor::shortname() const
{
    return m_shortname;
}

void Subcontractor::setShortname(const QString &newShortname)
{
    if (m_shortname == newShortname)
        return;
    m_shortname = newShortname;
    emit shortnameSubcontractorChanged();
}

const QString &Subcontractor::name() const
{
    return m_name;
}

void Subcontractor::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameSubcontractorChanged();
}

const QString &Subcontractor::nip() const
{
    return m_nip;
}

void Subcontractor::setNip(const QString &newNip)
{
    if (m_nip == newNip)
        return;
    m_nip = newNip;
    emit nipSubcontractorChanged();
}

const QString &Subcontractor::zip() const
{
    return m_zip;
}

void Subcontractor::setZip(const QString &newZip)
{
    if (m_zip == newZip)
        return;
    m_zip = newZip;
    emit zipSubcontractorChanged();
}

const QString &Subcontractor::city() const
{
    return m_city;
}

void Subcontractor::setCity(const QString &newCity)
{
    if (m_city == newCity)
        return;
    m_city = newCity;
    emit citySubcontractorChanged();
}

const QString &Subcontractor::street() const
{
    return m_street;
}

void Subcontractor::setStreet(const QString &newStreet)
{
    if (m_street == newStreet)
        return;
    m_street = newStreet;
    emit streetSubcontractorChanged();
}
