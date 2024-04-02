#ifndef TOTPMANAGER_HPP
#define TOTPMANAGER_HPP

#include <QObject>
#include <QImage>
#include <QString>
#include <QCryptographicHash>
#include <QQuickPaintedItem>
#include "qrcodegen/qrcodegen.hpp"
#include <QPainter>

class TOTPManager :  public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage qrImage READ getQrImage WRITE setQrImage NOTIFY qrImageChanged)

public:
    explicit TOTPManager(QObject *parent = nullptr);

    QByteArray generateRandomBytes(int size);
    QString convertToBase32(const QByteArray &bytes);
    QByteArray decodeBase32(const QString &input);
    void createQRCode(const QByteArray &secret_key , const QString &email, const QString &issuer);
    void paint(QPainter *painter) override;
    const QImage &getQrImage() const;
    void setQrImage(const QImage &image);
    int generateTOTP(const QByteArray &secret_key, unsigned long long timeStep, int returnDigits);

signals:

    void qrImageChanged();
    void secretChendged (const QByteArray secret);

private:

    QImage m_qrImage;
};

#endif // TOTPMANAGER_HPP
