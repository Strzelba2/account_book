#include "usbuser.hpp"
#include "loginstate.hpp"
#include <QDebug>

UsbUser::UsbUser(SessionManager* session,QObject *parent)
    : QObject(parent),m_session(session)
{
    initDriverPatchs();
}

void UsbUser::CheckForNewUsbDrives()
{
    qDebug() << "UsbUser::CheckForNewUsbDrives";
    DWORD driveMask = GetLogicalDrives();
    int i = 0;
    while (driveMask != 0)
    {
        if (driveMask & 1)
        {
            QString driveName = QString(QChar('A' + i)) + ":\\";
            if (GetDriveType(driveName.toStdWString().c_str()) == DRIVE_REMOVABLE)
            {
                if (!drivePaths.contains(driveName)) {
                    QString filePath = driveName + "data.json";
                    qDebug() << "path to file" << filePath;
                    emit usbConnected(filePath);
                    drivePaths.append(driveName);
                }
            }
        }
        driveMask >>= 1;
        i++;
    }
}

void UsbUser::initDriverPatchs()
{
    qDebug() << "UsbUser::initDriverPatchs";
    for (int i = 0; i < 26; ++i) {
        QString drivePath = QString(char('A' + i)) + ":\\";
        DWORD driveType = GetDriveType(drivePath.toStdWString().c_str());

        if (driveType != DRIVE_NO_ROOT_DIR && driveType != DRIVE_UNKNOWN) {
            drivePaths.append(drivePath);
        }
    }
}

void UsbUser::RemoveDrivePath(const QString &path)
{
    qDebug() << "UsbUser::RemoveDrivePath";
    int index = drivePaths.indexOf(path);
    if (index != -1) {
        drivePaths.remove(index);
    }
}

bool UsbUser::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    qDebug() << "UsbUser::nativeEventFilter";
    MSG* msg = reinterpret_cast<MSG*>(message);
    if (msg->message == WM_DEVICECHANGE) {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch (msg->wParam) {
            case DBT_DEVICEARRIVAL:
                if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
                    CheckForNewUsbDrives();
                }
                break;
            case DBT_DEVICEREMOVECOMPLETE:
                if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
                    QString removedDrivePath = GetDrivePathFromDBNotification(lpdb);
                    qDebug() << "path to remove" << removedDrivePath;
                    RemoveDrivePath(removedDrivePath);
                    emit usbDisconnected();
                }
                break;
        }
    }
    return false;
}

QString UsbUser::mixPasswordWithMessage(const QString &password, const QString &message) {
    qDebug() << "UsbUser::mixPasswordWithMessage";
    QString mixedMessage;
    int passwordIndex = 0;

    for (int i = 0; i < message.length(); ++i) {
        if (i % 2 == 0 && passwordIndex < password.length()) {
            mixedMessage += password[passwordIndex++];
        } else {
            mixedMessage += message[i];
        }
    }

    return mixedMessage;
}

QString  UsbUser::handleErrors() {
    qDebug() << "UsbUser::handleErrors";
    QString errorMessages;
    unsigned long errCode;

    while ((errCode = ERR_get_error())) {
        char errBuf[256];
        ERR_error_string_n(errCode, errBuf, sizeof(errBuf));
        qDebug() << "OpenSSL Error:" << errBuf;
        errorMessages += QString::fromLocal8Bit(errBuf) + "\n";

    }

    return errorMessages;

}

