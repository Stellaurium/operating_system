//
// Created by stellaura on 04/03/24.
//

#ifndef _LOGIC_BASIC__HPP_
#define _LOGIC_BASIC__HPP_

// 只有一个bool值的导线
// 事实上就是电路中的一个点 (导线上的所有电压都一样 抽象成一个点)
class Wire {
  public:
    bool signal;
    explicit Wire(bool b) : signal(b){};
    Wire() : Wire(false){};
    Wire &operator=(const Wire &right) = default;
    bool operator==(const Wire &right) const = default;

    // 重载逻辑运算符 方便构造 monitor
    Wire operator&&(const Wire &right) const { return Wire(this->signal && right.signal); }
    Wire operator||(const Wire &right) const { return Wire(this->signal || right.signal); }
    Wire operator!() const { return Wire(!this->signal); }
    Wire operator^(const Wire &right) const { return Wire(this->signal != right.signal); }
};


// 抽象类，表示所有电路设备的共同基类
class Device {
  protected:
    //
    bool is_state_changed = true;

  public:
    virtual void update() = 0;   // 纯虚函数，强制要求子类实现
    virtual ~Device() = default; // 虚析构函数，确保子类能正确析构

    [[nodiscard]] bool has_state_changed() const { return is_state_changed; }
};


// monitor，仅仅用来逻辑上监视电路，但是不执行任何的实际功能
// 传入一个 lambda 函数，函数打印的时候会返回 lambda表达式当前的值
// 从而完成了用户可以在外部定义 monitor 的功能
// 返回的类型是 Wire 统一 省的重写format了
template <typename Func>
// 两者等价 但是下面的更短
//    requires std::is_invocable_v<Func> && std::is_same_v<std::invoke_result_t<Func>, Wire>
    requires std::is_invocable_r_v<Wire, Func>
class Monitor {
  private:
    Func func;

  public:
    explicit Monitor(Func &&func_) : func(std::move(func_)) {}
    explicit Monitor(Func &func_) : func(func_) {}
    Wire operator()() const { return func(); }
};


#endif //_LOGIC_BASIC__HPP_
