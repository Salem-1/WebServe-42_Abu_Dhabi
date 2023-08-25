#!/usr/bin/python3
# import os
import os
from pprint import pprint
print("Set-Cookie: bgColor=red\r\n\r\n")


def print_env_variables():
    env_vars = os.environ
    pprint(dict(env_vars), width=1)

if __name__ == "__main__":
    print_env_variables()