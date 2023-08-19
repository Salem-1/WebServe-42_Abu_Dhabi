#include "../../includes/libs.hpp"
#include "../../includes/webserve.hpp"
#include "tester.hpp"


void    test_no_root_no_location_root()
{
    std::cout << "Make it after finishing locations inshalla" << std::endl;
};
void    test_error_page()
{
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT", "Normal method", "-");
}
void    test_index()
{
    one_location_test("location /;root /YoupiBanan; index ", "no indexes directive", "-");
    one_location_test("location /;root /YoupiBanan; index youbi.bla index.html", "2 indexes directive", "-");
    tokenized_conf tokenized_server =  dummy_location_fill("location /; root //; methods GET   ;\n index index.html;");
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    cmp_strings(fill.servers.servers[0]["/ index"], "index.html", "correct index passes index.html");
    tokenized_server =  dummy_location_fill("location /YoubiBanana; root /YoubiBanana/dir; methods GET   ;\n index youpi.bad_extension;");
    ServerFill    fil(tokenized_server);
    fil.parseTokens();
    cmp_strings(fil.servers.servers[0]["/YoubiBanana index"], "youpi.bad_extension", "correct index passes Youbi");
 
}
void    test_methods()
{
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT", "Normal method", "+");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods ", "empty method", "-");
    one_location_test("location /;root /YoupiBanan; methods GGET;", "wrong method", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST GET", "Repeated method", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT OPTION", "more than allowed method", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT GET", "repeated method", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE put", "bad case method", "-");

    tokenized_conf tokenized_server =  dummy_location_fill("location /;root /YoupiBanan; index index.html; autoindex on; methods GET ;");
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    cmp_strings(fill.servers.servers[0]["/ methods"], "GET", "correct GET method");
    tokenized_server =  dummy_location_fill("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST;");
    ServerFill    fil(tokenized_server);
    fil.parseTokens();
    cmp_strings(fil.servers.servers[0]["/ methods"], "GET POST", "methods GET POST");
    tokenized_server =  dummy_location_fill("location /;root /YoupiBanan; index index.html; autoindex on; methods DELETE PUT GET POST;");
    ServerFill    f(tokenized_server);
    f.parseTokens();
    cmp_strings(f.servers.servers[0]["/ methods"], "DELETE PUT GET POST", "methods DELETE PUT GET POST");
}

void    test_autoindex()
{
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on ", "no semicolon normal indexes directive", "+");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex;", "no option autoindex directive", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on off;", "2 autoindexes options directive", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex ol;", "wrong autoindexes options directive", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex of;", "wrong autoindexes options directive", "-");
    tokenized_conf tokenized_server =  dummy_location_fill("location /;root /YoupiBanan; index index.html; autoindex on;");
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    cmp_strings(fill.servers.servers[0]["/ autoindex"], "on", "correct autoindex on");
    tokenized_server =  dummy_location_fill("location /;root /YoupiBanan; index index.html; autoindex off;");
    ServerFill    fil(tokenized_server);
    fil.parseTokens();
    cmp_strings(fil.servers.servers[0]["/ autoindex"], "off", "correct autoindex off");
}

