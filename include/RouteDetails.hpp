/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteDetails.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 03:10:02 by jatan             #+#    #+#             */
/*   Updated: 2024/05/17 03:35:05 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTEDETAILS_HPP
#define ROUTEDETAILS_HPP

# include <string>

class RouteDetails
{
public:
    RouteDetails();
    ~RouteDetails();

    bool autoindex;
    std::string index;
    std::string root;
    int allowedMethods;
};

#endif