std::vector<unsigned char> UsbUser::encryptMessage(const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv) {
    qDebug() << "UsbUser::encryptMessage";
    EVP_CIPHER_CTX* ctx;
    int len;
    int ciphertext_len;
    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);

    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data()))
    {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size()))
    {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len))
    {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

std::vector<unsigned char> UsbUser::deriveKey(const QString &password, const std::vector<unsigned char> &salt) {
    std::vector<unsigned char> key(EVP_MAX_KEY_LENGTH);
    qDebug() << "UsbUser::deriveKey";
    if (!PKCS5_PBKDF2_HMAC(password.toStdString().c_str(), -1,
                           salt.data(), salt.size(), 10000,
                           EVP_sha256(), key.size(), key.data())) {
        handleErrors();
        return {};
    }
    return key;
}

void UsbUser::processAndEncrypt(const QString &password ,const QString &path) {
    qDebug() << "UsbUser::processAndEncrypt";
    QString newPassword = mixPasswordWithMessage(password, RANDOM_KEY);

    std::vector<unsigned char> salt(SHA256_DIGEST_LENGTH);
    RAND_bytes(salt.data(), salt.size());

    std::vector<unsigned char> key = deriveKey(newPassword, salt);

    if(key.empty())
    {
        emit usbError(handleErrors());
        return;
    }

    passKeyToSessionManager(key,salt);

    QString company = m_session->getUser()->company();

    auto enCompany = UsbUser::encryptMessage(QStringToVector(company),key,salt);

    std::vector<unsigned char> additionalSalt = generateDeterministicSalt(password);
    std::vector<unsigned char> keytoSalt = deriveKey(newPassword, additionalSalt);

    if(keytoSalt.empty())
    {
        emit usbError(handleErrors());
        return;
    }

    auto hash = UsbUser::encryptMessage(key, key, salt);

    if(hash.empty())
    {
        emit usbError(handleErrors());
        return;
    }

    auto saltEncrypt = UsbUser::encryptMessage(salt, keytoSalt, additionalSalt);

    if(saltEncrypt.empty())
    {
        emit usbError(handleErrors());
        return;
    }

    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject jsonObject;
        jsonObject["hash"] = QString(QByteArray(reinterpret_cast<const char*>(hash.data()), hash.size()).toBase64());
        jsonObject["salt"] = QString(QByteArray(reinterpret_cast<const char*>(saltEncrypt.data()), saltEncrypt.size()).toBase64());
        jsonObject["company"] = QString(QByteArray(reinterpret_cast<const char*>(enCompany.data()), enCompany.size()).toBase64());
        file.write(QJsonDocument(jsonObject).toJson());
    }
    file.close();

    std::string checksum = computeChecksum(path);

    auto encryptedChecksum = UsbUser::encryptMessage(std::vector<unsigned char>(checksum.begin(), checksum.end()), key, salt);

    if(encryptedChecksum.empty())
    {
        emit usbError(handleErrors());
        return;
    }

    QFileInfo fileInfo(path);
    QDir directory = fileInfo.dir();

    QString CheckSumPath = directory.absolutePath() + "checksum.enc";

    QFile checksumFile(CheckSumPath);
    if (checksumFile.open(QIODevice::WriteOnly)) {
        checksumFile.write(QByteArray(reinterpret_cast<const char*>(encryptedChecksum.data()), encryptedChecksum.size()));
    }
    checksumFile.close();
    m_session->getLoginState()->setCurrentState(LoginState::PIPUSB);
    emit userSaved();
}

