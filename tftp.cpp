#include <QFile>
//#include <QDebug>
#include <ctime>

#include "tftp.h"

namespace TFTP {

    bool TFTPClient::init() {
        WSADATA wsadata;
        // WinSock2.2
        if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
            sendlog(TFTP_LOG_ERROR, "startup faild. err == %d", GetLastError());
            WSACleanup();
            return false;
        }
        // Check WinSock version
        if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
            sendlog(TFTP_LOG_ERROR, "WinSock version not supported. err == %d", GetLastError());
            WSACleanup();
            return false;
        }
        return true;
    }

    bool TFTPClient::download_ipv4(const QString &filename_str,
                                   const QString &savepath_str,
                                   const TFTP_MODE mode,
                                   const int block_size,
                                   const int timeout,
                                   const int retries) {
        // time start
        clock_t start, end;
        start = clock();

        // convert QString to char*
        auto filename = filename_str.toLocal8Bit();
        auto savepath = savepath_str.toLocal8Bit();

        // create file
        auto *file = new QFile(savepath);
        if (!file->open(QIODevice::WriteOnly)) {
            sendlog(TFTP_LOG_ERROR, "create file failed");
            return false;
        }

        // create socket
        SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == INVALID_SOCKET) {
            sendlog(TFTP_LOG_ERROR, "create socket failed. err == %d", GetLastError());
            return false;
        }

        // set timeout
        int timeout_ms = timeout * 1000;
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout_ms, sizeof(timeout_ms)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "set timeout failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }

        // bind socket
        if (bind(sock, &clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "bind socket failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }

        // sendto RRQ packet
        RRQPacket rrq(filename.data(), mode, block_size, timeout);
        if (sendto(sock, (char *) &rrq, rrq.size,
                   0, &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "send RRQ failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }
        sendlog(TFTP_LOG_DEBUG, "send RRQ success");

        // get new port (from tftp-server)
        SOCKADDR fromAddr;
        int fromAddrLen = sizeof(fromAddr);

        // receive option acknowledgement
        auto packet = new TFTPPacket();
        if (recvfrom(sock, (char *) packet, sizeof(TFTPPacket),
                     0, &fromAddr, &fromAddrLen) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "receive OACK failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }

        // check option acknowledgement
        OACKPacket oack(packet);
        if (!oack.isValid) {
            sendlog(TFTP_LOG_ERROR, "receive OACK is invalid");
            solve_ERRORPacket(packet);
            closesocket(sock);
            return false;
        }
        sendlog(TFTP_LOG_DEBUG, "receive OACK success");
        sendlog(TFTP_LOG_INFO, "block size: %d", oack.block_size);
        sendlog(TFTP_LOG_INFO, "file size: %d", oack.file_size);
        sendlog(TFTP_LOG_INFO, "timeout: %d", oack.timeout);
        sendlog(TFTP_LOG_INFO, "packet num: %d", oack.file_size / oack.block_size + 1);

        // send ACK packet
        ACKPacket ack((unsigned short) 0);
        if (sendto(sock, (char *) &ack, ack.size,
                   0, &fromAddr, sizeof(fromAddr)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "send ACK(0) failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }
        sendlog(TFTP_LOG_DEBUG, "send ACK(0) success");

        // create buffer
        auto *buffer = new char[oack.block_size + 4];
        auto blksize = oack.block_size + 4;
        auto *data = (DATAPacket *) buffer;
        auto packetNum = oack.file_size / oack.block_size + 1;

//        qDebug() << packetNum;

        for (int blockNum = 1; blockNum <= packetNum; blockNum++) {
            // receive data
            int recvLen = 0;
            bool isTimeout = true;
            for (int i = 0; i < retries; i++) {
                recvLen = recvfrom(sock, buffer, blksize,
                                   0, &fromAddr, &fromAddrLen);
                // check data packet
                if (ntohs(data->opcode) == TFTP_OPCODE_DATA &&
                    ntohs(data->blockNum) == blockNum) {
                    sendlog(TFTP_LOG_DEBUG, "receive DATA(%d) success", blockNum);
                    isTimeout = false;
                    break;
                } else {
                    sendlog(TFTP_LOG_WARNING, "receive packet opcode(%d) blockNum(%d) is invalid",
                            ntohs(data->opcode), ntohs(data->blockNum));
                    solve_ERRORPacket(data);
                    // resend ACK
                    ACKPacket ack(blockNum - 1);
                    if (sendto(sock, (char *) &ack, ack.size,
                               0, &fromAddr, sizeof(fromAddr)) == SOCKET_ERROR) {
                        sendlog(TFTP_LOG_ERROR, "resend ACK(%d) failed. err == %d", blockNum - 1, GetLastError());
                        closesocket(sock);
                        return false;
                    }
                    sendlog(TFTP_LOG_WARNING, "resend ACK(%d) success", blockNum - 1);
                }
            }

            if (isTimeout) {
                sendlog(TFTP_LOG_ERROR, "receive DATA(%d) timeout", blockNum);
                closesocket(sock);
                return false;
            }

            // TODO:: multi thread
            // write file
            file->write(data->data, recvLen - 4);

            // send ACK
            ACKPacket ack(blockNum);
            if (sendto(sock, (char *) &ack, ack.size,
                       0, &fromAddr, sizeof(fromAddr)) == SOCKET_ERROR) {
                sendlog(TFTP_LOG_ERROR, "send ACK(%d) failed. err == %d", blockNum, GetLastError());
                closesocket(sock);
                return false;
            }
            sendlog(TFTP_LOG_DEBUG, "send ACK(%d) success", blockNum);

        }

        // clean up
        file->close();
        closesocket(sock);
        delete[] buffer;
        delete packet;
        delete file;

        // time end
        end = clock();
        double useTime = (double) (end - start) / CLOCKS_PER_SEC;
        sendlog(TFTP_LOG_INFO, "download success.");
        sendlog(TFTP_LOG_INFO, "use time: %lf s", useTime);
        sendlog(TFTP_LOG_INFO, "file size: %lf KB", (double) oack.file_size / 1024);
        sendlog(TFTP_LOG_INFO, "speed: %lf KB/s", (double) oack.file_size / 1024 / useTime);
        return true;
    }

    bool TFTPClient::upload_ipv4(const QString &filename_str,
                                 const QString &savepath_str,
                                 const TFTP_MODE mode,
                                 const int block_size,
                                 const int timeout,
                                 const int retries) {
        // time start
        clock_t start, end;
        start = clock();

        // convert QString to char*
        auto filename = filename_str.toLocal8Bit();
        auto savepath = savepath_str.toLocal8Bit();

        // read file
        auto *file = new QFile(savepath);
        if (!file->open(QIODevice::ReadOnly)) {
            sendlog(TFTP_LOG_ERROR, "open file failed");
            return false;
        }
        auto file_size = file->size();

        // create socket
        SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == INVALID_SOCKET) {
            sendlog(TFTP_LOG_ERROR, "create socket failed. err == %d", GetLastError());
            return false;
        }

        // set timeout
        int timeout_ms = timeout * 1000;
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout_ms, sizeof(timeout_ms)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "set timeout failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }

        // bind socket
        if (bind(sock, &clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "bind socket failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }

        // send WRQ packet
        WRQPacket wrq(filename.data(), mode, block_size, timeout);
        if (sendto(sock, (char *) &wrq, wrq.size,
                   0, &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "send WRQ failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }
        sendlog(TFTP_LOG_DEBUG, "send WRQ success");

        // get new port (from tftp-server)
        SOCKADDR fromAddr;
        int fromAddrLen = sizeof(fromAddr);

        // receive option acknowledgement
        auto packet = new TFTPPacket();
        if (recvfrom(sock, (char *) packet, sizeof(TFTPPacket),
                     0, &fromAddr, &fromAddrLen) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "receive OACK failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }

        // check option acknowledgement
        OACKPacket oack(packet);
        if (!oack.isValid) {
            sendlog(TFTP_LOG_ERROR, "receive OACK is invalid");
            solve_ERRORPacket(packet);
            closesocket(sock);
            return false;
        }
        sendlog(TFTP_LOG_DEBUG, "receive OACK success");
        sendlog(TFTP_LOG_INFO, "block size: %d", oack.block_size);
        sendlog(TFTP_LOG_INFO, "file size: %d", file_size);
        sendlog(TFTP_LOG_INFO, "timeout: %d", oack.timeout);
        sendlog(TFTP_LOG_INFO, "packet num: %d", file_size / oack.block_size + 1);

        // create buffer
        auto *buffer = new char[oack.block_size + 4];
        int blksize = oack.block_size + 4;
        auto packetNum = file_size / oack.block_size + 1;

        // create data
        auto data = (DATAPacket *) buffer;
        data->opcode = htons(TFTP_OPCODE_DATA);

        for (int blockNum = 1; blockNum <= packetNum; blockNum++) {
            // read file
            auto readLen = file->read(data->data, oack.block_size);
            sendlog(TFTP_LOG_DEBUG, "read file success. readLen == %d", readLen);

            if (readLen == -1) {
                sendlog(TFTP_LOG_ERROR, "read file failed");
                closesocket(sock);
                return false;
            }

            // send data
            data->blockNum = htons(blockNum);
            if (sendto(sock, (char *) data, readLen + 4,
                       0, &fromAddr, sizeof(fromAddr)) == SOCKET_ERROR) {
                sendlog(TFTP_LOG_ERROR, "send DATA(%d) failed. err == %d", blockNum, GetLastError());
                closesocket(sock);
                return false;
            }
            sendlog(TFTP_LOG_DEBUG, "send DATA(%d) success", blockNum);

            // receive ACK
            bool isTimeout = true;
            for (int i = 0; i < retries; i++) {
                if (recvfrom(sock, (char *) packet, sizeof(TFTPPacket),
                             0, &fromAddr, &fromAddrLen) == SOCKET_ERROR) {
                    sendlog(TFTP_LOG_ERROR, "receive ACK(%d) failed. err == %d", blockNum, GetLastError());
                    closesocket(sock);
                    return false;
                }

                // check ACK
                if (packet->opcode == htons(TFTP_OPCODE_ACK) &&
                    packet->blockNum == htons(blockNum)) {
                    sendlog(TFTP_LOG_DEBUG, "receive ACK(%d) success", blockNum);
                    isTimeout = false;
                    break;
                } else {
                    sendlog(TFTP_LOG_WARNING, "receive ACK(%d) is invalid", blockNum);
                    solve_ERRORPacket(packet);
                    // resend data
                    if (sendto(sock, (char *) data, readLen + 4,
                               0, &fromAddr, sizeof(fromAddr)) == SOCKET_ERROR) {
                        sendlog(TFTP_LOG_ERROR, "resend DATA(%d) failed. err == %d", blockNum, GetLastError());
                        closesocket(sock);
                        return false;
                    }
                    sendlog(TFTP_LOG_WARNING, "resend DATA(%d) success", blockNum);
                }
            }
            if (isTimeout) {
                sendlog(TFTP_LOG_ERROR, "receive ACK(%d) timeout", blockNum);
                closesocket(sock);
                return false;
            }
        }

        // clean up
        file->close();
        closesocket(sock);
        delete[] buffer;
        delete packet;
        delete file;

        // time end
        end = clock();
        double useTime = (double) (end - start) / CLOCKS_PER_SEC;
        sendlog(TFTP_LOG_INFO, "download success.");
        sendlog(TFTP_LOG_INFO, "use time: %lf s", useTime);
        sendlog(TFTP_LOG_INFO, "file size: %lf KB", (double) file_size / 1024);
        sendlog(TFTP_LOG_INFO, "speed: %lf KB/s", (double) file_size / 1024 / useTime);

        return true;
    }

    bool TFTPClient::setServer_ipv4(const QString &ip_str, const int port) {
        // set server address
        // convert QString to char*
        auto ip = ip_str.toLocal8Bit();
        auto *addr = (SOCKADDR_IN *) &serverAddr;
        addr->sin_family = AF_INET;
        addr->sin_port = htons(port);
        int ret = inet_pton(AF_INET, ip.data(), &addr->sin_addr);
        if (ret == 0) {
            sendlog(TFTP_LOG_ERROR, "ip address not valid (%s)", ip.data());
            return false;
        } else if (ret == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "ip address not valid. err == %d", WSAGetLastError());
            return false;
        }

        // set client address
        addr = (SOCKADDR_IN *) &clientAddr;
        addr->sin_family = AF_INET;
        addr->sin_port = htons(0);
        addr->sin_addr.s_addr = htonl(INADDR_ANY);

        return true;
    }

    bool TFTPClient::test_ipv4() {
        // create socket
        SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == INVALID_SOCKET) {
            sendlog(TFTP_LOG_ERROR, "create socket failed. err == %d", GetLastError());
            return false;
        }

        // set timeout
        int timeout_ms = TFTP_DEFAULT_TIMEOUT * 1000;
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout_ms, sizeof(timeout_ms)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "set timeout failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }

        // bind socket
        if (bind(sock, &clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "bind socket failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }

        // send empty packet
        if (sendto(sock, nullptr, 0, 0, &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "send test packet failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }
        sendlog(TFTP_LOG_DEBUG, "send test packet success");

        // receive ERR packet
        char buf[1024];
        SOCKADDR receiveAddr;
        int addrLen = sizeof(receiveAddr);
        if (recvfrom(sock, buf, sizeof(buf),
                     0, &receiveAddr, &addrLen) == SOCKET_ERROR) {
            sendlog(TFTP_LOG_ERROR, "receive test packet failed. err == %d", GetLastError());
            closesocket(sock);
            return false;
        }
        sendlog(TFTP_LOG_DEBUG, "receive test packet success");

        // close socket
        closesocket(sock);
        sendlog(TFTP_LOG_INFO, "test success");
        return true;
    }

    void TFTPClient::sendlog(const TFTP_LOG_LEVEL level, const char *msg, ...) {
        if (level < logLevel) return;

        char buf[1024];
        // write level
        strcpy_s(buf, "[");
        strcat_s(buf, TFTP_LOG_LEVEL_STR[level]);
        strcat_s(buf, "] ");

        char *p = buf + strlen(buf);
        int len = sizeof(buf) - strlen(buf);

        va_list args;
                va_start(args, msg);
        vsprintf_s(p, len, msg, args);
                va_end(args);

        emit log( buf);
    }

    void TFTPClient::solve_ERRORPacket(TFTPPacket *packet) {
        if(packet->opcode != htons(TFTP_OPCODE_ERROR)) return;
        int errorCode = ntohs(packet->code);
        char *errorMsg = packet->data;
        sendlog(TFTP_LOG_ERROR, "receive ERROR(%d): %s", errorCode, errorMsg);
    }
}