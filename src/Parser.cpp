/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 19:18:23 by hajmoham          #+#    #+#             */
/*   Updated: 2026/09/07 09:38:31 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <cstdlib>   // strtol
#include <cctype>    // isalpha, isalnum

// parseMessage: turns one raw IRC line into an ordered list of tokens.
// IRC format: [":" prefix SPACE] command [SPACE params] [SPACE ":" trailing]
// In  : "PRIVMSG #general :hello how are you"
// Out : ["PRIVMSG", "#general", "hello how are you"]
std::vector<std::string> Parser::parseMessage(const std::string &message)
{
    std::vector<std::string> tokens;
    std::string line = message;

    // Safety net: strip a trailing \r\n if the raw line still carries one.
    size_t end = line.find_last_not_of("\r\n");
    if (end == std::string::npos)
        return tokens;                 // empty / only \r\n -> no tokens
    line = line.substr(0, end + 1);

    size_t pos = 0;

    // Optional prefix: a line may begin with ":sender". Skip it if present
    // so it is not mistaken for the command.
    if (line[pos] == ':')
    {
        while (pos < line.size() && line[pos] != ' ')   // past the prefix word
            pos++;
        while (pos < line.size() && line[pos] == ' ')   // and the space after it
            pos++;
    }

    // Main loop: the same three cases as the mini parser.
    while (pos < line.size())
    {
        if (line[pos] == ' ')          // case 1: skip runs of spaces
        {
            pos++;
            continue;
        }
        if (line[pos] == ':')          // case 2: ':' starts trailing = rest of line
        {
            tokens.push_back(line.substr(pos + 1));   // +1 skips the ':' itself
            break;                     // trailing is always last
        }
        size_t start = pos;            // case 3: a normal word
        while (pos < line.size() && line[pos] != ' ')
            pos++;
        tokens.push_back(line.substr(start, pos - start));
    }
    return tokens;
}

// splitByComma: "a,b,c" -> ["a","b","c"]. Empty pieces skipped.
// Used by JOIN: "JOIN #chan1,#chan2".
std::vector<std::string> Parser::splitByComma(const std::string &str)
{
    std::vector<std::string> result;
    std::string current;

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == ',')             // comma -> end of current piece
        {
            if (!current.empty())
                result.push_back(current);
            current.clear();
        }
        else
            current += str[i];         // build piece char by char
    }
    if (!current.empty())              // the last piece after the loop
        result.push_back(current);
    return result;
}

// stringToInt: "42" -> 42. False if empty or has junk like "42abc".
// Used by MODE +l (user limit must be a real number).
bool Parser::stringToInt(const std::string &str, int &result)
{
    if (str.empty())
        return false;

    char *end;                                     // strtol points this at the
    long val = std::strtol(str.c_str(), &end, 10); // first char it can't convert
    if (*end != '\0')                              // leftover junk -> reject
        return false;

    result = static_cast<int>(val);
    return true;
}

// isValidNickname: not empty, <= 9 chars, first char a letter, rest are
// letters/digits or - _ [ ] { } \ |. Cast to unsigned char avoids UB
// when passing a negative char to isalpha/isalnum.
bool Parser::isValidNickname(const std::string &nick)
{
    if (nick.empty() || nick.length() > 9)
        return false;
    if (!std::isalpha(static_cast<unsigned char>(nick[0])))
        return false;

    for (size_t i = 1; i < nick.length(); ++i)
    {
        char c = nick[i];
        if (!std::isalnum(static_cast<unsigned char>(c)) &&
            c != '-' && c != '_' && c != '[' && c != ']' &&
            c != '{' && c != '}' && c != '\\' && c != '|')
            return false;
    }
    return true;
}
