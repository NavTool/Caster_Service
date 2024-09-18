#include "client_virtual.h"

client_virtual::client_virtual(json req, bufferevent *bev)
{
}

client_virtual::~client_virtual()
{
}

int client_virtual::start()
{
    return 0;
}

int client_virtual::stop()
{
    return 0;
}

int client_virtual::runing()
{
    return 0;
}

int client_virtual::bev_send_reply()
{
    return 0;
}

int client_virtual::transfer_sub_raw_data(const char *data, size_t length)
{
    return 0;
}

int client_virtual::publish_recv_raw_data()
{
    return 0;
}

void client_virtual::ReadCallback(bufferevent *bev, void *arg)
{
}

void client_virtual::EventCallback(bufferevent *bev, short events, void *arg)
{
}

void client_virtual::Auth_Login_Callback(const char *request, void *arg, AuthReply *reply)
{
}

void client_virtual::Caster_Register_Callback(const char *request, void *arg, catser_reply *reply)
{
}

void client_virtual::Caster_Sub_Callback(const char *request, void *arg, catser_reply *reply)
{
}
