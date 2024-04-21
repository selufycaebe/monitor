#ifndef REPLYMODEL_H
#define REPLYMODEL_H
#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <regex>
#include "../global.h"

class Data {
public:
    Data() = default;
    virtual ~Data() = default;

private:

public:
};

class ReplyModel {
public:
    ReplyModel() = default;
    virtual ~ReplyModel() = default;

private:
    int64_t code;
    std::string message;
    //std::optional<Data> data;

public:
    const int64_t & getCode() const { return code; }
    int64_t & getMutableCode() { return code; }
    void setCode(const int64_t & value) { this->code = value; }

    const std::string & getMessage() const { return message; }
    std::string & getMutableMessage() { return message; }
    void setMessage(const std::string & value) { this->message = value; }

    // std::optional<Data> getData() const { return data; }
    // void setData(std::optional<Data> value) { this->data = value; }
};


namespace model {
using nlohmann::json;
void from_json(const json & j, Data & x);
void to_json(json & j, const Data & x);

void from_json(const json & j, ReplyModel & x);
void to_json(json & j, const ReplyModel & x);

inline void from_json(const json & j, Data& x) {
}

inline void to_json(json & j, const Data & x) {
    j = json::object();
}

inline void from_json(const json & j, ReplyModel& x) {
    x.setCode(j.at("code").get<int64_t>());
    x.setMessage(j.at("message").get<std::string>());
    //x.setData(get_stack_optional<Data>(j, "data"));
}

inline void to_json(json & j, const ReplyModel & x) {
    j = json::object();
    j["code"] = x.getCode();
    j["message"] = x.getMessage();
    // if (x.getData()) {
    //     j["data"] = x.getData();
    // }
}
}


#endif // REPLYMODEL_H
