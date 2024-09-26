//
// Created by stellaura on 23/09/24.
//
#include "function_stack.h"
#include <catch2/catch_all.hpp>
#include <fmt/format.h>

int hanoi_recursive(int n, char from, char to, char aux) {
  if (n==1) {
	fmt::println("Move from {} to {}", from, to);
	return 1;
  }
  auto count1 = hanoi_recursive(n - 1, from, aux, to);
  fmt::println("Move from {} to {}", from, to);
  auto count2 = hanoi_recursive(n - 1, aux, to, from);
  return count1 + count2 + 1;
}

Frame construct_hanoi_frame(int n, char from, char to, char aux, int pc) {
  Frame f("hanoi");

  f.set_variable("n", n);
  f.set_variable("from", from);
  f.set_variable("to", to);
  f.set_variable("aux", aux);
  f.set_variable("pc", pc);
  f.set_variable("c1", 0);
  f.set_variable("c2", 0);

  return f;
}

void hanoi_onestep(FunctionFrameStack &frame_stack) {
  Frame &frame = frame_stack.top();
  // initialize variable
  auto &n = frame.get_variable<int>("n");
  auto &pc = frame.get_variable<int>("pc");
  auto &c1 = frame.get_variable<int>("c1");
  auto &c2 = frame.get_variable<int>("c2");
  auto &from = frame.get_variable<char>("from");
  auto &to = frame.get_variable<char>("to");
  auto &aux = frame.get_variable<char>("aux");

  auto current_pc = pc;
  pc++;
  // switch case for each step
  switch (current_pc) {
  case 1:
	if (n==1) {
	  fmt::println("Move from {} to {}", from, to);
	  frame_stack.return_value = 1;
	  frame_stack.pop();
	}
	break;
  case 2: frame_stack.push(construct_hanoi_frame(n - 1, from, aux, to, 1));
	break;
  case 3: c1 = std::get<int>(frame_stack.return_value);
	break;
  case 4: fmt::println("Move from {} to {}", from, to);
	break;
  case 5: frame_stack.push(construct_hanoi_frame(n - 1, aux, to, from, 1));
	break;
  case 6: c2 = std::get<int>(frame_stack.return_value);
	break;
  case 7: frame_stack.return_value = c1 + c2 + 1;
	frame_stack.pop();
	break;
  }
}

int hanoi_iterative(int n, char from, char to, char aux) {
  FunctionFrameStack frame_stack;
  frame_stack.push(construct_hanoi_frame(n, from, to, aux, 1));
  frame_stack.register_function("hanoi", hanoi_onestep);
  frame_stack.simulate();
  return std::get<int>(frame_stack.return_value);
}

int f(int);
int g(int);
int f(int n) { return (n <= 1) ? 1 : f(n - 1) + g(n - 2) + 1; }
int g(int n) { return (n <= 1) ? 1 : f(n + 1) + g(n - 1) + 1; }

Frame construct_f_frame(int n, int pc) {
  Frame f("f");

  f.set_variable("n", n);
  f.set_variable("pc", pc);
  f.set_variable("c1", 0);
  f.set_variable("c2", 0);

  return f;
}
Frame construct_g_frame(int n, int pc) {
  Frame f("g");

  f.set_variable("n", n);
  f.set_variable("pc", pc);
  f.set_variable("c1", 0);
  f.set_variable("c2", 0);

  return f;
}

void f_onestep(FunctionFrameStack &frame_stack);
void g_onestep(FunctionFrameStack &frame_stack);
void f_onestep(FunctionFrameStack &frame_stack) {
  Frame &frame = frame_stack.top();
  // initialize variable
  auto &n = frame.get_variable<int>("n");
  auto &pc = frame.get_variable<int>("pc");
  auto &c1 = frame.get_variable<int>("c1");
  auto &c2 = frame.get_variable<int>("c2");

  auto current_pc = pc;
  pc++;
  // switch case for each step
  switch (current_pc) {
  case 1:
	if (n <= 1) {
	  frame_stack.return_value = 1;
	  frame_stack.pop();
	}
	break;
  case 2: frame_stack.push(construct_f_frame(n - 1, 1));
	break;
  case 3: c1 = std::get<int>(frame_stack.return_value);
	break;
  case 4: frame_stack.push(construct_g_frame(n - 2, 1));
	break;
  case 5: c2 = std::get<int>(frame_stack.return_value);
	break;
  case 6: frame_stack.return_value = c1 + c2 + 1;
	frame_stack.pop();
  }
}
void g_onestep(FunctionFrameStack &frame_stack) {
  Frame &frame = frame_stack.top();
  // initialize variable
  auto &n = frame.get_variable<int>("n");
  auto &pc = frame.get_variable<int>("pc");
  auto &c1 = frame.get_variable<int>("c1");
  auto &c2 = frame.get_variable<int>("c2");

  auto current_pc = pc;
  pc++;
  // switch case for each step
  switch (current_pc) {
  case 1:
	if (n <= 1) {
	  frame_stack.return_value = 1;
	  frame_stack.pop();
	}
	break;
  case 2: frame_stack.push(construct_f_frame(n + 1, 1));
	break;
  case 3: c1 = std::get<int>(frame_stack.return_value);
	break;
  case 4: frame_stack.push(construct_g_frame(n - 1, 1));
	break;
  case 5: c2 = std::get<int>(frame_stack.return_value);
	break;
  case 6: frame_stack.return_value = c1 + c2 + 1;
	frame_stack.pop();
  }
}

int f_iterative(int n) {
  FunctionFrameStack frame_stack;
  frame_stack.push(construct_f_frame(n, 1));
  frame_stack.register_function("f", f_onestep);
  frame_stack.register_function("g", g_onestep);
  frame_stack.simulate();
  return std::get<int>(frame_stack.return_value);
}
int g_iterative(int n) {
  FunctionFrameStack frame_stack;
  frame_stack.push(construct_g_frame(n, 1));
  frame_stack.register_function("f", f_onestep);
  frame_stack.register_function("g", g_onestep);
  frame_stack.simulate();
  return std::get<int>(frame_stack.return_value);
}

TEST_CASE() {
  SECTION("recursive hanoi") {
	fmt::println("\"recursive hanoi\"");

	fmt::println("3 piece hanoi:");
	REQUIRE(hanoi_recursive(3, 'A', 'B', 'C')==7);

	fmt::println("4 piece hanoi:");
	REQUIRE(hanoi_recursive(4, 'A', 'B', 'C')==15);
  }

  SECTION("iterative hanoi") {
	fmt::println("\"iterative hanoi\"");

	fmt::println("1 piece hanoi:");
	REQUIRE(hanoi_iterative(1, 'A', 'B', 'C')==1);

	fmt::println("2 piece hanoi:");
	REQUIRE(hanoi_iterative(2, 'A', 'B', 'C')==3);

	fmt::println("3 piece hanoi:");
	REQUIRE(hanoi_iterative(3, 'A', 'B', 'C')==7);

	fmt::println("4 piece hanoi:");
	REQUIRE(hanoi_iterative(4, 'A', 'B', 'C')==15);
  }

  SECTION("test function call each other") {
	fmt::println("test function call each other:");
	fmt::println("f(4): {}", f(4));
	fmt::println("g(4): {}", g(4));
	fmt::println("g(9): {}", g(9));
	REQUIRE(f(3)==f_iterative(3));
	REQUIRE(g(3)==g_iterative(3));
	REQUIRE(g(8)==g_iterative(8));
	REQUIRE(f(5)==f_iterative(5));
  }
}