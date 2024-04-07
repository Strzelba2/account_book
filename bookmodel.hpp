#ifndef BOOKMODEL_HPP
#define BOOKMODEL_HPP

#include <QObject>
#include <QAbstractTableModel>
#include "book.hpp"
#include "databasemanager.hpp"

class BookModel : public QAbstractTableModel
{
    Q_OBJECT

    enum KsiegaRoles {
        IdRole = Qt::UserRole + 1,
        Column,
        AccountRole,
        ContractorRole,
        InvoiceRole,
        DateRole,
        AmountRole,
        MonthRole,
        CostRole,
        RevenueRole
    };

    enum  SortState {
        Ascending = 0,
        Descending = 1,
        NoSort = 3
    };

public:
    explicit BookModel(DatabaseManager* dbManager,QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool insertRows(int position, int rows, const QModelIndex &index)override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool insertRow(int row, const QModelIndex &parent = QModelIndex()) ;
    int findIndexById(int id) const;
    void removeRow(int row);
    void loadColumnOrder();
    void saveColumnOrder();
    void setUserEmail(const QString& userEmail);
    bool loadInitialData();
    void addNewEmptyBook();
    Book* findBookWithHighestId();
    Q_INVOKABLE void sort(int column, Qt::SortOrder order) override;
    Q_INVOKABLE bool checkIfID (const int& column);
    Q_INVOKABLE void changeColumnWidth(int column);
    Q_INVOKABLE void updateColumnOrder(int oldIndex, int newIndex);

signals:
    void columnWidtChanged(int column);
    void bookDataError(const QString error);

private:
    QList<Book*> m_books;
    QHash<int, int> columnToRoleMap;
    DatabaseManager* m_dbManager;
    QString m_UserEmail;
    int m_company_id;
    QMap<int, SortState> columnSortState;
};

#endif // BOOKMODEL_HPP
