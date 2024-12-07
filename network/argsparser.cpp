#include <stdio.h>
#include "argsparser.h"

argsparser::argsparser (int argc, char *argv[])
{
  m_argc = argc;
  m_argv = argv;
}

error argsparser::parse ()
{
  if (m_argc == 1)
    m_args.m_total_threads = 1;

  if (m_argc >= 2 && sscanf (m_argv[1], "%d", &m_args.m_total_threads) != 1)
    return error ("Invalid parameters!");

  return error (OK);
}

int argsparser::total_threads () const
{
  return m_args.m_total_threads;
}
