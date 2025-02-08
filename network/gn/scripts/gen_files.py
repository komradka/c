import os
import argparse
import subprocess
import sys

def update_file (content, filename):
    file = open (filename, "a+")
    file.seek (0)
    if content != file.read ():
        file.truncate (0)
        file.write (content)
    file.close ()

def make_gen_dirs (path):
        directory = os.path.dirname (path)
        if not os.path.exists (directory):
            os.makedirs (directory)

def get_tmp_output (output):
    return f'{output}_tmp'

def main ():
    parser = argparse.ArgumentParser ()
    parser.add_argument ('--rule', dest='rule', required=True, nargs=1)
    parser.add_argument ('--output', dest='output', required=True)

    args, unknownargs = parser.parse_known_args ()

    if args:
        rule = args.rule
        if unknownargs:
            rule += unknownargs

    make_gen_dirs (args.output)

    tmp_output = get_tmp_output (args.output)
    exit_code = subprocess.call (rule + ["-o", tmp_output], stdin=None, stderr=None)
    if exit_code != 0:
      sys.exit (exit_code)

    tmp_file = open (tmp_output, "r")
    update_file (tmp_file.read (), args.output)

    tmp_file.close ()
    os.remove (tmp_output)

if __name__ == '__main__':
    main ()
