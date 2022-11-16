#pragma once

#include <cstdlib>
#include <cstring>
#include <ctime>

#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>

#include <QObject>
#include <QString>
#include <QMessageBox>

namespace TFTP {

    constexpr char Default_IP[] = "127.0.0.1";
    constexpr char Default_Port[] = "69";

    // TFTP log level
    typedef enum {
        TFTP_LOG_DEBUG = 0,
        TFTP_LOG_INFO = 1,
        TFTP_LOG_WARNING = 2,
        TFTP_LOG_ERROR = 3
    } TFTP_LOG_LEVEL;

    constexpr char TFTP_LOG_LEVEL_STR[][10] = {
            "DEBUG",
            "INFO",
            "WARNING",
            "ERROR"
    };

    // TFTP magic numbers
    constexpr int TFTP_DEFAULT_PORT = 69;
    constexpr int TFTP_DEFAULT_TIMEOUT = 3;
    constexpr int TFTP_DEFAULT_RETRIES = 6;
    constexpr int TFTP_DEFAULT_BLOCK_SIZE = 512;

    // TFTP opcodes
    typedef enum {
        TFTP_OPCODE_RRQ = 1,
        TFTP_OPCODE_WRQ = 2,
        TFTP_OPCODE_DATA = 3,
        TFTP_OPCODE_ACK = 4,
        TFTP_OPCODE_ERROR = 5,
        TFTP_OPCODE_OACK = 6
    } TFTP_OPCODE;

    // TFTP mode
    typedef enum {
        TFTP_MODE_NETASCII = 1,
        TFTP_MODE_OCTET = 2
    } TFTP_MODE;

    constexpr char TFTP_MODE_STR[3][10] = {"", "netascii", "octet"};

    // TFTP error codes
    typedef enum {
        TFTP_ERROR_NOT_DEFINED = 0,
        TFTP_ERROR_FILE_NOT_FOUND = 1,
        TFTP_ERROR_ACCESS_VIOLATION = 2,
        TFTP_ERROR_DISK_FULL = 3,
        TFTP_ERROR_ILLEGAL_OPERATION = 4,
        TFTP_ERROR_UNKNOWN_TRANSFER_ID = 5,
        TFTP_ERROR_FILE_ALREADY_EXISTS = 6,
        TFTP_ERROR_NO_SUCH_USER = 7
    } TFTP_ERROR;

    // TFTP error strings
    constexpr char TFTP_ERROR_STR[8][50] = {
            "Not defined, see error message (if any).",
            "File not found.",
            "Access violation.",
            "Disk full or allocation exceeded.",
            "Illegal TFTP operation.",
            "Unknown transfer ID.",
            "File already exists.",
            "No such user."
    };

    // TFTP packet structure
    struct TFTPPacket {
    public:
        TFTPPacket() {
            memset(this, 0, sizeof(TFTPPacket));
        }

        // First two bytes are opcode
        unsigned short opcode;
        // Middle section
        union {
            // For error packets
            unsigned short code;
            // For data and ack packets
            unsigned short blockNum;
            // For RRQ and WRQ packets
            // filename is only two bytes, but can be written to and overwrite data section
            char filename[2];
            char option[2];
        };
        // Last is data section
        char data[TFTP_DEFAULT_BLOCK_SIZE];
    };

    struct RRQPacket : public TFTPPacket {
    public:
        int size;

        RRQPacket(const char *file, TFTP_MODE mode)
                : TFTPPacket() {
            opcode = htons(TFTP_OPCODE_RRQ);
            auto *p = filename;
            strcpy_s(p, strlen(file) + 1, file);
            p += strlen(file) + 1;
            strcpy_s(p, strlen(TFTP_MODE_STR[mode]) + 1, TFTP_MODE_STR[mode]);
            p += strlen(TFTP_MODE_STR[mode]) + 1;
            strcpy_s(p, strlen("tsize") + 1, "tsize");
            p += strlen("tsize") + 1;
            strcpy_s(p, strlen("0") + 1, "0");
            p += strlen("0") + 1;
            size = p - (char *) this;
        };

        RRQPacket(const char *file, TFTP_MODE mode, int block_size)
                : RRQPacket(file, mode) {
            auto *p = (char *) this + size;
            strcpy_s(p, strlen("blksize") + 1, "blksize");
            p += strlen("blksize") + 1;
            char blksize[100];
            _itoa_s(block_size, blksize, 10);
            strcpy_s(p, strlen(blksize) + 1, blksize);
            p += strlen(blksize) + 1;
            size = p - (char *) this;
        };

