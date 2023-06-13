#ifndef PARSER_HPP
# define PARSER_HPP

# include "../../includes/libs.hpp"
# include "../GET/GET.hpp" 
class Parser 
{
    public:
        typedef  std::map<std::string, std::vector<std::string> > packet_map;
    
    public:
        Parser();
        // Parser(char *buff);
        ~Parser();
        Parser(const Parser &obj2);
        Parser &operator= (const Parser &obj2);
        void            fill_header_request(std::string packet);
        void            parse(char *new_buffer);
        void            classify_packet();
        void            set_byteread_and_readsock(int bytes, int sock);
        void            visualize_request_packet();
        GET             GET_request;
        int             read_again;
        std::string     packet;
        int             bytes_read;
        int             read_sock;
        packet_map       request_headers;

    private:

};

#endif