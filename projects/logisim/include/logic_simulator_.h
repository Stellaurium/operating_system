//
// Created by stellaura on 29/02/24.
//

#ifndef _LOGIC_SIMULATOR__H_
#define _LOGIC_SIMULATOR__H_

#include "fmt/format.h"
#include <cassert>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

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

// 支持fotmat的格式化 以及打印
template <>
struct fmt::formatter<Wire> : fmt::formatter<int> {
    // 使用基础类型 bool 的格式化器作为基础
    template <typename FormatContext>
    auto format(const Wire &w, FormatContext &ctx) -> decltype(ctx.out()) {
        // 直接使用 bool 的格式化功能
        return fmt::formatter<int>::format(w.signal, ctx);
    }
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

// FlipFlop触发器 (D 触发器)
// Q* = D
class DFlipFlop : public Device {
  public:
    DFlipFlop(Wire *in_, Wire *out_) : in(in_), out(out_), pre_in(*in_), pre_out(*out_) {}
    void update() override {
        out->signal = in->signal;
        if (*in == pre_in && *out == pre_out) {
            is_state_changed = false;
        } else {
            is_state_changed = true;
        }
        pre_in = *in;
        pre_out = *out;
    }

  private:
    Wire *in;
    Wire *out;
    Wire pre_in;
    Wire pre_out;
};

class Gate : public Device {
  private:
    static bool compare_then_update(const std::vector<Wire *> &current, std::vector<Wire> &previous) {
        // 当有差别的时候 返回true
        bool ret_flag = false;
        // 检查大小是否相等
        if (current.size() != previous.size()) {
            ret_flag = true; // 大小不同，肯定不相等
        } else {
            // 比较每个元素
            for (size_t i = 0; i < current.size(); ++i) {
                if (*(current[i]) != previous[i]) {
                    ret_flag = true; // 只要有一个元素不相同，就返回false
                    break;
                }
            }
        }

        // 如果不同，更新previous向量
        if (ret_flag) {
            previous.clear();
            for (auto input : current) {
                previous.push_back(*input);
            }
        }
        return ret_flag;
    }

  protected:
    std::vector<Wire *> inputs;  // 输入线的列表
    std::vector<Wire *> outputs; // 输出线的列表
    std::vector<Wire> pre_inputs;
    std::vector<Wire> pre_outputs;
    void check_changed() {
        // 使用辅助函数比较inputs和pre_inputs, outputs和pre_outputs
        bool inputsChanged = compare_then_update(inputs, pre_inputs);
        bool outputsChanged = compare_then_update(outputs, pre_outputs);

        // 更新is_state_changed标志
        is_state_changed = inputsChanged || outputsChanged;
    }

    // 确保有输入和输出
    bool is_empty() { return inputs.empty() || outputs.empty(); }

  public:
    // 添加输入线
    template <typename... Args>
    void add_input(Args... args) {
        static_assert((std::is_same_v<Args, Wire *> && ...), "All arguments must be of type Wire*");
        (inputs.push_back(args), ...);
    }

    // 添加输出线
    template <typename... Args>
    void add_output(Args... args) {
        static_assert((std::is_same_v<Args, Wire *> && ...), "All arguments must be of type Wire*");
        (outputs.push_back(args), ...);
    }

    // 通过初始化列表 同时添加输入和输出导线
    void add_both_wire(std::initializer_list<Wire *> input_list, std::initializer_list<Wire *> output_list) {
        for (auto input : input_list) {
            inputs.push_back(input);
        }
        for (auto output : output_list) {
            outputs.push_back(output);
        }
    }

