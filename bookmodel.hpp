#ifndef BOOKMODEL_HPP
#define BOOKMODEL_HPP

#include <QObject>
#include <QAbstractTableModel>
#include "book.hpp"
#include "databasemanager.hpp"
#include "subcontractormodel.hpp"

class BookModel : public QAbstractTableModel
{
    Q_OBJECT

    enum  SortState {
        Ascending = 0,
        Descending = 1,
        NoSort = 3
    };

public:
    explicit BookModel(DatabaseManager* dbManager,QObject *parent = nullptr);

    enum KsiegaRoles {
        IdRole = Qt::UserRole + 1,
        Column,
        Row,
        AccountRole,
        ContractorRole,
        InvoiceRole,
        DateRole,
        AmountRole,
        MonthRole,
        CostRole,
        RevenueRole,
        RoleNameRole
    };

    Q_ENUM(KsiegaRoles)
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
    void setSubcontractorModel(SubcontractorModel* subcontractorModel);
    Book* findBookWithHighestId();
    Q_INVOKABLE void sort(int column, Qt::SortOrder order) override;
    Q_INVOKABLE void changeSorte(const int& column);
    Q_INVOKABLE bool checkIfID (const int& column);
    Q_INVOKABLE void changeColumnWidth(int column);
    Q_INVOKABLE void updateColumnOrder(int oldIndex, int newIndex);  
    Q_INVOKABLE void setMonth(const int& newMonth);

signals:
    void columnWidtChanged(int column);
    void bookDataError(const QString error);

private:
    QList<Book*> m_books;
    QList<Book*> m_filteredBooks;
    QHash<int, int> columnToRoleMap;
    DatabaseManager* m_dbManager;
    SubcontractorModel* m_subcontractorModel;
    QString m_UserEmail;
    int m_company_id;
    int m_month;
    QMap<int, SortState> columnSortState;
};

#endif // BOOKMODEL_HPP
