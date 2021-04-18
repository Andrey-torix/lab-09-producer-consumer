// Copyright 2020 Andreytorix
#pragma once
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/regex.hpp>
#include <string>
#include <gumbo.h>

std::string Network(std::string url) {
    try {
        std::string protocol;
        std::string host;
        std::string port;
        std::string target;
        boost::regex ex(
"(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
        boost::cmatch what;
        if (regex_match(url.c_str(), what, ex)) {
            protocol = std::string(what[1].first, what[1].second);
            host = std::string(what[2].first, what[2].second);
            port = std::string(what[3].first, what[3].second);
            target = std::string(what[4].first, what[4].second);
            if (port == "80" || port == "443") {
            } else {
                port = "80";
            }
        }
        int const version = 11;  //версия протокола http 1.1
        std::cout << "протокол:" << protocol << " Хост: " << host
            << " порт: " << port << " страница: " << target << std::endl;
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::resolver resolver(ioc);
        boost::beast::tcp_stream stream(ioc);
        auto const results = resolver.resolve(host, port);
        stream.connect(results);
        boost::beast::http::request<boost::beast::http::string_body> req{
            boost::beast::http::verb::get, target, version };
        req.set(boost::beast::http::field::host, host);
req.set(boost::beast::http::field::user_agent,
        BOOST_BEAST_VERSION_STRING);
        boost::beast::http::write(stream, req);
        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> res;
        boost::beast::http::read(stream, buffer, res);
        std::string body{ boost::asio::buffers_begin(res.body().data()),
                         boost::asio::buffers_end(res.body().data()) };
        boost::beast::error_code ec;
stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        if (ec && ec != boost::beast::errc::not_connected) {
            throw boost::beast::system_error{ ec };
        }
        return body;
    }
    catch (std::exception const& e) {
        std::cout << "Невозможно скачать страницу: " << url
            << " , ошибка: " << e.what() << std::endl;
        return "ERROR";
    }
}
