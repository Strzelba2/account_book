#include "sessionmanager.hpp"
#include <QDebug>
#include "usbuser.hpp"

SessionManager::SessionManager(DatabaseManager *dbManager, QObject *parent)
    : QObject(parent), m_user(new User(this)),m_dbManager(dbManager),
      m_bookModel(new BookModel(dbManager,this)), m_totpManager(new TOTPManager(this)),
      m_loginState(new LoginState(this)),m_subcontractorModel(new SubcontractorModel(dbManager,this))
{
    connect(m_totpManager, &TOTPManager::secretChendged, m_user, &User::setKey);
}

void SessionManager::loginAdmin(const QString &username, const QString &password)
{
    qDebug() << "SessionManager::loginAdmin";
    if (m_dbManager->loginAsAdmin(username, password))
    {
        qDebug() << "admin logged in";
        emit loginAdminFinished(true);
    }
    else
    {
        emit sessionError(m_dbManager->getLastDatabaseError());
    }
}

void SessionManager::checkUserAndCreate(const QString &user, const QString &password, const QString &email, const QString &company)
{
    qDebug() << "SessionManager::checkUserAndCreate";
    if(m_dbManager->userExists(user))
    {
        qDebug() << m_dbManager->userExists(user);
        qDebug() << "user exist";
        emit sessionError("user exist");
    }
    else
    {
        if(m_dbManager->CreateUserDb(user,password,email,company))
        {
            qDebug() << "CreateUserDB";
            m_loginState->setIsRegister(true);
            m_loginState->setCurrentState(LoginState::Register);
            if(updateUser(user)){
                emit userCreated(true);
            }
        }
        else
        {
            qDebug() << "error" << m_dbManager->getLastDatabaseError();
            emit sessionError(m_dbManager->getLastDatabaseError());
        }
    }
}

bool SessionManager::updateUser(const QString &user)
{
    qDebug() << "SessionManager::updateUser";
    QVariantMap userData = m_dbManager->getUserData(user);

    if (userData.contains("error")) {
        emit sessionError(userData["error"].toString());
        return false;
    }
    m_user->setId(userData["id"].toInt());
    m_user->setEmail(userData["email"].toString());
    m_user->setUsername(userData["name"].toString());
    m_user->setCompany(userData["company"].toString());
    m_bookModel->setUserEmail(userData["email"].toString());
    return true;
}

void SessionManager::dbClose()
{
    qDebug() << "SessionManager::dbClose";
    if(m_dbManager->closeDb())
    {
        m_loginState->setCurrentState(LoginState::LoginAdmin);
        m_user->clear();
        m_bookModel->setUserEmail("");
        emit dbClosed(true);
    }
    else
    {
        emit sessionError(m_dbManager->getLastDatabaseError());
    }
}

void SessionManager::generateQrCode()
{
    qDebug() << "SessionManager::generateQrCode";
    int keySize = 10;
    QByteArray secret_key = m_totpManager->generateRandomBytes(keySize);
    QString email = m_user->username();
    QString issuer = "EDART";

    m_totpManager->createQRCode(secret_key,email,issuer);
}

void SessionManager::saveSecretKey()
{
    qDebug() << "SessionManager::saveSecretKey";
    QByteArray secretKey = requestSecretKey();
    if (secretKey.isEmpty())
    {
       emit sessionError("secretkey is empty");
       return;
    }

    auto EncryptSecretKey = UsbUser::encryptMessage(std::vector<unsigned char>(secretKey.begin(), secretKey.end()), m_key, m_salt);
    if(EncryptSecretKey.empty())
    {
        emit sessionError("EncryptSecretKey is empty");
        return;
    }

    QString EncryptString = QString(QByteArray(reinterpret_cast<const char*>(EncryptSecretKey.data()), EncryptSecretKey.size()).toBase64());

    if(!m_dbManager->saveSecretKey(m_user->username(),EncryptString))
    {
        emit sessionError(m_dbManager->getLastDatabaseError());
        return;
    }
    m_user->clearSecretKey();
    m_loginState->setCurrentState(LoginState::QRCode);
    emit secredSaved(true);
}

