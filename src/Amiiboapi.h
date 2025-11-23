#ifndef AMIIBOAPI_H
#define AMIIBOAPI_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Amiiboapi {
private:
    std::string api_base = "https://amiiboapi.com/api";
    http_client_config client_config;
    
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Host"), U("www.amiiboapi.com"));
        request.headers().add(U("Content-Type"), U("application/json"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    std::string build_query_params(const std::map<std::string, std::string>& params) {
        if (params.empty()) return "";
        
        std::string query = "?";
        bool first = true;
        for (const auto& param : params) {
            if (!param.second.empty()) {
                if (!first) query += "&";
                // Кодируем значения параметров
                auto encoded_value = web::uri::encode_data_string(utility::conversions::to_string_t(param.second));
                query += param.first + "=" + utility::conversions::to_utf8string(encoded_value);
                first = false;
            }
        }
        return query;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    Amiiboapi(){
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> get_amiibo(const std::string& head="",const std::string& name="",const std::string& type="",const std::string& id="",const std::string& tail="",const std::string& gameseries="",const std::string& amiiboSeries="",const std::string& character="") {
        std::map<std::string, std::string> params;
        if (!gameseries.empty()) params["gameseries"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(gameseries)));
        if (!amiiboSeries.empty()) params["amiiboSeries"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(amiiboSeries)));
        if (!head.empty()) params["head"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(head)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (!id.empty()) params["id"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(id)));
        if (!tail.empty()) params["tail"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(tail)));
        return make_api_call("/amiibo/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> amiibo_list() {
        return make_api_call("/amiibo/","GET");
    }

    pplx::task<json::value> get_type(const std::string& key="",const std::string& name="") {
        std::map<std::string, std::string> params;
        if (!key.empty()) params["key"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(key)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/type/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> type_list() {
        return make_api_call("/type/","GET");
    }

    pplx::task<json::value> get_gameseries(const std::string& key="",const std::string& name="") {
        std::map<std::string, std::string> params;
        if (!key.empty()) params["key"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(key)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/gameseries/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> gameseries_list() {
        return make_api_call("/gameseries/","GET");
    }

    pplx::task<json::value> get_amiiboseries(const std::string& key="",const std::string& name="") {
        std::map<std::string, std::string> params;
        if (!key.empty()) params["key"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(key)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/amiiboseries/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> amiiboseries_list() {
        return make_api_call("/amiiboseries/","GET");
    }

    pplx::task<json::value> get_character(const std::string& key="",const std::string& name="") {
        std::map<std::string, std::string> params;
        if (!key.empty()) params["key"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(key)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/character/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> character_list() {
        return make_api_call("/character/","GET");
    }

    pplx::task<json::value> last_updated() {
        return make_api_call("/lastupdated/","GET");
    }
};
#endif
