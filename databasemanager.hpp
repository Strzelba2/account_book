#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <QObject>
#include <QtSql>
#include "book.hpp"
#include "subcontractor.hpp"

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
    bool fetchAllBooks(QList<Book*>& books ,const int& companyId);
    bool fetchAllSubcontractor(QList<Subcontractor*>& subconstractors);
    int getCompanyIdForUser(const QString& userEmail);
    int addEmptyBook(const int& companyId);
    int addSubcontractor(const QString &shortname, const QString &name, const QString &nip,
                          const QString &zip, const QString &city, const QString &street);
    bool grantFullAccessToUser(const QString& username, const QString& databaseName);
    bool updateBook(const int& id, const QString& columnName, const QVariant& value);
    bool updateSubcontractor(const int& id,const QString& columnName,  const QVariant& value);
    bool deleteSubcontractor (const int& id);
    bool validateDate(const QString& dateString);
    bool isNipUnique(const QString &nip);

signals:

public slots:


private:
    QString hashPassword(const QString& password);
    QString getHashedPassword(const QString &username);
    void handleError(const QSqlQuery &query);
    void handleError(const QSqlDatabase &db);
    void handleError(const QString& message);
    const QStringList bookColumnNames = {"account", "contractor", "invoice", "date","amount","cost", "revenue","month"};

    bool createTables();

    QSqlDatabase m_database;
    QString m_encryptionKey;
    QString dbError;
};

#endif // DATABASEMANAGER_HPP
