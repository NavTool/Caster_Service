#include "source_ntrip.h"
#include "knt/knt.h"

#define __class__ "source_ntrip"

source_ntrip::source_ntrip(json req, bufferevent *bev)
{
    _info = req;
    _bev = bev;
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

    spdlog::info("[{}]: delete connect , user [{}],  addr:[{}:{}]", __class__, _user_name, _ip, _port);
}

int source_ntrip::start()
{
    bufferevent_setcb(_bev, NULL, WriteCallback, EventCallback, this);

    _source_list = CASTER::Get_Source_Table_Text();
    
    build_source_table();
    bufferevent_enable(_bev, EV_WRITE);

    return 0;
}

int source_ntrip::stop()
{
    bufferevent_disable(_bev, EV_WRITE);

    json close_req;
    close_req["origin_req"] = _info;
    close_req["req_type"] = CLOSE_NTRIP_SOURCE;
    QUEUE::Push(close_req);

    spdlog::info("[{}]: close connect , user [{}],  addr:[{}:{}]", __class__, _user_name, _ip, _port);
    return 0;
}

void source_ntrip::WriteCallback(bufferevent *bev, void *arg)
{
    auto svr = static_cast<source_ntrip *>(arg);

    auto length = evbuffer_get_length(svr->_evbuf);
    if (length == 0)
    {
        auto UnsendBufferSize = evbuffer_get_length(bufferevent_get_output(bev));
        if (UnsendBufferSize == 0)
        {
            spdlog::info("[{}]: Send SourceTable Finished, user [{}] ,  addr:[{}:{}]", __class__, svr->_user_name, svr->_ip, svr->_port);
            svr->stop();
        }
    }
    else
    {
        bufferevent_write_buffer(bev, svr->_evbuf);
    }
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
        evbuffer_add_printf(_evbuf, "Ntrip-Flags: \r\n");
        evbuffer_add_printf(_evbuf, "Server: NTRIP CasterService_%s/2.0\r\n", PROJECT_TAG_VERSION);
        evbuffer_add_printf(_evbuf, "Date: %s\r\n", util_get_http_date().c_str());
        evbuffer_add_printf(_evbuf, "Connection: close\r\n");
        evbuffer_add_printf(_evbuf, "Content-Type: gnss/sourcetable\r\n");
        evbuffer_add_printf(_evbuf, "Content-Length: %ld\r\n", _source_list.size() + 16);
        evbuffer_add_printf(_evbuf, "\r\n");
        evbuffer_add(_evbuf, _source_list.c_str(), _source_list.size());
        evbuffer_add_printf(_evbuf, "ENDSOURCETABLE\r\n");
    }
    else
    {
        evbuffer_add_printf(_evbuf, "SOURCETABLE 200 OK\r\n");
        evbuffer_add_printf(_evbuf, "Server: NTRIP CasterService_%s/1.0\r\n", PROJECT_TAG_VERSION);
        evbuffer_add_printf(_evbuf, "Date: %s\r\n", util_get_http_date().c_str());
        evbuffer_add_printf(_evbuf, "Connection: close\r\n");
        evbuffer_add_printf(_evbuf, "Content-Type: text/plain\r\n");
        evbuffer_add_printf(_evbuf, "Content-Length: %ld\r\n", _source_list.size() + 16);
        evbuffer_add_printf(_evbuf, "\r\n");
        evbuffer_add(_evbuf, _source_list.c_str(), _source_list.size());
        evbuffer_add_printf(_evbuf, "ENDSOURCETABLE\r\n");
    }
    return 0;
}
