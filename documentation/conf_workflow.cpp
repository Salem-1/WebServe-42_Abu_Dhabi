//Bim Ellah elrahman Elraheem

the breakdown of the conf file as follows:
Yassin
         |
         V 
[server1 , server2, server3]
    /    \
    |     \
essentials \
            \
        [location1, location, location3]
---------------------------------------------- > Ahmed mahdy -> list of essentials, conf file with all options
            |
            |
            V 
        location check and break down



Workflow:
1- Take input (file.conf), output vector_servers< 
                                                std::pair<
                                                            std::string esentials,
                                                            std::vector<std::string> locations>
                                                          >
                                                > 
2-Ahmed Yassin, conf with all options 
    list of errors.
    list of options ---> essentials
        |
        v
    optional
3-Ahmed salem, Ahmed Mahdy  list and use it to convert Yassin parsing to fill server_conf
