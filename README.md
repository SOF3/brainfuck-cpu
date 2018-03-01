# brainfuck-cpu
This repo contains a Logisim schematic file for a programmable circuit implementing a 24-bit-datapath 24-bit-mempath [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) computer.

Structure:
* main: contains everything that functions as a "complete" "computer", including:
  * 1 24-bit ROM: contains 4-bit units of data. In each unit, bits 2:0 are encoded as BFC instructions. See the encoding specification below. Bit 3 is the special "shutdown" signal that hangs the clock.
  * 1 24-bit RAM: contains 8-bit units of data, serving as the memory byte array for the Brainfuck environment.
  * 1 main clock: the clock that regulates the synchronization of the whole system. This implementation should be synchronous
  * 1 stdin ROM: a counter+ROM complex providing the stdin pipe for the Brainfuck environment. It reads bytes from its ROM backend, incrementing the address when the CPU signals that a byte has been read.
  * 1 stdout RAM: a counter+RAM complex providing the stdout pipe for the Brainfuck environment. It appends bytes to its RAM backend.
  * 1 CPU: the instruction processor
* brainfuck/cpu:
  * Inputs:
    * clk (boolean): for synchronization with the main clock
    * instr (instr\_size): current instruction to execute
    * instr\_ptr (datapath\_size): the pointer to the current instruction
    * mem (cell\_size): the value of the current memory cell
    * mem\_ptr (mempath\_size): the pointer to the current memory cell
    * stdin (cell\_size): the stdin value to read, if needed
  * Outputs:
    * instr\_ptr (datapath\_size): the pointer to the next instruction
    * mem\_ptr (mempath\_size): the pointer to the next memory cell
    * writing\_mem (boolean): a boolean indicating whether the current memory cell should be updated to set\_mem (according to the Brainfuck language, an instruction that updates a memory cell would not update the memory pointer, so it is not necessary to distinguish it as the cell _before_ or the cell _after_ the instruction)
    * set\_mem (cell\_size): the value to be set into memory if writing\_mem is 1
    * stdout\_ready (boolean): a boolean indicating whether the "stdout" output should be appended to the stdout pipe
    * stdout (cell\_size): the value to be appended to the stdout pipe if stdout\_ready is 1
    * stdin\_read (boolean): a boolean indicating whether a new cell should be pulled from the stdin pipe, i.e. the current one has been consumed
  * Internal registers:
    * a 6-bit counter used for finding the matching `]` when `[` is executed with memory 0 (i.e. a while loop has broken)
    * a Register24Stack64 subcircuit for storing pointers to previous `[` instructions.
* Register24Stack64:
  * Uses a 6-bit-mempath 24-bit-cell RAM
  * Implements a first-in-last-out "stack" data structure for 24-bit data cells
  * Has 3 valid input combinations:
    * `~push & ~pop`: PEEK action, not modifying the RAM. Outputs the tail entry (last-inserted entry) in the stack.
    * `push & ~pop`: PUSH action, appending insert\_data to the stack. Output is undefined.
    * `~push & pop`: POP action, removing the last-inserted entry from the stack. Output is undefined.
  * Needs formal proof to determine if brainfuck/cpu might experience bugs due to a PUSH/POP action not being able to output the tail value in the same tick. If issues exist, may need doueble-ticking.

### BFC 3-bit encoding
BFC means "Brainfuck compiled". It is a simple mapping of Brainfuck instructions into 3-bit units.
The following bitsets correspond to symbolic instructions in the Brainfuck language:

| Bitset (2:0) | Symbol | Synopsis |
| :----: | :----: | :----: |
| `000` | `>` | Increments the memory pointer |
| `001` | `<` | Decrements the memory pointer |
| `010` | `+` | Increments the memory value |
| `011` | `-` | Decrements the memory value |
| `100` | `.` | Sends the current memory value to stdout |
| `101` | `,` | Consumes the current stdin value and stores it in the current memory value |
| `110` | `[` | If current memory value is 0, skips to the matching `]` behind, which is not necessarily the immediately following one |
| `111` | `]` | If current memory value is not 0, jumps back to the matching `[` before |

### Memory allocation
* boolean: 1 bit
* instr\_size: 3 bits
* datapath\_size: 24 bits
* mempath\_size: 24 bits
* cell\_size: 8 bits

### Synchronization
The whole circuit is currently synchronous with the same main clock. However, constraints in stdin and Register24Stack64 may require double-ticking. Formal proof is required to confirm.

### RESET
This implementation defines that all memory cells start at 0.

### The `bfc` utility
bfc.cpp is a standalone C++ file that can be compiled to build Brainfuck programs.

Accepting the argument FILE\_BASENAME, bfc will parse the {FILE\_BASENAME}.bf file

Then it will emit the following files:
* {FILE\_BASENAME}.min.bf: The syntactically minified Brainfuck program code, i.e. only instruction bytes are left.
* {FILE\_BASENAME}.bfc: The Brainfuck code is compiled into the