void    test_location_root()
{
    tokenized_conf tokenized_server =  dummy_location_fill("location /; root /YoubiBanana; methods GET   ;\n index youpi.bad_extension;");
    ServerFill    fill(tokenized_server);

    positive_essential_try_catch(fill, "Normal location");
    cmp_strings(fill.servers.servers[0]["/"], fill.servers.servers[0]["root"] + "/YoubiBanana", "correct location root");
    one_location_test("location /;root ;", "root has no path", "-");
    one_location_test("location /;root /youbi not youbi;", "root has 3 pathes", "-");
    one_location_test("location /;root /youbi ;", "normal location", "+");
    tokenized_server =  dummy_location_fill("location /; root /put-test; methods GET   ;\n index youpi.bad_extension;");
    ServerFill    fi(tokenized_server);
    fi.parseTokens();
    cmp_strings(fi.servers.servers[0]["/"], fi.servers.servers[0]["root"] + "/put-test", "put-test location root ");
    tokenized_server =  dummy_location_fill("location /; root /put-tgxfhkljhcgvhjklhgfchvjklhcgfhvjbklhvgcfxgvjhbkbhv21346jvvv5hvvh3crfqyvewctiy23ewugvwdvavhgvdbahfgvdbhgsvudyibahsgvdfyilhsvfyidhsgyusyghyfgygvbiugglersg2sd62gest; methods GET   ;\n index youpi.bad_extension;");
    ServerFill    f(tokenized_server);
    f.parseTokens();
    cmp_strings(f.servers.servers[0]["/"],f.servers.servers[0]["root"] +  "/put-tgxfhkljhcgvhjklhgfchvjklhcgfhvjbklhvgcfxgvjhbkbhv21346jvvv5hvvh3crfqyvewctiy23ewugvwdvavhgvdbahfgvdbhgsvudyibahsgvdfyilhsvfyidhsgyusyghyfgygvbiugglersg2sd62gest", "crazy long location root ");
    tokenized_server =  dummy_location_fill("location /; root /; methods GET   ;\n index youpi.bad_extension;");
    ServerFill    fe(tokenized_server);
    fe.parseTokens();
    cmp_strings(fe.servers.servers[0]["/"], fe.servers.servers[0]["root"] + "/", "/ location root ");
}

void    cmp_strings(std::string left, std::string right, std::string msg)
{
    if (left == right)
      std::cout << BOLDGREEN << msg  << " test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << msg  <<  " test failed 😱" << RESET << std::endl;
}

void    cmp_bool(bool left, bool right, std::string msg)
{
    if (left == right)
      std::cout << BOLDGREEN << msg  << " test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << msg  <<  " test failed 😱" << RESET << std::endl;
}

void one_location_test(std::string location_case, std::string msg, std::string sign)
{
    tokenized_conf tokenized_server =  dummy_location_fill(location_case);
    ServerFill    fill(tokenized_server);

    if (sign == "+")
        positive_essential_try_catch(fill, msg);
    else
        negative_essential_try_catch(fill, msg);
}


void    test_lcation_firstLine()
{
    tokenized_conf tokenized_server =  dummy_location_fill("locaion /; methods GET   ;\n index youpi.bad_extension;");
    ServerFill    fill(tokenized_server);
    negative_essential_try_catch(fill, "bad location word");
    tokenized_server.clear();
    tokenized_server =  dummy_location_fill("location location /; methods GET   ;\n index youpi.bad_extension;");
    ServerFill filled(tokenized_server);
    negative_essential_try_catch(filled, "repeated location word");
    tokenized_server.clear();
    tokenized_server =  dummy_location_fill("location /;");
    ServerFill filld(tokenized_server);
    negative_essential_try_catch(filld, "no options just location word");
    tokenized_server.clear();
    tokenized_server =  dummy_location_fill("location ;root index.html;");
    ServerFill fi(tokenized_server);
    negative_essential_try_catch(fi, "no path provided");
    tokenized_server.clear();
    tokenized_server =  dummy_location_fill("location / /d ;root index.html;");
    ServerFill fil(tokenized_server);
    negative_essential_try_catch(fil, "2 pathes path provided");
}

