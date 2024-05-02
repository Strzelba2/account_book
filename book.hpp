#ifndef BOOK_HPP
#define BOOK_HPP

#include <QObject>
#include <QDate>

class Book : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idBookChanged)
    Q_PROPERTY(QString account READ account WRITE setAccount NOTIFY AccountBookChanged)
    Q_PROPERTY(QString contractor READ contractor WRITE setContractor NOTIFY ContractorBookChanged)
    Q_PROPERTY(QString invoice READ invoice WRITE setInvoice NOTIFY InvoiceBookChanged)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY DateBookChanged)
    Q_PROPERTY(double amount READ amount WRITE setAmount NOTIFY AmountBookChanged)
    Q_PROPERTY(double cost READ cost WRITE setCost NOTIFY CostBookChanged)
    Q_PROPERTY(double revenue READ revenue WRITE setRevenue NOTIFY RevenueBookChanged)
    Q_PROPERTY(int month READ month WRITE setMonth NOTIFY MonthBookChanged)
    Q_PROPERTY(int companyId READ companyId WRITE setCompanyId NOTIFY CompanyIdBookChanged)
    Q_PROPERTY(int subcontractorId READ subcontractorId WRITE setSubcontractorId NOTIFY SubcontractorIdBookChanged)

    int m_id;

    QString m_account;
    QString m_contractor;
    QString m_invoice;
    QDate m_date;
    double m_amount;
    int m_month;
    double m_cost;
    double m_revenue;
    int m_companyId;
    int m_subcontractorId;

public:
    explicit Book(QObject *parent = nullptr);

    int id() const;
    void setId(int newId);

    const QString &account() const;
    void setAccount(const QString &newAccount);

    const QString &contractor() const;
    void setContractor(const QString &newContractor);

    const QString &invoice() const;
    void setInvoice(const QString &newInvoice);

    const QDate &date() const;
    void setDate(const QDate &newDate);

    double amount() const;
    void setAmount(double newAmount);

    int month() const;
    void setMonth(int newMonth);

    bool isFullyPopulated() const;

    double cost() const;
    void setCost(double newCost);

    double revenue() const;
    void setRevenue(double newRevenue);

    int companyId() const;
    void setCompanyId(int newCompanyId);

    int subcontractorId() const;
    void setSubcontractorId(int newSubcontractorId);

signals:

    void idBookChanged();
    void AccountBookChanged();
    void ContractorBookChanged();
    void InvoiceBookChanged();
    void DateBookChanged();
    void AmountBookChanged();
    void MonthBookChanged();
    void CostBookChanged();
    void RevenueBookChanged();
    void CompanyIdBookChanged();
    void SubcontractorIdBookChanged();
};

#endif // BOOK_HPP
