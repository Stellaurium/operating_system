//
// Created by stellaura on 04/03/24.
//

#ifndef _LOGIC_GATE_H_
#define _LOGIC_GATE_H_

#include "logic_basic_.hpp"
#include <stdexcept>
#include <vector>
#include <memory>

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

    // 同时添加输入和输出导线
    void add_both_wire(std::vector<Wire *> input_list, std::vector<Wire *> output_list) {
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

// 可以直接构建 获得门电路的 unique_pointer
template <typename T>
    requires std::is_base_of_v<Gate, std::remove_cv_t<std::remove_reference_t<T>>>
std::unique_ptr<std::remove_cv_t<std::remove_reference_t<T>>> make_gate(std::vector<Wire *> input_list,
                                                                        std::vector<Wire *> output_list) {
    auto ptr = std::make_unique<std::remove_cv_t<std::remove_reference_t<T>>>();
    ptr->add_both_wire(input_list, output_list);
    return ptr; // 直接返回ptr，不需要std::move
}


#endif //_LOGIC_GATE_H_
