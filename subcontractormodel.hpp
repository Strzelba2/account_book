#ifndef SUBCONTRACTORMODEL_HPP
#define SUBCONTRACTORMODEL_HPP

#include <QObject>
#include <QAbstractListModel>
#include "databasemanager.hpp"
#include "subcontractor.hpp"

class SubcontractorModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SubcontractorRoles {
        IdRole = Qt::UserRole + 1,
        ShortNameRole,
        NameRole,
        NipRole,
        ZipRole,
        CityRole,
        StreetRole
    };

    explicit SubcontractorModel(DatabaseManager* dbManager,QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    bool loadInitialData();
    QString getSubcontractorShortNameById(const int& id) const;
    Q_INVOKABLE void addSubcontractor(const QString &shortname, const QString &name, const QString &nip,
                          const QString &zip, const QString &city, const QString &street);
    Q_INVOKABLE void removeSubcontractor(const int& index);
    Q_INVOKABLE QVariantMap get(const int& index) const;

signals:
    void subcontractorDataError(const QString error);
    void subcontractorDataChanged();
    void subcontractorCloseWindow();

private:
    QList<Subcontractor*> m_subcontractor;
    DatabaseManager* m_dbManager;

};

#endif // SUBCONTRACTORMODEL_HPP
