#include "databasemanager.hpp"
#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>


DatabaseManager::DatabaseManager(const QString &dbName, QObject *parent)
    : QObject(parent)
{
    m_database = QSqlDatabase::addDatabase("QPSQL","Rozliczenie");
    m_database.setHostName("localhost");
    m_database.setPort(5432);
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.open())
    {
        qDebug() << "DatabaseManager::~DatabaseManager() database opened";
    }
    m_database.close();
    QSqlDatabase::removeDatabase(m_database.connectionName());
}


bool DatabaseManager::loginAsAdmin(const QString& adminUser, const QString& adminPass) {

    qDebug() << "DatabaseManager::loginAsAdmin";
    if (m_database.open())
    {
       m_database.close();
    }

    m_database.setUserName(adminUser);
    m_database.setPassword(adminPass);

    if (m_database.open())
    {
        qDebug() << "Database opened successfully.";
        return m_database.open();
    }

    qDebug() << "Error: " << m_database.lastError().text();
    handleError(m_database);
    return false;
}

bool DatabaseManager::loginAsUser(const QString &User, const QString &Pass, const QString& company)
{
    qDebug() << "DatabaseManager::loginAsUser";
    m_database.setUserName(User);
    m_database.setPassword(Pass);
    m_database.setDatabaseName(company);

    if (m_database.open())
    {
        qDebug() << "Database opened successfully.";
        return m_database.open();
    }

    qDebug() << "Error: " << m_database.lastError().text();
    handleError(m_database);
    return false;
}
bool DatabaseManager::closeDb()
{
    qDebug() << "DatabaseManager::closeDb";
    m_database.setUserName("");
    m_database.setPassword("");
    m_database.close();
    if (!m_database.isOpen())
    {
        return true;
    }
    return false;

}

bool DatabaseManager::openDb()
{
    qDebug() << "DatabaseManager::openDb";
    if (!m_database.isOpen())
    {
        return m_database.open();
    }
    return true;
}

QString DatabaseManager::getLastDatabaseError() const
{
    qDebug() << "DatabaseManager::getLastDatabaseError";
    return dbError;
}

bool DatabaseManager::CreateUserDb(const QString &username, const QString &password, const QString &email, const QString &company)
{
    qDebug() << "DatabaseManager::CreateUserDb";
    if (!m_database.open())
    {
        qDebug() << "Nie udało się połączyć jako admin:" << m_database.lastError().text();
        handleError(m_database);
        return false;
    }
    QString hashedPassword = hashPassword(password);
    QString createUser = QString("CREATE USER %1 WITH ENCRYPTED PASSWORD '%2'").arg(username, password);

    QSqlQuery queryUser = m_database.exec(createUser);

    if (queryUser.isActive())
    {
        qDebug() << "User created successfully.";
        QString createCompany = QString("CREATE DATABASE "+company);
        QSqlQuery queryCompany = m_database.exec(createCompany);
        if (queryCompany.isActive())
        {
            QString createAccess = QString("GRANT CONNECT ON DATABASE %1 TO %2").arg(company,username);
            QSqlQuery queryAccess = m_database.exec(createAccess);
            if (!queryAccess.isActive())
            {
                qDebug() << "DataBase was not crated succefully."<< m_database.lastError().text();
                handleError(m_database);
                m_database.close();
                return false;
            }
        }
        else
        {
            qDebug() << "DataBase was not crated succefully."<< m_database.lastError().text();
            handleError(m_database);
            m_database.close();
            return false;
        }
    }
    else
    {
        qDebug() << "DataBase was not crated succefully. "<< m_database.lastError().text();
        handleError(m_database);
        m_database.close();
        return false;
    }
    m_database.close();
    m_database.setDatabaseName(company.toLower());
    if (!m_database.open())
    {
        qDebug() << "can't open database again" << m_database.lastError().text();
        handleError(m_database);
        return false;
    }
    if (!createTables()) {
        qDebug() << "table can not be created";
        m_database.close();
        return false;
    }

    QSqlQuery Companyquery(m_database);
    Companyquery.prepare("INSERT INTO Company (name) VALUES (:name)");
    Companyquery.bindValue(":name", company);

    if (Companyquery.exec()) {
        QVariant lastId = Companyquery.lastInsertId();
        QSqlQuery UserQuery(m_database);
        qDebug() << "UserQuery";
        UserQuery.prepare("INSERT INTO Users (name, hashpassword, email, company_id ) VALUES (:username, :hashpassword, :email, :company_id)");
        UserQuery.bindValue(":username", username);
        UserQuery.bindValue(":hashpassword", hashedPassword);
        UserQuery.bindValue(":email", email);
        UserQuery.bindValue(":company_id", lastId);

        if (!UserQuery.exec()) {
            qDebug() << "Error During create User";
            handleError(UserQuery);
            m_database.close();
            return false;
        }
    }
    else
    {
        qDebug() << "Error During create Company";
        handleError(Companyquery);
        m_database.close();
        return false;
    }

    m_database.close();
    return true;
}

