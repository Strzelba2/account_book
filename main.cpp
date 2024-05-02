#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "databasemanager.hpp"
#include "sessionmanager.hpp"
#include "viewservice.hpp"
#include "usbuser.hpp"
#include "totpmanager.hpp"
#include "loginstate.hpp"
#include "bookmodel.hpp"

#include <QQmlContext>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    DatabaseManager dbManager("Rozliczenie.db");
    SessionManager session(&dbManager);


    UsbUser UsbUser(&session);
    app.installNativeEventFilter(&UsbUser);

    ViewService viewService(&session);

    qmlRegisterType<LoginState>("com.enum", 1, 0, "LoginState");
    qmlRegisterType<BookModel>("com.mycompany.bookmodel", 1, 0, "BookModel");

    qmlRegisterType<TOTPManager>("MyComponents", 1, 0, "TOTPManager");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("viewService", &viewService);
    engine.rootContext()->setContextProperty("UsbUser", &UsbUser);

    const QUrl url(u"qrc:/ROZLICZENIE/main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