void SessionManager::verifyTOTP( int userToken)
{
    qDebug() << "SessionManager::verifyTOTP";
    QString dbSecretKey = m_dbManager->getSecretKey(m_user->username());
    if(dbSecretKey.isEmpty())
    {
        emit sessionError(m_dbManager->getLastDatabaseError());
        return;
    }
    QByteArray encryptedSecretKey = QByteArray::fromBase64(dbSecretKey.toUtf8());

    auto decryptedsecretKey = UsbUser::decryptMessage(std::vector<unsigned char>(encryptedSecretKey.begin(), encryptedSecretKey.end()), m_key, m_salt);
    if(decryptedsecretKey.empty())
    {
        emit sessionError("an error occurred during data decryption ");
        return ;
    }
    QByteArray secretKey(reinterpret_cast<char*>(decryptedsecretKey.data()), decryptedsecretKey.size());

    int timestep = 30;
    int digits = 6;
    int totp = m_totpManager->generateTOTP(secretKey, timestep, digits);
    if (totp == userToken)
    {
        if(m_loginState->isRegister())
        {
            if(m_dbManager->updateActiveStatus(m_user->username(),true ))
            {
                if(!m_dbManager->grantFullAccessToUser(m_user->username(),m_user->company()))
                {
                    emit sessionError(m_dbManager->getLastDatabaseError());
                    return;
                }
            }
            else
            {
                emit sessionError(m_dbManager->getLastDatabaseError());
                return;
            }
            m_loginState->setIsRegister(false);
            m_dbManager->closeDb();
            emit veryficationStatus("verification successful please log in again");
        }
        if(!m_bookModel->loadInitialData()){
            m_bookModel->addNewEmptyBook();
        }
        if(m_subcontractorModel->loadInitialData()){
            m_bookModel->setSubcontractorModel(m_subcontractorModel);
        }else{
            emit sessionError("can not open Database");
            return;
        }
        emit loginUserFinished (true);
    }
    else
    {
        emit sessionError("not correct totp code");
    }
}

TOTPManager *SessionManager::getTOTPManager() const
{
    qDebug() << "SessionManager::getTOTPManager";
    return m_totpManager;
}


bool SessionManager::isAuthorizedToAccessKey() const
{
    qDebug() << "SessionManager::isAuthorizedToAccessKey()";
    if(m_loginState->isRegister())
    {
        return true;
    }
    return false;
}

QByteArray SessionManager::requestSecretKey() const
{
    qDebug() << "SessionManager::requestSecretKey";
    return m_user->getSecretKeyForSession(this);
}

void SessionManager::receiveKeyAndSalt(const std::pair<std::vector<unsigned char>, std::vector<unsigned char> > &keyAndSalt)
{
    qDebug() << "SessionManager::receiveKeyAndSalt";
    m_key = keyAndSalt.first;
    m_salt = keyAndSalt.second;
}

void SessionManager::loginUser(const QString &username, const QString &password)
{
    qDebug() << "SessionManager::loginUser";
    if (m_dbManager->openDb())
    {
        m_dbManager->closeDb();
    }
    QString company = m_user->company();
    if(!m_dbManager->loginAsUser(username,password,company))
    {
        qDebug() << "password or username not walid";
        emit sessionError(m_dbManager->getLastDatabaseError());
        return;
    }
    if (!m_dbManager->validatePassword(username, password))
    {
        qDebug() << "password not walid";
        emit sessionError(m_dbManager->getLastDatabaseError());
        return;
    }
    m_user->clear();
    updateUser(username);

    emit totpAuthorization();
}

void SessionManager::firstLoginUser(const QString &username, const QString &password)
{
    qDebug() << "SessionManager::firstLoginUser";
    if(!m_dbManager->openDb())
    {
        qDebug() << "error opendb";
        emit sessionError(m_dbManager->getLastDatabaseError());
        return;
    }
    if(!m_dbManager->userExists(username))
    {
        qDebug() << "user do not exist";
        emit sessionError(m_dbManager->getLastDatabaseError());
        return;
    }

    if (!m_dbManager->validatePassword(username, password))
    {
        qDebug() << "pasword not walid";
        emit sessionError(m_dbManager->getLastDatabaseError());
        return;
    }

    m_user->clear();
    updateUser(username);

    emit totpAuthorization();
}

User *SessionManager::getUser() const
{
    qDebug() << "SessionManager::getUser";
    return m_user;
}




LoginState *SessionManager::getLoginState() const
{
    return m_loginState;
}

BookModel *SessionManager::bookModel() const
{
    return m_bookModel;
}

void SessionManager::setBookModel(BookModel *newBookModel)
{
    if (m_bookModel == newBookModel)
        return;
    m_bookModel = newBookModel;
    emit bookModelChanged();
}

SubcontractorModel *SessionManager::getSubcontractorModel() const
{
    return m_subcontractorModel;
}
