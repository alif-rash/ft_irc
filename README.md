*This project has been created as part of the 42 curriculum by hajmoham, raalifa, samirza.*

# ft_irc — A Custom Internet Relay Chat Server

## Description

`ft_irc` is our own implementation of an Internet Relay Chat (IRC) server, built
from scratch in **C++98**. It is a deep dive into network programming: how a
real-time chat protocol actually works under the hood.

The server is a robust, **non-blocking TCP server** that handles many clients
at once without ever hanging. It uses a **single `poll()`** event loop to
multiplex every operation — accepting connections, reading requests, and
broadcasting messages — all on one thread, with no forking. Incoming data is
buffered per client and reassembled into complete commands, so fragmented or
partial packets are handled correctly.

On top of the networking layer, the server implements:

* **Authentication** with a connection password (`PASS`), plus nickname and
  username registration (`NICK`, `USER`).
* **Channels** — joining, leaving, and forwarding every channel message to all
  other members (`JOIN`, `PART`, `PRIVMSG`).
* **Operators vs. regular users**, and the operator-only commands: `KICK`,
  `INVITE`, `TOPIC`, and `MODE`.
* **Channel modes**: `+i` (invite-only), `+t` (topic restricted to operators),
  `+k` (channel key/password), `+o` (grant/revoke operator), and `+l` (user
  limit).
* Connection keep-alive and clean disconnect (`PING`, `PONG`, `QUIT`, `CAP`).

## Instructions

### Prerequisites

* A Unix-based operating system (Linux or macOS).
* A C++ compiler supporting the C++98 standard (`g++` or `clang++`).
* The `make` utility.

### Compilation

A standard `Makefile` builds the project. From the repository root:

| Command | What it does |
|---|---|
| `make` | Compiles the project (produces `ircserv`) |
| `make clean` | Removes object files |
| `make fclean` | Removes object files and the `ircserv` executable |
| `make re` | Rebuilds everything from scratch |

The project compiles with `-Wall -Wextra -Werror -std=c++98`.

### Execution

Run the server with a port and a connection password:

```bash
./ircserv <port> <password>
```

* `<port>` — the port the server listens on (e.g. `6667`).
* `<password>` — the password every client must send to connect.

Example:

```bash
./ircserv 6667 pass
```

### Connecting a client

**With a reference IRC client (irssi):**

```bash
irssi
/connect localhost 6667 pass
/join #test
```

**With netcat (raw protocol):**

```bash
nc -C localhost 6667
```

then register and use the server:

```
PASS pass
NICK bob
USER bob 0 * :Bob
JOIN #test
PRIVMSG #test :hello everyone
```

## Resources

### IRC protocol
* **RFC 1459 & RFC 2812** — the official IRC specifications, used for the message
  format (`:prefix command params :trailing`), the registration handshake,
  numeric reply codes, and channel-operator behaviour.
* An **IRC protocol reference PDF** used specifically for understanding command
  **parsing** (tokenizing a raw line, the `:trailing` rule, and mapping tokens
  to command handlers).

### Network programming
* **Beej's Guide to Network Programming** — our primary reference for sockets and
  the `socket` / `bind` / `listen` / `accept` flow, and for working with TCP
  streams.
* Manual pages for **`poll(2)`, `socket(2)`, `setsockopt(2)`, and `fcntl(2)`** —
  used for non-blocking I/O and the single-`poll()` event loop.
* A **TCP visualization video** (YouTube) — used to build an intuition for how a
  TCP connection and byte-stream buffering behave in practice.

### C++ and general references
* **cppreference.com** — C++98 standard-library reference (`std::string`,
  `std::vector`, `std::map`).
* **GeeksforGeeks** — socket programming examples and C++ STL usage.
* **W3Schools** — quick C++ syntax and standard-library refreshers.

### Use of AI

AI tools were used as a learning and productivity aid, **not** as a code
generator for the graded logic. Specifically:

* **Concept & debugging support** — explaining socket and IRC-protocol concepts,
  and helping us trace our own bugs to their source (for example, a channel-ID
  collision caused by using `map.size()` as a key, which we then fixed
  ourselves).
* **Test generation** — producing test cases and command sequences to validate
  command behaviour, error handling, and edge cases.
* **Documentation** — structuring and phrasing this `README.md` and our internal
  testing notes.

All core logic — socket management, the `poll()` event loop, the parsing
routines, and the command handlers — was written, tested, and is fully
understood by each team member for the part they own.