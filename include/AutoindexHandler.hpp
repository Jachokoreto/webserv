/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoindexHandler.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:20 by jatan             #+#    #+#             */
/*   Updated: 2024/05/16 19:47:36 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HANDLER_HPP
# define AUTOINDEX_HANDLER_HPP

# include <iostream>

class AutoindexHandler
{
public:
    AutoindexHandler();
    // AutoindexHandler( AutoindexHandler const & src );
    // AutoindexHandler & operator=( AutoindexHandler const & rhs );
    ~AutoindexHandler();

    void handleRequest(std::string req, std::string res);
};



#endif /* ****************************************************** AUTOINDEX_HANDLER_HPP */