void UsbUser::processAndDecrypt(const QString &password, const QString &path)
{
    qDebug() << "UsbUser::processAndDecrypt";
    QString newPassword = mixPasswordWithMessage(password, RANDOM_KEY);

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        emit usbError("can not open file.Probably not the correct flash drive");
        return;
    }
    QByteArray fileData = file.readAll();
    QJsonObject jsonObject = QJsonDocument::fromJson(fileData).object();
    file.close();

    QByteArray encryptedHash = QByteArray::fromBase64(jsonObject["hash"].toString().toUtf8());
    QByteArray encryptedSalt = QByteArray::fromBase64(jsonObject["salt"].toString().toUtf8());
    QByteArray encryptedCompany = QByteArray::fromBase64(jsonObject["company"].toString().toUtf8());

    std::vector<unsigned char> additionalSalt = generateDeterministicSalt(password);
    std::vector<unsigned char> keytoSalt = deriveKey(newPassword, additionalSalt);

    if(keytoSalt.empty())
    {
        emit usbError(handleErrors());
        return;
    }

    auto salt = UsbUser::decryptMessage(std::vector<unsigned char>(encryptedSalt.begin(), encryptedSalt.end()), keytoSalt, additionalSalt);

    if(salt.empty())
    {
        emit usbError(handleErrors());
        return;
    }

    std::vector<unsigned char> key = deriveKey(newPassword, salt);

    if(key.empty())
    {
        emit usbError(handleErrors());
        return;
    }

    auto hash = UsbUser::decryptMessage(std::vector<unsigned char>(encryptedHash.begin(), encryptedHash.end()), key, salt);

    if(hash.empty())
    {
        emit usbError(handleErrors());
        return;
    }
    auto company = UsbUser::decryptMessage(std::vector<unsigned char>(encryptedCompany.begin(), encryptedCompany.end()), key, salt);

    m_session->getUser()->setCompany(QString::fromUtf8(reinterpret_cast<const char*>(company.data()), company.size()));

    QFileInfo fileInfo(path);
    QDir directory = fileInfo.dir();

    QString CheckSumPath = directory.absolutePath() + "checksum.enc";

    QFile fileCheckSum(CheckSumPath);
    if (!fileCheckSum.open(QIODevice::ReadOnly)) {

        emit usbError("Can not open file");
        return;
    }
    QByteArray fileDataCheckSum = fileCheckSum.readAll();
    fileCheckSum.close();
    auto checksum = UsbUser::decryptMessage(std::vector<unsigned char>(fileDataCheckSum.begin(), fileDataCheckSum.end()), key, salt);
    if(checksum.empty())
    {
        emit usbError(handleErrors());
        return;
    }
    std::string checksum_new = computeChecksum(path);

    if ((key == hash)&&(checksum==std::vector<unsigned char>(checksum_new.begin(), checksum_new.end()))) {
        qDebug() << "Password correct" ;
        m_session->getLoginState()->setCurrentState(LoginState::Login);
        passKeyToSessionManager(key,salt);
        emit userConnected();
    } else {
        qDebug() << "Password incorrect" ;
        emit usbError("Password incorrect");
    }
}

std::string UsbUser::computeChecksum(const QString &filePath) {
    qDebug() << "UsbUser::computeChecksum" ;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char *>(fileData.constData()), fileData.size(), hash);
        file.close();
        return std::string(reinterpret_cast<const char*>(hash), SHA256_DIGEST_LENGTH);
    }
    return "";
}

std::vector<unsigned char> UsbUser::QStringToVector(const QString &str)
{
    qDebug() << "UsbUser::QStringToVector" ;
    QByteArray byteArray = str.toUtf8();
    return std::vector<unsigned char>(byteArray.begin(), byteArray.end());
}

void UsbUser::passKeyToSessionManager(const std::vector<unsigned char>& key,const std::vector<unsigned char> &salt)
{
    qDebug() << "UsbUser::passKeyToSessionManager" ;
    if (m_session->isAuthorizedToAccessKey()|| m_session->getLoginState()->currentState() == LoginState::Login)
    {
        qDebug() << "UsbUser::passKeyToSessionManager" ;
        std::pair<std::vector<unsigned char>, std::vector<unsigned char>> keyAndSalt = std::make_pair(key, salt);
        m_session->receiveKeyAndSalt(keyAndSalt);
    }
}

QString UsbUser::GetDrivePathFromDBNotification(PDEV_BROADCAST_HDR lpdb)
{
    qDebug() << "UsbUser::GetDrivePathFromDBNotification" ;
    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
    DWORD driveMask = lpdbv->dbcv_unitmask;

    for (char driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter) {
        if (driveMask & 1) {
            return QString(driveLetter) + ":\\";
        }
        driveMask >>= 1;
    }

    return QString();
}


std::vector<unsigned char> UsbUser::decryptMessage(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    std::vector<unsigned char> plaintext(ciphertext.size());

    qDebug() << "UsbUser::decryptMessage" ;
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data()))
    {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    if(1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()))
    {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len))
    {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(plaintext_len);
    return plaintext;
}

std::vector<unsigned char> UsbUser::generateDeterministicSalt(const QString& password) {
    qDebug() << "UsbUser::generateDeterministicSalt" ;
    std::string passwordStr = password.toStdString();

    std::vector<unsigned char> salt(SHA256_DIGEST_LENGTH);
    SHA256(reinterpret_cast<const unsigned char*>(passwordStr.c_str()), passwordStr.length(), salt.data());

    return salt;
}


