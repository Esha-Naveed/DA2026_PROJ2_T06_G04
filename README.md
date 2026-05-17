# DA2026_PROJ2_T06_G04

### Project description

This is a greedy algorithm and heuristic based project focused on solving the Compiler Register Allocation problem. The tool acts as a compiler backend component, mapping high-level variables (_webs_) to a limited set of physical registers to optimize program performance and minimize memory access. 

To achieve this, the system implements a tiered approach to allocation, ranging from _Basic Graph Coloring_ to more advanced techniques such as _Web Spilling_ and _Web Splitting_ to handle register pressure, alongside a custom optimized allocation strategy designed for enhanced efficiency.
___
### System Architecture
When a source file is processed, the data flows linearly through the codebase in the following sequence:

* **main.cpp (The Coordinator): The entry point. It contains the execution mode flags, '-b' for batch execution or defaults to interactive mode.

* **parser.cpp & dataStruct.h (Data Ingestion & Web Building): Reads and cleans the input files. Removes comments, records variable configurations, and handles Live Range Analysis by checking for overlapping lines. It groups independent variables called Webs saved inside dataStruct.h.

* **createGraph.cpp & graph.h (Interference Analysis): Takes the parsed webs and runs overlapping tests. If two webs are active at the same program, they interfere. The file uses custom adjacency-list graph structural framework (from graph.h) to construct an interference network where vertices represent webs and edges indicate conflicts.

* **regAlloc.cpp (Allocation Engines): The core algorithm. It runs Chaitin's Graph Coloring Heuristic using an active vertex simplification stack. Depending on user choice or setup, it runs:

    * *Basic Allocation: Simple coloring; fails if the graph is too complex.

    * *Web Spilling Heuristic: Spills high-degree conflict nodes out to memory.

    * *Web Splitting Heuristic: Recursively bisects long ranges into smaller sub-webs.

    * *Custom Allocation: A tailored optimization variant.

* **output_file.cpp (Result Generation): The final step. It extracts the finalized allocation data from the interference graph and formats the output into standard text logs containing register slots or memory markers.
___
### **Collaborators:**

    - Esha Naveed (up202400330)
    - Clara Correia (up202404979)
___
