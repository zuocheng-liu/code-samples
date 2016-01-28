#ifndef BYTESTREAM_H 
#define BYTESTREAM_H 

#include "Slim/Net/CGI/Definition.h"
#include <Poco/Net/StreamSocket.h> 
#include <istream>
#include <ostream>
#include <streambuf>
using Poco::Net::StreamSocket;
typedef unsigned char BYTE;
typedef std::basic_streambuf<BYTE> byte_streambuf;
typedef std::basic_istream<BYTE> byte_istream;
typedef std::basic_ostream<BYTE> byte_ostream;
typedef byte_streambuf::int_type int_type;

class InputStreamBuffer : public byte_streambuf {
    public :
        enum {
            BUFFER_SIZE = 8192 // 0x2000
        };

        InputStreamBuffer(StreamSocket& socket);
        ~InputStreamBuffer();
    protected :
        int_type underflow();
    private :
        char_type _buffer[BUFFER_SIZE];
        StreamSocket& _socket;
}; // end class InputStreamBuffer

class OutputStreamBuffer : public byte_streambuf {
    public :
        enum {
            BUFFER_SIZE = 8192 // 0x2000
        };

        OutputStreamBuffer(StreamSocket& socket);
        ~OutputStreamBuffer();
    protected :
        int_type overflow(int_type c);
        int sync();
    private :
        int flushBuffer();
        char_type _buffer[BUFFER_SIZE];
        StreamSocket& _socket;
}; // end class OutputStreamBuffer

class InputStream : public byte_istream {
    public:
        InputStream(InputStreamBuffer& buf);
        InputStream(InputStreamBuffer* buf);
        ~InputStream();
        
        InputStream& getline(char_type* s, std::streamsize n, char_type delim);
        InputStream& getline(char_type* s, std::streamsize n);
        InputStream& getline(ByteArray& s, std::streamsize n);
        InputStream& getline(char* s, std::streamsize n);
        
        template <class C>
        InputStream& operator>> (C& val) {
            return getline((char_type*)&val, sizeof(C));
        }

}; // end class InputStream

class OutputStream : public byte_ostream {
    public:
        OutputStream(OutputStreamBuffer& buf);
        OutputStream(OutputStreamBuffer* buf);
        ~OutputStream();
        
        OutputStream& write(const ByteArray& s, std::streamsize n);
        OutputStream& write(const char_type* s, std::streamsize n);
        OutputStream& write(const char* s, std::streamsize n);
        
        template <class C>
        OutputStream& operator<< (const C& val) {
            return write((char_type*)&val, (std::streamsize)sizeof(C));
        }
}; // end class OutputStream

#endif 
