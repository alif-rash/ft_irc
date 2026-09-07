/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 19:17:51 by hajmoham          #+#    #+#             */
/*   Updated: 2026/09/07 09:38:06 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

// Parser groups every function that turns raw text into usable pieces.
// All methods are "static": you call them as Parser::parseMessage(line)
// WITHOUT making a Parser object first, because parsing needs no stored
// data -- it just takes input and returns output.
class Parser
{
public:
    // Split one raw IRC line into tokens: [command, param1, ..., trailing]
    static std::vector<std::string> parseMessage(const std::string &message);

    // Split "a,b,c" into ["a","b","c"] (used by JOIN for multiple channels)
    static std::vector<std::string> splitByComma(const std::string &str);

    // Turn a string into an int safely. False if not a clean number.
    static bool stringToInt(const std::string &str, int &result);

    // Check an IRC nickname is valid: starts with a letter, <= 9 chars.
    static bool isValidNickname(const std::string &nickname);
};

#endif