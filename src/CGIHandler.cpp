#include "CGIHandler.hpp"

/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

CGIHandler::CGIHandler() : _logger(Logger("CGIHandler"))
{
}

CGIHandler::~CGIHandler()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool CGIHandler::handleRequest(const Request &req, Response &res, RouteDetails &routeDetails, const std::string &fullPath)
{
	(void)routeDetails;
	(void)fullPath;
	// We use two pipes
	// First pipe to send input string from parent
	// Second pipe to send concatenated string from child

	int c[2]; // Used to store two ends of first pipe
	int p[2]; // Used to store two ends of second pipe

	if (routeDetails.cgiPass.empty())
	{
		return false;
	}
	if (pipe(c) == -1)
	{
		perror("pipe failed");
		return true;
	}
	if (pipe(p) == -1)
	{
		perror("pipe failed");
		return true;
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return true;
	}

	this->_logger.info("URI: " + req.getUri());
	if (pid == 0)
	{							  // Child process
		close(c[1]);			  // Close chile write end
		dup2(c[0], STDIN_FILENO); // Redirect stdin to pipe read end
		close(c[0]);			  // Close read end

		dup2(p[1], STDOUT_FILENO); // Redirect stdout to pipe write end
		close(p[0]);			   // Close parent read end
		close(p[1]);			   // Close write end

		std::vector<const char *const> env;
		std::vector<const char *const> arg;

		env.push_back(utl::toString("CONTENT_LENGTH=" + utl::toString(req.getBody().length())).c_str());
		env.push_back(utl::toString("PATH_INFO=" + req.getUri()).c_str()); // the
		env.push_back(utl::toString("REQUEST_METHOD=" + req.getMethod()).c_str());
		env.push_back("SERVER_PROTOCOL=HTTP/1.1");
		env.push_back(NULL);

		arg.push_back(strdup(routeDetails.cgiPass.c_str()));
		arg.push_back(strdup(req.getUri().c_str()));
		arg.push_back(NULL);

		execve(arg[0], (char *const *)arg.data(), (char *const *)env.data());
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
	{				 // Parent process
		close(p[1]); // Close write end

		close(c[0]);												// Close read end
		write(c[1], req.getBody().c_str(), req.getBody().length()); // Write body to cgi pipe
		close(c[1]);												// Close write end

		// int fork_status;
		// waitpid(-1, &fork_status, WNOHANG);

		// Read the CGI output from the pipe
		char buffer[300];
		std::string response;
		ssize_t bytes_read = -1;
		this->_logger.log("start read...");
		while (bytes_read != 0)
		{
			bytes_read = read(p[0], buffer, sizeof(buffer));
			if (bytes_read > 0) {
			this->_logger.log("in read response: " + std::string(buffer, bytes_read));
			buffer[bytes_read] = '\0';
			response.append(buffer, bytes_read);
			}
		}
		waitpid(pid, NULL, 0);
		close(p[0]); // Close read end
		this->_logger.log("CGI response: " + response);

		res.setBody(response);
	}
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/