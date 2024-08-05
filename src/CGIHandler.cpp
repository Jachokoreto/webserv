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
					std::cout << errno << std::endl;
					usleep(2000);
				}
				else
				{
					std::cout << "error on write, not eagain" << std::endl;
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

	int initialFd = open("temp-initial-file.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	int outputFd = open("temp-output-file.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);

	// Set pipes to non-blocking to handle large data volumes better
	// fcntl(c[1], F_SETFL, O_NONBLOCK);
	// fcntl(p[0], F_SETFL, O_NONBLOCK);

	std::string requestBody = request.getBody();
	writeAllBytes(initialFd, const_cast<char *>(requestBody.c_str()), requestBody.size());
	lseek(initialFd, 0, SEEK_SET);

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return true;
	}

	if (pid == 0)
	{								   // Child process
		dup2(initialFd, STDIN_FILENO); // Redirect stdin to pipe read end
		// close(initialFd);

		dup2(outputFd, STDOUT_FILENO); // Redirect stdout to pipe write end
		// close(outputFd);

		std::vector<const char *const> env;
		std::vector<const char *const> arg;

		env.push_back(strdup(utl::toString("CONTENT_LENGTH=" + utl::toString(request.getBody().length())).c_str()));
		env.push_back(strdup(utl::toString("PATH_INFO=" + request.getUri()).c_str())); // the
		env.push_back(strdup(utl::toString("REQUEST_METHOD=" + request.getMethod()).c_str()));
		env.push_back("SERVER_PROTOCOL=HTTP/1.1");
		env.push_back(NULL);

		// arg.push_back(strdup(routeDetails.cgiPass.c_str()));
		// arg.push_back(strdup(request.getUri().c_str()));
		arg.push_back("/usr/bin/python3");
		arg.push_back("cgi-python.py");
		arg.push_back(NULL);

		execve(arg[0], const_cast<char *const *>(arg.data()), const_cast<char *const *>(env.data()));
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
	{ // Parent process
		// close(initialFd);
		close(outputFd);
		// if (!request.getBody().empty())
		// {
		// 	std::string tmp = request.getBody();
		// 	std::cout << "body len: " << tmp.length() << std::endl;
		// 	writeAllBytes(c[1], (char *)tmp.c_str(), tmp.length());
		// }

		// close(c[1]);
		waitpid(pid, NULL, 0);

		// std::string responseBody;
		// char buffer[4096];
		// ssize_t bytes_read;
		// this->_logger.log("start read...");
		// while ((bytes_read = read(outputFd, buffer, sizeof(buffer) - 1)) > 0)
		// {
		// 	// buffer[bytes_read] = '\0';
		// 	responseBody.append(std::string(buffer, bytes_read));
		// }

		std::string responseBody;
		std::ifstream inFile("temp-output-file.txt");
		if (!inFile.is_open())
		{
			std::cerr << "Failed to read output file." << std::endl;
			return false;
		}

		std::string line;
		while (getline(inFile, line))
		{
			responseBody += line + "\n";
		}
		inFile.close();

		// close(p[0]); // Close read end
		close(initialFd);
		close(outputFd);
		remove("temp-initial-file.txt");
		remove("temp-output-file.txt");

		response.setStatusCode(200);

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
		// responseBody = responseBody.substr(responseBody.find("\r\n\r\n") + 4);
		response.addHeader("Content-Type", "*/*");
		response.setBody(responseBody);
		// 135226
		// response.setResponseString(responseBody);
		// close(c[0]);
		// close(p[1]);
		return true;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/