tokenized_conf    dummy_location_fill(std::string location)
{
    tokenized_conf tokenized_server;

    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    locations.push_back(location);
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    return (tokenized_server);
}
tokenized_conf    dummy_conf_tokens()
{
    tokenized_conf tokenized_server;

    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    locations.push_back("location /; methods GET   ;\n index youpi.bad_extension;");
    locations.push_back("location /put_test/; methods PUT     ;\n  root /intra/YoupiBanane/PUT/;");
    locations.push_back("location .bla ;     \ncgi-bin /path/to/your/cgi_test_socket.sock;\n");
    locations.push_back("location /post_body; methods POST     ;\n  client_max_body_size 100m;client_max_body_size \t\t100m;");
    locations.push_back("location .bla; methods POST ; ");
    locations.push_back("location /directory/;root /intra/YoupiBanane/;\nindex youpi.bad_extension;\nautoindex on;\nerror_page 404 error_pages/404.html;");
    locations.push_back("location /directory/;root /intra/YoupiBanane/;\nindex youpi.bad_extension;\nautoindex on;\nerror_page 404 error_pages/404.html; redirection 301 /PUT/;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    return (tokenized_server);
}


void    test_Essentials()
{
    tokenized_conf tokenized_server = dummy_conf_tokens();
    ServerFill    fill(tokenized_server);
    test_emptyEssentials(fill);
    test_lenEssentials(fill);
    test_mixSpacesEssentials(fill);
    test_essentialOccurance(fill);
    test_listenConf(fill);
    test_hostNameConf(fill);
    test_rootConf(fill);
    test_indexConf(fill);
    test_bodySizeConf(fill);
    test_manyConfs(fill);
    test_repeated_port();
    // runServer(servers);
}


tokenized_conf    fill_second_essential(std::string second_essential)
{
    tokenized_conf tokenized_server = dummy_conf_tokens();
    tokenized_server.push_back(dummy_conf_tokens()[0]);
    tokenized_server[1].first = second_essential;
    return (tokenized_server);
}



void    test_bodySizeConf(ServerFill &fill)
{
    conf    &servers = fill.servers.servers;
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot  intra/YoupiBanane;\nindex index.html;\nclient_max_body_size;";
    negative_essential_try_catch(fill, "empty client_max_body_size");
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot  intra/YoupiBanane;\nindex index.html;\nclient_max_body_size 100;";
    fill.parseTokens();
    if (!servers[0].empty() && inMap(servers[0], "Max-Body"))
        std::cout << BOLDGREEN << "has client_max_body_size key configuration test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "has client_max_body_size key configuration test failed 😱" << RESET << std::endl;
    if (!servers[0].empty() && inMap(servers[0], "Max-Body") && servers[0]["Max-Body"] == "100")
        std::cout << BOLDGREEN << "has client_max_body_size = 100 configuration test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "has client_max_body_size = 100 configuration test failed 😱" << RESET << std::endl;
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot  intra/YoupiBanane;\nindex index.html;\nclient_max_body_size 100s;";
    negative_essential_try_catch(fill, "non numeric client_max_body_size");
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot  intra/YoupiBanane;\nindex index.html;\nclient_max_body_size -100;";
    negative_essential_try_catch(fill, "negative client_max_body_size");
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot  intra/YoupiBanane;\nindex index.html;\nclient_max_body_size 100*;";
    negative_essential_try_catch(fill, "char client_max_body_size");
}

void    test_indexConf(ServerFill &fill)
{
    conf    &servers = fill.servers.servers;
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot  intra/YoupiBanane;\nindex;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "NO index test");
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot  intra/YoupiBanane;\nindex index.html;\nclient_max_body_size 100;";
    fill.parseTokens();
    if (!servers[0].empty() && inMap(servers[0], "index"))
        std::cout << BOLDGREEN << "has index key configuration test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "has index key configuration test failed 😱" << RESET << std::endl;
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot /;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    fill.parseTokens();
    if (!servers[0].empty() && inMap(servers[0], "root") && servers[0]["index"] == "youpi.bad_extension")
        std::cout << BOLDGREEN << "has youpi.bad_extension configuration test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "has youpi.bad_extension configuration test failed 😱" << RESET << std::endl;
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot /;\nindex index.html;\nclient_max_body_size 100;";
    fill.parseTokens();
    if (!servers[0].empty() && inMap(servers[0], "root") && servers[0]["index"] == "index.html")
        std::cout << BOLDGREEN << "has index.html configuration test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "has index.html configuration test failed 😱" << RESET << std::endl;
}

