#ifndef FILE_H
#define FILE_H


#include <eventlog/eventlog.h>

class file
{
    public:
        file();
        ~file();

        bool initFile();
        bool testWriteFile();
        bool closeFile();
        void writeTelem();
        void writeError();


    protected:

    private:
};

#endif // FILE_H