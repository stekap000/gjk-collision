#include <iostream>

// Based on GJK paper from 1988.

/*
(1)
    Distance between two sets is the minimal distance between all possible elements of those sets.
    To find this distance, we would need to look at all possible pairs of elements, find their
    distances, and then find the minimal one.
(2)
    We can represent more complicated objects as unions of simpler objects.
(3),(4)
    Distance between two objects that are represented as unions can be found by looking at all
    pairs of their simpler objects and finding distances between them (same as in (1)), and then
    taking the smallest of those calculated distances.
(5)
    Spherical extension of some original object is the new object that contains all points of the
    original object, but also add points that are at distance 'r' from original object. If the
    distance that we choose is 'r', then we are making 'rth spherical extension'. Intuitively,
    if we observe 2D case with a square being the original object, we can construct 'rth spherical
    extension' by first making the circle of radius 'r' and placing its center in one of the square's
    vertices. Then, we just move circle all over the circumference of the square, such that its center
    always lies on that circumference. All points touched by that circle, along with all points of
    original square, together make 'rth spherical extension'.
(6)
    This defines distance between spherical extensions. Because of (3), we know definition for
    distance between two original objects. Let's say that such distance is 'D'. If we now spherically
    extend one original object by using radius 'Ra', boundary of that object will be closer to second
    object by the length of radius 'Ra' ie. the distance will be 'D - Ra'. If we also spherically extend
    second object by 'Rb', the final distance will be 'D - Ra - Rb'. What if this distance turns out to be
    negative? That would mean that our two spherical extensions intersect, and since we are interested in
    distances for the sake of answering the question about whether objects collide, we can consider all
    negative values as the same thing, that is telling us that the objects collided. For this purpose, paper
    introduces '+' function that does nothing to positive values, but converts all negative values to '0' ie.
    no distance.
(7),(8)
    This defines distance between objects that are made up of multiple spherical extensions. Since spherical
	extension is just another kind of object, (3) and (4) apply to it. The only difference is that now, we
	need to use distance between spherical extensions from (6) to calculate distance between objects made up
	of multiple spherical extensions.
(9)
    This basically tells us how we can see object in some space. Idea is that we have original object, along
	with additional information about its configuration. Current configuration is represented with vector in
	some configuration space. Practically, this just means that we have parameters like position of object in
	our world, its rotation etc. Position and rotation are important for collision since they change relation
	between our object. For this reason, paper mentions these two. This formula tells us that, if we have
	object in its base state 'Ci' (for example, model that we import from .obj file is defined in its local
	coordinate system) and we have configuration information 'q', then we can use that information 'q' to
	figure our position and rotation of base state, thus producing actual object state. We produce this state
	by simply applying rotation to base state and then translation.
(10)
    
*/

int main() {
	std::cout << "Works!" << std::endl;
	
	return 0;
}