void    test_rootConf(ServerFill &fill)
{
    conf    &servers = fill.servers.servers;
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot / intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "2 roots test");
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    fill.parseTokens();
    if (!servers[0].empty() && inMap(servers[0], "root"))
        std::cout << BOLDGREEN << "has root key configuration test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "has root key configuration test failed 😱" << RESET << std::endl;
    if (!servers[0].empty() && inMap(servers[0], "root") && servers[0]["root"] == fill.servers.getPwd() + "/" + "intra/YoupiBanane")
        std::cout << BOLDGREEN << "has root key configuration test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "has root key configuration test failed 😱" << RESET << std::endl;
    fill._conf_tokens[0].first = "listen 333;\nserver_name local_host;\nroot /;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    fill.parseTokens();
    // std::cout << servers[0]["root"] << std::endl;
    if (!servers[0].empty() && inMap(servers[0], "root") && servers[0]["root"] == fill.servers.getPwd()  + "/")
        std::cout << BOLDGREEN << "has root key configuration test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "has root key configuration test failed 😱" << RESET << std::endl;

}
void    test_hostNameConf(ServerFill &fill)
{
    conf    &servers = fill.servers.servers;
    fill._conf_tokens[0].first = "listen 333s;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "Correct server names ");

    if (servers.empty())
        std::cout << BOLDRED << "Empty server configuration test failed 😱" << RESET << std::endl;
    else
        std::cout << BOLDGREEN << "Empty server configuration test passed 😀" << RESET <<std::endl;
    fill._conf_tokens[0].first = "listen 2345;\nserver_name 127.0.0.1 \tlocal_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    positive_essential_try_catch(fill, "multiple  server names ");
    
    fill.parseTokens();
    if (servers.empty() || !inMap(servers[0], "server_name"))
        std::cout << BOLDRED << "hostname found in server test failed 😱" << RESET << std::endl;
    else
        std::cout << BOLDGREEN << "hostname found in server test passed 😀" << RESET <<std::endl;
    if (!servers.empty()  && servers[0]["server_name"] == "127.0.0.1 local_host")
        std::cout << BOLDGREEN << "hostname found in server test passed 😀" << RESET <<std::endl;
    else
        std::cout << BOLDRED << "hostname found in server test failed 😱" << RESET << std::endl;
}
void    test_listenConf(ServerFill &fill)
{
    fill._conf_tokens[0].first = "listen 333s;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "string port ");
    fill._conf_tokens[0].first = "listen -333;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "negative port ");
    fill._conf_tokens[0].first = "listen #333;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "sympol port ");
    fill._conf_tokens[0].first = "listen 23413425365678;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "large port number ");
    fill._conf_tokens[0].first = "listen  234;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    positive_essential_try_catch(fill, "1 normal port");
    fill._conf_tokens[0].first = "listen  2342 234;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "2 ports in same listen");
    fill._conf_tokens[0].first = "listen 234 2342 234;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "2 ports in same listen");
    fill.multiple_ports.clear();
    fill._conf_tokens[0].first = "listen 234; listen 5000;listen 4000;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    positive_essential_try_catch(fill, "3 legal ports in same listen");

    
}

