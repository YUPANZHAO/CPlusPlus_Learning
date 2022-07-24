class MessageImp;

class Message {
protected:
    MessageImp* messageImp;
public:
    Message(MessageImp *Imp) : messageImp(Imp) {}
    
    virtual void Login(string username, string password) = 0;
    virtual void SendMessage(string message) = 0;
    virtual void SendPicture(Image image) = 0;

    virtual ~Message() {} 
};

class MessageImp {
public:
    virtual void PlaySound() = 0;
    virtual void DrawShape() = 0;
    virtual void WriteText() = 0;
    virtual void Connect() = 0;
    
    virtual ~MessageImp() {}
};

// 平台实现
class PCMessageImp : public MessageImp {
public:
    virtual void PlaySound() {
        // ...
    }
    virtual void DrawShape() {
        // ...
    }
    virtual void WriteText() {
        // ...
    }
    virtual void Connect() {
        // ...
    }
};

class MobelMessageImp : public MessageImp {
public:
    virtual void PlaySound() {
        // ...
    }
    virtual void DrawShape() {
        // ...
    }
    virtual void WriteText() {
        // ...
    }
    virtual void Connect() {
        // ...
    }
};

// 业务抽象
class MessageLite : public Message {
public:
    MessageLite(MessageImp *Imp) : Message(Imp) {}

    virtual void Login(string username, string password) {
        messageImp->Connect();
        // ...
    }
    virtual void SendMessage(string message) {
        messageImp->WriteText();
        // ...
    }
    virtual void SendPicture(Image image) {
        messageImp->DrawShape();
        // ...
    }
};

class MessagePerfect : public Message {
public:
    MessagePerfect(MessageImp *Imp) : Message(Imp) {}

    virtual void Login(string username, string password) {
        // ...
        messageImp->PlaySound();
        // ...
        messageImp->Connect();
        // ...
    }
    virtual void SendMessage(string message) {
        // ...
        messageImp->PlaySound();
        // ...
        messageImp->WriteText();
        // ...
    }
    virtual void SendPicture(Image image) {
        // ...
        messageImp->PlaySound();
        // ...
        messageImp->DrawShape();
        // ...
    }
};

int main() {

    // 运行时装配
    MessageImp *mImp = new PCMessageImp();
    Message *m = new MessagePerfect(mImp);

    /**
     * 假设有n个不同的平台实现，和m种不同的业务抽象
     * 那么通过桥模式，在运行时再对平台和业务进行组合
     * 就能够在同样有n*m种组合的情况下，减少类的数量
     * 
     * 其原理是通过把继承转换成组合，利用虚方法实现多态调用，就能够实现业务的组合
     */

    return 0;
}