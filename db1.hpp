#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <QObject>
#include <QtSql>
#include "user.hpp"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(const QString& dbName,QObject *parent = nullptr);

     Q_INVOKABLE void authenticateUser(const QString& username, const QString& password);
     Q_INVOKABLE void createUser(const QString &username, const QString &password);
     Q_INVOKABLE void closeDb();
     User* getUser() const;

signals:
     void userChanged();
     void errorUser(const QString error);

public slots:
    void onUserDestroyed();

private:
     bool checkIfDatabaseExists(const QString& dbName);
     void createDatabase();

     void saveEncryptionKeyToJson(const QString& key);
     QString readEncryptionKeyFromJson();
     QString generateRandomKey();

     QString hashPassword(const QString& password);


     QSqlDatabase m_database;
     User* m_user;
     QString m_encryptionKey;

};

#endif // DATABASEMANAGER_HPP
