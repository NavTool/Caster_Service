#include "source_ntrip.h"
#include "knt/knt.h"

#define __class__ "source_ntrip"

source_ntrip::source_ntrip(json req, bufferevent *bev, std::shared_ptr<process_queue> queue, redisAsyncContext *sub_context, redisAsyncContext *pub_context)
{
    _info = req;
    _bev = bev;
    _queue = queue;
    _pub_context = pub_context;

    _user_name = req["user_name"];
    if (req["ntrip_version"] == "Ntrip/2.0")
    {
        _NtripVersion2 = true;
    }

    int fd = bufferevent_getfd(_bev);
    _ip = util_get_user_ip(fd);
    _port = util_get_user_port(fd);

    _evbuf = evbuffer_new();
}

source_ntrip::~source_ntrip()
{
    bufferevent_free(_bev);
    evbuffer_free(_evbuf);
}

int source_ntrip::start()
{
    bufferevent_setcb(_bev, NULL, WriteCallback, EventCallback, this);
    // bufferevent_enable(_bev, EV_READ);

    return 0;
}

int source_ntrip::stop()
{
    bufferevent_disable(_bev, EV_READ | EV_WRITE);

    json close_req;
    close_req["origin_req"] = _info;

    _queue->push_and_active(close_req, CLOSE_NTRIP_SOURCE);

    spdlog::info("Source List: close connect , user [{}],  addr:[{}:{}]", _user_name, _ip, _port);
    return 0;
}

int source_ntrip::set_source_list(std::string list)
{
    _list = list;
    build_source_table();
    bufferevent_enable(_bev, EV_WRITE);

    return 0;
}

void source_ntrip::ReadCallback(bufferevent *bev, void *arg)
{
    auto svr = static_cast<source_ntrip *>(arg);
}

void source_ntrip::WriteCallback(bufferevent *bev, void *arg)
{
    auto svr = static_cast<source_ntrip *>(arg);

    auto length = evbuffer_get_length(svr->_evbuf);
    if (length == 0)
    {
        svr->stop();
    }
    bufferevent_write_buffer(bev, svr->_evbuf);
    length = evbuffer_get_length(svr->_evbuf);
}

void source_ntrip::EventCallback(bufferevent *bev, short events, void *arg)
{
    auto svr = static_cast<source_ntrip *>(arg);

    spdlog::info("[{}:{}]: {}{}{}{}{}{} , user [{}], , addr:[{}:{}]",
                 __class__, __func__,
                 (events & BEV_EVENT_READING) ? "read" : "-",
                 (events & BEV_EVENT_WRITING) ? "write" : "-",
                 (events & BEV_EVENT_EOF) ? "eof" : "-",
                 (events & BEV_EVENT_ERROR) ? "error" : "-",
                 (events & BEV_EVENT_TIMEOUT) ? "timeout" : "-",
                 (events & BEV_EVENT_CONNECTED) ? "connected" : "-", svr->_user_name, svr->_ip, svr->_port);

    svr->stop();
}

int source_ntrip::build_source_table()
{
    if (_NtripVersion2)
    {
        evbuffer_add_printf(_evbuf, "HTTP/1.1 200 OK\r\n");
        evbuffer_add_printf(_evbuf, "Ntrip-Version: Ntrip/2.0\r\n");
        evbuffer_add_printf(_evbuf, "Ntrip-Flag: st_filter,st_auth,st_match,st_strict,rtsp\r\n");
        evbuffer_add_printf(_evbuf, "Server: Ntrip ExampleCaster/2.0\r\n");
        evbuffer_add_printf(_evbuf, "Date: Tue, 01 Jan 2008 14:08:15 GMT\r\n");
        evbuffer_add_printf(_evbuf, "Connection: close\r\n");
        evbuffer_add_printf(_evbuf, "Content-Type: gnss/sourcetable\r\n");
        evbuffer_add_printf(_evbuf, "Content-Length: %ld\r\n", _list.size());
        evbuffer_add_printf(_evbuf, "\r\n");
        evbuffer_add(_evbuf, _list.c_str(), _list.size());
    }
    else
    {
        evbuffer_add_printf(_evbuf, "SOURCETABLE 200 OK\r\n");
        evbuffer_add_printf(_evbuf, "Server: Ntrip ExampleCaster 2.0/1.0\r\n");
        evbuffer_add_printf(_evbuf, "Connection: close\r\n");
        evbuffer_add_printf(_evbuf, "Content-Type: text/plain\r\n");
        evbuffer_add_printf(_evbuf, "Content-Length: %ld\r\n", _list.size());
        evbuffer_add_printf(_evbuf, "\r\n");
        evbuffer_add(_evbuf, _list.c_str(), _list.size());
    }

    return 0;
}