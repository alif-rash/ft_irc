/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:17:50 by raalifa           #+#    #+#             */
/*   Updated: 2026/09/09 13:17:50 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>

class Server;
class Client;

void handleInvite(Server &server, Client &client, const std::vector<std::string> &params);
void handleJoin(Server &server, Client &client, const std::vector<std::string> &params);
void handleKick(Server &server, Client &client, const std::vector<std::string> &params);
void handleMode(Server &server, Client &client, const std::vector<std::string> &params);
void handlePart(Server &server, Client &client, const std::vector<std::string> &params);
void handlePrivmsg(Server &server, Client &client, const std::vector<std::string> &params);
void handleTopic(Server &server, Client &client, const std::vector<std::string> &params);

void handleUser(Server &server, Client &client, const std::vector<std::string> &params);
void handlePass(Server &server, Client &client, const std::vector<std::string> &params);
void handleNick(Server &server, Client &client, const std::vector<std::string> &params);

#endif
