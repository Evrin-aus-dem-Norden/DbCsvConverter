﻿#include <QString>
#include <QDir>
#include <QtTest>
#include <QtTest/QtTest>
#include <QCryptographicHash>
#include "../converter.h"

class DbCsvUnitTest : public QObject
{
    Q_OBJECT

public:
    DbCsvUnitTest();
    ~DbCsvUnitTest();

private Q_SLOTS:
    //void initTestCase();
    //void cleanupTestCase();
    //void init();
    //void cleanup();

    //void readFromDbToModelTest();
    //void writeFromModelToCsvTest();

    void readFromCsvToModelTest();
    void writeFromModelToDb();
    void csvToDb();
    void csvToDbAndBack();
};

DbCsvUnitTest::DbCsvUnitTest()
{
}

DbCsvUnitTest::~DbCsvUnitTest()
{
}

bool dbComparison(QString db1, QString db2)
{
    if (!QFile::exists(db1))
    {
       return false;
    }

    if (!QFile::exists(db2))
    {
       return false;
    }

    QSqlDatabase sdb1 = QSqlDatabase::addDatabase("QSQLITE", "fstConnection");
    sdb1.setDatabaseName(db1);
    if (!sdb1.open())
    {
        return false;
    }

    QSqlDatabase sdb2 = QSqlDatabase::addDatabase("QSQLITE","sndConnection");
    sdb2.setDatabaseName(db2);
    if (!sdb2.open())
    {
        return false;
    }

    QStringList tables_1 = sdb1.tables();
    QStringList tables_2 = sdb2.tables();
    if (tables_1 != tables_2)
    {
       return false;
    }
    for (int i = 0; i < tables_1.size(); i++)
    {
        QSqlRecord columns1 = sdb1.record(tables_1[i]);
        QSqlRecord columns2 = sdb2.record(tables_2[i]);
        if  (columns1 != columns2)
        {
            return false;
        }

        for (int j = 0; j<columns1.count(); j++)
        {
            if(columns1.field(j) != columns2.field(j))
            {
                return false;
            }
        }

        QList<QStringList> rows1, rows2;

        QSqlQuery query1("SELECT * FROM " + tables_1[i], sdb1);
        while (query1.next())
        {
            rows1 << QStringList();
            for(int j = 0; j < columns1.count(); j++)
                 rows1.last() << query1.value(j).toString();
        }

        QSqlQuery query2("SELECT * FROM " + tables_2[i], sdb2);
        while (query2.next())
        {
            rows2 << QStringList();
            for(int j = 0; j < columns2.count(); j++)
                 rows2.last() << query2.value(j).toString();
        }
        if (rows1 != rows2)
        {
            return false;
        }
    }
    return true;
}

void DbCsvUnitTest::readFromCsvToModelTest()
{
    ConverterModel *testModel = new ConverterModel();
    ConverterModel *originalModel = new ConverterModel();
    //Заполнение тестируемых данных
    //testModel->setDbName("../DbCsvUnitTest/OriginalFiles/DbTest");
    testModel->setCsvName("../DbCsvUnitTest/OriginalFiles/CsvTest.csv");
    //testModel->setTableName("TableTest");
    //Заполнение результрующих данных
    //OriginalModel->setDbName("../DbCsvUnitTest/ResultFiles/DbTest_tested");
    //OriginalModel->setCsvName("../DbCsvUnitTest/ResultFiles/CsvTest.csv");
    //OriginalModel->setTableName("TableTest");
    originalModel->header<<"IntCol"<<"RealCol"<<"TextCol";
    originalModel->rows<<(QStringList()<<"1"<<"4"<<"IN,SH;as\\HmvV");
    originalModel->rows<<(QStringList()<<"2"<<"7"<<"XstKj/vhvod");
    originalModel->rows<<(QStringList()<<"3"<<"3,5"<<"Qk kX|uNVaH");
    originalModel->rows<<(QStringList()<<"4"<<"5"<<"OuTXo^EW»SxJ");
    originalModel->rows<<(QStringList()<<"5"<<"7,3"<<"VShM copp");
    testModel->readFromCsvToModel();
    QVERIFY(testModel->operator==(originalModel));
}

