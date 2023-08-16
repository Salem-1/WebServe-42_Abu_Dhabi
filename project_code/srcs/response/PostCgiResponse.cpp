#include "Respond.hpp"

std::string ReadAndWirte(int infd, int outfd, std::string &body) {

    int 		kq = kqueue(); // Create a new kernel event queue
	std::string	output;
	int 		write_size = 10;
	ssize_t		n_read = 0;
	size_t		pos = 0;
    if (kq == -1) {
        std::cerr << "Failed to create kqueue" << std::endl;
        throw(std::runtime_error("Failed to create kqueue"));
    }

    struct kevent kev[2]; // Array to hold the kevent instances

    // Set up the kevent for read events
    EV_SET(&kev[0], outfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);

    // Set up the kevent for write events
    EV_SET(&kev[1], infd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, nullptr);

    // Register the kevent instances with the kqueue
    int ret = kevent(kq, kev, 2, nullptr, 0, nullptr);
    if (ret == -1) {
        std::cerr << "Failed to register kevents with kqueue" << std::endl;
        throw(std::runtime_error("Failed to register kevents with kqueue"));
    }

    while (true) {
        struct kevent events[2]; // Array to hold the triggered events

        // Wait for events to occur
        int nevents = kevent(kq, nullptr, 0, events, 2, nullptr);
        if (nevents == -1) {
            std::cerr << "Failed to wait for events" << std::endl;
            throw(std::runtime_error("Failed to wait for events"));
        }

        // Process the triggered events
        for (int i = 0; i < nevents; i++) {
            if (events[i].filter == EVFILT_READ)
			{
                // Handle read event
				char buffer[write_size];
				ssize_t count = read(outfd, buffer, write_size);
				if (count == -1) {
					perror("read failed");
					throw(std::runtime_error("read faild"));
				} else if (count == 0 && pos >= body.length()) 
				{
					break;
				} else if (count != 0)
				{
					output.append(buffer, count);
				}
                std::cout << "Read event occurred on fd: " << events[i].ident << std::endl;
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
                std::cout << "Write event occurred on fd: " << events[i].ident << std::endl;
            }
        }
    }

    return 0;
}
