#include "../../includes/libs.hpp"
#include "../../includes/webserve.hpp"
#include "tester.hpp"



void    cmp_configs(conf &parsed_conf, conf &standards_conf)
{
    std::set<std::string>   compared;
    std::string             msg;
    int                     i = 0;
    
    for (conf::iterator parsed = parsed_conf.begin()
            ; parsed != parsed_conf.end(); parsed++)
    {
            std::cout << BOLDYELLOW<< "--------------------------------------------------- " << RESET << std::endl;
            std::cout << BOLDYELLOW<< "---------------SERVER <<<" << i << ">>>---------------------- " << RESET << std::endl;
            std::cout << BOLDYELLOW<< "--------------------------------------------------- " << RESET << std::endl;
        for (stringmap::iterator it = parsed->begin()
            ; it != parsed->end(); it++)
        {
            if (inMap(standards_conf[i], it->first))
            {
                msg = "\"" + it->first + "\"" + " : \"" + it->second + "\"" + "\n\"" + it->first + "\"" + " : \"" + standards_conf[i][it->first]+ "\"";          
                cmp_strings(it->second, standards_conf[i][it->first], msg);
                compared.insert(it->first);
            }
            else
            {
                std::cout << RED << it->first  <<  " not in standard 😱" << RESET << std::endl;
                std::cout << RED << "\"" <<it->first  <<  "\" : \"" << it->second << "\"" <<  RESET << std::endl;
                std::cout << BOLDYELLOW<< "--------------------------------------------------- " << RESET << std::endl;
            }

        }

        for (stringmap::iterator standard = standards_conf[i].begin(); standard != standards_conf[i].end(); standard++)
        {
            if (!inSet(compared, standard->first))
            {
                std::cout << BOLDRED << standard->first  <<  " not in our conf 😱" << RESET << std::endl;
                std::cout << BOLDRED << "\"" <<standard->first  <<  "\" : \"" << standard->second << "\"" <<  RESET << std::endl;
                std::cout << BOLDYELLOW<< "--------------------------------------------------- " << RESET << std::endl;
            }
        }
        
        i++;
    }
}
    //                if (left == right)
    //   std::cout << BOLDGREEN << msg  << " test passed 😀" << RESET <<std::endl;
    // else
