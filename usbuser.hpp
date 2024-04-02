#ifndef USBDETECTOR_HPP
#define USBDETECTOR_HPP

#include <QAbstractNativeEventFilter>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <windows.h>
#include <dbt.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <QJsonDocument>
#include <QJsonObject>
#include "sessionmanager.hpp"

class UsbUser : public QObject , public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit UsbUser(SessionManager* session,QObject *parent = nullptr);

    Q_INVOKABLE void processAndEncrypt(const QString &password, const QString &path);
    Q_INVOKABLE void processAndDecrypt(const QString &password ,const QString &path);

    QString  handleErrors();
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override ;
    QString mixPasswordWithMessage(const QString &password, const QString &message);
    static std::vector<unsigned char> encryptMessage(const std::vector<unsigned char>& plaintext,
                                                    const std::vector<unsigned char>& key,
                                                    const std::vector<unsigned char>& iv);
    static std::vector<unsigned char> decryptMessage(const std::vector<unsigned char>& ciphertext,
                                                    const std::vector<unsigned char>& key,
                                                    const std::vector<unsigned char>& iv);
    std::vector<unsigned char> deriveKey(const QString &password, const std::vector<unsigned char> &salt);
    std::vector<unsigned char> generateDeterministicSalt(const QString& password);
    std::string computeChecksum(const QString &filePath);
    std::vector<unsigned char> QStringToVector(const QString& str);

    void passKeyToSessionManager(const std::vector<unsigned char>& key,const std::vector<unsigned char> &salt);

signals:
    void usbConnected(const QString path);
    void usbDisconnected();
    void userConnected();
    void userSaved();
    void usbError(const QString error);

private:

    QString GetDrivePathFromDBNotification(PDEV_BROADCAST_HDR lpdb);
    void CheckForNewUsbDrives();
    void initDriverPatchs();
    void RemoveDrivePath(const QString &path);

    static constexpr const char* RANDOM_KEY = "YUL21WEdgIrfCP84qaZCKBDpjVS1UOAXlk2qZ9cbpi23wndf1LUvYhnovoh92nuf";

    SessionManager* m_session;
    QVector<QString> drivePaths;
};

#endif // USBDETECTOR_HPP
