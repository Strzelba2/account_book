#ifndef VIEWSERVICE_HPP
#define VIEWSERVICE_HPP

#include <QObject>
#include "sessionmanager.hpp"

class ViewService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SessionManager* sessionManager READ getSessionManager CONSTANT)

public:
    explicit ViewService(SessionManager* session, QObject *parent = nullptr);

    Q_INVOKABLE void loginAdmin(const QString& username, const QString& password);
    Q_INVOKABLE void dbClose();
    Q_INVOKABLE void registerUser(const QString &user, const QString &password, const QString &email, const QString &company);
    Q_INVOKABLE void generateQrCode();
    Q_INVOKABLE void saveSecret();
    Q_INVOKABLE void verifyTOTP(const int& userToken);
    Q_INVOKABLE void login(const QString& username, const QString& password);

    SessionManager *getSessionManager() const;

signals:

    void dbClosedStatus(bool success);
    void registerStatus(bool success);
    void dataSession();


private slots:

    void handleRegisterUserFinished(bool success);
    void handleDbClose(bool success);

private:

    SessionManager* m_session;
};

#endif // VIEWSERVICE_HPP
