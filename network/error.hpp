#include <string>

#pragma once

#define RETURN_IF_FAIL(err) \
    if (!(err).is_ok())     \
    {                       \
        return err;         \
    }

#define OK 0

class error
{
private:
    int error_code = 0;
    std::string message;
    std::string file_name;
    
public:
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
    operator std::string () const
    {
        return message;
    }

    const int get_error_code() const
    {
        return error_code;
    }

    const bool is_ok() const
    {
        return !error_code;
    }

    void set_filename(std::string name)
    {
        file_name = name;
    }

    const std::string &get_filename()
    {
        return file_name;
    }
};  