void    test_manyConfs(ServerFill &fill)
{
    fill.servers.servers.clear();
    fill.multiple_ports.clear();
    fill._conf_tokens[0].first = "listen 234; listen 5000;listen 4000;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    positive_essential_try_catch(fill, "3 legal ports in same listen");
    if (fill.multiple_ports.size() != 3)
        std::cout << BOLDRED << "Test size 3 multiple ports exists failed 😱" ;
    else
        std::cout << BOLDGREEN << "3 servers exists test passed 😀" ;
    std::cout << " We have " << fill.multiple_ports.size() << " ports" << RESET << std::endl;
    if (fill.servers.servers.size() != 3)
        std::cout << BOLDRED << "Test 3 servers exists failed 😱" ;
    else
        std::cout << BOLDGREEN << "3 servers exists test passed 😀";
    std::cout << " We have " << fill.servers.servers.size() << " servers" << RESET << std::endl;

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

void    test_emptyEssentials(ServerFill &fill)
{
    fill._conf_tokens[0].first = "";

    try
    {
        fill.parseTokens();
        std::cout << BOLDRED << "Test empty Essentials failed 😱" << RESET << std::endl;

    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << '\n';
    }
    fill._conf_tokens[0].first = "listen 3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
 
    positive_essential_try_catch(fill, "non empty");    
}

void    test_mixSpacesEssentials(ServerFill &fill)
{
    fill._conf_tokens[0].first = "lis\nten             3490;\nserver\n_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "bad new live");
    fill._conf_tokens[0].first = "listen\t3490;\nserver_name 127.0.0.1\tlocal_host;\nroot\tintra/YoupiBanane;\nindex\tyoupi.bad_extension;\nclient_max_body_size 100;";
    positive_essential_try_catch(fill, "horizontal tab");
    fill._conf_tokens[0].first = "listen\v3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    positive_essential_try_catch(fill, "vertical tab");
    fill._conf_tokens[0].first = "listen\t\v\t\t3490;\t\t\t\t\vserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    positive_essential_try_catch(fill, "mix tab and spaces tab");
    fill._conf_tokens[0].first = "listen 3490;server_name 127.0.0.1 local_host;root intra/YoupiBanane;\nindex youpi.bad_extension; client_max_body_size 100;";
    positive_essential_try_catch(fill, "mix tab and spaces tab");
}

void   test_essentialOccurance(ServerFill &fill)
{
    fill._conf_tokens[0].first = "server_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "missed listen");
    fill._conf_tokens[0].first = "listen     3490;root intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "missed servername");
    fill._conf_tokens[0].first = "server_name 127.0.0.1 local_host;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    negative_essential_try_catch(fill, "missed root");
}

void    test_lenEssentials(ServerFill &fill)
{
    fill._conf_tokens[0].first = "listen ";
    negative_essential_try_catch(fill, "Small string");
    fill._conf_tokens[0].first = "listen 3490\nserver name;";
    negative_essential_try_catch(fill, "Small string");
    fill._conf_tokens[0].first = "listen 3490\nserver_name localhost;root /";
    negative_essential_try_catch(fill, "missed semi colon");
    fill._conf_tokens[0].first = "listenn 3490;\nserver_name localhost;root /";
    negative_essential_try_catch(fill, "bad listen word");
    fill._conf_tokens[0].first = "listen 3490;\nserve_name localhost;root /";
    negative_essential_try_catch(fill, "bad server word");
    fill._conf_tokens[0].first = "listen 3490;\nserve_name localhost;root /";
    negative_essential_try_catch(fill, "bad server root");
    fill._conf_tokens[0].first = "listen 3490;\nserver_name localhost;root /; indexx shsj.html";
    negative_essential_try_catch(fill, "bad index root");
    fill._conf_tokens[0].first = "listen 3490;\nserver_name localhost;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad index root");
    fill._conf_tokens[0].first = "listen ;\nserver_name localhost;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    fill._conf_tokens[0].first = "listen 3490;\nserver_name ;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    fill._conf_tokens[0].first = "listen 3490;\nserver_name localhost;root / asdsad saDSA; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    fill._conf_tokens[0].first = "listen 3490;\nserver_namec another localhost;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    fill._conf_tokens[0].first = "listen 3490;\nserver_namec another localhost;listen 300;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    // positive_essential_try_catch(fill, "bad word string");
}

void    negative_essential_try_catch(ServerFill &fill, std::string TestCase)
{
    try
    {
        fill.parseTokens();
        std::cout << BOLDRED << TestCase << " failed 😱" << RESET << std::endl;

    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << '\n';
        std::cout << BOLDGREEN << TestCase << " test passed 😀" << RESET <<std::endl;
    }
}

void    positive_essential_try_catch(ServerFill &fill, std::string TestCase)
{
    try
    {
        fill.parseTokens();
        std::cout << BOLDGREEN << TestCase << " test passed 😀" << RESET <<std::endl;

    }
    catch(const std::exception& e)
    {
        std::cout << BOLDRED << TestCase << " failed 😱" << RESET << std::endl;
        std::cerr << e.what() << '\n';
    }
}

void    test_repeated_port()
{
    std::string second_essential = "listen 3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    tokenized_conf tokenized_server = fill_second_essential(second_essential);
    ServerFill    fill(tokenized_server);
    negative_essential_try_catch(fill, "Duplicated port with same server name");
    second_essential = "listen 3400;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    tokenized_server = fill_second_essential(second_essential);
    ServerFill    filled(tokenized_server);
    positive_essential_try_catch(filled, "non dublicate port port");
    second_essential = "listen 3490;\nserver_name 127.0.0.1 ;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    tokenized_server = fill_second_essential(second_essential);
    ServerFill    filler(tokenized_server);
    negative_essential_try_catch(filler, "dublicate port with duplicate server name");
    second_essential = "listen 3490;\nserver_name defualt server ;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    tokenized_server = fill_second_essential(second_essential);
    ServerFill    f(tokenized_server);
    positive_essential_try_catch(f, "dublicate port with different server name");
}