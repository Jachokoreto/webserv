/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:42 by jatan             #+#    #+#             */
/*   Updated: 2024/05/17 01:17:01 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <iostream>
// #include <chrono>
#include <ctime>

#include "colors.h"

class Logger
{
public:
	Logger(std::string context);
	~Logger();

	void log(std::string str);
	void info(std::string str);
	void warning(std::string str);
	void error(std::string str);

private:
	void output(std::string color, std::string str);
	Logger();
	std::string _context;
};

std::ostream &			operator<<( std::ostream & o, Logger const & i );

#endif