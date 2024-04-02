#include "bookmodel.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

BookModel::BookModel(DatabaseManager *dbManager,QObject *parent)
    : QAbstractTableModel(parent),m_dbManager(dbManager)
{
    loadColumnOrder();
    for (int i = 0; i < columnToRoleMap.size(); ++i) {
            columnSortState[i] = SortState::NoSort;
        }
    columnSortState[0] = SortState::Ascending;
}

int BookModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
//    qDebug() << "BookModel::rowCount";
    return m_books.count();
}

int BookModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
//    qDebug() << "BookModel::columnCount";
    return 9;
}

QVariant BookModel::data(const QModelIndex &index, int role) const
{

    qDebug() << "BookModel::data: " << index.row() << "column:" << index.column() << "role:" << role;
    if (!index.isValid() || index.row() < 0 || index.row() >= m_books.size()){
        qDebug() << "ivalid index or row:";
        return QVariant();
    }
    const Book *book = m_books.at(index.row());
    if (!book){
        qDebug() << "ivalid book:";
        return QVariant();
    }
    if (role == Column) {
        qDebug() << "index of column:" << index.column();
        return index.column();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        role = columnToRoleMap.value(index.column(), Qt::UserRole);
    }

    switch (role) {
        case IdRole: return book->id();
        case AccountRole: return book->account();
        case ContractorRole: return book->contractor();
        case InvoiceRole: return book->invoice();
        case DateRole: return book->date().toString("yyyy-MM-dd");
        case AmountRole: return book->amount();
        case CostRole: return book->cost();
    case RevenueRole: return book->revenue();
        case MonthRole: return book->month();
        default: return QVariant();
    }
    return QVariant();
}

bool BookModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "BookModel::setData";
    Book *book = m_books.at(index.row());
    if (!book)
        return false;
    bool changed = false;
    if (index.isValid() && role == Qt::EditRole) {
        role = columnToRoleMap.value(index.column(), Qt::UserRole);
        switch (role) {
            case IdRole:
                if (book->id() != value.toInt()){
                    book->setId(value.toInt());
                    changed = true;}
                break;
            case AccountRole:
                if (book->account() != value.toString()){
                    QString columnName = "account";
                    if (m_dbManager->updateBook(book->id(), columnName, value)){
                        book->setAccount(value.toString());
                        changed = true;
                    }
                }
                break;
            case ContractorRole:
                if (book->contractor() != value.toString()){
                    QString columnName = "contractor";
                    if (m_dbManager->updateBook(book->id(), columnName, value)){
                        book->setContractor(value.toString());
                        changed = true;
                    }
                }
                break;
            case InvoiceRole:
                if (book->invoice() != value.toString()){
                    QString columnName = "invoice";
                    if (m_dbManager->updateBook(book->id(), columnName, value)){
                        book->setInvoice(value.toString());
                        changed = true;
                    }
                }
                break;
            case DateRole:
                if (book->date() != value.toDate()){
                    QString columnName = "date";
                    if (m_dbManager->updateBook(book->id(), columnName, value)){
                        book->setDate(QDate::fromString(value.toString(), "yyyy-MM-dd"));
                        changed = true;
                    }
                }
                break;
            case AmountRole:
                if (book->amount() != value.toDouble()){
                    QString columnName = "amount";
                    if (m_dbManager->updateBook(book->id(), columnName, value)){
                        book->setAmount(value.toDouble());
                        changed = true;
                    }
                }
                break;
            case CostRole:
                if (book->cost() != value.toDouble()){
                    QString columnName = "cost";
                    if (m_dbManager->updateBook(book->id(), columnName, value)){
                        book->setCost(value.toDouble());
                        changed = true;
                    }
                }
                break;
            case RevenueRole:
                if (book->revenue() != value.toDouble()){
                    QString columnName = "revenue";
                    if (m_dbManager->updateBook(book->id(), columnName, value)){
                        book->setRevenue(value.toDouble());
                        changed = true;
                    }
                }
                break;
            case MonthRole:
                if (book->month() != value.toInt()){
                    QString columnName = "month";
                    if (m_dbManager->updateBook(book->id(), columnName, value)){
                        book->setMonth(value.toInt());
                        changed = true;
                    }
                }
                break;
            default: return false;
        }
    }
    if (changed) {
        qDebug() << "before changed";
        Book* latestBook = findBookWithHighestId();
        if ( latestBook->isFullyPopulated()) {
            addNewEmptyBook();
        }
        else
        {
            emit dataChanged(index, index, QVector<int>() << role);
            emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
        }
        return true;
    }
    return false;
}

