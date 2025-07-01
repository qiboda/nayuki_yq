export module core.misc:singleton;

/// \brief Singleton template class
/// \note: 需要friend声明以及前置声明，具体例子见 `Logger` class
export template <typename T>
class Singleton
{
  protected:
    Singleton()
    {
    }

    virtual ~Singleton()
    {
    }

  private:
    // Even if declare a copy constructor (define it as deleted in the
    // declaration), no move constructor will be declared implicitly. So, I did
    // not delete move constructor explicitly. The assign operator is same with
    // the copy constructor.
    Singleton& operator=( const Singleton& ) = delete;
    Singleton( const Singleton& ) = delete;

  public:
    static inline T& GetInstance()
    {
        // 使用局部静态变量优点：相对于动态申请，保证程序结束时会执行单例的析构函数，且代码更加简洁
        // 但有一些缺点，初始化时机和结束时机不易控制。
        // 使用全局的则可以在在函数调用前初始化和函数调用结束后析构。
        // static T instance;
        return instance;
    }

    static inline T* GetInstancePtr()
    {
        return &GetInstance();
    }

    static T instance;
};

template <typename T>
T Singleton<T>::instance;
