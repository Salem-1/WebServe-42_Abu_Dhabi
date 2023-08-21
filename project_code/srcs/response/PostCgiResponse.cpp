#include "Respond.hpp"

std::string ReadAndWirte(int infd, int outfd, std::string &body)
{

	int 		kq = kqueue(); // Create a new kernel event queue
	std::string	output;
	size_t 		write_size = 1000;
	size_t 		read_size = 1000;
	size_t		pos = 0;
	fcntl(infd, F_SETFL, O_NONBLOCK);
	fcntl(outfd, F_SETFL, O_NONBLOCK);
	if (kq == -1)
	{
		close(infd);
		close(outfd);
		std::cerr << "Failed to create kqueue" << std::endl;
		return("501");
		throw(std::runtime_error("Failed to create kqueue"));
	}
	struct kevent kev[2];
	try
	{
		EV_SET(&kev[0], outfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
		EV_SET(&kev[1], infd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
		int ret = kevent(kq, kev, 2, NULL, 0, NULL);
		if (ret == -1)
			throw(std::runtime_error("Failed to register kevents with kqueue"));
		while (true) {
			struct kevent events[2]; 
			// Wait for events to occur
			int nevents = kevent(kq, NULL, 0, events, 2, NULL);
			if (nevents == -1)
				throw(std::runtime_error("Failed to wait for events"));
			for (int i = 0; i < nevents; i++) {
				if (events[i].filter == EVFILT_READ)
				{
					char buffer[read_size];
					ssize_t count = read(outfd, buffer, read_size);
					if (count == -1)
						throw(std::runtime_error("read faild"));
					else if (count == 0 && pos >= body.length()) 
					{
						close(outfd);
						EV_SET(&kev[0], outfd, EVFILT_READ , EV_DELETE, 0, 0, NULL);
						EV_SET(&kev[1], infd, EVFILT_WRITE , EV_DELETE, 0, 0, NULL);
						return output;
					} else if (count != 0)
					{
						output.append(buffer, count);
					}
				} else if (events[i].filter == EVFILT_WRITE)
				{
					if (pos < body.length())
					{
						if (write_size > body.length() - pos)
							write_size = body.length() - pos;
						ssize_t count = write(infd, body.c_str() + pos, write_size);
						if (count == -1) 
							throw(std::runtime_error("write faild"));
						pos += count;
					}
					if (pos >= body.length())
						close(infd);
				}
			}
		}
	}
	catch(const std::exception& e)
	{
		close(infd);
		close(outfd);
		EV_SET(&kev[0], outfd, EVFILT_READ , EV_DELETE, 0, 0, NULL);
		EV_SET(&kev[1], infd, EVFILT_WRITE , EV_DELETE, 0, 0, NULL);
		std::cerr << e.what() << '\n';

	}
	return "501";
}
