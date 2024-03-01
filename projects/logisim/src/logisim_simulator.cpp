//
// Created by stellaura on 29/02/24.
//
#include "catch2/catch_all.hpp"
#include "fmt/format.h"
#include "logic_simulator_.h"
#include <memory>

// int main() {
TEST_CASE("test for jyy example","[test]") {
    //  初始化

    // 电路的核心部分
    // 需要的导线
    Wire line1, line2, line3, line4, line5;
    // 连接电路
    DFlipFlop flip_flop_1{&line1, &line2};
    DFlipFlop flip_flop_2{&line5, &line3};
    NotGate not_gate{};
    not_gate.add_input(&line2);
    not_gate.add_output(&line4);
    AndGate and_gate{};
    and_gate.add_input(&line3, &line4);
    and_gate.add_output(&line1);
    NOrGate nor_gate{};
    nor_gate.add_input(&line2, &line3);
    nor_gate.add_output(&line5);

    // 最终的输出结果的导线 只连接一端
    Wire a, b, d, e;
    // 变量声明修饰符跟着变量走!!!
    // 为了减轻大脑🧠负担 一行一个!!!
    Wire &c{line4};
    Wire &f{line5};
    Wire &g{line2};
    b = Wire(true);
    NotGate finial_not_gate{};
    finial_not_gate.add_input(&line3, &line3, &line3);
    finial_not_gate.add_output(&a, &d, &e);

    // 将上面的所有设备装到遍历容器中 绝对的抽象化 可扩展性
    // 触发器在一个时钟周期只需要update一次
    // 但是门电路需要一直更新一直更新 直到再也没有更新
    std::vector<Device *> flip_flop_devices;
    flip_flop_devices.push_back(&flip_flop_1);
    flip_flop_devices.push_back(&flip_flop_2);
    std::vector<Device *> gate_devices;
    gate_devices.push_back(&not_gate);
    gate_devices.push_back(&and_gate);
    gate_devices.push_back(&nor_gate);
    gate_devices.push_back(&finial_not_gate);

    // 每次循环结束要进行的任务
    auto print_func = [&]() {
        fmt::println("A = {}; B = {}; C = {}; D = {}; E = {}; F = {}; G = {};", a, b, c, d, e, f, g);
        // 一定要刷新以下缓冲区 不然会卡着什么都没有
        fflush(stdout);
    };

    // 执行代码
    start_simulation(flip_flop_devices, gate_devices, print_func);
}

TEST_CASE("test for the 0-7 clock circuit which I designed","[extend]") {
    // 初始化

    // 容器直接写到最前面 然后往里面放东西就行
    // 触发器，门电路的名字并不重要
    std::vector<std::unique_ptr<Device>> flip_flop_devices;
    std::vector<std::unique_ptr<Device>> gate_devices;

    // 核心部分
    // 核心导线
    Wire line[10];
    // 触发器
    flip_flop_devices.push_back(std::make_unique<DFlipFlop>(&line[0], &line[1]));
    flip_flop_devices.push_back(std::make_unique<DFlipFlop>(&line[2], &line[3]));
    flip_flop_devices.push_back(std::make_unique<DFlipFlop>(&line[7], &line[6]));
    // 门电路
    // 优雅的直接放入类型
    gate_devices.push_back(make_gate<NotGate>({&line[1]}, {&line[0]}));
    gate_devices.push_back(make_gate<XorGate>({&line[1], &line[3]}, {&line[2]}));
    gate_devices.push_back(make_gate<NotGate>({&line[3]}, {&line[4]}));
    gate_devices.push_back(make_gate<AndGate>({&line[4], &line[6]}, {&line[5]}));
    gate_devices.push_back(make_gate<XorGate>({&line[1], &line[6]}, {&line[9]}));
    gate_devices.push_back(make_gate<AndGate>({&line[3], &line[9]}, {&line[8]}));
    gate_devices.push_back(make_gate<OrGate>({&line[5], &line[8]}, {&line[7]}));

    // 输出电路
    // 先不设置任何输出电路 仅仅是检测程序是否正常运行

    // 定义执行函数
    // 每次循环结束要进行的任务
    auto print_func = [&]() {
        fmt::println("Q3 = {}; Q2 = {}; Q1 = {};", line[6], line[3], line[1]);
        // 一定要刷新以下缓冲区 不然会卡着什么都没有
        fflush(stdout);
    };

    // 执行
    start_simulation(flip_flop_devices, gate_devices, print_func);
}