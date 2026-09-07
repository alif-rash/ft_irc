/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajmoham <hajmoham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 21:03:29 by hajmoham          #+#    #+#             */
/*   Updated: 2026/09/07 21:03:29 by hajmoham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdlib>
#include <cerrno>
#include <csignal>
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
    if (errno != 0 || *end != '\0' || value < 1 || value > 65535)
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
    Server server(port, password);
    server.run();
    return 0;
}
