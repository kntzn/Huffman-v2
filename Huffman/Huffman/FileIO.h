#pragma once
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

class FileIO
    {
    private:
        // Filename
        std::string filename;
        // File Handler
        int fileHandler = -1;
        // File Size
        unsigned long int fileSize = 0;

        // Output flag
        bool output = false;

        // Replace dictionary
        char dict [256] = { };
        bool replFlag = false;

    public:
        FileIO (bool debugOutput = false)
            {
            output = debugOutput;

            for (int i = 0; i < 256; i++)
                dict [i] = i;
            }

        void open (char filename [])
            {
            _sopen_s (&fileHandler, filename, _O_RDWR | _O_CREAT, _SH_DENYNO, _S_IREAD | _S_IWRITE);
            if (fileHandler == -1)
                {
                printf ("Failed to open %s", filename);
                exit (1);
                }

            // File statistics
            struct stat fileStat;
            stat (filename, &fileStat);

            fileSize = fileStat.st_size;

            if (output)
                printf ("%d bytes found\n", fileSize);
            }
        void close ()
            {
            // Closing the file
            _close (fileHandler);
            fileSize = 0;
            }

        void load (char* &buffer, long int &bufSize)
            {
            buffer = nullptr;
            buffer = (char*) calloc (fileSize, sizeof (int));
            assert (buffer != nullptr);

            // Reading file
            if ((bufSize = _read (fileHandler, buffer, fileSize)) == -1)
                {
                switch (errno)
                    {
                    /*
                    case EBADF:
                    perror ("Bad file descriptor!");
                    break;
                    case ENOSPC:
                    perror ("No space left on device!");
                    break;
                    case EINVAL:
                    perror ("Invalid parameter: buffer was NULL!");
                    break;
                    */
                    default:
                        perror ("Unexpected error!");
                        break;
                    }
                }
            else if (output)
                printf ("Read %u bytes from file\n", bufSize);

            fileSize = bufSize;
            }
        void save (const char* buffer, long int bufSize)
            {
            // Writing buffer's data to file
            int writtenToFile = _write (fileHandler, buffer, bufSize);
            if (writtenToFile == -1)
                {
                switch (errno)
                    {
                    /*case EBADF:
                    perror ("Bad file descriptor");
                    break;
                    case ENOSPC:
                    perror ("No space left");
                    break;
                    case EINVAL:
                    perror ("Invalid parameter");
                    break;*/
                    default:
                        perror ("Unexpected error");
                    }
                }
            else if (output)
                printf ("Wrote %d bytes to file\n", writtenToFile);
            }

        void fastLoad (char filename [], char* &buffer, long int &bufSize)
            {
            open (filename);
            load (buffer, bufSize);
            close ();
            }
        void fastSave (char filename [], const char* buffer, long int bufSize)
            {
            open (filename);
            save (buffer, bufSize);
            close ();
            }
    };