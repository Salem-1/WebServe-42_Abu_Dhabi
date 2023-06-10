#ifndef PARSER_HPP
# define PARSER_HPP

# include "../../includes/libs.hpp"

class Parser 
{
    public:
        Parser();
        Parser(char *buff, int bytes_read);
        ~Parser();
        Parser(const Parser &obj2);
        Parser &operator= (const Parser &obj2);
        void    parse(char *new_buffer);
        int     read_again;
        //not sure how to parse this
        char    *buffer;
        int     bytes_read;
    
    private:

};

#endif