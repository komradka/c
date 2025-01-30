import argparse
import subprocess
import sys

def main ():
    parser = argparse.ArgumentParser ()
    parser.add_argument ('--cmd', dest='cmd', required=True, nargs=1)

    args, unknownargs = parser.parse_known_args ()

    if args:
        cmd = args.cmd
        if unknownargs:
            cmd += unknownargs

    exit_code = subprocess.call (cmd, stdin=None, stderr=None)
    if exit_code != 0:
      sys.exit (exit_code)

if __name__ == '__main__':
    main ()
