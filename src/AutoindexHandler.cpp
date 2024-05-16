#include "AutoindexHandler.hpp"



/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

AutoindexHandler::AutoindexHandler()
{
}

AutoindexHandler::~AutoindexHandler()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string AutoindexHandler::loadHtmlTemplate(void)
{
	std::ifstream file("/Users/user/sidess/webserv/autoindex/autoindex.html");
	std::stringstream autoindexTempl;

	if (!file.is_open()) {
		std::cerr << "Failed to open HTML template file:" << std::endl;
		return "";
	}
	autoindexTempl << file.rdbuf();
	file.close();
	return autoindexTempl.str();
}



void AutoindexHandler::handleRequest(const Request &req, Response &res, std::string root)
{
	DIR* dir;
	dirent* entry;
	std::string path = root + req.getResource();
	std::ifstream file("/autoindex/autoindex.html");
	std::string autoindexTempl;
	std::string listingHtml;

	autoindexTempl = loadHtmlTemplate();
	if (autoindexTempl.empty()) {
		res.errorResponse(500, "Failed to load HTML template.");
		return;
	}

	if ((dir = opendir(path.c_str())) == nullptr) {
		std::cerr << "Failed to open directory: " << path << std::endl;
		res.setBody( "<html><body><p>Error opening directory.</p></body></html>");
	}

	while ((entry = readdir(dir)) != nullptr) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue; // Skip . and ..

		std::string name = entry->d_name;
		std::string link = (utility::isDirectory(path + "/" + name)) ? name + "/" : name;
		listingHtml += "<li><a href='" + link + "'>" + name + "</a></li>\n";
	}
	closedir(dir);

	size_t pos = autoindexTempl.find("<!-- %DIRECTORY_CONTENTS% -->");
	if (pos != std::string::npos) {
		autoindexTempl.replace(pos, strlen("<!-- %DIRECTORY_CONTENTS% -->"), listingHtml);
	}

	res.setBody(autoindexTempl);

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
