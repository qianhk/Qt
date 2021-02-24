#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <Windows.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_tcpServer = new QTcpServer(this);
    m_tcpServer->listen(QHostAddress::Any, 8090);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(btnClicked()));
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    connect(m_tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(errorStringSlot()));
}

Widget::~Widget()
{
    delete ui;
     m_tcpServer->close();
}

void Widget::newConnectionSlot()
{
    qDebug() << "newConnectionSlot80() called";
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    sendMsg();
}

void Widget::errorStringSlot()
{
    qDebug() << m_tcpServer->errorString();
}

int choose = 0;

void Widget::btnClicked()
{
    QString testChinese = "abc中文1234";
    testChinese.append(QString::fromWCharArray(L"凯凯\u4e2d\u6587"));
    qDebug() << "testChinese1:" << testChinese;
//    MessageBoxW(0, testChinese.toStdWString().c_str(), L"abcd好caption", MB_OK);
    if(choose == 0){

        ui->pushButton->setText("no Cookie");
        choose++;
    }
    else if(choose == 1){

        ui->pushButton->setText("domin");
        choose++;
    }
    else if(choose == 2){

        ui->pushButton->setText("httpOnly");
        choose++;
    }
    else if(choose == 3){

        ui->pushButton->setText("redirect");
        choose++;
    }
    else{

        ui->pushButton->setText("have max-age");
        choose = 0;
    }
}

void Widget::sendMsg()
{

    QString contentStr;
    QString str;
    contentStr = "<html>"
                 "<head>"
                 "<title>"
                 "Socket 80"
                 "</title>"
                 "</head>"
                 "<body>"
                 "<h1>Socket 80</h1>";
    contentStr += QString::fromWCharArray(L"chinese中文123");
    contentStr += "<h1 id='id_cookie'></h1>"
                 "</body>"
                 "<script>"
                 "  document.getElementById('id_cookie').innerHTML=document.cookie"
                 "</script>"
                 "</html>";
    qDebug() << contentStr;

    if(ui->pushButton->text() == "have max-age"){
        
        str = "HTTP/1.1 200 OK\r\n";
        str.append("Server:nginx\r\n");
        str.append("Content-Type:text/html\r\n");
        str.append("Access-Control-Allow-Origin: *\r\n");
        str.append("Set-Cookie: id=123456; max-age=10\r\n");
        str.append("Set-Cookie: name=123456\r\n");
        str.append(QString("Content-Length:%1\r\n\r\n").arg(contentStr.size()));
        str.append(contentStr);
    }
    else if(ui->pushButton->text() == "no Cookie"){

        str = "HTTP/1.1 200 OK\r\n";
        str.append("Server:nginx\r\n");
        str.append("Content-Type:text/html\r\n");
        str.append("Access-Control-Allow-Origin: *\r\n");
        str.append(QString("Content-Length:%1\r\n\r\n").arg(contentStr.size()));
        str.append(contentStr);
    }
    else if(ui->pushButton->text() == "httpOnly"){

        str = "HTTP/1.1 200 OK\r\n";
        str.append("Server:nginx\r\n");
        str.append("Content-Type:text/html\r\n");
        str.append("Set-Cookie: keyHttpOnly=valueHttpOnly;HttpOnly\r\n");
        str.append("Set-Cookie: keyNoHttpOnly=valueNoHttpOnly\r\n");
        str.append("Access-Control-Allow-Origin: *\r\n");
        str.append(QString("Content-Length:%1\r\n\r\n").arg(contentStr.size()));
        str.append(contentStr);
    } else if(ui->pushButton->text() == "redirect") {
        str = "HTTP/1.1 302 Moved Temporarily\r\n";
        str.append("Server:nginx\r\n");
        str.append("Content-Type:text/html\r\n");
        str.append("Set-Cookie: keyHttpOnly=valueHttpOnly;HttpOnly\r\n");
        str.append("Set-Cookie: keyNoHttpOnly=valueNoHttpOnly\r\n");
        str.append("Access-Control-Allow-Origin: *\r\n");
        str.append(QString::fromWCharArray(L"Location: http://weibo.com/?testKey=中文\r\n"));
        str.append(QString("Content-Length:%1\r\n\r\n").arg(contentStr.size()));
        str.append(contentStr);
    }
    else{

        str = "HTTP/1.1 200 OK\r\n";
        str.append("Server:nginx\r\n");
        str.append("Content-Type:text/html\r\n");
        str.append("Access-Control-Allow-Origin: *\r\n");
        str.append("Set-Cookie: id=123456;domain=it1995.cn\r\n");
        str.append("Set-Cookie: name=123456\r\n");
        str.append(QString("Content-Length:%1\r\n\r\n").arg(contentStr.size()));
        str.append(contentStr);
    }
    
    m_tcpSocket->write(str.toUtf8());
}
