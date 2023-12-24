# 3D Sorting Visualization

The goal of this project is to generate 3D-printable visualizations of common sorting algorithms. 

This is accomplished by:
 - Instrument sorting algorithms to give snapshot of their internal state at each 'epoch'.
 - At each epoch, make consecutive prisms with height corresponding to the value of the given element.
 - Convert prisms to triangles.
 - Write triangles to 3D geometry file (`.stl` :yuck:).


![](assets/slicing.gif)

> Slicing the 3D object shows the progression of the sorting algorithms over time.
