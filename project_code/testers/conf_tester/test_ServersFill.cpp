#include "../../includes/libs.hpp"
#include "../../includes/webserve.hpp"

tokenized_conf    dummy_conf_tokens()
{
    tokenized_conf tokenized_server;

    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    locations.push_back("location / {\nlimit_except GET {\n    deny all;\n}\ntry $uri $uri/ youpi.bad_extension;");
    locations.push_back("location /put_test/ {\nlimit_except PUT {\n    deny all;\n}\n alias /intra/YoupiBanane/PUT/;");
    locations.push_back("location .bla {\n        include fastcgi_params;\nfastcgi_pass unix:/path/to/your/cgi_test_socket.sock;\n fastcgi_param SCRIPT_FILENAME /path/to/your/cgi_test_executable;");
    locations.push_back("location /post_body {\nlimit_except POST {\n    deny all;\n}\n client_max_body_size 100m;");
    locations.push_back("location /directory/\nalias /intra/YoupiBanane/;\nindex youpi.bad_extension;\ntry_files $uri $uri/ /youpi.bad_extension;\nautoindex on;\ntry 404 /erros/error1.html;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    return (tokenized_server);
}

void visualize_tokens(tokenized_conf &tokens)
{
    for (tokenized_conf::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        std::cout << BOLDYELLOW << "essentials: " << RESET << std::endl;
        std::cout << BOLDMAGENTA << it->first << RESET << std::endl;
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << BOLDYELLOW << "Location " << i << RESET << std::endl;
            std::cout << it->second[i] << RESET << std::endl;
        }
    }
}