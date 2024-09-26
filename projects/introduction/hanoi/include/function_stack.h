//
// Created by stellaura on 23/09/24.
//
#pragma once
#include <map>
#include <stack>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <functional>

class Frame {
private:
  std::string function_name;
  std::map<std::string, std::variant<char, int, double>> variables;

public:
  Frame(std::string &&name) {
	function_name = std::move(name);
  }
  Frame(const std::string &name) {
	function_name = name;
  }

  void set_variable(const std::string &name,
					std::variant<char, int, double> value) {
	variables[name] = value;
  }

  template<typename T>
  T &get_variable(const std::string &name) {
	if (!variables.contains(name)) {
	  throw std::runtime_error("Variable not found");
	}
	T &value = std::get<T>(variables[name]);
	return value;
  }

  auto get_function_name() { return function_name; }
};

class FunctionFrameStack {
private:
  std::stack<Frame> function_stack;
  std::map<std::string, std::function<void(FunctionFrameStack &)>> function_map;

public:
  std::variant<char, int, double> return_value = 0;

  template<typename T> requires std::is_same_v<std::decay_t<T>, Frame>
  void push(T &&frame) {
	function_stack.push(std::forward<T>(frame));
  };

  void pop() {
	function_stack.pop();
  };

  Frame &top() { return function_stack.top(); };

  bool empty() { return function_stack.empty(); }

  //注册外部的函数到该类中，是的这个类可以找到可以执行的代码
  void register_function(const std::string &name,
						 std::function<void(FunctionFrameStack &)> func) {
	function_map[name] = func;
  }

  // 执行当前函数栈里面的内容
  void simulate() {
	while (!this->empty()) {
	  auto &frame = this->top();
	  auto name = frame.get_function_name();
	  if (function_map.contains(name)) {
		function_map[name](*this);
	  }
	  else{
		throw std::runtime_error("Function " + name + " not found");
	  }
	}
  }
};
