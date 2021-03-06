/*
 * Copyright 2017 deepstreamHub GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef DEEPSTREAM_CLIENT_HPP
#define DEEPSTREAM_CLIENT_HPP

#include <deepstream/core/fwd.hpp>
#include <deepstream/core/buffer.hpp>
#include <deepstream/core/event.hpp>
#include <deepstream/core/presence.hpp>
#include <deepstream/core/ws.hpp>

#include <cstdint>

#include <functional>
#include <iosfwd>
#include <memory>
#include <string>
#include <ostream>

namespace deepstream {

typedef unsigned long SubscriptionId;

enum class ConnectionState {
    CLOSED,
    AWAIT_CONNECTION,
    CHALLENGING,
    CHALLENGING_WAIT,
    AWAIT_AUTHENTICATION,
    AUTHENTICATING,
    OPEN,
    RECONNECTING,
    ERROR
};

enum class PayloadType : char {
    STRING = 'S',
    OBJECT = 'O',
    NUMBER = 'N',
    NULL_ = 'L',
    TRUE = 'T',
    FALSE = 'F',
    UNDEFINED = 'U'
};

inline std::ostream &operator<<(std::ostream &os, ConnectionState state) {
    const char* states[] = {
        "CLOSED",
        "AWAIT_CONNECTION",
        "CHALLENGING",
        "CHALLENGING_WAIT",
        "AWAIT_AUTHENTICATION",
        "AUTHENTICATING",
        "OPEN",
        "RECONNECTING",
        "ERROR"
    };
    os << states[static_cast<int>(state)];
    return os;
}

struct Client {

    typedef std::function<void(Buffer &&)> LoginCallback;

    Client(const std::string &, WSHandler &, ErrorHandler &);

    ~Client();

    Client() = delete;

    Client(const Client&) = delete;

    Client& operator=(const Client&) = delete;

    Client(Client&&) = delete;

    /**
     * Given a client in `AWAIT_CONNECTION` state, this function attempts to
     * log in with the given authentication data.
     *
     * The format of the user authentication data depends on the <a
     * href="https://deepstream.io/tutorials/core/security-overview/">authentication</a>
     * method used by the server.
     *
     * @param[in] auth User authentication data
     * @param[out] p_user_data On a successful reeturn, store the user data
     * in `p_user_data` if the reference is not `NULL`.
     */
    void login(const Buffer &auth, const LoginCallback &);

    void close();

    ConnectionState get_connection_state() const;

private:
    const std::unique_ptr<Connection> p_connection_;
    SubscriptionId subscription_counter_;

public:
    Event event;
    Presence presence;
};
}

#endif
