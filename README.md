# Simulating-Binding-to-Surfaces
This C++ project defines a class "Box" that can have multiple slots for balls
in it. The subclass "Multicolor_Box" inherits from "Box" and adds the ability
to have both light and dark balls. The Boxes represent streptavidin molecules
attached to surfaces and so the default constructor for a "Box" or "Multicolor_Box"
object is a box with three slots (streptavidin is tetravalent, but one "slot" is
taken up by the attachment to the surface). The program Balls_and_Boxes.cpp uses the class to simulate a mixture of free biotin (dark) and biotinylated Cy3-DNA (bright) binding to a collection of Boxes. For now, just run the C++ code in your own
IDE or just run the .exe which will allow you to choose the number of boxes
and the number of dark and light balls to place in the boxes. It will save a .txt
to your local directory containing a histogram of the number of boxes containing
0, 1, 2, or 3 balls and the percentage of boxes that has 1, 2, or 3 bright balls out of the ones that have at least one bright ball. You are free to use the class for your own projects. In the 
Future I will separate the class into a header file, but until then feel free to
copy the code for the class definitions into your own projects.

To be completed: 1) Automatic plotting of histogram. I need to learn how to do visual interfaces with C++!
