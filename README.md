# [Circ]uit Ana[lyzer] 
I'm going to be honest, I got really tired of analyzing circuits by hand. If I feel like it, I may implement this into a QT application for circuit analysis.

Development Path: 
1. Test suite evaluation (Conversion to Google Test)
2. Network Creation
   - [x] Proof of concept (Nodes and Elements)
   - [x] Evaluate memory safety of the network 
3. Network Simplification (Resistors in DC)
4. Loop Detection
5. KVL Analysis
6. KCL Analysis
7. Node/Mesh Analysis
8. Thevenin/Norton Equivalence
9. Introduction of Capacitors and Inductors

Nice to Haves:
1. Conversion to the Boost Graph. This would allow for visualizations of the network via the dot visualizer.

Development Notes:
- I've split the project into two branches. One branch holds what will be for now, the legacy pointer implementation of the network. The other branch will hold a new implementation that runs exclusively on UIDs in place of the pointers.