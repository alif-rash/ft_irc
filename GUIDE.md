# ft_irc — Cheat Sheet & Run-Through

Everything you need to run and use the server. Two parts:
**Part 1** = how to run it (nc + irssi walkthrough). **Part 2** = every command.

Server name in replies is `ft_irc`. Default example port `6667`, password `pass`.

---

# PART 1 — HOW TO RUN IT

## Start the server (always do this first)

```
make
./ircserv 6667 pass
```

- Prints `Server listening on port 6667`, then waits. Leave it running.
- Stop it with `Ctrl+C` (clean shutdown).
- `Could not bind` → an old server is still up. Run `pkill ircserv` then retry, or use a different port (`6668`).

---

## Option A — Connect with `nc` (raw, you type the protocol)

Open a **new terminal**:

```
nc -C localhost 6667
```

Cursor blinks = connected. Now register (type each line, press Enter):

```
PASS pass
NICK bob
USER bob 0 * :Bob
```

You should get: `:ft_irc 001 bob :Welcome to FT_IRC Network ...` = registered.

Then use any command from Part 2, e.g.:

```
JOIN #test
PRIVMSG #test :hello everyone
```

Notes for nc:
- `nc -C` sends the required `\r\n` line ending. If `-C` isn't supported, use:
  `printf 'PASS pass\r\nNICK bob\r\nUSER bob 0 * :Bob\r\n' | nc localhost 6667`
- After `QUIT`, the server closes the socket — typing more gives `Broken pipe`. That's correct.

---

## Option B — Connect with `irssi` (real client, what evaluators use)

Open a **new terminal**:

```
irssi
```

Inside irssi (it sends PASS/NICK/USER for you):

```
/connect localhost 6667 pass
/nick bob
/join #test
```

Then just type in the channel window to chat.

### irssi keys & slash-commands

| You want to...          | Do this                          |
|-------------------------|----------------------------------|
| Switch window           | `Alt+1`, `Alt+2`, ... (or `Ctrl+N` / `Ctrl+P`) |
| See who's in a channel  | `/names`                         |
| Join a channel          | `/join #chan`                    |
| Leave a channel         | `/part #chan`                    |
| Private message         | `/msg amy hi there`              |
| Set topic               | `/topic new topic`               |
| Give operator           | `/mode #chan +o amy`             |
| Invite-only on          | `/mode #chan +i`                 |
| Set channel key         | `/mode #chan +k secret`          |
| Kick someone            | `/kick #chan amy reason`         |
| Invite someone          | `/invite amy #chan`              |
| Quit irssi              | `/quit`                          |

### Test two users
Open a second terminal → `irssi` → `/connect localhost 6667 pass` → `/nick amy` → `/join #test`.
Now messages one sends to `#test` appear in the other's window.

---

# PART 2 — COMMAND REFERENCE

Legend: `< >` = required, `[ ]` = optional. Commands are case-insensitive.
"Op only" = must be a channel operator (the `@` in the names list).

## Registration (do these first, in order)

| Command | Syntax | What it does |
|---|---|---|
| PASS | `PASS <password>` | Sends the server password. Must be first. |
| NICK | `NICK <nickname>` | Sets your nickname. Must start with a letter, max 9 chars. |
| USER | `USER <user> 0 * :<realname>` | Sets username + real name. Completes registration → `001`. |

## Messaging

| Command | Syntax | What it does |
|---|---|---|
| PRIVMSG (channel) | `PRIVMSG <#chan> :<text>` | Sends a message to everyone in the channel (not back to you). |
| PRIVMSG (user) | `PRIVMSG <nick> :<text>` | Sends a private message to one user. |
| NOTICE-style | (use PRIVMSG) | — |

## Channels

| Command | Syntax | What it does |
|---|---|---|
| JOIN | `JOIN <#chan> [key]` | Joins (or creates) a channel. First joiner becomes operator. `key` needed if `+k` is set. |
| PART | `PART <#chan> [:reason]` | Leaves the channel. Channel is deleted when the last member leaves. |
| TOPIC (view) | `TOPIC <#chan>` | Shows the current topic. |
| TOPIC (set) | `TOPIC <#chan> :<topic>` | Sets the topic. If `+t` is set, op only. |

## Operator commands (op only)

| Command | Syntax | What it does |
|---|---|---|
| KICK | `KICK <#chan> <nick> [:reason]` | Removes a user from the channel. |
| INVITE | `INVITE <nick> <#chan>` | Invites a user (needed to join if `+i` is set). |
| TOPIC | `TOPIC <#chan> :<topic>` | Change topic (op only when `+t`). |
| MODE | `MODE <#chan> <flags> [args]` | Changes channel settings — see below. |

## Channel modes (MODE, op only)

| Mode | Syntax | Meaning |
|---|---|---|
| +i / -i | `MODE <#chan> +i` | Invite-only on/off. |
| +t / -t | `MODE <#chan> +t` | Only ops can change the topic. |
| +k / -k | `MODE <#chan> +k <key>` | Set/remove channel password (key). |
| +o / -o | `MODE <#chan> +o <nick>` | Give/take operator status. |
| +l / -l | `MODE <#chan> +l <number>` | Set/remove the user limit. |

## Connection / misc

| Command | Syntax | What it does |
|---|---|---|
| PING | `PING :<token>` | Server replies `PONG <token>`. Keep-alive. |
| PONG | `PONG :<token>` | Reply to a server PING. |
| QUIT | `QUIT [:reason]` | Disconnects you and removes you from all channels. |
| CAP | `CAP ...` | Capability negotiation (irssi sends this on connect; handled). |

---

# QUICK REFERENCE — common reply codes

| Code | Means |
|---|---|
| 001 | Welcome — you're registered |
| 331 / 332 | No topic set / here's the topic |
| 353 / 366 | Names list / end of names (`@` = operator) |
| 341 | Inviting (INVITE confirmation) |
| 401 | No such nick/channel |
| 403 | No such channel |
| 404 | Cannot send to channel (you're not in it) |
| 421 | Unknown command |
| 431 / 432 / 433 | No nickname / bad nickname / nickname in use |
| 441 / 442 | User not in channel / you're not in channel |
| 443 | User already in channel |
| 451 | You have not registered |
| 461 | Not enough parameters |
| 462 | You may not reregister |
| 464 | Password incorrect |
| 471 / 473 / 475 | Channel full / invite-only / bad key |
| 482 | You're not channel operator |

---

# THINGS TO REMEMBER (behaviour, not bugs)

- Lowering `+l` below the current member count does NOT kick anyone — the limit is checked only on JOIN.
- If the operator leaves without passing `+o`, the channel keeps running with no operator (no auto-promote). Empty channel = deleted.
- Numeric replies come from the server (`:ft_irc 001 ...`); command echoes come from the user (`:bob!bob@localhost JOIN ...`).
- Test the `nc -C` partial-packet case for defense: connect, type `PAS`, `Ctrl+D`, `S pass`, Enter — it rebuilds into `PASS pass`.