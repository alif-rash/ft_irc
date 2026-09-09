/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:35 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:35 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Server.hpp"
#include <cstdlib>
#include <csignal>
#include <exception>
#include <stdexcept>
void handleSignal(int signal);
int main(int ac, char **av)
{
    try
    {
        if (ac != 3)
            throw std::runtime_error("Usage: " + std::string(av[0]) + " <port> <password>");

        char *end;
        long value = strtol(av[1], &end, 10);
        if (*end != '\0' || value < 1 || value > 65535)
            throw std::runtime_error("Invalid port number: " + std::string(av[1]));

        int port = static_cast<int>(value);
        std::string password = av[2];
        if (password.empty())
            throw std::runtime_error("Password cannot be empty.");

        signal(SIGINT, handleSignal);
        Server server(port, password);
        server.run();
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    return 0;
}
