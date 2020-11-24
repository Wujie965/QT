#include "socket.h"

TestSocket::TestSocket(QWidget* parent): QWidget(parent){}

TestSocket::~TestSocket(){}

/**
 * @brief TestSocket::udp_send_data
 * @param sen_data
 * @param host
 * @param port
 * 使用 UDP 协议动态发送数据
 */
void TestSocket::udp_send_data(QString sen_data, const QHostAddress &host, quint16 port){
    //初始化 UDP 协议发送对象
    sen_qus = new QUdpSocket(this);
    //使用 UDP 协议发送从参数传来的数据，发送数据的 IP 与端口号也通过穿的参数来确定
#if SOCKET_INCLUDE_CHINESE
    sen_qus -> writeDatagram(sen_data.toUtf8(), host, port);
#else
    //该方式发送数据，接收到的中文乱码且解码后少字
    sen_qus -> writeDatagram(sen_data.toStdString().c_str(), sen_data.length(), host, port);
#endif
}

/**
 * @brief TestSocket::udp_receive_data
 * @param host
 * @param port
 * @return
 * 使用 UDP 协议动态接受数据
 */
void TestSocket::udp_receive_data(const QHostAddress &host, quint16 port){
    //初始化 UDP 协议接收对象
    rec_qus = new QUdpSocket(this);
    rec_qus -> bind(host, port);

    //绑定监听事件
#if SOCKET_METHOD_ONE   //方法一：
    connect(rec_qus, &QUdpSocket::readyRead, [this](){
        while (rec_qus -> hasPendingDatagrams()) {
            datagram = rec_qus -> receiveDatagram();
        }
        QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
        qDebug() << utf8 -> toUnicode(datagram.data());
    });
#else   //方法二：
    connect(rec_qus, &QUdpSocket::readyRead, [this](){
        while (rec_qus -> hasPendingDatagrams()) {
            int size = int(rec_qus -> pendingDatagramSize());
            char* pBuffer = new char[size + 1];//分配报文大小的内存
            memset(pBuffer, 0, size + 1);
            rec_qus->readDatagram(pBuffer, size);
            qDebug() << pBuffer;
        }
    });
#endif
}
