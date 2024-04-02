#include "loginstate.hpp"
#include <QDebug>

LoginState::LoginState(QObject *parent)
    : QObject(parent),m_isRegister(false),m_currentState(LoginAdmin)
{

}

LoginState::State LoginState::currentState() const
{
    qDebug() << "LoginState::currentState";
    return m_currentState;
}

void LoginState::setCurrentState(State newState)
{
    qDebug() << "LoginState::setCurrentState";
    if (m_currentState != newState) {
        m_currentState = newState;
    }
}

bool LoginState::isRegister() const
{
    qDebug() << "LoginState::isRegister()";
    return m_isRegister;
}

void LoginState::setIsRegister(bool isRegister)
{
    qDebug() << "LoginState::setIsRegister";
    if (m_isRegister == isRegister)
        return;
    m_isRegister = isRegister;
}
