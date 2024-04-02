#include "totpmanager.hpp"
#include <QDateTime>
#include <QByteArray>
#include <ctime>
#include <openssl/hmac.h>
#include <QRandomGenerator>
#include <QMessageAuthenticationCode>
#include <QDebug>


TOTPManager::TOTPManager(QObject *parent)
    : QQuickPaintedItem(qobject_cast<QQuickItem*>(parent))
{

}

QByteArray TOTPManager::generateRandomBytes(int size)
{
    qDebug() << "TOTPManager::generateRandomBytes";
    QByteArray randomBytes;
    randomBytes.resize(size);

    QRandomGenerator *generator = QRandomGenerator::system();
    for (int i = 0; i < size; ++i) {
        randomBytes[i] = static_cast<char>(generator->bounded(256));
    }

    return randomBytes;
}

QString TOTPManager::convertToBase32(const QByteArray &bytes)
{
    qDebug() << "TOTPManager::convertToBase32";
    const char base32Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QString base32String;

    int bits = 0;
    int value = 0;
    int pos = 0;

    while (pos < bytes.length()) {
        value = (value << 8) | (bytes[pos++] & 0xFF);
        bits += 8;

        while (bits >= 5) {
            base32String.append(base32Alphabet[(value >> (bits - 5)) & 0x1F]);
            bits -= 5;
        }
    }

    if (bits > 0) {
        base32String.append(base32Alphabet[(value << (5 - bits)) & 0x1F]);
    }

    while (base32String.length() % 8 != 0) {
        base32String.append('=');
    }

    return base32String;
}

QByteArray TOTPManager::decodeBase32(const QString &input)
{
    qDebug() << "TOTPManager::decodeBase32";
    QMap<char, quint8> base32Map;
        const QString base32Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
        for (int i = 0; i < base32Chars.length(); ++i) {
            base32Map[base32Chars[i].toLatin1()] = i;
        }

        QByteArray output;
        int buffer = 0;
        int bitsLeft = 0;
        int count = 0;

        for (QChar c : input) {
            if (c.isSpace()) {
                continue;
            }
            if (!base32Map.contains(c.toLatin1())) {
                // Nieprawidłowy znak
                return QByteArray();
            }

            buffer <<= 5;
            buffer |= base32Map[c.toLatin1()];
            bitsLeft += 5;

            if (bitsLeft >= 8) {
                output.append(static_cast<char>((buffer >> (bitsLeft - 8)) & 0xFF));
                bitsLeft -= 8;
            }
            count++;
        }

        return output;
}


void TOTPManager::createQRCode(const QByteArray &secret_key , const QString &email, const QString &issuer)
{
    qDebug() << "TOTPManager::createQRCode";
    using qrcodegen::QrCode;
    using qrcodegen::QrSegment;
    QString secretKeyBase32 = convertToBase32(secret_key);
    QString message = QString("otpauth://totp/%1?secret=%2&issuer=%3").arg(email, secretKeyBase32, issuer);

    std::vector<QrSegment> segs = QrSegment::makeSegments(message.toUtf8().constData());
    const QrCode qr = QrCode::encodeSegments(segs, QrCode::Ecc::LOW);

    const int size = qr.getSize();
    QImage image(size, size, QImage::Format_RGB32);
    QColor black(Qt::black);
    QColor white(Qt::white);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            image.setPixel(x, y, qr.getModule(x, y) ? black.rgb() : white.rgb());
        }
    }

    m_qrImage = image;
    emit secretChendged(secret_key);

    qDebug() << "secret was sent";

    emit qrImageChanged();
    update();
}

void TOTPManager::paint(QPainter *painter)
{
    qDebug() << "TOTPManager::paint";
    if (!m_qrImage.isNull()) {
        QRectF boundingRect = this->boundingRect();
        painter->drawImage(boundingRect, m_qrImage);
    }
}

const QImage &TOTPManager::getQrImage() const
{
    qDebug() << "TOTPManager::getQrImage()";
    return m_qrImage;
}

void TOTPManager::setQrImage(const QImage &image)
{
    qDebug() << "TOTPManager::setQrImage";
    if (image != m_qrImage) {
                m_qrImage = image;
                emit qrImageChanged();
                update();
            }
}

int TOTPManager::generateTOTP(const QByteArray &secret_key, unsigned long long timeStep, int returnDigits)
{
    qDebug() << "TOTPManager::generateTOTP " ;
    quint64 time = static_cast<quint64>(QDateTime::currentSecsSinceEpoch() / timeStep);

    QByteArray message(8, '\0');
    for (int i = 7; i >= 0; i--) {
        message[i] = time & 0xff;
        time >>= 8;
    }

    QByteArray hash = QMessageAuthenticationCode::hash(message, secret_key, QCryptographicHash::Sha1);

    int offset = hash[hash.size() - 1] & 0x0f;
    int binary = ((hash[offset] & 0x7f) << 24)
              | ((hash[offset + 1] & 0xff) << 16)
              | ((hash[offset + 2] & 0xff) << 8)
              | (hash[offset + 3] & 0xff);

    int totp = binary % static_cast<int>(std::pow(10, returnDigits));
    return totp;
}
