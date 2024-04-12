#pragma once

#include <event2/event.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef void (*msgcallback)(void *arg, const char *msg, size_t data_length);
typedef void (*datacallback)(void *arg, const void *data, size_t data_length);

namespace CORS_C
{
    int Init(json conf, event_base *base);
    int Free();

    int Clear();

    //Server端接口
        //注册一个虚拟挂载点（传名称、坐标、回调函数（激活函数，关闭函数）

        //推送挂载点数据

    //Client端接口
        //查询最近挂载点

        //订阅挂载点

   


    // int Send_Common_Base_Online_Msg(const char *mount_point, const char *user_name, const char *connect_key);
    // int Send_Common_Base_Offline_Msg(const char *mount_point, const char *user_name, const char *connect_key);

    // int Send_SYS_Relay_Base_Online_Msg(const char *mount_point, const char *user_name, const char *connect_key);
    // int Send_SYS_Relay_Base_Offline_Msg(const char *mount_point, const char *user_name, const char *connect_key);

    // int Send_TRD_Relay_Base_Online_Msg(const char *mount_point, const char *user_name, const char *connect_key);
    // int Send_TRD_Relay_Base_Offline_Msg(const char *mount_point, const char *user_name, const char *connect_key);

    // int Send_Rover_Online_Msg(const char *mount_point, const char *user_name, const char *connect_key);
    // int Send_Rover_Offline_Msg(const char *mount_point, const char *user_name, const char *connect_key);

    // int Pub_Base_Raw_Data(const char *channel, const unsigned char *data, size_t data_length);
    // int Sub_Base_Raw_Data(const char *channel, datacallback cb, void *arg);
    // int UnSub_Base_Raw_Data(const char *channel);

    // int Pub_Rover_Raw_Data(const char *channel, const unsigned char *data, size_t data_length);
    // int Sub_Rover_Raw_Data(const char *channel, datacallback cb, void *arg);
    // int UnSub_Rover_Raw_Data(const char *channel);

    // int Get_Common_Source_List(msgcallback cb, void *arg);
    // int Get_SYS_Relay_List(msgcallback cb, void *arg);
    // int Get_TRD_Relay_List(msgcallback cb, void *arg);

    // int Check_Mount_Point_is_Online(const char *mount_point, msgcallback cb, void *arg);
    // int Check_Client_User_is_Online(const char *user_name, msgcallback cb, void *arg);

} // namespace CORS_C


namespace CORS_S
{
     
    
} // namespace CORS_S