        RRQPacket(const char *file, TFTP_MODE mode, int block_size, int timeout)
                : RRQPacket(file, mode, block_size) {
            auto *p = (char *) this + size;
            strcpy_s(p, strlen("timeout") + 1, "timeout");
            p += strlen("timeout") + 1;
            char timeout_str[100];
            _itoa_s(timeout, timeout_str, 10);
            strcpy_s(p, strlen(timeout_str) + 1, timeout_str);
            p += strlen(timeout_str) + 1;
            size = p - (char *) this;
        };
    };

    struct WRQPacket : public RRQPacket {
    public:
        WRQPacket(const char *file, TFTP_MODE mode)
                : RRQPacket(file, mode) {
            opcode = htons(TFTP_OPCODE_WRQ);
        };

        WRQPacket(const char *file, TFTP_MODE mode, int block_size)
                : RRQPacket(file, mode, block_size) {
            opcode = htons(TFTP_OPCODE_WRQ);
        };

        WRQPacket(const char *file, TFTP_MODE mode, int block_size, int timeout)
                : RRQPacket(file, mode, block_size, timeout) {
            opcode = htons(TFTP_OPCODE_WRQ);
        };
    };

    struct ACKPacket : public TFTPPacket {
    public:
        int size = 4;
        bool isValid = false;

        explicit ACKPacket(unsigned short blockNum) : TFTPPacket() {
            opcode = htons(TFTP_OPCODE_ACK);
            this->blockNum = htons(blockNum);
        };
    };

    struct DATAPacket : public TFTPPacket {

    };

    struct OACKPacket {
    public:
        bool isValid = false;
        int block_size = 0;
        int file_size = 0;
        int timeout = 0;

        explicit OACKPacket(TFTPPacket *packet) {
            if (packet->opcode == htons(TFTP_OPCODE_OACK)) {
                isValid = true;
                auto *p = packet->option;
                while (*p != '\0') {
                    if (strcmp(p, "blksize") == 0) {
                        p += strlen("blksize") + 1;
                        block_size = strtol(p, nullptr, 10);
                        p += strlen(p) + 1;
                    } else if (strcmp(p, "tsize") == 0) {
                        p += strlen("tsize") + 1;
                        file_size = strtol(p, nullptr, 10);
                        p += strlen(p) + 1;
                    } else if (strcmp(p, "timeout") == 0) {
                        p += strlen("timeout") + 1;
                        timeout = strtol(p, nullptr, 10);
                        p += strlen(p) + 1;
                    } else {
                        isValid = false;
                        break;
                    }
                }
            }
        }
    };

    class TFTPClient : public QObject {
    Q_OBJECT
    public:
        bool init();

        explicit TFTPClient(const TFTP_LOG_LEVEL level, QObject *parent = nullptr)
                : QObject(parent) { init(), logLevel = level; };

        ~TFTPClient() override { WSACleanup(); }

        void connectToSlot(const QObject *receiver, const char *member) {
            connect(this, SIGNAL(log(QString)), receiver, member);
        }

        bool setServer_ipv4(const QString &ip,
                            const int port = TFTP_DEFAULT_PORT);

        bool test_ipv4();

        bool download_ipv4(const QString &filename,
                           const QString &savepath,
                           const TFTP_MODE mode = TFTP_MODE_OCTET,
                           const int block_size = TFTP_DEFAULT_BLOCK_SIZE,
                           const int timeout = TFTP_DEFAULT_TIMEOUT,
                           const int retries = TFTP_DEFAULT_RETRIES);


        bool upload_ipv4(const QString &filename,
                         const QString &savepath,
                         const TFTP_MODE mode = TFTP_MODE_OCTET,
                         const int block_size = TFTP_DEFAULT_BLOCK_SIZE,
                         const int timeout = TFTP_DEFAULT_TIMEOUT,
                         const int retries = TFTP_DEFAULT_RETRIES);

        void solve_ERRORPacket(TFTPPacket *packet);

    private:
        void sendlog(const TFTP_LOG_LEVEL level, const char *msg, ...);

        SOCKADDR serverAddr{};
        SOCKADDR clientAddr{};

        TFTP_LOG_LEVEL logLevel;

    signals:
        void log(const QString &msg);
    };
}
