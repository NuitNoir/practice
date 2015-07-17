
void comment_timestamp(std::string comment) {
	time_t now = time(0);
	// char* dt = ctime(&now);
	tm *ltm = localtime(&now);
	std::cout << ltm->tm_hour << ':' << ltm->tm_min << ' ' << ltm->tm_sec << "\t" << comment << std::endl;
}