#include <iostream>
#include <vector>
/* The goal of this task is to implement a simulator for
 * one-dimensional cellular automata. You will implement this
 * simulator as a class, the interface of which is described below
 * You are free to add additional methods and data members to the
 * class, and additional classes and functions to the file, as you
 * see fit. You must, however, keep the entire interface in this
 * single file. The implementation can be in either ‹cellular.hpp›
 * or in ‹cellular.cpp›. Only these two files will be submitted. */

/* The class ‹automaton_state› represents the state of a 1D,
 * infinite binary cellular automaton. The ‹set› and ‹get› methods
 * can be passed an arbitrary index. */


bool compute_rule(int rule, bool left, bool middle, bool right);

class automaton_state
{
    std::vector<bool> plus;
    std::vector<bool> minus;
public:
    bool _def = false;
    automaton_state() {
        set(1, false);
        set(-2, false);
    } /* create a blank state (all cells are 0) */
    void set( int index, bool value) {
        if (index >= 0) {
            if (index + 1 >= static_cast<int>(plus.size())) {
                plus.resize(index + 2, _def);
            }
            plus[index] = value;
        }
        else {
            if (abs(index) >= static_cast<int>(minus.size())) {
                minus.resize(abs(index) + 2, _def);
            }
            minus[abs(index) - 1] = value;
        }
    } /* change the given cell */

    bool get(int index ) const {
        if (index >= 0) {
            if(index >= static_cast<int>(plus.size())) {
                return _def;
            }
            return plus[index];
        }
        if(abs(index) >= static_cast<int>(minus.size())) {
                return _def;
        }
        return minus[abs(index) - 1];
    }

    void step(int rule) {
        std::vector<bool> tmp_plus = plus;
        std::vector<bool> tmp_minus = minus;
        bool tmp_def = _def;
        _def = compute_rule(rule, _def, _def, _def);
        set(0, compute_rule(rule, tmp_minus[0], tmp_plus[0], tmp_plus[1]));
        set(-1, compute_rule(rule, tmp_minus[1], tmp_minus[0], tmp_plus[0]));
        set(tmp_plus.size() - 1, compute_rule(rule, tmp_plus[tmp_plus.size() - 2], tmp_plus[tmp_plus.size() - 1], tmp_def));
        set(-tmp_minus.size(), compute_rule(rule, tmp_minus[tmp_minus.size() - 2], tmp_minus[tmp_minus.size() - 1], tmp_def));
        for(size_t i = 1; i < tmp_plus.size() - 1; i++) {
            set(i, compute_rule(rule, tmp_plus[i - 1], tmp_plus[i], tmp_plus[i + 1]));
        }
        for(size_t i = 1; i < tmp_minus.size() - 1; i++) {
            set(-(i + 1), compute_rule(rule, tmp_minus[i + 1], tmp_minus[i], tmp_minus[i - 1]));
        }
    }
};

/* The ‹automaton› class represents the automaton itself. The
 * automaton is arranged as a cross, with a horizontal and a
 * vertical automaton, which are almost entirely independent (each
 * has its own state and its own rule), with one twist: the center
 * cell (index 0 in both automata) is shared. The new state of the
 * shared center cell (after a computation step is performed in both
 * automata independently) is obtained by combining the two values
 * (that either automaton would assign to that cell) using a
 * specified boolean binary operator. The new center is obtained as
 * ‹horizontal_center OP vertical_center›. The state can look, for
 * example, like this:
 *
 *                            ┌───┐
 *                            │ … │
 *                            ├───┤
 *                            │ 1 │
 *                            ├───┤
 *                            │ 0 │
 *                ┌───┬───┬───┼───┼───┬───┬───┐
 *                │ … │ 0 │ 0 │ 1 │ 1 │ 0 │ … │
 *                └───┴───┴───┼───┼───┴───┴───┘
 *                            │ 0 │
 *                            ├───┤
 *                            │ 1 │
 *                            ├───┤
 *                            │ … │
 *                            └───┘
 *
 * The automaton keeps its state internally and allows the user to
 * perform simulation on this internal state. Initially, the state
 * of the automaton is 0 (false) everywhere. The rules for both the
 * vertical and the horizontal component are given to the
 * constructor by their Wolfram code.
 *
 * The center-combining operator is given by the same type of code,
 * but instead of 3 cells, only 2 need to be combined: there are
 * only 16 such operators (compared to 256 rules for each of the
 * automata). The input vectors to the binary operator are numbered
 * by their binary code as:
 *
 *  │  left │ right │ index │
 *  ├┄┄┄┄┄┄┄│┄┄┄┄┄┄┄│┄┄┄┄┄┄┄│
 *  │   0   │   0   │   0   │
 *  │   0   │   1   │   1   │
 *  │   1   │   0   │   2   │
 *  │   1   │   1   │   3   │
 *
 * The operator code is then a 4-digit binary number, e.g 0110 gives
 * the code for ‹xor› (0 0 → 0, 0 1 → 1, 1 0 → 1, 1 1 → 0) while
 * 1000 gives code for ‹and› (everything is zero except if both
 * inputs are 1). And so on and so forth. The same process but with
 * 3 input cells is used to construct the Wolfram code for the
 * automata. */

class automaton
{
    int _h_rule;
    int _v_rule; 
    int _center_rule;
public:
    int _shared;
    automaton_state vertical_state;
    automaton_state horizontal_state;
    enum direction { vertical, horizontal };
    automaton( int h_rule, int v_rule, int center ): _h_rule(h_rule), _v_rule(v_rule), _center_rule(center) {}
    
    bool read() const {
        return _shared;
    }
    
    bool step_center() const {
        switch (horizontal_state.get(0) * 2 + vertical_state.get(0))
        {
        case 0b11:
            return  static_cast<bool>(_center_rule & 0b1000);
        case 0b10:
            return  static_cast<bool>(_center_rule & 0b0100);
        case 0b01:
            return  static_cast<bool>(_center_rule & 0b0010);
        case 0b00:
            return  static_cast<bool>(_center_rule & 0b0001);
        }
        return false;
    }

    void set( direction dir, int index, bool value ) {
        if (index == 0) {
            _shared = value;
        }
        if (dir == vertical) {
            vertical_state.set(index, value);
        }
        else {
            horizontal_state.set(index, value);
        }
    }
    void step() {
        vertical_state.step(_v_rule);
        horizontal_state.step(_h_rule);
        _shared = step_center();
        vertical_state.set(0, _shared);
        horizontal_state.set(0, _shared);
    }
    void run( int steps ) {
        for(int i = 0; i < steps; i++) {
            step();
        }
    }
};

/* The ‹compute_cell› function takes two rule numbers, two initial
 * states, a center operator and a number of steps. It then computes
 * the value of the central cell after n steps of the automaton such
 * described. Like above, the number of steps is a non-negative
 * number. Assume that the center cell in both input states has the
 * same value. */

bool compute_cell( int vertical_rule, int horizontal_rule,
int center_op,
const automaton_state &vertical_state,
const automaton_state &horizontal_state,
int steps );
