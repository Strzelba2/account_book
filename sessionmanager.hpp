#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include <QObject>
#include "user.hpp"
#include "databasemanager.hpp"
#include "bookmodel.hpp"
#include "totpmanager.hpp"
#include "loginstate.hpp"

class User;

class SessionManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TOTPManager* totpManager READ getTOTPManager CONSTANT)
    Q_PROPERTY(User* user READ getUser CONSTANT)
    Q_PROPERTY(LoginState* loginState READ getLoginState CONSTANT)
    Q_PROPERTY(BookModel* bookModel READ bookModel WRITE setBookModel NOTIFY bookModelChanged)

public:
    explicit SessionManager(DatabaseManager* dbManager,QObject *parent = nullptr);

    void loginAdmin(const QString& username, const QString& password);
    void checkUserAndCreate(const QString& user, const QString& password,
                            const QString& email, const QString& company);
    bool updateUser(const QString& user);
    void dbClose();
    void generateQrCode();
    void saveSecretKey();

    void verifyTOTP(int userToken);

    TOTPManager *getTOTPManager() const;

    bool isAuthorizedToAccessKey() const;
    QByteArray requestSecretKey()const;
    void receiveKeyAndSalt(const std::pair<std::vector<unsigned char>, std::vector<unsigned char>>& keyAndSalt);

    void loginUser(const QString& username, const QString& password);
    void firstLoginUser(const QString& username, const QString& password);
    User *getUser() const;

    LoginState *getLoginState() const;

    BookModel *bookModel() const;
    void setBookModel(BookModel *newBookModel);

signals:
    void loginAdminFinished(bool success);
    void loginUserFinished(bool success);
    void dbClosed(bool success);
    void sessionError(const QString error);
    void userCreated(bool success);
    void secredSaved(bool success);
    void totpAuthorization();
    void veryficationStatus(const QString message);

    void bookModelChanged();

private:

    User* m_user;
    DatabaseManager* m_dbManager;
    BookModel* m_bookModel;
    TOTPManager* m_totpManager;
    LoginState* m_loginState;

    std::vector<unsigned char> m_key;
    std::vector<unsigned char> m_salt;
};

#endif // SESSIONMANAGER_HPP
