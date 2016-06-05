#include "ByteStream.h"

InputStreamBuffer::InputStreamBuffer(StreamSocket& socket) : _socket(socket) {
    setg(_buffer, _buffer, _buffer);
}

InputStreamBuffer::~InputStreamBuffer() {
}

int_type InputStreamBuffer::underflow() {
    int len = _socket.receiveBytes(_buffer, BUFFER_SIZE);
    if (len <= 0) {
        return traits_type::eof();
    }
    setg(_buffer, _buffer, _buffer + len);
    return *gptr();
}

// InputStreamBuffer ends

OutputStreamBuffer::OutputStreamBuffer(StreamSocket& socket) : _socket(socket) {
    setp(_buffer, _buffer + BUFFER_SIZE);
}

OutputStreamBuffer::~OutputStreamBuffer() {
    sync();
}

int_type OutputStreamBuffer::overflow(int_type c) {
    if (flushBuffer() < 0) {
        return traits_type::eof();
    }
    return c;
}

int OutputStreamBuffer::sync() {
    if (flushBuffer() < 0) {
        return -1;
    }
    return 0;
}

int OutputStreamBuffer::flushBuffer() {
    int len = pptr() - pbase();
    int count = _socket.sendBytes(_buffer, len);
    if (count <= 0) {
        return -1;
    }
    pbump(-len);
    return len;
}

// OutputStreamBuffer ends

InputStream::InputStream(InputStreamBuffer& buf) : byte_istream(&buf) {
}

InputStream::InputStream(InputStreamBuffer* buf) : byte_istream(buf) {
}


InputStream::~InputStream() {
}

InputStream& InputStream::getline(char_type* s, std::streamsize n, char_type delim) {
    std::streamsize count  = 0;
    __streambuf_type* sb = this->rdbuf();
    const int_type eof = traits_type::eof();
    const int_type idelim = traits_type::to_int_type(delim);
    int_type byte = sb->sgetc();
    while (count + 1 < n 
            && !traits_type::eq_int_type(byte, eof)
            && !traits_type::eq_int_type(byte, idelim)) {
        *s++ = traits_type::to_char_type(byte);
        ++ count;
        byte = sb->snextc();
    }
    return *this;
}

InputStream& InputStream::getline(char_type* s, std::streamsize n) {
    std::streamsize count  = 0;
    __streambuf_type* sb = this->rdbuf();
    int_type byte = sb->sgetc();
    const int_type eof = traits_type::eof();
    while (count + 1 < n && !traits_type::eq_int_type(byte, eof)) {
        *s++ = traits_type::to_char_type(byte);
        ++ count;
        byte = sb->snextc();
    }     
    return *this;
}

InputStream& InputStream::getline(ByteArray& s, std::streamsize n) {
    char_type* p = const_cast<char_type *>(s.c_str());
    return getline(p, n);
}

InputStream& InputStream::getline(char* s, std::streamsize n) {
    char_type* p = (char_type*)s;
    return getline(p, n);
}
/* InputStream ends */

OutputStream::OutputStream(OutputStreamBuffer& buf) : byte_ostream(&buf) {
}

OutputStream::OutputStream(OutputStreamBuffer* buf) : byte_ostream(buf) {
}

OutputStream::~OutputStream() {
}

OutputStream& OutputStream::write(const ByteArray& s, std::streamsize n) {
    char_type* p = const_cast<char_type *>(s.c_str());
    int size = s.size();
    std::streamsize len = size < n ? size : n;
    return write(p, len);
}

OutputStream& OutputStream::write(const char_type* s, std::streamsize n) {
    byte_ostream::write(s, n);
    return *this;
}

OutputStream& OutputStream::write(const char* s, std::streamsize n) {
    char_type* p = (char_type*)s;
    return write(s, n);
}
/* OutputStream ends */