void    test_dummy_intra_fill_config()
{
    tokenized_conf tokenized_server;
    std::vector<std::string> locations;
     std::string              essentials = "listen 3490;server_name 127.0.0.1  ;root /intra/YoupiBanane;index youpi.bad_extension;\nDELETE_path POST; client_max_body_size 10000000000;";    
    locations.push_back("location /; methods GET   ;");
    locations.push_back("location /directory; \nroot /;\nindex youpi.bad_extension;\n");
    locations.push_back("    location /post_body ;\n methods POST ;\nclient_max_body_size 100;\n");
    locations.push_back("location .bla;index   /../cgi-bin/cgi_tester;\n");
    locations.push_back("location /cgi-bin; root ../intra;");
    locations.push_back("location /put_test; methods PUT;  root /PUT/;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    
    locations.clear();
    essentials = " listen      4444; server_name 127.0.0.1 localhost; root        /intra/YoupiBanane; DELETE_path POST; index       youpi.bad_extension;";    
    locations.push_back("location /;  index   youpi.bad_extension;;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    
    locations.clear();
    essentials = " listen      5555; server_name 127.0.0.1 localhost; root        /intra/website; index       index.html;";    
    locations.push_back("location / ; index   index.html; error_page 404 not_found.html;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
   
    locations.clear();
    essentials = "    listen      4444; server_name defaultserver; DELETE_path POST; root        /intra/website; index       index.html;";    
    locations.push_back("location / ; methods GET DELETE;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    // positive_essential_try_catch(fill, "filled intra config ");
    Config standard;
    cmp_configs(fill.servers.servers, standard.servers);
    // fill.servers.visualize_config();
    // cmp_bool((standard.servers == fill.servers.servers),1  ,"servers are equal");
    // standard.visualize_config();
}

void    test_intra_config()
{
    tokenized_conf tokenized_server;
    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;server_name 127.0.0.1  ;root /intra/YoupiBanane;index youpi.bad_extension;\nDELETE_path POST; client_max_body_size 10000000000;";    
    locations.push_back("location /; methods GET   ;");
    locations.push_back("location /directory; \nroot /;\nindex youpi.bad_extension;\n");
    locations.push_back("    location /post_body ;\n methods POST ;\nclient_max_body_size 100;\n");
    locations.push_back("location .bla;index   /../cgi-bin/cgi_tester;\n");
    locations.push_back("location /cgi-bin; root ../intra;");
    locations.push_back("location /put_test; methods PUT;  root /PUT/;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    // positive_essential_try_catch(fill, "filled intra config ");
    // fill.servers.visualize_config();
    Config standard;
    // cmp_bool((standard.servers == fill.servers.servers),1  ,"servers are equal");
    cmp_configs(fill.servers.servers, standard.servers);
    // standard.visualize_config();
}



void    test_root_index()
{
    tokenized_conf tokenized_server;
    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;\nserver_name 127.0.0.1 local_host;root /Youpibanana;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    
    locations.push_back("location /; methods DELETE   ;");
    locations.push_back("location /put_test/; methods POST     ;\n  root /intra/YoupiBanane/PUT/;");
    locations.push_back("location .bla ;    index /path/to/your/cgi_test_socket.sock;\n");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    essentials = "listen 5555;\nserver_name second_server.com;root /second; \nindex index_second_server.html;\nclient_max_body_size 100;";

    essentials = "listen 5555;\nserver_name second_server.com;root /second; \nindex index_second_server.html;\nclient_max_body_size 100;";
    locations.push_back("location /; methods GET   ;\n index youpi.bad_extension;");
    locations.push_back("location /put_test/; methods PUT     ;\n  root /intra/YoupiBanane/PUT/;");
    locations.push_back("location .bla ;    index /path/to/your/cgi_test_socket.sock;\n");
    locations.push_back("location /post_body; methods POST     ;\n  client_max_body_size 100;client_max_body_size \t\t100;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    ServerFill    fill(tokenized_server);
    positive_essential_try_catch(fill, "root given in the essential or location");
    cmp_strings(fill.servers.servers[0]["/"], fill.servers.servers[0]["root"] + "/" + fill.servers.servers[0]["index"], "Correct root index location");
    cmp_strings(fill.servers.servers[1]["/"], fill.servers.servers[1]["root"] + "/" + fill.servers.servers[1]["index"], "Correct root index location");
   

}
void    test_no_root_no_location_root()
{
    tokenized_conf tokenized_server;

    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;\nserver_name 127.0.0.1 local_host;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    locations.push_back("location /; methods GET   ;\n index youpi.bad_extension;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    ServerFill    fill(tokenized_server);
    negative_essential_try_catch(fill, "no root given in the essential or location");
};

void    test_multiple_servers()
{
    tokenized_conf tokenized_server;

    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;\nserver_name 127.0.0.1 local_host;root /;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    locations.push_back("location /; methods DELETE   ;\n index youpi.bad_extension;");
    locations.push_back("location /put_test/; methods POST     ;\n  root /intra/YoupiBanane/PUT/;");
    locations.push_back("location .bla ;    index /path/to/your/cgi_test_socket.sock;\n");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    essentials = "listen 5555;\nserver_name second_server.com;root /second; \nindex index_second_server.html;\nclient_max_body_size 100;";
    locations.push_back("location /; methods GET   ;\n index youpi.bad_extension;");
    locations.push_back("location /put_test/; methods PUT     ;\n  root /intra/YoupiBanane/PUT/;");
    locations.push_back("location .bla ;    index /path/to/your/cgi_test_socket.sock;\n");
    locations.push_back("location /post_body; methods POST     ;\n  client_max_body_size 100;client_max_body_size \t\t100;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    ServerFill    fill(tokenized_server);
    positive_essential_try_catch(fill, "no root given in the essential or location");
    cmp_strings(fill.servers.servers[0]["/ methods"], "DELETE", "second server has GET method allowed");
    cmp_strings(fill.servers.servers[0]["/put_test/ methods"], "POST", "second server has PUT method allowed");
    cmp_strings(fill.servers.servers[1]["/ methods"], "DELETE", "second server has GET method allowed");
    cmp_strings(fill.servers.servers[1]["/put_test/ methods"], "POST", "second server has PUT method allowed");
    // fill.servers.visualize_config();

}

void    non_sensedirective_location()
{
    one_location_test("location /;  index inedex.html; hala w'3ala;", "Non sense input in location", "-");
    one_location_test("location /;  index inedex.html; nonsense non nonsense;", "Non sense input in location", "-");

}
void    test_cgi_extentions_location()
{
    one_location_test("location .bla;  index;", "index has no value", "-");
    one_location_test("location .bla;  index  /cgi-bin/cgi_tester;", "index value", "+");
    one_location_test("location .bla;  error_page 404 error.html; root /Youbibanana; client_max_body_size 100", "Should have only index", "-");
    tokenized_conf tokenized_server =  dummy_location_fill("location .bla;  index  /cgi-bin/cgi_tester;");
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    cmp_strings(fill.servers.servers[0][".bla"], fill.servers.servers[0]["root"]+ "/cgi-bin/cgi_tester", "correct cgi path");
    // std::cout << fill.servers.servers[0]["root"]+ "/cgi-bin/cgi_tester" << std::endl;
}
void    test_bodySize_location()
{
    one_location_test("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size 100", "healthy client max body", "+");
    one_location_test("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size ", "no number max body", "-");
    one_location_test("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size 1000000000 ", "large max body", "+");
    one_location_test("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size 100dvs", "non numeric client max body", "-");
    one_location_test("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size -100", "negative client max body", "-");
    one_location_test("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size 100 200 ", "2 numbers max body", "-");
    one_location_test("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size 10000000000 ", "overflow max body", "-");
    tokenized_conf tokenized_server =  dummy_location_fill("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size 100000");
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    cmp_strings(fill.servers.servers[0]["/ Max-Body"], "100000", "correct max body 100000");
    tokenized_server =  dummy_location_fill("location /;  error_page 404 error.html; root /Youbibanana; client_max_body_size 1");
    ServerFill    fil(tokenized_server);
    fil.parseTokens();
    cmp_strings(fil.servers.servers[0]["/ Max-Body"], "1", "correct max body 100000");

}

void    test_cgi_bin()
{
    one_location_test("location /cgi-bin;  error_page 404 error.html; root /Youbibanana", "healthy root cgi-bin", "+");
    one_location_test("location /cgi-bin;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection 301 another.html", "non healthy cgi-bin", "-");
    one_location_test("location /cgi-bin;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection 301 another.html", "has redirection cgi-bin", "-");
    one_location_test("location /cgi-bin;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;", "has index cgi-bin", "-");
    one_location_test("location /cgi-bin;  error_page 404 error.html; autoindex on;" , "has autoindex cgi-bin", "-");
    one_location_test("location /cgi-bin;  error_page 404 error.html;", "no root cgi-bin", "-");
    one_location_test("location /cgi-bin;  error_page 404 error.html; root exec.out; root /another", "double root root cgi-bin", "-");
    tokenized_conf tokenized_server =  dummy_location_fill("location /cgi-bin;  error_page 404 error.html; root /Youbibanana");
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    cmp_strings(fill.servers.servers[0]["/cgi-bin"], fill.servers.servers[0]["root"]+ "/Youbibanana", "correct cgi-bin route");
    tokenized_server =  dummy_location_fill("location /cgi-bin;root /; ");
    ServerFill    fil(tokenized_server);
    fil.parseTokens();
    cmp_strings(fil.servers.servers[0]["/cgi-bin"], fil.servers.servers[0]["root"]+ "/", "correct cgi-bin route");

}
void    test_redirection()
{
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection /ransomware attacks/ransom.html  302 ;redirection  /ddos attacks/ddos.html 301;", "healthy redirection params", "+");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection  /ddos attacks/ddos.html 301", "healthy redirection params", "+");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection  another.html", "2 redirection params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection /ddos 301 index index", "4 redirection params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection /ddos 304 another.html 4", "4 redirection params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection  /ddos 3012 another.html", "bad status code redirection params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection  /ddos 30. another.html", "non numeric status code redirection params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on;  error_page 404 error.html;redirection /ddos -30 another.html", "negative status code redirection params", "-");
   
   

}
void    check_redirection_out()
{
    tokenized_conf tokenized_server =  dummy_location_fill("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT;redirection /ransomware attacks/ransom.html  302; redirection  /ddos attacks/ddos.html 301;");
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    cmp_strings(fill.servers.servers[0]["/ redirection"], "/ransomware attacks/ransom.html 302 , /ddos attacks/ddos.html 301", "correct redirection 301");
    tokenized_server =  dummy_location_fill("location /YoupiBanana;root /; index index.html; autoindex on; methods GET POST DELETE PUT;redirection /ransomware attacks/ransom.html  302; redirection  /ddos attacks/ddos.html 301;");
    ServerFill    fil(tokenized_server);
    fil.parseTokens();
    
    cmp_strings(fil.servers.servers[0]["/YoupiBanana redirection"], "/ransomware attacks/ransom.html 302 , /ddos attacks/ddos.html 301", "correct redirection 301");
    tokenized_server =  dummy_location_fill("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT;redirection /ransomware attacks/ransom.html  302; redirection  /ddos attacks/ddos.html 301;");
    ServerFill    filll(tokenized_server);
    filll.parseTokens();
    cmp_strings(filll.servers.servers[0]["/ redirection"], "/ransomware attacks/ransom.html 302 , /ddos attacks/ddos.html 301", "correct redirection 300");
    // std::cout << filll.servers.servers[0]["/ redirection"] << std::endl;
    // std::cout << "/ransomware attacks/ransom.html 302 , /ddos attacks/ddos.html 301" << std::endl;
    tokenized_server =  dummy_location_fill("location /YoupiBanana;root /; index index.html; autoindex on; methods GET POST DELETE PUT;redirection /ransomware attacks/ransom.html  302; redirection  /ddos attacks/ddos.html 301;");
    ServerFill    fi(tokenized_server);
    fi.parseTokens();
    cmp_strings(fi.servers.servers[0]["/YoupiBanana redirection"], "/ransomware attacks/ransom.html 302 , /ddos attacks/ddos.html 301", "correct redirection 301");
}
void    test_error_page()
{
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page 404 error.html;", "healthy error pgae params", "+");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page;", "missed error pgae params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page error.html;", "missed error pgae params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page 40. error.html;", "missed error pgae params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page -40 error.html;", "non numeric error pgae params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page 404 error.html 403;", "missed error pgae params", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page 4044 error.html;", "wrong error pgae num", "-");
    one_location_test("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page 40 error.html;", "missed error pgae params", "-");
    tokenized_conf tokenized_server =  dummy_location_fill("location /;root /YoupiBanan; index index.html; autoindex on; methods GET POST DELETE PUT; error_page 404 error.html");
    ServerFill    fill(tokenized_server);
    fill.parseTokens();
    cmp_strings(fill.servers.servers[0]["/ error_page"], "404 " + fill.servers.servers[0]["root"] +  "/error.html", "correct error page error.html");
    tokenized_server =  dummy_location_fill("location /YoupiBanana;root /; index index.html; autoindex on; methods GET POST DELETE PUT; error_page 403 notfound.html; error_page 500 server_error.html");
    ServerFill    fil(tokenized_server);
    fil.parseTokens();
    cmp_strings(fil.servers.servers[0]["/YoupiBanana error_page"], "403 " + fil.servers.servers[0]["root"] +  "/notfound.html , 500 " + fil.servers.servers[0]["root"] +  "/server_error.html", "correct error page notfound.html");
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
    locations.push_back("location .bla ;    index /path/to/your/cgi_test_socket.sock;\n");
    locations.push_back("location /post_body; methods POST     ;\n  client_max_body_size 100;client_max_body_size \t\t100;");
    locations.push_back("location .bla; index cgi-tester ; ");
    locations.push_back("location /directory/;root /intra/YoupiBanane/;\nindex youpi.bad_extension;\nautoindex on;\nerror_page 404 error_pages/404.html;");
    locations.push_back("location /directory/;root /intra/YoupiBanane/;\nindex youpi.bad_extension;\nautoindex on;\nerror_page 404 error_pages/404.html; redirection /PUT/ /NOT_PUT/ 301 ;");
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


void    test_DELETE_path()
{
    tokenized_conf tokenized_server;
    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    locations.push_back("location /; methods GET   ;\n index youpi.bad_extension;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));

    ServerFill fill(tokenized_server);
    positive_essential_try_catch(fill, "normal server no delete path");
    cmp_strings(fill.servers.servers[0]["DELETE path"], "POST", "default post is there");
    essentials = "listen 4490;\nserver_name 127.0.0.1 local_host; DELETE_path PUT/files;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size 100;";
    locations.push_back("location /; methods GET   ;\n index youpi.bad_extension;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    ServerFill f(tokenized_server);
    positive_essential_try_catch(f, "provided delete path");
    cmp_strings(f.servers.servers[1]["DELETE path"], "PUT/files", "successfully modified delete path");
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