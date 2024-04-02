#include "user.hpp"
#include <QDebug>

User::User(QObject *parent)
    : QObject(parent), m_id(-1)
{

}

void User::clear()
{
    qDebug() << "User::clear()";
    setId(0);
    setEmail("");
    setUsername("");
    m_secretKey = "";
}

int User::id() const
{
    qDebug() << "User::id()";
    return m_id;
}

void User::setId(int newId)
{
    qDebug() << "User::setId";
    if (m_id == newId)
        return;
    m_id = newId;
    emit idUserChanged();
}

const QString &User::username() const
{
    qDebug() << "User::username";
    return m_username;
}

void User::setUsername(const QString &newUsername)
{
    qDebug() << "User::setUsername";
    if (m_username == newUsername)
        return;
    m_username = newUsername;
    emit usernameChanged();
}


const QString &User::email() const
{
    qDebug() << "User::email";
    return m_email;
}

void User::setEmail(const QString &newEmail)
{
    qDebug() << "User::setEmail";
    if (m_email == newEmail)
        return;
    m_email = newEmail;
    emit userEmailChanged();
}

QByteArray User::getSecretKeyForSession(const SessionManager *sessionManager) const
{
    qDebug() << "User::getSecretKeyForSession";
    if (sessionManager->isAuthorizedToAccessKey())
    {
        return m_secretKey;
    }
    return QByteArray();
}

void User::clearSecretKey()
{
    qDebug() << "User::clearSecretKey";
    if(!m_secretKey.isEmpty())
    {
        m_secretKey.clear();
    }
}

void User::setKey(const QByteArray &secret)
{
    qDebug() << "User::setKey";
    if (m_secretKey == secret)
        return;
    m_secretKey = secret;
}

const QString &User::company() const
{
    qDebug() << "User::company";
    return m_company;
}

void User::setCompany(const QString &newCompany)
{
    qDebug() << "User::setCompany";
    if (m_company == newCompany)
        return;
    m_company = newCompany;
    emit userCompanylChanged();
}