Qt::ItemFlags BookModel::flags(const QModelIndex &index) const
{
    qDebug() << "BookModel::flags";
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QHash<int, QByteArray> BookModel::roleNames() const
{
    qDebug() << "BookModel::roleNames()";
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[Qt::EditRole] = "edit";
    roles[Column] = "column";
    roles[IdRole] = "id";
    roles[AccountRole] = "account";
    roles[ContractorRole] = "contractor";
    roles[InvoiceRole] = "invoice";
    roles[DateRole] = "date";
    roles[AmountRole] = "amount";
    roles[CostRole] = "cost";
    roles[RevenueRole] = "revenue";
    roles[MonthRole] = "month";
    return roles;
}

bool BookModel::insertRows(int position, int rows, const QModelIndex &index)
{
    qDebug() << "BookModel::insertRows";
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row) {
        m_books.insert(position, new Book(this));
    }
    endInsertRows();
    return true;
}

QVariant BookModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    qDebug() << "BookModel::headerData";
    if (role != Qt::DisplayRole)
            return QVariant();

    if (orientation == Qt::Horizontal) {
        role = columnToRoleMap.value(section, Qt::UserRole);
        switch (role) {
            case IdRole: return QStringLiteral("ID");
            case AccountRole: return QStringLiteral("Account");
            case ContractorRole: return QStringLiteral("Contractor");
            case InvoiceRole: return QStringLiteral("Invoice");
            case DateRole: return QStringLiteral("Date");
            case AmountRole: return QStringLiteral("Amount");
            case CostRole: return QStringLiteral("Cost");
            case RevenueRole: return QStringLiteral("Revenue");
            case MonthRole: return QStringLiteral("Month");
        }
    }
    if(orientation == Qt::Vertical){
        return QVariant::fromValue(section + 1) ;
    }
    return QVariant();
}

bool BookModel::insertRow(int row, const QModelIndex &parent)
{
    qDebug() << "BookModel::insertRow";
    beginInsertRows(parent, row, row);
    m_books.insert(row, new Book(this));
    endInsertRows();
    return true;
}

int BookModel::findIndexById(int id) const
{
    qDebug() << "BookModel::findIndexById";
    for (int i = 0; i < m_books.size(); ++i) {
        if (m_books.at(i)->id() == id) {
            return i;
        }
    }
    return -1;
}

void BookModel::removeRow(int row)
{
    qDebug() << "BookModel::removeRow";
    if (row >= 0 && row < m_books.size())
    {
        beginRemoveRows(QModelIndex(), row, row);
        delete m_books.takeAt(row);
        endRemoveRows();
    }
}

void BookModel::loadColumnOrder()
{
    qDebug() << "BookModel::loadColumnOrder";
    QFile file("columnOrder.json");
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        QJsonObject columnMap = obj["columnToRoleMap"].toObject();
        for (auto key : columnMap.keys()) {
            int column = key.toInt();
            int role = columnMap[key].toInt();
            columnToRoleMap[column] = role;
        }
        file.close();
    } else {
        columnToRoleMap[0] = IdRole;
        columnToRoleMap[1] = AccountRole;
        columnToRoleMap[2] = ContractorRole;
        columnToRoleMap[3] = InvoiceRole;
        columnToRoleMap[4] = DateRole;
        columnToRoleMap[5] = AmountRole;
        columnToRoleMap[6] = CostRole;
        columnToRoleMap[7] = RevenueRole;
        columnToRoleMap[8] = MonthRole;

        saveColumnOrder();

    }
}

void BookModel::saveColumnOrder()
{
    qDebug() << "BookModel::saveColumnOrder";
    QFile file("columnOrder.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject obj;
        QJsonObject columnMap;
        for (auto it = columnToRoleMap.begin(); it != columnToRoleMap.end(); ++it) {
            columnMap[QString::number(it.key())] = it.value();
        }
        obj["columnToRoleMap"] = columnMap;

        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
    }
}

