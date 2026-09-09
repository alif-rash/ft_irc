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
void handleSignal(int signal);
int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }
    char *end;
    long value = strtol(av[1], &end, 10);
    if (*end != '\0' || value < 1 || value > 65535)
    {
        std::cerr << "Invalid port number: " << av[1] << std::endl;
        return 1;
    }
    int port = static_cast<int>(value);
    std::string password = av[2];
    if (password.empty())
    {
        std::cerr << "Password cannot be empty." << std::endl;
        return 1;
    }
    signal(SIGINT, handleSignal);
    try
    {
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