QString DatabaseManager::hashPassword(const QString &password)
{
    qDebug() << "DatabaseManager::hashPassword";
    QByteArray passwordData = password.toUtf8();
    QByteArray hashed = QCryptographicHash::hash(passwordData, QCryptographicHash::Sha256);
    return QString(hashed.toHex());
}

QString DatabaseManager::getHashedPassword(const QString &username)
{
    qDebug() << "DatabaseManager::getHashedPassword";
    QSqlQuery query(m_database);
    query.prepare("SELECT hashpassword FROM Users WHERE name = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
       qDebug() << "Error getting hashpassword from the database: " << query.lastError();
       handleError(query);
       return QString();
    }
    if (query.next()) {
        return query.value(0).toString();
    }

    return QString();
}

void DatabaseManager::handleError(const QSqlQuery &query)
{
    qDebug() << "DatabaseManager::handleError";
    QSqlError error = query.lastError();
    if (error.isValid()) {
        dbError = error.text();
    }
}

void DatabaseManager::handleError(const QSqlDatabase &db)
{
    qDebug() << "DatabaseManager::handleError";
    QSqlError error = db.lastError();
    if (error.isValid()) {
        dbError = error.text();
    }
}

void DatabaseManager::handleError(const QString &message)
{
    qDebug() << "DatabaseManager::handleError";
    if(!message.isEmpty()){
        dbError = message;
    }
}

bool DatabaseManager::createTables()
{
    qDebug() << "DatabaseManager::createTables";
    QSqlQuery queryCompany = m_database.exec("CREATE TABLE Company (id SERIAL PRIMARY KEY, name VARCHAR(100))");

    if (!queryCompany.isActive())
    {
        qDebug() << "table Company can not be created";
        handleError(m_database);
        return false;
    }
    QSqlQuery queryUser = m_database.exec("CREATE TABLE IF NOT EXISTS Users ("
                                          "id SERIAL PRIMARY KEY, "
                                          "name VARCHAR(255) NOT NULL, "
                                          "hashpassword VARCHAR(255) NOT NULL, "
                                          "encrypted_secret_key VARCHAR(255), "
                                          "email VARCHAR(255) NOT NULL, "
                                          "company_id INTEGER, "
                                          "FOREIGN KEY (company_id) REFERENCES Company(id) ON DELETE CASCADE, "
                                          "is_active BOOLEAN DEFAULT false)");
    if (!queryUser.isActive())
    {
        qDebug() << "table Users can not be created";
        handleError(m_database);
        return false;
    }
    QSqlQuery querySubcontractors = m_database.exec("CREATE TABLE IF NOT EXISTS Subcontractors ("
                                          "id SERIAL PRIMARY KEY, "
                                          "shortname VARCHAR(255) NOT NULL, "
                                          "name VARCHAR(255) NOT NULL, "
                                          "NIP VARCHAR(255), "
                                          "zip VARCHAR(255) NOT NULL, "
                                          "city VARCHAR(255) NOT NULL, "
                                          "street VARCHAR(255) NOT NULL)");
    if (!querySubcontractors.isActive())
    {
        qDebug() << "table Subcontractors can not be created";
        handleError(m_database);
        return false;
    }
    QSqlQuery queryBook = m_database.exec("CREATE TABLE IF NOT EXISTS Book ("
                                          "id SERIAL PRIMARY KEY,"
                                          "company_id INTEGER, "
                                          "FOREIGN KEY (company_id) REFERENCES Company(id) ON DELETE CASCADE, "
                                          "subcontractors_id INTEGER, "
                                          "FOREIGN KEY (subcontractors_id) REFERENCES Subcontractors(id) ON DELETE RESTRICT, "
                                          "account VARCHAR(255),"
                                          "contractor VARCHAR(255),"
                                          "invoice VARCHAR(255),"
                                          "date DATE,"
                                          "amount DECIMAL(10, 2),"
                                          "cost DECIMAL(10, 2),"
                                          "revenue DECIMAL(10, 2),"
                                          "month INTEGER)");

    if (!queryBook.isActive())
    {
        qDebug() << "table Book can not be created" ;
        handleError(m_database);
        return false;
    }
    QSqlQuery queryAssets = m_database.exec("CREATE TABLE IF NOT EXISTS Assets ("
                                          "id SERIAL PRIMARY KEY,"
                                          "company_id INTEGER, "
                                          "FOREIGN KEY (company_id) REFERENCES Company(id) ON DELETE CASCADE, "
                                          "name VARCHAR(255),"
                                          "classification VARCHAR(255),"
                                          "comment VARCHAR(255),"
                                          "date DATE,"
                                          "amount DECIMAL(10, 2),"
                                          "period INTEGER,"
                                          "monthly_amount DECIMAL(10, 2),"
                                          "remaining_amount DECIMAL(10, 2))");

    if (!queryAssets.isActive())
    {
        qDebug() << "table Book can not be created" ;
        handleError(m_database);
        return false;
    }
    QSqlQuery queryAssetsToBooks = m_database.exec("CREATE TABLE IF NOT EXISTS AssetsToBooks ("
                                          "id SERIAL PRIMARY KEY,"
                                          "assets_id INTEGER, "
                                          "FOREIGN KEY (assets_id) REFERENCES Assets(id), "
                                          "book_id INTEGER, "
                                          "FOREIGN KEY (book_id) REFERENCES Book(id))");

    if (!queryAssetsToBooks.isActive())
    {
        qDebug() << "table AssetsToBooks can not be created" ;
        handleError(m_database);
        return false;
    }

    return true;
}



