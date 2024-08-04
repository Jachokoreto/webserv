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
		this->_logger.info("No cgi pass set");
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

	while (bytes > 0)
	{
		size_t bytesToWrite = bytes;
		if (bytesToWrite > CHUNK_SIZE)
		{
			bytesToWrite = CHUNK_SIZE;
		}

		ssize_t bytesWritten = write(fd, buf, bytesToWrite);
		usleep(100000);
		if (bytesWritten <= 0)
		{
			if (bytesWritten == -1)
			{
				if (errno == EAGAIN)
				{
					sleep(1);
				}
				else
				{
					std::cout << "error on write, not eagain" << std::endl;
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
	// We use two pipes
	// First pipe to send input string from parent
	// Second pipe to send concatenated string from child

	int c[2]; // Used to store two ends of first pipe
	int p[2]; // Used to store two ends of second pipe

	// char buffer[PATH_MAX];
	// if (getcwd(buffer, sizeof(buffer)) != nullptr)
	// {
	// 	std::cout << "Current Path: " << buffer << std::endl;
	// }
	// else
	// {
	// 	perror("getcwd() error");
	// }

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

	// if (fcntl(c[1], F_SETFL, O_NONBLOCK) < 0)
	// 	exit(2);
	// if (fcntl(p[0], F_SETFL, O_NONBLOCK) < 0)
	// 	exit(2);

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return true;
	}

	if (pid == 0)
	{							  // Child process
		dup2(c[0], STDIN_FILENO); // Redirect stdin to pipe read end
		// close(c[0]);			  // Close read end
		close(c[1]); // Close chile write end

		dup2(p[1], STDOUT_FILENO); // Redirect stdout to pipe write end
		close(p[0]);			   // Close parent read end
		// close(p[1]);			   // Close write end

		std::vector<const char *const> env;
		std::vector<const char *const> arg;

		env.push_back(utl::toString("CONTENT_LENGTH=" + utl::toString(request.getBody().length())).c_str());
		env.push_back(strdup(utl::toString("PATH_INFO=" + request.getUri()).c_str())); // the
		env.push_back(strdup(utl::toString("REQUEST_METHOD=" + request.getMethod()).c_str()));
		env.push_back("SERVER_PROTOCOL=HTTP/1.1");
		env.push_back(NULL);

		arg.push_back(strdup(routeDetails.cgiPass.c_str()));
		arg.push_back(strdup(request.getUri().c_str()));
		arg.push_back(NULL);

		execve(arg[0], (char *const *)arg.data(), (char *const *)env.data());
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
	{ // Parent process

		usleep(1000000);
		if (!request.getBody().empty())
		{
			std::string tmp = request.getBody();
			std::cout << "body len: " << tmp.length() << std::endl;
			writeAllBytes(c[1], (char *)tmp.c_str(), tmp.length());
		}
		close(c[0]); // Close write end
		close(c[1]);
		close(p[1]); // Close write end

		char buffer[6000 + 1];
		std::string responseBody;
		size_t total_bytes = 0;
		ssize_t bytes_read = -1;
		this->_logger.log("start read...");
		while (bytes_read != 0)
		{
			bytes_read = read(p[0], buffer, 6000);
			if (bytes_read > 0)
			{
				buffer[bytes_read] = '\0';
				responseBody.append(std::string(buffer, bytes_read));
			}
			total_bytes += bytes_read;
			usleep(10000);
		}
		waitpid(pid, NULL, 0);
		close(p[0]); // Close read end

		response.setStatusCode(200);
		// size_t needle = responseBody.find("\r\n\r\n");
		// std::cout << "After substr:" <<  responseBody.substr(0, needle) << std::endl;
		// response.parseHeaders(utl::splitStringByDelim(responseBody.substr(0, needle), '\n'));
		// // Create an output file stream (ofstream) object
		std::ofstream outFile;

		// Open the file
		outFile.open("example");

		// Check if the file is open
		if (!outFile.is_open())
		{
			std::cerr << "Failed to open the file." << std::endl;
			return 1;
		}

		// Write the string to the file
		outFile << responseBody;

		// Close the file stream
		outFile.close();

		std::cout << "Data has been written to the file." << std::endl;
		std::cout << "Total bytes appended: " << total_bytes << std::endl;
		// responseBody = responseBody.substr(responseBody.find("\r\n\r\n") + 4);
		response.addHeader("Content-Type", "*/*");
		response.setBody(responseBody);
		// 135226
		// response.setResponseString(responseBody);
		close(c[0]);
		close(p[1]);
		return true;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/