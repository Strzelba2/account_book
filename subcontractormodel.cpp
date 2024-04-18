#include "subcontractormodel.hpp"


SubcontractorModel::SubcontractorModel(DatabaseManager *dbManager, QObject *parent)
    :QAbstractListModel(parent), m_dbManager(dbManager)
{

}

int SubcontractorModel::rowCount(const QModelIndex &parent) const
{
    return m_subcontractor.count();
}

QVariant SubcontractorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_subcontractor.size()){
        qDebug() << "ivalid index or row:";
        return QVariant();
    }
    const Subcontractor* subcontractor = m_subcontractor.at(index.row());
    if (!subcontractor){
        qDebug() << "ivalid subcontractor:";
        return QVariant();
    }
    if (role == IdRole) {
        return subcontractor->id();
    }
    if (role == ShortNameRole) {
        return subcontractor->shortname();
    }
    if (role == NameRole) {
        return subcontractor->name();
    }
    if (role == NipRole) {
        return subcontractor->nip();
    }
    if (role == ZipRole) {
        return subcontractor->zip();
    }
    if (role == CityRole) {
        return subcontractor->city();
    }
    if (role == StreetRole) {
        return subcontractor->street();
    }
    return QVariant();
}

bool SubcontractorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Subcontractor* subcontractor = m_subcontractor.at(index.row());
    bool changed = false;

    switch (role) {
        case ShortNameRole:
            {
                if (subcontractor->shortname() != value.toString()){
                    QString columnName = "shortname";
                    if (m_dbManager->updateSubcontractor(subcontractor->id(), columnName, value)){
                        subcontractor->setShortname(value.toString());
                        changed = true;
                    }else{
                        emit subcontractorDataError(m_dbManager->getLastDatabaseError());
                    }
                }
            }
            break;
        case NameRole:
            {
                if (subcontractor->name() != value.toString()){
                    QString columnName = "name";
                    if (m_dbManager->updateSubcontractor(subcontractor->id(), columnName, value)){
                        subcontractor->setName(value.toString());
                        changed = true;
                    }else{
                        emit subcontractorDataError(m_dbManager->getLastDatabaseError());
                    }
                }
            }
            break;
        case NipRole:
            {
                if (subcontractor->nip() != value.toString()){
                    QString columnName = "NIP";
                    if (m_dbManager->updateSubcontractor(subcontractor->id(), columnName, value)){
                        subcontractor->setNip(value.toString());
                        changed = true;
                    }else{
                        emit subcontractorDataError(m_dbManager->getLastDatabaseError());
                    }
                }
            }
            break;
        case ZipRole:
            {
                if (subcontractor->nip() != value.toString()){
                    QString columnName = "zip";
                    if (m_dbManager->updateSubcontractor(subcontractor->id(), columnName, value)){
                        subcontractor->setZip(value.toString());
                        changed = true;
                    }else{
                        emit subcontractorDataError(m_dbManager->getLastDatabaseError());
                    }
                }
            }
            break;
        case CityRole:
            {
                if (subcontractor->city() != value.toString()){
                    QString columnName = "city";
                    if (m_dbManager->updateSubcontractor(subcontractor->id(), columnName, value)){
                        subcontractor->setCity(value.toString());
                        changed = true;
                    }else{
                        emit subcontractorDataError(m_dbManager->getLastDatabaseError());
                    }
                }
            }
            break;
        case StreetRole:
            {
                if (subcontractor->street() != value.toString()){
                    QString columnName = "street";
                    if (m_dbManager->updateSubcontractor(subcontractor->id(), columnName, value)){
                        subcontractor->setStreet(value.toString());
                        changed = true;
                    }else{
                        emit subcontractorDataError(m_dbManager->getLastDatabaseError());
                    }
                }
            }
            break;
        default: return false;
    }

    if( changed){
        emit dataChanged(index,index,QVector<int>() << role);
        return true;
    }
    return false;
}

QHash<int, QByteArray> SubcontractorModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ShortNameRole] = "shortName";
    roles[NameRole] = "name";
    roles[NipRole] = "nip";
    roles[ZipRole] = "zip";
    roles[CityRole] = "city";
    roles[StreetRole] = "street";
    return roles;
}

bool SubcontractorModel::loadInitialData()
{
    qDebug() << "SubcontractorModel::loadInitialData" ;
    if (!m_dbManager) {
        qWarning() << "DatabaseManager not set!";
        emit subcontractorDataError("DatabaseManager not set!");
        return false;
    }
    return m_dbManager->fetchAllSubcontractor(m_subcontractor);
}

void SubcontractorModel::addSubcontractor(const QString &shortname, const QString &name, const QString &nip,
                                          const QString &zip, const QString &city, const QString &street)
{
    qDebug() << "SubcontractorModel::addSubcontractor" ;
    if (!m_dbManager) {
        qWarning() << "DatabaseManager not set!";
        emit subcontractorDataError("DatabaseManager not set!");
        return;
    }
    if (shortname.isEmpty()) {
        emit subcontractorDataError("the shortName field must be filled in");
        return;
    }
    if (name.isEmpty()) {
        emit subcontractorDataError("the Name field must be filled in");
        return;
    }
    if (nip.isEmpty()) {
        emit subcontractorDataError("the NIP field must be filled in");
        return;
    }
    if (zip.isEmpty()) {
        emit subcontractorDataError("the Zip code field must be filled in");
        return;
    }
    if (city.isEmpty()) {
        emit subcontractorDataError("the City field must be filled in");
        return;
    }
    if (street.isEmpty()) {
        emit subcontractorDataError("the Street field must be filled in");
        return;
    }
    if ( !m_dbManager->isNipUnique(nip)) {
        qWarning() << "NIP is not unique or DatabaseManager not set!";
        emit subcontractorDataError("NIP is not unique or DatabaseManager not set!");
        return;
    }
    int newId = m_dbManager->addSubcontractor(shortname, name, nip, zip, city, street);
    if (newId == -1) {
        qDebug() << "Error creating a new empty book.";
        emit subcontractorDataError("Error creating a new subcontractor.");
        return;
    }

    auto newSubcontractor = new Subcontractor(this);
        newSubcontractor->setId(newId);
        newSubcontractor->setShortname(shortname);
        newSubcontractor->setName(name);
        newSubcontractor->setNip(nip);
        newSubcontractor->setZip(zip);
        newSubcontractor->setCity(city);
        newSubcontractor->setStreet(street);

        beginInsertRows(QModelIndex(), m_subcontractor.count(), m_subcontractor.count());
        m_subcontractor.append(newSubcontractor);
        endInsertRows();

        emit subcontractorDataChanged();
}

void SubcontractorModel::removeSubcontractor(const int& index)
{
    if (index < 0 || index >= m_subcontractor.size()){
        emit subcontractorDataError("Incorrect object index.");
        return;
    }

    int id = m_subcontractor.at(index)->id();
    if(m_dbManager->deleteSubcontractor(id)){
        beginRemoveRows(QModelIndex(), index, index);
        m_subcontractor.removeAt(index);
        endRemoveRows();
    }else{
        emit subcontractorDataError(m_dbManager->getLastDatabaseError());
    }

}