bool DatabaseManager::userExists(const QString &username)
{
    qDebug() << "DatabaseManager::userExists";
    QString CheckUser = QString("SELECT 1 FROM pg_roles WHERE rolname = '%1'").arg(username);
    QSqlQuery queryCheckUser = m_database.exec(CheckUser);

    if (queryCheckUser.next()) {
        qDebug() << "user exist: ";
        return true;
    }

    qDebug() << "user do not exist: ";
    handleError(m_database);
    return false;
}

bool DatabaseManager::updateActiveStatus(const QString &username,const bool& status)
{
    qDebug() << "DatabaseManager::updateActiveStatus";
    QSqlQuery query(m_database);
    query.prepare("SELECT is_active FROM Users WHERE name = :username");
    query.bindValue(":username", username);

    if (query.exec()) {
        if (query.next()) {
            bool isActive = query.value(0).toBool();
            if (!isActive) {
                QSqlQuery updateQuery(m_database);
                updateQuery.prepare("UPDATE Users SET is_active = :isActive WHERE name = :username");
                updateQuery.bindValue(":username", username);
                updateQuery.bindValue(":isActive", status);

                return updateQuery.exec();
            }
        }
    }
    return false;
}

bool DatabaseManager::saveSecretKey(const QString &username, const QString &secretKey)
{
    qDebug() << "DatabaseManager::saveSecretKey";
    QSqlQuery query(m_database);
    query.prepare("UPDATE Users SET encrypted_secret_key = :key WHERE name = :name");
    query.bindValue(":key", secretKey);
    query.bindValue(":name", username);

    if(!query.exec())
    {
        handleError(query);
    }

    return query.exec();
}

bool DatabaseManager::validatePassword(const QString &username, const QString &password)
{
    qDebug() << "DatabaseManager::validatePassword";
    QString hashedPassword = hashPassword(password);
    QString dbHashedPassword = getHashedPassword(username);

    if (dbHashedPassword.isEmpty())
    {
        return false ;
    }

    if( hashedPassword != dbHashedPassword)
    {
        return false ;
    }
    return true;
}

QVariantMap DatabaseManager::getUserData(const QString &username)
{
    qDebug() << "DatabaseManager::getUserData";
    QVariantMap userData;
    if (!m_database.open())
    {
        qDebug() << "can not open database" << m_database.lastError().text();
        userData["error"] = m_database.lastError().text();
        return userData;
    }
    QSqlQuery query(m_database);
    query.prepare("SELECT Users.id, Users.email, Users.name, Company.name AS company_name "
                  "FROM Users "
                  "JOIN Company ON Users.company_id = Company.id "
                  "WHERE Users.name = :username");
    query.bindValue(":username", username);
;
    query.bindValue(":username", username);
    if (!query.exec()) {
        userData["error"] = query.lastError().text();
        return userData;
    }

    if (query.next()) {
        userData["id"] = query.value(0);
        userData["email"] = query.value(1);
        userData["name"] = query.value(2);
        userData["company"] = query.value(3);
    } else {
        userData["error"] = "User do not exist";
    }
    return userData;
}


