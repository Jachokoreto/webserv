/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 22:42:52 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 02:19:00 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>

namespace utility
{
	/**
	 * Splits a given string into a vector of substrings based on delimiter (either space or newline).
	 *
	 * @param str The string to be split
	 * @param delim The delimiter
	 * @return A vector of substrings extracted from the input string.
	 */
	std::vector<std::string> splitStringByDelim(const std::string &str, const char delim);
	
	bool isDirectory(const std::string &path);
}

#endif
