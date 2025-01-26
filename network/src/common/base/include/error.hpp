#include <string>

#pragma once

#define RETURN_IF_FAIL(FUNC) \
    do                       \
    {                        \
        auto RET = (FUNC);   \
        if (!RET.is_ok())    \
            return RET;      \
    } while(0)

#define OK 0

class error
{
private:
    int error_code = 0;
    std::string message;
    std::string file_name;
    
public:
    error() = default;

    error(const std::string message)
    {
        this->message = message;
        error_code = -1;
    }
    error(const std::string message, const std::string filename)
    {
        this->message = message;
        file_name = filename;
        error_code = -1;
    }
    error (const std::string message, const int error_code)
    {
        this->message = message;
        this->error_code = error_code;
    }
    error (const int error_code)
    {
        this->error_code = error_code;
        message = "";
    }

    error(const error &rhs)
    {
        message = rhs.message;
        file_name = rhs.file_name;
        error_code = rhs.error_code;
    }
    operator std::string () const
    {
        return message;
    }

    int get_error_code() const
    {
        return error_code;
    }

    bool is_ok() const
    {
        return !error_code;
    }

    std::string description () const
    {
      return message;
    }

    const char *description_cstr () const
    {
      return message.c_str ();
    }

    void set_filename(std::string name)
    {
        file_name = name;
    }

    std::string &get_filename()
    {
        return file_name;
    }

    error &operator=(error rhs)
    {
        message = rhs.message;
        error_code = rhs.error_code;
        return *this;
    }
};  