QString DatabaseManager::getSecretKey(const QString &username)
{
    qDebug() << "DatabaseManager::getSecretKey";
    QSqlQuery query(m_database);
    query.prepare("SELECT encrypted_secret_key FROM Users WHERE name = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error getting secret key from the database: " << query.lastError();
        handleError(query);
        return QString();
    }

    if (query.next()) {
        return query.value(0).toString();
    }

    return QString();
}

bool DatabaseManager::fetchAllBooks(QList<Book *> &books, const int& companyId)
{
    qDebug() << "DatabaseManager::fetchAllBooks";
    QSqlQuery query(m_database);
    query.prepare("SELECT id, account, contractor, invoice, date, amount, cost, revenue, month FROM Book WHERE company_id = :companyId");
    query.bindValue(":companyId", companyId);

    if (!query.exec()) {
        handleError(query);
        return false;
    }

    while (query.next()) {
        Book* book = new Book(this);
        book->setId(query.value(0).toInt());
        book->setAccount(query.value(1).toString());
        book->setContractor(query.value(2).toString());
        book->setInvoice(query.value(3).toString());

        QDate date = QDate::fromString(query.value(4).toString(), "yyyy-MM-dd");
        book->setDate(date);
        book->setAmount(query.value(5).toDouble());
        book->setCost(query.value(6).toDouble());
        book->setRevenue(query.value(7).toDouble());
        book->setMonth(query.value(8).toInt());

        books.append(book);
    }

    return !books.isEmpty();

}

bool DatabaseManager::fetchAllSubcontractor(QList<Subcontractor*> &subconstractors)
{
    qDebug() << "DatabaseManager::fetchAllSubcontractor";
    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM Subcontractors");

    if (!query.exec()) {
        handleError(query);
        return false;
    }

    while (query.next()) {
        Subcontractor* subcontractor = new Subcontractor(this);
        subcontractor->setId(query.value(0).toInt());
        subcontractor->setShortname(query.value(1).toString());
        subcontractor->setName(query.value(2).toString());
        subcontractor->setNip(query.value(3).toString());
        subcontractor->setZip(query.value(4).toString());
        subcontractor->setCity(query.value(5).toString());
        subcontractor->setStreet(query.value(6).toString());

        subconstractors.append(subcontractor);
    }

    return true;
}

int DatabaseManager::getCompanyIdForUser(const QString &userEmail)
{
    qDebug() << "DatabaseManager::getCompanyIdForUser";
    QSqlQuery query(m_database);
    query.prepare("SELECT company_id FROM Users WHERE email = :email");
    query.bindValue(":email", userEmail);

    if (!query.exec()) {
        handleError(query);
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "user doesn't exist";
        return -1;
    }
}

int DatabaseManager::addEmptyBook(const int& companyId)
{
    qDebug() << "DatabaseManager::addEmptyBook";
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO Book (company_id, account, contractor, invoice, date, amount, cost, revenue, month) VALUES (:company_id, '', '', '', NULL, 0, 0, 0, 0) RETURNING id;");
    query.bindValue(":company_id", companyId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        handleError(query);
        return -1;
    }

    if (query.next()) {
        qDebug() << "next";
        return query.value(0).toInt();
    }

    return -1;
}

int DatabaseManager::addSubcontractor(const QString &shortname, const QString &name, const QString &nip,
                                       const QString &zip, const QString &city, const QString &street)
{
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO Subcontractors (shortname, name, nip, zip, city, street) VALUES (:shortname, :name, :nip, :zip, :city, :street) RETURNING id;");
    query.bindValue(":shortname", shortname);
    query.bindValue(":name", name);
    query.bindValue(":nip", nip);
    query.bindValue(":zip", zip);
    query.bindValue(":city", city);
    query.bindValue(":street", street);

    if (!query.exec()) {
        handleError(query);
        qDebug() << "Error adding new Subcontractors:" << query.lastError().text();
        return -1;
    }
    if (query.next()) {
        qDebug() << "next";
        return query.value(0).toInt();
    }
    return -1;
}

