//
// Created by stellaura on 29/02/24.
//
// #include "catch2/catch_all.hpp"
#include "fmt/format.h"
#include "logisim.hpp"
#include <memory>

void jyy_example() {
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
    start_simulation_with_container(flip_flop_devices, gate_devices, print_func);
}

void eight_numbers_example() {
    // 初始化

    // 容器直接写到最前面 然后往里面放东西就行
    // 触发器，门电路的名字并不重要
    std::vector<std::unique_ptr<Device>> flip_flop_devices;
    std::vector<std::unique_ptr<Device>> gate_devices;

    // 核心导线
    Wire line[10];
    // 触发器
    flip_flop_devices.push_back(std::make_unique<DFlipFlop>(&line[0], &line[1]));
    flip_flop_devices.push_back(std::make_unique<DFlipFlop>(&line[2], &line[3]));
    flip_flop_devices.push_back(std::make_unique<DFlipFlop>(&line[7], &line[6]));
    // 门电路
    // 优雅的传参方式
    gate_devices.push_back(make_gate<NotGate>({&line[1]}, {&line[0]}));
    gate_devices.push_back(make_gate<XorGate>({&line[1], &line[3]}, {&line[2]}));
    gate_devices.push_back(make_gate<NotGate>({&line[3]}, {&line[4]}));
    gate_devices.push_back(make_gate<AndGate>({&line[4], &line[6]}, {&line[5]}));
    gate_devices.push_back(make_gate<XorGate>({&line[1], &line[6]}, {&line[9]}));
    gate_devices.push_back(make_gate<AndGate>({&line[3], &line[9]}, {&line[8]}));
    gate_devices.push_back(make_gate<OrGate>({&line[5], &line[8]}, {&line[7]}));

    // 输出电路
    Wire &q1{line[1]};
    Wire &q2{line[3]};
    Wire &q3{line[6]};

    // bool表达式化简的网站 https://tma.main.jp/logic/index_en.html
    // ~a~c + ac + b
    Monitor a{[&]() { return q2 || !(q1 ^ q3); }};
    // ~b~c + bc + ~a
    Monitor b{[&]() { return !((q3) && (q1 ^ q2)); }};
    // ~b + c + a
    Monitor c{[&]() { return q3 || !(q2) || q1; }};
    // a~bc + ~a~c + ~ab + b~c
    Monitor d{[&]() { return q3 && !q2 && q1 || !q3 && !q1 || !q3 && q2 || q2 && !q1; }};
    // ~a~c + b~c
    Monitor e{[&]() { return !(q1) && (q2 || !(q3)); }};
    // ~b~c + a~b + a~c
    Monitor f{[&]() { return (!(q2) && !(q1)) || (q3 && (!(q2) || !(q1))); }};
    // ~ab + a~b + b~c
    Monitor g{[&]() { return (q2 ^ q3) || (q2 && !(q1)); }};

    // 定义执行函数
    // 每次循环结束要进行的任务
    auto print_func = [&]() {
        fmt::println("A = {}; B = {}; C = {}; D = {}; E = {}; F = {}; G = {};", a, b, c, d, e, f, g);
        //        fmt::println("Q3 = {}; Q2 = {}; Q1 = {};", q3, q2, q1);
        // 一定要刷新以下缓冲区 不然会卡着什么都没有
        fflush(stdout);
    };

    // 执行
    start_simulation_with_container(flip_flop_devices, gate_devices, print_func);
}

// 再封装的这层其实意义不大
void easy_way() {
    LogicSimulator logic_simulator{10};

    logic_simulator.add_flip_flop<DFlipFlop>(0, 1);
    logic_simulator.add_flip_flop<DFlipFlop>(2, 3);
    logic_simulator.add_flip_flop<DFlipFlop>(7, 6);

    logic_simulator.add_gate<NotGate>({1}, {0});
    logic_simulator.add_gate<XorGate>({1, 3}, {2});
    logic_simulator.add_gate<NotGate>({3}, {4});
    logic_simulator.add_gate<AndGate>({4, 6}, {5});
    logic_simulator.add_gate<XorGate>({1, 6}, {9});
    logic_simulator.add_gate<AndGate>({3, 9}, {8});
    logic_simulator.add_gate<OrGate>({5, 8}, {7});

    // 输出电路
    Wire &q1{logic_simulator.array_ptr[1]};
    Wire &q2{logic_simulator.array_ptr[3]};
    Wire &q3{logic_simulator.array_ptr[6]};

    // ~a~c + ac + b
    Monitor a{[&]() { return q2 || !(q1 ^ q3); }};
    // ~b~c + bc + ~a
    Monitor b{[&]() { return !((q3) && (q1 ^ q2)); }};
    // ~b + c + a
    Monitor c{[&]() { return q3 || !(q2) || q1; }};
    // a~bc + ~a~c + ~ab + b~c
    Monitor d{[&]() { return q3 && !q2 && q1 || !q3 && !q1 || !q3 && q2 || q2 && !q1; }};
    // ~a~c + b~c
    Monitor e{[&]() { return !(q1) && (q2 || !(q3)); }};
    // ~b~c + a~b + a~c
    Monitor f{[&]() { return (!(q2) && !(q1)) || (q3 && (!(q2) || !(q1))); }};
    // ~ab + a~b + b~c
    Monitor g{[&]() { return (q2 ^ q3) || (q2 && !(q1)); }};

    auto print_func = [&]() {
        fmt::println("A = {}; B = {}; C = {}; D = {}; E = {}; F = {}; G = {};", a, b, c, d, e, f, g);
        //        fmt::println("Q3 = {}; Q2 = {}; Q1 = {};", q3, q2, q1);
        // 一定要刷新以下缓冲区 不然会卡着什么都没有
        fflush(stdout);
    };

    logic_simulator.start_simulation(print_func);
}

// TEST_CASE("test for jyy example", "[test]") { jyy_example(); }

// TEST_CASE("test for the 0-7 clock circuit which I designed", "[extend]") { eight_numbers_example(); }

int main() {
    easy_way();
    return 0;
}
