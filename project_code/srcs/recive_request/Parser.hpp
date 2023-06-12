#ifndef PARSER_HPP
# define PARSER_HPP

# include "../../includes/libs.hpp"
# include "../GET/GET.hpp" 
class Parser 
{
    public:
        Parser();
        // Parser(char *buff);
        ~Parser();
        Parser(const Parser &obj2);
        Parser &operator= (const Parser &obj2);
        void            parse(char *new_buffer);
        void            set_byteread_and_readsock(int bytes, int sock);
        GET             GET_request;
        int             read_again;
        std::string     packet;
        int             bytes_read;
        int             read_sock;

    private:

};

#endif