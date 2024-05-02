#include "book.hpp"
#include <QDebug>

Book::Book(QObject *parent)
    : QObject{parent}
{

}

int Book::id() const
{
    qDebug() <<"Book::id()";
    return m_id;
}

void Book::setId(int newId)
{
    qDebug() <<"Book::setId";
    if (m_id == newId)
        return;
    m_id = newId;
    emit idBookChanged();
}

const QString &Book::account() const
{
    qDebug() << "Book::account()";
    return m_account;
}

void Book::setAccount(const QString &newAccount)
{
    qDebug() << "Book::setAccount";
    if (m_account == newAccount)
        return;
    m_account = newAccount;
    emit AccountBookChanged();
}

const QString &Book::contractor() const
{
    qDebug() << "Book::contractor()";
    return m_contractor;
}

void Book::setContractor(const QString &newContractor)
{
    qDebug() << "Book::setContractor" << newContractor;
    if (m_contractor == newContractor)
        return;
    m_contractor = newContractor;
    emit ContractorBookChanged();
}

const QString &Book::invoice() const
{
    qDebug() << "Book::invoice()";
    return m_invoice;
}

void Book::setInvoice(const QString &newInvoice)
{
    qDebug() << "Book::setInvoice" << newInvoice;
    if (m_invoice == newInvoice)
        return;
    m_invoice = newInvoice;
    emit InvoiceBookChanged();
}

const QDate &Book::date() const
{
    qDebug() << "Book::date()";
    return m_date;
}

void Book::setDate(const QDate &newDate)
{
    qDebug() << "Book::setDate";
    if (m_date == newDate)
        return;
    m_date = newDate;
    emit DateBookChanged();
}

double Book::amount() const
{
    qDebug() << "Book::amount";
    return m_amount;
}

void Book::setAmount(double newAmount)
{
    qDebug() << "Book::setAmount";
    if (qFuzzyCompare(m_amount, newAmount))
        return;
    m_amount = newAmount;
    emit AmountBookChanged();
}

int Book::month() const
{
    qDebug() << "Book::month()";
    return m_month;
}

void Book::setMonth(int newMonth)
{
    qDebug() << "Book::setMonth";
    if (m_month == newMonth)
        return;
    m_month = newMonth;
    emit MonthBookChanged();
}

bool Book::isFullyPopulated() const
{
    qDebug() << "Book::isFullyPopulated()";
    if(!m_account.isEmpty() && !m_contractor.isEmpty() && !m_invoice.isEmpty() && m_date.isValid()
            && m_amount>0 && m_month > 0 && (m_cost>0 ||  m_revenue>0)){
        return true;
    }
    return false;
}

double Book::cost() const
{
    qDebug() << "Book::cost()" ;
    return m_cost;
}

void Book::setCost(double newCost)
{
    qDebug() << "Book::setCost";
    if (qFuzzyCompare(m_cost, newCost))
        return;
    m_cost = newCost;
    emit CostBookChanged();
}

double Book::revenue() const
{
    qDebug() << "Book::revenue()";
    return m_revenue;
}

void Book::setRevenue(double newRevenue)
{
    qDebug() << "Book::setRevenue";
    if (qFuzzyCompare(m_revenue, newRevenue))
        return;
    m_revenue = newRevenue;
    emit RevenueBookChanged();
}

int Book::companyId() const
{
    qDebug() << "Book::companyId()";
    return m_companyId;
}

void Book::setCompanyId(int newCompanyId)
{
    qDebug() << "Book::setCompanyId";
    if (m_companyId == newCompanyId)
        return;
    m_companyId = newCompanyId;
    emit CompanyIdBookChanged();
}

int Book::subcontractorId() const
{
    return m_subcontractorId;
}

void Book::setSubcontractorId(int newSubcontractorId)
{
    if (m_subcontractorId == newSubcontractorId)
        return;
    m_subcontractorId = newSubcontractorId;
    emit SubcontractorIdBookChanged();
}
