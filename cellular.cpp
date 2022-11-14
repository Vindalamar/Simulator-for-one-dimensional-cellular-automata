#include "cellular.hpp"

/* You can put implementations of non-template methods and functions
 * into this file, as you see fit. Do not put anything after the
 * ‹main› function, and make sure its prototype remains untouched.
 * Feel free to add testing code to ‹main›. */

bool compute_rule(int rule, bool left, bool middle, bool right) {
    switch (static_cast<int>(left) * 4 + static_cast<int>(middle) * 2 + static_cast<int>(right))
    {
    case 0b111:
        return  static_cast<bool>(rule & 0b10000000);
    case 0b110:
        return  static_cast<bool>(rule & 0b01000000);
    case 0b101:
        return  static_cast<bool>(rule & 0b00100000);
    case 0b100:
        return  static_cast<bool>(rule & 0b00010000);
    case 0b011:
        return  static_cast<bool>(rule & 0b00001000);
    case 0b010:
        return  static_cast<bool>(rule & 0b00000100);
    case 0b001:
        return  static_cast<bool>(rule & 0b00000010);
    case 0b000:
        return  static_cast<bool>(rule & 0b00000001);
    }
    return false;
}


bool compute_cell( int vertical_rule, int horizontal_rule,
int center_op,
const automaton_state &vertical_state,
const automaton_state &horizontal_state,
int steps ) {
    automaton a(horizontal_rule, vertical_rule, center_op);
    a.horizontal_state = horizontal_state;
    a.vertical_state = vertical_state;
    a._shared = vertical_state.get(0);
    a.run(steps);
    return a.read();
}


int main() {
    automaton_state s;
    s.set( 0, true );
    bool corr = compute_cell( 4, 4, 1, s, s, 2 );
    bool res_cor = 1;
    std::cout << corr;


    automaton_state hs;
    automaton_state vs;
    hs.set( 3, true );
    vs.set( 1, true );
    bool cell = compute_cell( 4, 0, 1, vs, hs, 3 ); 
    bool res_cor2 = 0;
    std::cout << cell;    


    // automaton_state s;
    // s.set( -2, true );
    // s.set( 3, true );
    // bool cell = compute_cell( 2, 2, 1, s, s, 3 );
    // bool res_cor3 = 1;
    // std::cout << cell;    

    // automaton_state s;
    // s.set( 0, true );
    // bool cell = compute_cell( 0, 0, 1, s, s, 1 ); // vr = 0 hr = 0 op = 1 d = 1
    // bool res_cor3 = 1;
    // std::cout << cell;       

    // automaton_state hs;
    // automaton_state vs;
    // hs.set( 3, true );
    // vs.set( 1, true );
    // bool cell = compute_cell( 4, 0, 1, vs, hs, 1 ); // vr = 4 hr = 0 op = 1 d = 1
    // std::cout << cell;
    // bool res_cor4 = 1;                             // cell = false corr = true 

    // automaton_state s;
    // s.set( 0, true );
    // bool cell = compute_cell( 0, 0, 0b1000, s, s, 1 ); // r = 0 d = 1 
    // std::cout << cell;                            // cell = true corr = false 
    // bool res_cor4 = 0; 

    // automaton a( 0, 0, 0b1110 );                       // r = 0 
    // a.set( automaton::horizontal, -1, true );             // x = true 
    // a.set( automaton::horizontal, 0, true );        // y = true 
    // a.set( automaton::horizontal, 1, true );              // z = true 
    // a.step();
    // bool cell = a.read();
    // bool res_cor5 = 0;
    // std::cout << cell;  

    // automaton_state s;
    // s.set( -2, true );
    // s.set( 3, true );
    // bool cell = compute_cell( 0, 0, 1, s, s, 1 ); // vr = 0 hr = 0 op = 1 d = 1
    // bool res_cor6 = 1;
    // std::cout << cell;
}
