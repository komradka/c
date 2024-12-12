#include "error.hpp"

#pragma once
struct cmd_args
{
  int m_total_threads = 0;
};

class argsparser
{
public:
  argsparser (int argc, char *argv[]);
  ~argsparser () = default;

  error parse ();

  int total_threads () const;

private:
  int m_argc;
  char **m_argv;

  cmd_args m_args;
};