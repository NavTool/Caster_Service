#include "knt/knt.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <random>


std::string util_random_string(int string_len)
{
    std::string rand_str;

    std::random_device rd;  // non-deterministic generator
    std::mt19937 gen(rd()); // to seed mersenne twister.

    for (int i = 0; i < string_len; i++)
    {
        switch (gen() % 3)
        {
        case 0:
            rand_str += gen() % 26 + 'a';
            break;
        case 1:
            rand_str += gen() % 26 + 'A';
            break;
        case 2:
            rand_str += gen() % 10 + '0';
            break;

        default:
            break;
        }
    }

    return rand_str;
}

std::string util_cal_connect_key(const char *ServerIP, int serverPort, const char *ClientIP, int clientPort)
{
    std::string base = "0123456789ABCDEF"; // 定义16进制表示的基本符号集合

    char hexIp1[9] = "", hexIp2[9] = ""; // 存放转换后的十六进制字符串
    char hexPort1[5] = "", hexPort2[5] = "";

    int octets1[4]; // IPv4地址由四个八位组成
    sscanf(ServerIP, "%d.%d.%d.%d", &octets1[0], &octets1[1], &octets1[2], &octets1[3]);

    for (int i = 0; i < 4; ++i)
    {
        snprintf(hexIp1 + i * 2, sizeof(hexIp1), "%02X", static_cast<unsigned>(octets1[i]));
    }

    int octets2[4]; // IPv4地址由四个八位组成
    sscanf(ClientIP, "%d.%d.%d.%d", &octets2[0], &octets2[1], &octets2[2], &octets2[3]);

    for (int i = 0; i < 4; ++i)
    {
        snprintf(hexIp2 + i * 2, sizeof(hexIp2), "%02X", static_cast<unsigned>(octets2[i]));
    }

    snprintf(hexPort1, sizeof(hexPort1), "%04X", serverPort);
    snprintf(hexPort2, sizeof(hexPort2), "%04X", clientPort);

    char key[25] = "";

    sprintf(key, "%s%s%s%s", hexIp1, hexPort1, hexIp2, hexPort2);

    // if (serverPort == 0)
    //     return nullptr; // 处理特殊情况，当输入为0时直接返回"0"

    // while (serverPort > 0)
    // {
    //     int remainder = serverPort % 16;                    // 取余数
    //     hexPort1.insert(hexPort1.begin(), base[remainder]); // 在结果前面添加对应的十六进制符号
    //     serverPort /= 16;                                   // 更新被除数
    // }

    // if (clientPort == 0)
    //     return nullptr; // 处理特殊情况，当输入为0时直接返回"0"

    // while (clientPort > 0)
    // {
    //     int remainder = clientPort % 16;                    // 取余数
    //     hexPort2.insert(hexPort2.begin(), base[remainder]); // 在结果前面添加对应的十六进制符号
    //     clientPort /= 16;                                   // 更新被除数
    // }

    return std::string() = key;
}

std::string util_cal_connect_key(int fd)
{
    struct sockaddr_in sa1;
    socklen_t len1 = sizeof(sa1);
    if (getsockname(fd, (struct sockaddr *)&sa1, &len1))
    {
        return std::string();
    }

    struct sockaddr_in sa2;
    socklen_t len2 = sizeof(sa2);
    if (getpeername(fd, (struct sockaddr *)&sa2, &len2))
    {
        return std::string();
    }
    std::string Serverip = inet_ntoa(sa1.sin_addr);
    int serverport = ntohs(sa1.sin_port);
    std::string Clientip = inet_ntoa(sa2.sin_addr);
    int clientport = ntohs(sa2.sin_port);

    return util_cal_connect_key(Serverip.c_str(), serverport, Clientip.c_str(), clientport);
}

std::string util_port_to_key(int port)
{

    std::string base = "0123456789ABCDEF"; // 定义16进制表示的基本符号集合

    char key[5] = "";

    snprintf(key, sizeof(key), "%04X", port);
    // if(key.size())

    return std::string() = key;
}

std::string util_get_user_ip(int fd)
{
    struct sockaddr_in sa2;
    socklen_t len2 = sizeof(sa2);
    if (getpeername(fd, (struct sockaddr *)&sa2, &len2))
    {
        return std::string();
    }

    std::string Clientip = inet_ntoa(sa2.sin_addr);
    int clientport = ntohs(sa2.sin_port);

    return Clientip;
}
int util_get_user_port(int fd)
{

    struct sockaddr_in sa2;
    socklen_t len2 = sizeof(sa2);
    if (getpeername(fd, (struct sockaddr *)&sa2, &len2))
    {
        return 0;
    }

    std::string Clientip = inet_ntoa(sa2.sin_addr);
    int clientport = ntohs(sa2.sin_port);

    return clientport;
}
std::string util_get_date_time()
{

    time_t now = time(0);                 // 获取当前时间的time_t类型值
    struct tm *tm_info = localtime(&now); // 将time_t类型值转换为struct tm类型的本地时间信息
    char buffer[80];                      // 存放格式化后的日期时间字符串
    strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", tm_info);

    std::string tm = buffer;

    return tm;
}
std::string util_get_space_time()
{
    return std::string("0000/00/00 00:00:00");
}
std::string util_get_http_date()
{
    std::time_t now = std::time(nullptr);
    std::tm* gmt = std::gmtime(&now);
 
    std::ostringstream oss;
    oss << std::put_time(gmt, "%a, %d %b %Y %H:%M:%S GMT");
    return oss.str();


    return std::string();
}