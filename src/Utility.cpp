/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 22:45:19 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 02:19:03 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utility.hpp"

std::vector<std::string> utility::splitStringByDelim(const std::string &str, const char delim)
{
	std::vector<std::string> result;
	std::istringstream stream(str);

	if (delim == '\n')
	{
		std::string line;
		while (std::getline(stream, line))
			result.push_back(line);
	}
	else if (delim == ' ')
	{
		std::string word;
		while (stream >> word)
			result.push_back(word);
	}

	return result;
}

bool utility::isDirectory(const std::string &path)
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
		return false;
	return S_ISDIR(statbuf.st_mode);
}