#ifndef LOGINSTATE_HPP
#define LOGINSTATE_HPP

#include <QObject>

class LoginState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State currentState READ currentState CONSTANT)
    Q_PROPERTY(bool isRegister READ isRegister CONSTANT)

public:
    explicit LoginState(QObject *parent = nullptr);

    enum State {
        LoginAdmin,
        Register,
        PIPUSB,
        QRCode,
        Login
    };
    Q_ENUM(State)

    State currentState() const;
    void setCurrentState(State newState);

    bool isRegister () const;
    void setIsRegister (bool isRegister);

private:

    State m_currentState;

    bool m_isRegister;

};

#endif // LOGINSTATE_HPP
