# <center>oamx</center>
_A Game Boy emulator written in C, focused on accuracy, transparency, and hardware-level correctness._

## Project status

oamx is an experimental Game Boy emulator still under active development. It is not feature-complete and should be considered unstable. Hardware behavior is implemented with a focus on accuracy and clarity of code rather than performance or user experience.  
Bugs, visual glitches, and unexpected behavior are to be expected.

Despite being in early development, the emulator is already capable of running real commercial games and passes CPU validation tests.

## **Current Capabilities**

-   **CPU Emulation:** Sharp LR35902 implemented with cycle-accurate execution.

-   **Memory Subsystem:** Full memory map implementation with support for ROM, WRAM, VRAM, HRAM, OAM, IO registers, and interrupt control.
-   **MBC Support:** Currently supports **MBC1** (bank switching working on real commercial ROMs).
    
-   **Interrupt Handling:** Functional, though timing edge-cases may be incomplete.
    
-   **PPU:** Rendering pipeline implemented; enough to support basic graphics output for tested games.

## **Compatibility**
The following ROMs are known to boot and run to a playable state:

- Kirby's Dream Land;
- The Legend of Zelda: Link’s Awakening (with minor visual artifacts);
- Tetris.

<table>
  <tr>
    <td align="center"><img src="./assets/kirbys.gif" width="200px"/></td>
    <td align="center"><img src="./assets/zelda.gif" width="200px"/></td>
    <td align="center"><img src="./assets/tetris.gif" width="200px"/></td>
  </tr>
</table>

## **Debugger**

A dedicated branch of this repository contains an experimental debugger under active development. This version is designed as a hardware-inspection tool intended for emulator development, reverse engineering, and architectural study of the Game Boy system.

### **Current Capabilities**

-   **CPU State Inspection:**  
    Displays the full CPU register set (AF, BC, DE, HL, SP, PC), interrupt master enable state, and CPU flags with real-time updates.
    
-   **Tile Viewer (Basic Implementation):**  
    A preliminary tiles visualization tool is available. It reads and decodes tile data directly from VRAM and displays raw tile output for debugging graphics memory.
    

### **Purpose**

This debugger is not intended for user-facing gameplay features. Its primary goal is to provide cycle-level visibility into internal emulator state to aid in verifying correctness, diagnosing timing issues, and understanding low-level Game Boy behavior.

### **Future Roadmap**

Planned debugger features are:

-   Full PPU state visualization (mode transitions, scanline state, OAM inspection)
    
-   Memory inspection and editing tools
    
-   Breakpoints on memory access, interrupts, and opcode execution
    
-   Real-time VRAM, OAM, and background map viewers

    

Once the debugger is sufficiently mature, it will be merged into the main branch as an optional development mode, which can be launched using the `-d` argument.

## **Disclaimer**

This emulator is under active development and intended for learning and experimentation rather than professional use. Features may be incomplete and behavior may change over time. Contributions, testing, and technical discussion are welcome.

In the future, once the emulator reaches a more stable state, installation and usage documentation will be provided.