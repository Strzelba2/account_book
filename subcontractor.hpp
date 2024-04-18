#ifndef SUBCONTRACTOR_HPP
#define SUBCONTRACTOR_HPP

#include <QObject>

class Subcontractor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idSubcontractorChanged)
    Q_PROPERTY(QString shortname READ shortname WRITE setShortname NOTIFY shortnameSubcontractorChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameSubcontractorChanged)
    Q_PROPERTY(QString nip READ nip WRITE setNip NOTIFY nipSubcontractorChanged)
    Q_PROPERTY(QString zip READ zip WRITE setZip NOTIFY zipSubcontractorChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY citySubcontractorChanged)
    Q_PROPERTY(QString street READ street WRITE setStreet NOTIFY streetSubcontractorChanged)
    int m_id;

    QString m_shortname;

    QString m_name;

    QString m_nip;

    QString m_zip;

    QString m_city;

    QString m_street;

public:
    explicit Subcontractor(QObject *parent = nullptr);

    int id() const;
    void setId(int newId);

    const QString &shortname() const;
    void setShortname(const QString &newShortname);

    const QString &name() const;
    void setName(const QString &newName);

    const QString &nip() const;
    void setNip(const QString &newNip);

    const QString &zip() const;
    void setZip(const QString &newZip);

    const QString &city() const;
    void setCity(const QString &newCity);

    const QString &street() const;
    void setStreet(const QString &newStreet);

signals:

    void idSubcontractorChanged();
    void shortnameSubcontractorChanged();
    void nameSubcontractorChanged();
    void nipSubcontractorChanged();
    void zipSubcontractorChanged();
    void citySubcontractorChanged();
    void streetSubcontractorChanged();
};

#endif // SUBCONTRACTOR_HPP