void DbCsvUnitTest::writeFromModelToDb()
{
    ConverterModel *testModel = new ConverterModel();
    ConverterModel *originalModel = new ConverterModel();

    testModel->setDbName("../DbCsvUnitTest/ResultFiles/DbTest_tested");
    //testModel->setCsvName("../DbCsvUnitTest/ResultFiles/CsvTest.csv");
    testModel->setTableName("TableTest");

    testModel->header<<"IntCol"<<"RealCol"<<"TextCol";
    testModel->rows<<(QStringList()<<"1"<<"4"<<"IN,SH;as\\HmvV");
    testModel->rows<<(QStringList()<<"2"<<"7"<<"XstKj/vhvod");
    testModel->rows<<(QStringList()<<"3"<<"3,5"<<"Qk kX|uNVaH");
    testModel->rows<<(QStringList()<<"4"<<"5"<<"OuTXo^EW»SxJ");
    testModel->rows<<(QStringList()<<"5"<<"7,3"<<"VShM copp");
    testModel->writeFromModelToDb();
    originalModel->setDbName("../DbCsvUnitTest/OriginalFiles/DbTest");
    //originalModel->setCsvName("../DbCsvUnitTest/OriginalFiles/CsvTest.csv");
    originalModel->setTableName("TableTest");
    //originalModel->readFromDbToModel();
    //QVERIFY(testModel->operator==(originalModel));
    QString DbTest_tested("../DbCsvUnitTest/ResultFiles/DbTest_tested"), DbTest("../DbCsvUnitTest/OriginalFiles/DbTest");
    QVERIFY(dbComparison(DbTest_tested, DbTest));
}

void DbCsvUnitTest::csvToDb(){
    ConverterModel *testModel = new ConverterModel();
    ConverterModel *originalModel = new ConverterModel();

    testModel->setDbName("../DbCsvUnitTest/ResultFiles/DbTest_tested");
    testModel->setCsvName("../DbCsvUnitTest/OriginalFiles/CsvTest.csv");
    testModel->setTableName("TableTest");

    testModel->readFromCsvToModel();
    testModel->writeFromModelToDb();

    originalModel->setDbName("../DbCsvUnitTest/OriginalFiles/DbTest");
    //originalModel->setCsvName("../DbCsvUnitTest/OriginalFiles/CsvTest.csv");
    originalModel->setTableName("TableTest");
    //originalModel->readFromDbToModel();
    //QVERIFY(testModel->operator==(originalModel));
    QString DbTest_tested("../DbCsvUnitTest/ResultFiles/DbTest_tested"), DbTest("../DbCsvUnitTest/OriginalFiles/DbTest");
    QVERIFY(dbComparison(DbTest_tested, DbTest));
}

void DbCsvUnitTest::csvToDbAndBack(){
    ConverterModel *testModel = new ConverterModel();
    QDir dir;
    QString DbTest, CsvTest, CsvResult;
    DbTest = "../DbCsvUnitTest/ResultFiles/DbTest_tested";
    testModel->setDbName( DbTest);
    CsvTest = "../DbCsvUnitTest/OriginalFiles/CsvTest.csv";
    testModel->setCsvName(CsvTest);
    testModel->setTableName("TableTest");

    testModel->readFromCsvToModel();
    testModel->writeFromModelToDb();
    CsvResult = "../DbCsvUnitTest/OriginalFiles/CsvTest_tested.csv";
    testModel->csvname = CsvResult;

    testModel->readFromDbToModel();
    testModel->writeFromModelToCsv();

    QCryptographicHash hash1( QCryptographicHash::Sha1 );
    QCryptographicHash hash2( QCryptographicHash::Sha1 );
    QFile file1( CsvTest );
    QFile file2( CsvResult );
    if ( file1.open( QIODevice::ReadOnly ) ) {
        hash1.addData( file1.readAll() );
    } else {
        // Handle "cannot open file" error
    }
    if ( file2.open( QIODevice::ReadOnly ) ) {
        hash2.addData( file2.readAll() );
    } else {
        // Handle "cannot open file" error
    }
    QByteArray sig1 = hash1.result();
    QByteArray sig2 = hash2.result();
    QVERIFY(sig1 == sig2);
}

QTEST_APPLESS_MAIN(DbCsvUnitTest)

#include "tst_dbcsv.moc"