    virtual ~Gate() {}
};

// and or nand nor 等都支持多个输入 如果有多个输出，每个输出结果都一样
// 只有最基础的默认构造函数，需要自己手动添加输入输出的节点
class AndGate : public Gate {
  public:
    void update() override {
        if (is_empty())
            return;

        bool result = true;
        for (auto input : inputs) {
            result &= input->signal;
        }
        for (auto output : outputs) {
            output->signal = result;
        }

        check_changed();
    }
};

class OrGate : public Gate {
  public:
    void update() override {
        if (is_empty())
            return;

        bool result = false;
        for (auto input : inputs) {
            result |= input->signal; // 对所有输入进行或操作
        }
        for (auto output : outputs) {
            output->signal = result;
        }

        check_changed();
    }
};

class NAndGate : public Gate {
  public:
    void update() override {
        if (is_empty())
            return;

        bool result = true;
        for (auto input : inputs) {
            result &= input->signal; // 对所有输入进行与操作
        }
        for (auto output : outputs) {
            output->signal = !result; // 对结果取反
        }

        check_changed();
    }
};

class NOrGate : public Gate {
  public:
    void update() override {
        if (is_empty())
            return;

        bool result = false;
        for (auto input : inputs) {
            result |= input->signal; // 对所有输入进行或操作
        }
        for (auto output : outputs) {
            output->signal = !result; // 对结果取反
        }

        check_changed();
    }
};

class XorGate : public Gate {
  public:
    void update() override {
        if (is_empty())
            return;

        bool result = false;
        for (auto input : inputs) {
            result ^= input->signal; // 对所有输入进行异或操作
        }
        for (auto output : outputs) {
            output->signal = result;
        }

        check_changed();
    }
};

class NXorGate : public Gate {
  public:
    void update() override {
        if (is_empty())
            return;

        bool result = false;
        for (auto input : inputs) {
            result ^= input->signal; // 对所有输入进行异或操作
        }
        for (auto output : outputs) {
            output->signal = !result;
        }

        check_changed();
    }
};

class NotGate : public Gate {
  public:
    // 注意：NOT门通常只有一个输入
    // 像缺氧的实现 如果有多个信号的输入，则每个都区反
    // 不过需要要求 inputs outputs 的元素数量相等
    void update() override {
        if (is_empty())
            return;

        // 检查inputs和outputs的数量是否相等
        if (inputs.size() != outputs.size()) {
            throw std::logic_error("The number of inputs and outputs must be equal for NotGate.");
        }

        // 如果数量相等，则对每个输入和对应的输出进行取反操作
        for (size_t i = 0; i < inputs.size(); ++i) {
            outputs[i]->signal = !inputs[i]->signal;
        }

        check_changed();
    }
};

template <typename Container>
concept IterableContainer = requires(Container c) {
    { std::begin(c) } -> std::forward_iterator;
    { std::end(c) } -> std::forward_iterator;
};

template <typename Container>
concept DevicePointerContainer = IterableContainer<Container> && requires {
    typename Container::value_type;
    requires std::same_as<typename Container::value_type, Device *> ||
                 std::same_as<typename Container::value_type, std::unique_ptr<Device>>;
};

// 执行函数 模拟执行电路
template <typename FlipFlopContainer, typename GateContainer, typename Func>
    requires std::is_invocable_v<Func> && std::same_as<std::invoke_result_t<Func>, void> &&
             DevicePointerContainer<FlipFlopContainer> && DevicePointerContainer<GateContainer>
void start_simulation(FlipFlopContainer &flip_flop_devices, GateContainer &gate_devices, Func func,
                      std::size_t second_sleep = 1,
                      std::size_t max_iter_times = std::numeric_limits<std::size_t>::max() - 1) {
    bool anyUpdated = false;
    std::size_t max_iter;
    while (max_iter < max_iter_times) {
        // update flip flop
        for (auto &device : flip_flop_devices) {
            device->update();
        }

        // update gate circuit
        do {
            anyUpdated = false;
            for (auto &device : gate_devices) {
                device->update(); // 多态调用
                if (device->has_state_changed()) {
                    anyUpdated = true;
                }
            }
        } while (anyUpdated);
        func();
        sleep(second_sleep);
        max_iter++;
    }
}

// 添加代码 可以直接构建 制定门电路的 unique_pointer
template <typename T>
    requires std::is_base_of_v<Gate, std::remove_cv_t<std::remove_reference_t<T>>>
std::unique_ptr<std::remove_cv_t<std::remove_reference_t<T>>> make_gate(std::initializer_list<Wire *> input_list,
                                                                        std::initializer_list<Wire *> output_list) {
    auto ptr = std::make_unique<std::remove_cv_t<std::remove_reference_t<T>>>();
    ptr->add_both_wire(input_list, output_list);
    return ptr; // 直接返回ptr，不需要std::move
}

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

// Monitor的format支持
// fmt 的format需要正确的继承和是的format函数 写错了就找不到函数在哪了
template <typename Func>
struct fmt::formatter<Monitor<Func>> : fmt::formatter<Wire> {
    // 使用基础类型 bool 的格式化器作为基础
    template <typename FormatContext>
    auto format(const Monitor<Func> &monitor, FormatContext &ctx) -> decltype(ctx.out()) {
        Wire wire = monitor();
        return fmt::formatter<Wire>::format(wire, ctx);
    }
};

#endif //_LOGIC_SIMULATOR__H_
