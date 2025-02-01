import argparse
import subprocess
import sys

def make_args_gn (build_dir):
    path = f'out/{build_dir}/args.gn'

    args_gn = open (path, "w")
    args_gn.write ('qmake_path = "qmake"\n')

    is_debug = "true" if build_dir == "debug" else "false"
    args_gn.write (f'is_debug = {is_debug}\n')

    args_gn.close ()


def main ():
    parser = argparse.ArgumentParser ()
    parser.add_argument ('-c', dest='c', choices=['release', 'debug'], required=True, nargs=1)

    args = parser.parse_args ()

    build_dir = "".join (args.c)

    out_dir = f'out/{build_dir}'
    exit_code = subprocess.call (['gn', 'gen', out_dir])
    if exit_code != 0:
      sys.exit (exit_code)

    make_args_gn (build_dir)

if __name__ == '__main__':
    main ()
