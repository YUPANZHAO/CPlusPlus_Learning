class Stream {
public:
    virtual char* Read(int number) = 0;
    virtual void Seek(int position) = 0;
    virtual void Write(char* buf, int number) = 0;

    virtual ~Stream() = 0;
};

class FileStream : public Stream {
public:
    virtual char* Read(int number) {
        // ...
    }
    virtual void Seek(int position) {
        // ...
    }
    virtual void Write(char* buf, int number) {
        // ...
    }
    virtual ~FileStream() {
        // ...
    }
};

class NetworkStream : public Stream {
public:
    virtual char* Read(int number) {
        // ...
    }
    virtual void Seek(int position) {
        // ...
    }
    virtual void Write(char* buf, int number) {
        // ...
    }
    virtual ~NetworkStream() {
        // ...
    }
};

class MemoryStream : public Stream {
public:
    virtual char* Read(int number) {
        // ...
    }
    virtual void Seek(int position) {
        // ...
    }
    virtual void Write(char* buf, int number) {
        // ...
    }
    virtual ~MemoryStream() {
        // ...
    }
};

class DecoratorStream : public Stream {
protected:
    Stream* stream;
public:
    DecoratorStream(Stream* stream) : stream(stream) {}
    virtual ~DecoratorStream() {}
};

class CryptoStream : public DecoratorStream {
public:
    CryptoStream(Stream* stream) : DecoratorStream(stream) {}
    virtual char* Read(int number) {
        // ...
        stream->Read(number);
        // ...
    }
    virtual void Seek(int position) {
        // ...
        stream->Seek(position);
        // ...
    }
    virtual void Write(char* buf, int number) {
        // ...
        stream->Write(buf, number);
        // ...
    }
};

class BufferedStream : public DecoratorStream {
public:
    BufferedStream(Stream* stream) : DecoratorStream(stream) {}
    virtual char* Read(int number) {
        // ...
        stream->Read(number);
        // ...
    }
    virtual void Seek(int position) {
        // ...
        stream->Seek(position);
        // ...
    }
    virtual void Write(char* buf, int number) {
        // ...
        stream->Write(buf, number);
        // ...
    }
};

class CryptoBufferedStream : public DecoratorStream {
public:
    CryptoBufferedStream(Stream* stream) : DecoratorStream(stream) {}
    virtual char* Read(int number) {
        // ...
        stream->Read(number);
        // ...
    }
    virtual void Seek(int position) {
        // ...
        stream->Seek(position);
        // ...
    }
    virtual void Write(char* buf, int number) {
        // ...
        stream->Write(buf, number);
        // ...
    }
};

int main() {

    FileStream f1;

    CryptoStream f2(&f1);

    BufferedStream f3(&f2);

    CryptoBufferedStream f4(&f1);

    return 0;
}