void BookModel::setUserEmail(const QString &userEmail)
{
    qDebug() << "BookModel::setUserEmail";
    if (m_UserEmail == userEmail)
        return;
    m_UserEmail = userEmail;
}

void BookModel::changeColumnWidth(int column)
{
    qDebug() << "BookModel::changeColumnWidth: "  << column;
    emit columnWidtChanged(column);
}

void BookModel::updateColumnOrder(int oldIndex, int newIndex)
{
    qDebug() << "BookModel::updateColumnOrder " ;
    if (oldIndex != newIndex && columnToRoleMap.contains(oldIndex) && columnToRoleMap.contains(newIndex)) {
        int oldRole = columnToRoleMap[oldIndex];
        int newRole = columnToRoleMap[newIndex];

        columnToRoleMap[oldIndex] = newRole;
        columnToRoleMap[newIndex] = oldRole;

        saveColumnOrder();

        emit layoutChanged();
    }
}

bool BookModel::loadInitialData()
{
    qDebug() << "BookModel::loadInitialData " ;
    if (!m_dbManager) {
        qWarning() << "DatabaseManager not set!";
        return false;
    }
    m_company_id = m_dbManager->getCompanyIdForUser(m_UserEmail);

    return m_dbManager->fetchAllBooks(m_books ,m_company_id);
}

void BookModel::addNewEmptyBook()
{
    qDebug() << "BookModel::addNewEmptyBook " ;
    int newId = m_dbManager->addEmptyBook(m_company_id);
    if (newId == -1) {
        qDebug() << "Error creating a new empty book.";
        return;
    }
    Book* newBook = new Book(this);
    newBook->setId(newId);
    newBook->setCompanyId(m_company_id);
    newBook->setDate(QDate());

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_books.append(newBook); // Załóżmy, że m_data to lista przechowująca twoje książki
    endInsertRows();
}

Book *BookModel::findBookWithHighestId()
{
    qDebug() << "BookModel::findBookWithHighestId " ;
    if (m_books.isEmpty()) {
        return nullptr;
    }
    Book* bookWithHighestId = m_books.first();
    foreach (Book* book, m_books) {
        if (book->id() > bookWithHighestId->id()) {
            bookWithHighestId = book;
        }
    }

    return bookWithHighestId;
}

void BookModel::sort(int column, Qt::SortOrder order)
{
    qDebug() << "BookModel::sort " ;
    SortState currentState = columnSortState.value(column, SortState::NoSort);
    if (currentState == SortState::Ascending){
        order = Qt::DescendingOrder;
        columnSortState[column] = SortState::Descending;
    }else{
        columnSortState[column] = SortState::Ascending;
    }

    beginResetModel();
    int role = columnToRoleMap.value(column, Qt::UserRole);
    auto comparator = [ order , role](const Book *a, const Book *b) -> bool {
        switch (role) {
            case IdRole:
                return order == Qt::AscendingOrder ? a->id() < b->id() : a->id() > b->id();
            case AccountRole:
                return order == Qt::AscendingOrder ? a->account() < b->account() : a->account() > b->account();
            case ContractorRole:
                return order == Qt::AscendingOrder ? a->contractor() < b->contractor() : a->contractor() > b->contractor();
            case InvoiceRole:
                return order == Qt::AscendingOrder ? a->invoice() < b->invoice() : a->invoice() > b->invoice();
            case DateRole:
                return order == Qt::AscendingOrder ? a->date() < b->date() : a->date() > b->date();
            case AmountRole:
                return order == Qt::AscendingOrder ? a->amount() < b->amount() : a->amount() > b->amount();
            case CostRole:
                return order == Qt::AscendingOrder ? a->cost() < b->cost() : a->cost() > b->cost();
            case RevenueRole:
                return order == Qt::AscendingOrder ? a->revenue() < b->revenue() : a->revenue() > b->revenue();
            case MonthRole:
                return order == Qt::AscendingOrder ? a->month() < b->month() : a->month() > b->month();
            default:
                return false;
        }
    };

    std::sort(m_books.begin(), m_books.end(), comparator);

    endResetModel();

    for(auto &key : columnSortState.keys()) {
        if(key != column) {
            columnSortState[key] = SortState::NoSort;
        }
    }
}
