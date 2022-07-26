#include <string>
#include <list>
#include <iostream>
#include <string.h>

using namespace std;
// 观察者抽象类
class Observer {
public:
    virtual void Update(const void* data) = 0;
};
// 事件抽象类
class Subject {
public:
    // 增加观察者
    virtual void Attach(Observer* observer) {
        m_observerList.push_back(observer);
    }
    // 去除观察者
    virtual void Detach(Observer* observer) {
        m_observerList.remove(observer);
    }
    // 通知抽象
    virtual void Notify(const void* data) {
        auto iter = m_observerList.begin();
        while(iter != m_observerList.end()) {
            (*iter)->Update(data);
            ++iter;
        }
    }
private:
    // 观察者列表
    list<Observer*> m_observerList; 
};
// 单词分割类，继承事件抽象类
class WordsSplitter : public Subject {
public:
    WordsSplitter(const string& words, char space)
        : m_words(move(words)), m_space(space) {} 
    ~WordsSplitter() {}

    list<string> split() {
        list<string> wordList;
        int len = m_words.length();
        int pre = 0;
        string oneword;
        for(int i=0; i < len; ++i) {
            if(m_words[i] == m_space) {
                if(pre < i) {
                    oneword = move(m_words.substr(pre, i - pre));
                    wordList.push_back(oneword);
                    // 分割出一个单词，通知观察者
                    Notify(oneword.c_str());
                }
                pre = i + 1;
            }
        }
        if(pre < len) {
            oneword = move(m_words.substr(pre, len - pre));
            wordList.push_back(m_words.substr(pre, len - pre));
            // 分割出一个单词，通知观察者
            Notify(oneword.c_str());
        }
        return wordList;
    }
private:
    string m_words;
    char m_space;
};
// 获得一个单词就打印一个单词
class GetOneWordShowOneWord : public Observer {
public:
    virtual void Update(const void* data) {
        cout << (char*) data << endl;
    }
};
// 观察目前解析到第几个单词
class ProgressValue : public Observer {
public:
    ProgressValue() : number(0) {}
    virtual void Update(const void* data) {
        cout << ++number << " : ";
    }
private:
    int number;
};

int main() {

    string words = "one   two three four";
    WordsSplitter splitter(words, ' ');
    ProgressValue progressValue;
    GetOneWordShowOneWord progress;
    splitter.Attach(&progressValue);
    splitter.Attach(&progress);
    list<string> wordsList = splitter.split();

    for(string word : wordsList) {
        cout << word << ' ';
    }
    cout << endl;

    return 0;
}

/*
运行结果：
1 : one
2 : two
3 : three
4 : four
one two three four
*/