#ifndef CK_CUSTOMER_H
#define CK_CUSTOMER_H
#include <QDialog>
#include <QHash>
namespace Ui {
class CK_Customer;
}

struct _position
{
    qint64 contentBegin;
    int contentLength;
};

class CK_Customer : public QDialog
{
    Q_OBJECT

public:
    explicit CK_Customer(QWidget *parent = 0);
    ~CK_Customer();

private slots:


    void on_pushButtonInsert_clicked();

    void on_listWidgetCustomerName_itemSelectionChanged();

    void on_pushButtonDelete_clicked();

    void on_pushButtonModify_clicked();

    void on_pushButtonNew_clicked();

    void on_pushButtonCommit_clicked();

private:
    QHash<QString, QString> hash;

    Ui::CK_Customer *ui;

    int posBegin;
    int posEnd;
    void clear();
    void deselect();
    void load();
    void commit();
    void refresh();
};

#endif // CK_CUSTOMER_H

