//
// Created by stellaura on 04/03/24.
//

#ifndef _LOGIC_FORMAT__H_
#define _LOGIC_FORMAT__H_


#include "fmt/format.h"
#include "logic_basic_.hpp"

// Wire 的fotmat的格式化
template <>
struct fmt::formatter<Wire> : fmt::formatter<int> {
    // 使用基础类型 bool 的格式化器作为基础
    template <typename FormatContext>
    auto format(const Wire &w, FormatContext &ctx) -> decltype(ctx.out()) {
        // 直接使用 bool 的格式化功能
        return fmt::formatter<int>::format(w.signal, ctx);
    }
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



#endif //_LOGIC_FORMAT__H_
