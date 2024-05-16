/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoindexHandler.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:20 by jatan             #+#    #+#             */
/*   Updated: 2024/05/17 02:07:03 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HANDLER_HPP
# define AUTOINDEX_HANDLER_HPP

# include <iostream>
# include <fstream>
# include <dirent.h>   // Library for directory handling
# include <cstring>
# include <sys/stat.h> // For retrieving file details
# include "RequestHandler.hpp"
# include "Utility.hpp"

class AutoindexHandler : public RequestHandler
{
public:
    AutoindexHandler();
    // AutoindexHandler( AutoindexHandler const & src );
    // AutoindexHandler & operator=( AutoindexHandler const & rhs );
    ~AutoindexHandler();


    /** Handles the request for autoindexing.
     *
     * This function is responsible for handling the request for autoindexing. It takes in the request object `req`, the response object `res`, and the root directory path `root`. It performs the necessary operations to generate the autoindex page and store the result in response.
     *
     * @param req The request object containing the client's request information.
     * @param res The response object used to send the response back to the client.
     * @param root The root directory path to find the resource.
     *
     * @return * void 
    */
    void handleRequest(const Request &request, Response &response, std::string root);
private:
    std::string loadHtmlTemplate(void);
};



#endif /* ****************************************************** AUTOINDEX_HANDLER_HPP */