#include "CGIHandler.hpp"
#include <fstream>

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

bool CGIHandler::checkIfHandle(const Request &request, RouteDetails &routeDetails, const std::string &fullPath)
{
	(void)request;
	(void)fullPath;
	if (routeDetails.cgiPass.empty())
	{
		this->_logger.log("No cgi pass set");
		return false;
	}
	return true;
}

ssize_t writeAllBytes(int fd, char *data, size_t bytes)
{
	// need to do pointer arithmetic on the value so
	// it can't be a void *
	char *buf = data;

	ssize_t totalWritten = -1;

	// fd_set fds;
	// FD_ZERO(&fds);
	// FD_SET(fd, &fds);
	// struct timeval tv = {1, 0}; // 3 secs, 0 usecs

	while (bytes > 0)
	{
		size_t bytesToWrite = bytes;
		if (bytesToWrite > CHUNK_SIZE)
		{
			bytesToWrite = CHUNK_SIZE;
		}
		// ssize_t bytesWritten = 0;

		ssize_t bytesWritten = write(fd, buf, bytesToWrite);
		if (bytesWritten <= 0)
		{
			if (bytesWritten == -1)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					usleep(2000);
				}
				else
				{
					perror("write:");
					break;
				}
			}
		}
		buf += bytesWritten;
		totalWritten += bytesWritten;
		if (totalWritten != 0)
		{
			std::cout << "\x1b[1A" << "\x1b[2K"; // Delete current line
		}
		std::cout << "totalWritten " << totalWritten << std::endl;
		bytes -= bytesWritten;
	}

	return (totalWritten);
}

bool CGIHandler::handleRequest(const Request &request, Response &response, RouteDetails &routeDetails, const std::string &fullPath)
{
	(void)routeDetails;
	(void)fullPath;

	// int initialFd = open("temp-initial-file.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	// int outputFd = open("temp-output-file.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);

	int c[2];
	int p[2];

	if (pipe(c) == -1 || pipe(p) == -1)
	{
		perror("pipe failed");
		return true;
	}

	// Set pipes to non-blocking to handle large data volumes better
	// fcntl(c[1], F_SETFL, O_NONBLOCK);
	// fcntl(p[0], F_SETFL, O_NONBLOCK);

	// std::string requestBody = request.getBody();
	// writeAllBytes(initialFd, const_cast<char *>(requestBody.c_str()), requestBody.size());
	// lseek(initialFd, 0, SEEK_SET);

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return true;
	}

	if (pid == 0)
	{							  // Child process
		dup2(c[0], STDIN_FILENO); // Redirect stdin to pipe read end
		close(c[1]);

		dup2(p[1], STDOUT_FILENO); // Redirect stdout to pipe write end
		close(p[0]);

		std::vector<const char *const> env;
		std::vector<const char *const> arg;

		env.push_back(strdup(utl::toString("CONTENT_LENGTH=" + utl::toString(request.getBody().length())).c_str()));
		env.push_back(strdup(utl::toString("PATH_INFO=" + request.getUri()).c_str())); // the
		env.push_back(strdup(utl::toString("REQUEST_METHOD=" + request.getMethod()).c_str()));
		env.push_back("SERVER_PROTOCOL=HTTP/1.1");
		env.push_back(NULL);

		// if (request.getHeader("X-Secret-Header-For-Test") != "")
		// {
		// 	std::cout << "Runinn CGI Tester" << std::endl;
		// 	arg.push_back(strdup(routeDetails.cgiPass.c_str()));
		// 	arg.push_back(strdup(request.getUri().c_str()));
		// }
		// else
		// {
		arg.push_back("/usr/bin/python3");
		std::cout << "fullPath: " << fullPath << std::endl;
		arg.push_back(strdup(fullPath.c_str()));
		// }
		arg.push_back(NULL);

		execve(arg[0], const_cast<char *const *>(arg.data()), const_cast<char *const *>(env.data()));
		perror("execve failed");
		// exit(EXIT_FAILURE);
		return false;
	}
	else
	{ // Parent process
		// close(initialFd);
		// close(outputFd);
		if (!request.getBody().empty())
		{
			std::string tmp = request.getBody();
			writeAllBytes(c[1], (char *)tmp.c_str(), tmp.length());
		}

		close(c[0]);
		close(c[1]);
		close(p[1]);

		std::string responseBody;
		char buffer[4096];
		ssize_t bytes_read;
		while ((bytes_read = read(p[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			responseBody.append(std::string(buffer));
		}
		waitpid(pid, NULL, 0);
		close(p[0]);
		response.setStatusCode(200);
		response.addHeader("Content-Type", "*/*");


		size_t pos = responseBody.find("\r\n\r\n");
		std::string headers;
		// std::string body;

		if (pos != std::string::npos)
		{
			headers = responseBody.substr(0, pos);
			responseBody = responseBody.substr(pos + 4); // skip "\r\n\r\n"
		}
		else
		{
			// Fallback for just "\n\n" if "\r\n\r\n" isn't found
			pos = responseBody.find("\n\n");
			if (pos != std::string::npos)
			{
				headers = responseBody.substr(0, pos);
				responseBody = responseBody.substr(pos + 2); // skip "\n\n"
			}
		}

		response.setBody(responseBody);
		return true;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/