bool DatabaseManager::grantFullAccessToUser(const QString &username, const QString &databaseName)
{
    qDebug() << "DatabaseManager::grantFullAccessToUser";
    QString getAccess = QString("GRANT ALL PRIVILEGES ON DATABASE %1 TO %2").arg(databaseName.toLower(),username);
    QSqlQuery queryGetAccess = m_database.exec(getAccess);
    if(!queryGetAccess.isActive())
    {
        handleError(m_database);
        return false;
    }

    QString getAccessTable = QString("GRANT INSERT, UPDATE, SELECT, DELETE ON ALL TABLES IN SCHEMA public TO  %1 ").arg(username);
    QSqlQuery queryGetAccessTable = m_database.exec(getAccessTable);
    if(!queryGetAccessTable.isActive())
    {
        handleError(m_database);
        return false;
    }
    QString getAccessBookId = QString("GRANT USAGE, SELECT, UPDATE ON SEQUENCE book_id_seq TO  %1 ").arg(username);
    QSqlQuery queryGetAccessBookId = m_database.exec(getAccessBookId);
    if(!queryGetAccessBookId.isActive())
    {
        handleError(m_database);
        return false;
    }

    QString getAccessSubcontractorId = QString("GRANT USAGE, SELECT, UPDATE ON SEQUENCE subcontractors_id_seq TO  %1 ").arg(username);
    QSqlQuery queryGetAccessSubcontractorId = m_database.exec(getAccessSubcontractorId);
    if(!queryGetAccessSubcontractorId.isActive())
    {
        handleError(m_database);
        return false;
    }
    return true;
}

bool DatabaseManager::updateBook(const int& id, const QString &columnName, const QVariant &value)
{
    qDebug() << "DatabaseManager::updateBook";

    if (!bookColumnNames.contains(columnName)) {
        handleError("column does not exist");
        return false;
    }
    QSqlQuery query(m_database);
    query.prepare(QString("UPDATE Book SET %1 = :value WHERE id = :id").arg(columnName));
    if((columnName == bookColumnNames.at(0) || columnName == bookColumnNames.at(1) ||
        columnName == bookColumnNames.at(2)) && !value.canConvert<QString>()){
        handleError(QString("Incorrect %1 value .%1 must be a string").arg(columnName));
        return false;
    }
    else if(columnName == bookColumnNames.at(1) && !value.canConvert<QString>()){
        handleError("Incorrect Account value .Account must be a string");
        return false;
    }
    else if (columnName == bookColumnNames.at(3) && !validateDate(value.toString())) {
        handleError("Incorrect date value .The date must have the format yyyy-MM-dd");
        return false;
    }
    else if ((columnName == bookColumnNames.at(4) || columnName == bookColumnNames.at(5) ||
             columnName == bookColumnNames.at(6)) && !value.canConvert<double>()){
         handleError(QString("Incorrect %1 value .%1 must be a double").arg(columnName));
         return false;
     }
    else if (columnName == bookColumnNames.at(7)  && !value.canConvert<int>()){
         handleError(QString("Incorrect %1 value .%1 must be a int").arg(columnName));
         return false;
     }

    switch (value.typeId()) {
        case QMetaType::QDate:
            query.bindValue(":value", value.toDate().toString(Qt::ISODate));
            break;
        case QMetaType::Double:
            query.bindValue(":value", value.toDouble());
            break;
        case QMetaType::Int:
            query.bindValue(":value", value.toInt());
            break;
        case QMetaType::QString:
            query.bindValue(":value", value.toString());
            break;
        default:
            qDebug() << "Niezdefiniowany typ dla kolumny";
            return false;
    }
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "can not update book:" << query.lastError();
        handleError(query);
        return false;
    }

    return true;
}

bool DatabaseManager::updateSubcontractor(const int &id,const QString& columnName,  const QVariant &value)
{
    QSqlQuery query(m_database);
    query.prepare(QString("UPDATE Subcontractors SET %1 = :value WHERE id = :id").arg(columnName));
    query.bindValue(":value", value.toString());
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "can not update Subcontractor:" << query.lastError();
        handleError(query);
        return false;
    }
    return true;
}

bool DatabaseManager::deleteSubcontractor(const int &id)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM Subcontractors WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Error deleting subcontractor from database:" << query.lastError().text();
        handleError(query);
        return false;
    }
    return true;
}

bool DatabaseManager::validateDate(const QString &dateString)
{
    qDebug() << "DatabaseManager::validateDate";
    QDate date = QDate::fromString(dateString, "yyyy-MM-dd");
    if (date.isValid()) {
        qDebug() << "Date is correct:" ;
        return true;
    } else {
        qDebug() << "Date is not correct";
        return false;
    }
}

bool DatabaseManager::isNipUnique(const QString &nip)
{
    qDebug() << "DatabaseManager::isNipUnique";
    QSqlQuery query(m_database);

    query.prepare("SELECT COUNT(*) FROM Subcontractors WHERE nip = :nip");
    query.bindValue(":nip", nip);
    if (!query.exec()) {
        handleError(query);
        return false;
    }
    if (query.next()) {
        return query.value(0).toInt() == 0;
    }
    return false;
}






