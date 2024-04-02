#include "viewservice.hpp"
#include <QDebug>

ViewService::ViewService(SessionManager *session, QObject *parent)
    : QObject(parent),m_session(session)
{
    if (m_session == nullptr)
    {
         qWarning() << "ViewService: sessionManager is null";
    }
    else
    {
        qInfo("sesionManager is not null");
    }
    connect(m_session, &SessionManager::dbClosed, this, &ViewService::handleDbClose);
    connect(m_session, &SessionManager::userCreated, this, &ViewService::handleRegisterUserFinished);

}

void ViewService::loginAdmin(const QString &username, const QString &password)
{
    qDebug() << "ViewService::loginAdmin";
    m_session->loginAdmin(username, password);
}

void ViewService::dbClose()
{
    qDebug() << "ViewService::dbClose";
    m_session->dbClose();
}

void ViewService::registerUser(const QString &user, const QString &password, const QString &email, const QString &company)
{
    qDebug() << "ViewService::registerUser";
    m_session->checkUserAndCreate(user,password,email,company);
}

void ViewService::generateQrCode()
{
    qDebug() << "ViewService::generateQrCode";
    m_session->generateQrCode();
}

void ViewService::saveSecret()
{
    qDebug() << "ViewService::saveSecret";
    m_session->saveSecretKey();
}

void ViewService::verifyTOTP(const int& userToken)
{
    qDebug() << "ViewService::verifyTOTP";
    m_session->verifyTOTP(userToken);
}

void ViewService::login(const QString &username, const QString &password)
{
    qDebug() << "ViewService::login";
    if(m_session->getLoginState()->isRegister())
    {
        m_session->firstLoginUser(username,password);
    }
    else
    {
        m_session->loginUser(username,password);
    }
}

void ViewService::handleRegisterUserFinished(bool success)
{
    qDebug() << "ViewService::handleRegisterUserFinished: " << success;
    emit registerStatus(success);
}

void ViewService::handleDbClose(bool success)
{
    qDebug() << "ViewService::handleDbClose: " << success;
    emit dbClosedStatus(success);
}


SessionManager *ViewService::getSessionManager() const
{
    qDebug() << "ViewService::getSessionManager: ";
    return m_session;
}



