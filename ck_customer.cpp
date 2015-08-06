
#include "ck_customer.h"
#include "ui_ck_customer.h"
#include <QtWidgets>
#include <QDebug>

CK_Customer::CK_Customer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CK_Customer)
{
    ui->setupUi(this);
    CK_Customer::load();
}

CK_Customer::~CK_Customer()
{
    delete ui;
}

void CK_Customer::on_listWidgetCustomerName_itemSelectionChanged()
{
    QStringMatcher matcher;
    QString buffer = hash.value(ui->listWidgetCustomerName->currentItem()->text().mid(0,11));

    matcher.setPattern("<Customer Number>");
    posBegin = matcher.indexIn(buffer, 0)+matcher.pattern().length();
    matcher.setPattern("</Customer Number>");
    posEnd = matcher.indexIn(buffer, 0);
    ui->lineEditNumber->setText(buffer.mid(posBegin,posEnd-posBegin));

    matcher.setPattern("<Customer Name>");
    posBegin = matcher.indexIn(buffer, 0)+matcher.pattern().length();
    matcher.setPattern("</Customer Name>");
    posEnd = matcher.indexIn(buffer, 0);
    ui->lineEditName->setText(buffer.mid(posBegin,posEnd-posBegin));

    matcher.setPattern("<Customer Etc>");
    posBegin = matcher.indexIn(buffer, 0)+matcher.pattern().length();
    matcher.setPattern("</Customer Etc>");
    posEnd = matcher.indexIn(buffer, 0);
    ui->textEditEtc->setText(buffer.mid(posBegin,posEnd-posBegin));
}

void CK_Customer::on_pushButtonInsert_clicked()
{
    QString customerInfo;
    QString customerNumber;
    QString customerName;
    QString customerEtc;

    customerNumber = ui->lineEditNumber->text();
    customerName = ui->lineEditName->text();
    customerEtc = ui->textEditEtc->toPlainText();

    customerInfo = "<Customer Number>"+customerNumber+"</Customer Number>" +
                   "<Customer Name>"+customerName+"</Customer Name>"+
                   "<Customer Etc>"+customerEtc+"</Customer Etc>";

    hash.insert(customerNumber, customerInfo);

    CK_Customer::clear();
    CK_Customer::refresh();
}

void CK_Customer::on_pushButtonDelete_clicked()
{
    hash.remove(ui->listWidgetCustomerName->currentItem()->text().mid(0,11));
    CK_Customer::clear();
    CK_Customer::refresh();
}

void CK_Customer::on_pushButtonCommit_clicked()
{
    CK_Customer::commit();
}

void CK_Customer::on_pushButtonModify_clicked()
{
        hash.remove(ui->listWidgetCustomerName->currentItem()->text().mid(0,11));
        QString customerInfo;
        QString customerNumber;
        QString customerName;
        QString customerEtc;

        customerNumber = ui->lineEditNumber->text();
        customerName = ui->lineEditName->text();
        customerEtc = ui->textEditEtc->toPlainText();

        customerInfo = "<Customer Number>"+customerNumber+"</Customer Number>" +
                       "<Customer Name>"+customerName+"</Customer Name>"+
                       "<Customer Etc>"+customerEtc+"</Customer Etc>";

        hash.insert(customerNumber, customerInfo);

        qint32 rowOld = ui->listWidgetCustomerName->currentRow();
        CK_Customer::clear();
        CK_Customer::refresh();
        ui->listWidgetCustomerName->setCurrentRow(rowOld);

}

void CK_Customer::deselect()
{
    ui->listWidgetCustomerName->clearSelection();
    ui->lineEditNumber->setText("");
    ui->lineEditName->setText("");
    ui->textEditEtc->setText("");
}

void CK_Customer::clear()
{
    ui->lineEditNumber->setText("");
    ui->lineEditName->setText("");
    ui->textEditEtc->setText("");
    ui->listWidgetCustomerName->clear();
}

void CK_Customer::load()
{
    QFile fileOutIdx;
    fileOutIdx.setFileName("../CyberK_Customer/CustomerIdx.txt");
    QTextStream streamIn;
    QString buffer;
    QString customerNumber;
    QString customerInfo;
    QStringMatcher matcher;

    streamIn.setDevice(&fileOutIdx);
    streamIn.setCodec("Utf-8");
    streamIn.device()->open(QFile::ReadOnly|QFile::Text);
    while(!streamIn.atEnd())
    {
        buffer = streamIn.readLine();
        matcher.setPattern("</Customer Number>");
        qint32 posTemp = matcher.indexIn(buffer,0);
        qint32 posLen = matcher.pattern().length();
        customerNumber = buffer.mid(posLen-1, posTemp - (posLen-1));
        customerInfo = buffer;
        hash.insert(customerNumber, customerInfo);
    }

    CK_Customer::refresh();

    fileOutIdx.flush();
    fileOutIdx.close();
}

void CK_Customer::on_pushButtonNew_clicked()
{
    CK_Customer::deselect();
}

void CK_Customer::commit()
{
    QFile fileOutIdx;
    fileOutIdx.setFileName("../CyberK_Customer/CustomerIdx.txt");

    QTextStream streamOut;
    streamOut.setDevice(&fileOutIdx);
    streamOut.setCodec("Utf-8");
    streamOut.device()->open(QFile::WriteOnly);
    QHashIterator<QString, QString> itor(hash);
    while(itor.hasNext())
    {
        itor.next();
        streamOut << itor.value().toUtf8()<<"\r\n";
    }

    fileOutIdx.flush();
    fileOutIdx.close();

    CK_Customer::clear();
    CK_Customer::load();
}

void CK_Customer::refresh()
{
    QString buffer;
    QStringMatcher matcher;

    //hash를 순환하기 위한 반복자
    QHashIterator<QString, QString> itor(hash);
    while (itor.hasNext())
    {
        itor.next();
        buffer = itor.value().toUtf8();
        matcher.setPattern("<Customer Name>");
        posBegin = matcher.indexIn(buffer, 0)+matcher.pattern().length();
        matcher.setPattern("</Customer Name>");
        posEnd = matcher.indexIn(buffer, 0);

        ui->listWidgetCustomerName->addItem(itor.key() + " " + buffer.mid(posBegin,posEnd-posBegin));
    }
}
