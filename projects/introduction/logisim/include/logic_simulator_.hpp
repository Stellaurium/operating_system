//
// Created by stellaura on 29/02/24.
//

#ifndef _LOGIC_SIMULATOR__HPP_
#define _LOGIC_SIMULATOR__HPP_

#include "logic_basic_.hpp"
#include "logic_gate_.hpp"
#include <chrono>           // 对于std::chrono::milliseconds
#include <concepts>         // 对于概念，比如requires、std::same_as
#include <initializer_list> // 对于std::initializer_list
#include <iterator>         // 对于std::begin, std::end
#include <limits>           // 对于std::numeric_limits
#include <memory>           // 对于std::unique_ptr
#include <thread>           // 对于std::this_thread::sleep_for
#include <type_traits>      // 对于std::is_base_of_v, std::is_invocable_v, std::invoke_result_t
#include <utility>          // 可能需要，如果使用std::move

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
void start_simulation_with_container(FlipFlopContainer &flip_flop_devices, GateContainer &gate_devices, Func func,
                      std::size_t millisecond = 1000,
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
        std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
        max_iter++;
    }
}

// 将所有的细节包裹起来，允许用户仅仅指明添加的类型，然后以及其参数
// 这层封装的意义不大 用起来区别并不明显
class LogicSimulator {
  public:
    Wire *array_ptr{};
    explicit LogicSimulator(std::size_t number) : number_of_wire(number) { array_ptr = new Wire[number_of_wire]; };
    ~LogicSimulator() { delete[] array_ptr; }

    // 触发器的版本
    template <typename T, typename... Args>
        requires(std::is_integral_v<Args> && ...)
    // 没有管用户输入的是不是负数
    void add_flip_flop(Args... args) {
        flip_flop_devices.push_back(std::make_unique<T>(&array_ptr[args]...));
    }

    // 门电路的版本
    template <typename T>
        requires(std::is_base_of_v<Gate, T> )
    void add_gate(std::vector<std::size_t> input_list, std::vector<std::size_t> output_list) {
        gate_devices.push_back(make_gate<T>(get_wire_pointers(input_list), get_wire_pointers(output_list)));
    }

    template<typename Func>
    void start_simulation(Func func){
        start_simulation_with_container(flip_flop_devices, gate_devices, func);
    }

  private:
    std::size_t number_of_wire;
    std::vector<std::unique_ptr<Device>> flip_flop_devices;
    std::vector<std::unique_ptr<Device>> gate_devices;

    template <typename Int>
        requires std::is_integral_v<Int>
    [[nodiscard]] std::vector<Wire *> get_wire_pointers(const std::vector<Int> &input_list) {
        std::vector<Wire *> result;
        for (const auto &index : input_list) {
            result.push_back(&array_ptr[index]);
        }
        return result;
    }
};

#endif //_LOGIC_SIMULATOR__HPP_
