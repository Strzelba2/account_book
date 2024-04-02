#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <QObject>
#include <QtSql>
#include "book.hpp"


class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(const QString& dbName,QObject *parent = nullptr);
    ~DatabaseManager();

    bool loginAsAdmin(const QString& adminUser, const QString& adminPass);
    bool loginAsUser(const QString& User, const QString& Pass, const QString& company);
    bool closeDb();
    bool openDb();
    QString getLastDatabaseError() const;
    bool CreateUserDb(const QString& username, const QString& password,
                      const QString& email, const QString& company);
    bool userExists(const QString &username);
    bool updateActiveStatus(const QString& username,const bool& status);
    bool saveSecretKey(const QString &username, const QString &secretKey);
    bool validatePassword(const QString& username, const QString& password);
    QVariantMap getUserData(const QString& username);
    QString getSecretKey(const QString &username);
    bool fetchAllBooks(QList<Book*>& books ,int companyId);
    int getCompanyIdForUser(const QString& userEmail);
    int addEmptyBook(int companyId);
    bool grantFullAccessToUser(const QString& username, const QString& databaseName);
    bool updateBook(int id, const QString& columnName, const QVariant& value);

signals:

public slots:


private:
    QString hashPassword(const QString& password);
    QString getHashedPassword(const QString &username);
    void handleError(const QSqlQuery &query);
    void handleError(const QSqlDatabase &db);

    bool createTables();

    QSqlDatabase m_database;
    QString m_encryptionKey;
    QString dbError;
};

#endif // DATABASEMANAGER_HPP
