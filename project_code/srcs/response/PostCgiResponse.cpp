#include "Respond.hpp"

std::string ReadAndWirte(int infd, int outfd, std::string &body)
{

	int 		kq = kqueue(); // Create a new kernel event queue
	std::string	output;
	size_t 		write_size = 1000;
	size_t 		read_size = 1000;
	size_t		pos = 0;
	if (kq == -1) {
		std::cerr << "Failed to create kqueue" << std::endl;
		throw(std::runtime_error("Failed to create kqueue"));
	}

	struct kevent kev[2]; // Array to hold the kevent instances

	// Set up the kevent for read events
	EV_SET(&kev[0], outfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

	// Set up the kevent for write events
	EV_SET(&kev[1], infd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);

	// Register the kevent instances with the kqueue
	int ret = kevent(kq, kev, 2, NULL, 0, NULL);
	if (ret == -1) {
		std::cerr << "Failed to register kevents with kqueue" << std::endl;
		throw(std::runtime_error("Failed to register kevents with kqueue"));
	}

	while (true) {
		struct kevent events[2]; // Array to hold the triggered events

		// Wait for events to occur
		int nevents = kevent(kq, NULL, 0, events, 2, NULL);
		if (nevents == -1) {
			std::cerr << "Failed to wait for events" << std::endl;
			throw(std::runtime_error("Failed to wait for events"));
		}

		// Process the triggered events
		for (int i = 0; i < nevents; i++) {
			if (events[i].filter == EVFILT_READ)
			{
				// Handle read event
				char buffer[read_size];
				ssize_t count = read(outfd, buffer, read_size);
				if (count == -1) {
					perror("read failed");
					throw(std::runtime_error("read faild"));
				} else if (count == 0 && pos >= body.length()) 
				{
					close(outfd);
					EV_SET(&kev[0], outfd, EVFILT_READ , EV_DELETE, 0, 0, NULL);
					EV_SET(&kev[1], infd, EVFILT_WRITE , EV_DELETE, 0, 0, NULL);
					return output;
				} else if (count != 0)
				{
					output.append(buffer, count);
				}
				// std::cout << "Read event occurred on fd: " << events[i].ident << std::endl;
			} else if (events[i].filter == EVFILT_WRITE)
			{
				// Handle write event
				if (pos < body.length())
				{
					if (write_size > body.length() - pos)
						write_size = body.length() - pos;
					ssize_t count = write(infd, body.c_str() + pos, write_size);
					if (count == -1) {
						perror("write failed");
						throw(std::runtime_error("write faild"));
					}
					pos += count;
				}
				if (pos >= body.length())
					close(infd);
				// std::cout << "Write event occurred on fd: " << events[i].ident << std::endl;
			}
		}
	}

    // if (kevent(kq, &event[0], 1, NULL, 0, NULL) < 0)
    //     kqueError("failed to remove read  event kque: ");
    // if (kevent(kq, &event[1], 1, NULL, 0, NULL) < 0)
    //     kqueError("failed to remove write  event kque: ");
	EV_SET(&kev[0], outfd, EVFILT_READ , EV_DELETE, 0, 0, NULL);
	EV_SET(&kev[1], infd, EVFILT_WRITE , EV_DELETE, 0, 0, NULL);
	return output;
}
