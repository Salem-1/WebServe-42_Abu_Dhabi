#ifndef TESTER_HPP
# define TESTER_HPP
#include "../../includes/libs.hpp"
#include "../../includes/webserve.hpp"

tokenized_conf          dummy_conf_tokens();
void                    visualize_tokens(tokenized_conf &tokens);
void                    test_emptyEssentials(ServerFill &fill);
void                    negative_essential_try_catch(ServerFill &fill, std::string TestCase);
void                    test_lenEssentials(ServerFill &fill);
void                    positive_essential_try_catch(ServerFill &fill, std::string TestCase);
void                    test_mixSpacesEssentials(ServerFill &fill);
void                    test_essentialOccurance(ServerFill &fill);
void                    test_listenConf(ServerFill &fill);
void                    test_hostNameConf(ServerFill &fill);
void                    test_rootConf(ServerFill &fill);
void                    test_indexConf(ServerFill &fill);
void                    test_bodySizeConf(ServerFill &fill);
void                    test_manyConfs(ServerFill &fill);
void                    test_repeated_port();
tokenized_conf          fill_second_essential(std::string second_essential);
#endif