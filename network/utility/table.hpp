#pragma once

template <typename T>
class table
{
private:
    T *t;

    unsigned int col;
    unsigned int row;

public:
    table(unsigned int col, unsigned int row)
    {
        this->col = col;
        this->row = row;

        t = new T[col * row];
    }

    ~table()
    {
        delete[] t;
    }

    T &get_element(unsigned int i, unsigned int j)
    {
        return t[i * col + j];
    }
};