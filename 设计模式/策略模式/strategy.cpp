// 税率计算抽象类
class TaxStrategy {
public:
    virtual double Calculate() = 0;
    virtual ~TaxStrategy() {}
};
// 中国的税率
class CNTax : public TaxStrategy {
public:
    virtual double Calculate() {
        // ...
    }
};
// 美国的税率
class USTax : public TaxStrategy {
public:
    virtual double Calculate() {
        // ...
    }
};
// 日本的税率
class JPTax : public TaxStrategy {
public:
    virtual double Calculate() {
        // ...
    }
};
// 税率的计算
class SalesOrder {
private:
    TaxStrategy *strategy;

public:
    SalesOrder(StrategyFactory* strategyFactory) {
        // 通过工厂决定是哪个国家的税率
        this->strategy = strategyFactory->NewStrategy();
    }    
    ~SalesOrder() {
        delete this->strategy;
    }
    double Calculate() {
        // ...
        double val = strategy->Calculate(); // 多态调用
        // ...
        return val;
    }
};