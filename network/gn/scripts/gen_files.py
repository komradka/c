import os
import argparse
import subprocess
import sys

def make_gen_dirs (path):
    for dir_name in path:
        directory = os.path.dirname (dir_name)
        if not os.path.exists (directory):
            os.makedirs (directory)


def main ():
    parser = argparse.ArgumentParser ()
    parser.add_argument ('--rule', dest='rule', required=True, nargs=1)
    parser.add_argument ('--output', dest='output', required=True, nargs=1)

    args, unknownargs = parser.parse_known_args ()

    if args:
        rule = args.rule
        if unknownargs:
            rule += unknownargs

    make_gen_dirs (args.output)

    exit_code = subprocess.call (rule, stdin=None, stderr=None)
    if exit_code != 0:
      sys.exit (exit_code)


if __name__ == '__main__':
    main ()
