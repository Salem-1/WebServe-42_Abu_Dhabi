#!/usr/bin/python3
# import os
import os
from pprint import pprint
import secrets

my_list = ['red', 'blue', 'green', 'purple', 'orange', 'teal', 'gray', 'brown', 'magenta']
color = secrets.choice(my_list)
print(f'''Set-Cookie: bgColor={color}; Path=/ \r\n\r\n''')


def print_env_variables():
    env_vars = os.environ
    pprint(dict(env_vars), width=1)

if __name__ == "__main__":
    print_env_variables()