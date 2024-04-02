#ifndef USER_HPP
#define USER_HPP

#include <QObject>
#include "sessionmanager.hpp"

class SessionManager;

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idUserChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY userEmailChanged)
    Q_PROPERTY(QString company READ company WRITE setCompany NOTIFY userCompanylChanged)

public:
    explicit User(QObject *parent = nullptr);

    void clear();
    void clearSecretKey();

    int id() const;
    void setId(int newId);

    const QString &username() const;
    void setUsername(const QString &newUsername);

    const QString &email() const;
    void setEmail(const QString &newEmail);

    QByteArray getSecretKeyForSession(const SessionManager* sessionManager) const;

    const QString &company() const;
    void setCompany(const QString &newCompany);

signals:

    void idUserChanged();
    void usernameChanged();
    void userEmailChanged();
    void userCompanylChanged();

public slots:

    void setKey(const QByteArray& secret);

private:

    int m_id;
    QString m_username;
    QString m_email;
    QByteArray m_secretKey;
    QString m_company;
};

#endif // USER_HPP
