#include "databasemanager.hpp"
#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

DatabaseManager::DatabaseManager(const QString &dbName, QObject *parent)
    : QObject(parent), m_user(nullptr), m_encryptionKey(readEncryptionKeyFromJson())
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setConnectOptions("QSQLITE_USE_CIPHER=1");
    m_database.setDatabaseName(dbName);
    m_database.setPassword(m_encryptionKey);

    if (!checkIfDatabaseExists(dbName)) {
        qDebug() << "createDatabase.";
        createDatabase();
    }

    if (m_database.open()) {
        qDebug() << "Database opened successfully.";
    } else {
        qDebug() << "Error: " << m_database.lastError().text();
    }

}

void DatabaseManager::authenticateUser(const QString &username, const QString &password)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT id, passwordHash FROM Users WHERE username = ?");
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        QString storedHashedPassword = query.value(1).toString();
        QString inputHashedPassword = hashPassword(password);

        if (storedHashedPassword == inputHashedPassword) {

            if (!m_user) {
                m_user = new User(this);
            }
            m_user->setId(query.value(0).toInt());
            m_user->setUsername(username);

            emit userChanged();
            return ;
        }
    }
    emit errorUser("Error from user in database: " + query.lastError().text());
}

void DatabaseManager::createUser(const QString &username, const QString &password)
{
    QSqlQuery checkQuery(m_database);
    checkQuery.prepare("SELECT * FROM User WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (checkQuery.exec() && checkQuery.next()) {
        emit errorUser("User already exists.");
        return;
    }
    qDebug() << "username:" + username;
    qDebug() << "password:" + password;
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO Users (id, username, passwordHash) VALUES (NULL,:username, :password)");
    query.bindValue(":username", username);
    QString hashedPassword = hashPassword(password);
    query.bindValue(":password", hashedPassword);

    if (!query.exec()) {

        emit errorUser("Error inserting user into database: " + query.lastError().text());
        return;
    }

    emit userChanged();
}

void DatabaseManager::closeDb()
{
    if (m_database.open()) {
        m_database.close();
        qDebug() << "DB closed";
    }
}


User *DatabaseManager::getUser() const
{
    return m_user;
}

void DatabaseManager::onUserDestroyed()
{
    m_user = nullptr;
    emit userChanged();
}

bool DatabaseManager::checkIfDatabaseExists(const QString &dbName)
{
    return QFile::exists(dbName);
}

void DatabaseManager::createDatabase()
{

    if (m_database.open()) {
        qDebug() << "Database opened successfully.";
    } else {
        qDebug() << "Error: " << m_database.lastError().text();
    }

    QSqlQuery query(m_database);

    query.exec("CREATE TABLE IF NOT EXISTS Users ("
               "id INTEGER PRIMARY KEY, "
               "username TEXT, "
               "passwordHash TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS Books ("
               "id INTEGER PRIMARY KEY, "
               "user_id INTEGER, "
               "title TEXT, "
               "author TEXT, "
               "FOREIGN KEY (user_id) REFERENCES Users(id) ON DELETE CASCADE)");

    qDebug() << "Database crated successfully.";
}

void DatabaseManager::saveEncryptionKeyToJson(const QString &key)
{
    QJsonObject json;
    json["encryptionKey"] = key;

    QJsonDocument doc(json);
    QFile file("encryptionKey.json");

    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    } else {
        qInfo() << "Unable to save encryption key to JSON file.";
    }
}

QString DatabaseManager::readEncryptionKeyFromJson()
{
    QFile file("encryptionKey.json");

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject json = doc.object();
        if (json.contains("encryptionKey")) {
            return json["encryptionKey"].toString();
        }
        file.close();
    }

    QString newKey = generateRandomKey();
    saveEncryptionKeyToJson(newKey);

    return newKey;
}

QString DatabaseManager::generateRandomKey()
{
    QString uuidString = QUuid::createUuid().toString();
    QByteArray uuidByteArray = uuidString.toUtf8();
    QByteArray hashed = QCryptographicHash::hash(uuidByteArray, QCryptographicHash::Sha256);
    QString key = QString(hashed.toHex());

    return key;
}

QString DatabaseManager::hashPassword(const QString &password)
{
    QByteArray passwordData = password.toUtf8();
    QByteArray hashed = QCryptographicHash::hash(passwordData, QCryptographicHash::Sha256);
    return QString(hashed.toHex());
}
