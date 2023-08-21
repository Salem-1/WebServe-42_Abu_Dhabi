#ifndef TESTER_HPP
# define TESTER_HPP
#include "../../includes/libs.hpp"
#include "../../includes/webserve.hpp"

tokenized_conf  dummy_conf_tokens();
void            visualize_tokens(tokenized_conf &tokens);
void            test_emptyEssentials(ServerFill &fill);
void            negative_essential_try_catch(ServerFill &fill, std::string TestCase);
void            test_lenEssentials(ServerFill &fill);
void            positive_essential_try_catch(ServerFill &fill, std::string TestCase);
void            test_mixSpacesEssentials(ServerFill &fill);
void            test_essentialOccurance(ServerFill &fill);
void            test_listenConf(ServerFill &fill);
void            test_hostNameConf(ServerFill &fill);
void            test_rootConf(ServerFill &fill);
void            test_indexConf(ServerFill &fill);
void            test_bodySizeConf(ServerFill &fill);
void            test_manyConfs(ServerFill &fill);
void            test_repeated_port();
tokenized_conf  fill_second_essential(std::string second_essential);
void            test_Essentials();
void            test_lcation_firstLine();
tokenized_conf  dummy_location_fill(std::string location);
void            test_location_root();
void            one_location_test(
                    std::string location_case, std::string msg, std::string sign);
void            cmp_strings(std::string left, std::string right, std::string msg);
void            cmp_bool(bool left, bool right, std::string msg);
void            test_index();
void            test_autoindex();
void            test_methods();
void            test_error_page();
void            test_redirection();
void            test_cgi_bin();
void            test_bodySize_location();
void            test_cgi_extentions_location();
void            non_sensedirective_location();
void            test_no_root_no_location_root();
void            test_multiple_servers();
void            test_DELETE_path();
void            test_root_index();
void            test_intra_config();
void            test_dummy_intra_fill_config();
void            check_redirection_out();
void            cmp_configs(conf &parsed_conf, conf &standards_conf);
#endif  