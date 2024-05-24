#include <iostream>

// Based on GJK paper from 1988.

/*
(1)
 - Distance between two sets is the minimal distance between all possible elements of those sets.
 - To find this distance, we would need to look at all possible pairs of elements, find their
   distances, and then find the minimal one.
(2)
 - We can represent more complicated objects as unions of simpler objects.
(3),(4)
 - Distance between two objects that are represented as unions can be found by looking at all
   pairs of their simpler objects and finding distances between them (same as in (1)), and then
   taking the smallest of those calculated distances.
(5)
 - Spherical extension of some original object is the new object that contains all points of the
   original object, but also add points that are at distance 'r' from original object. If the
   distance that we choose is 'r', then we are making 'rth spherical extension'. Intuitively,
   if we observe 2D case with a square being the original object, we can construct 'rth spherical
   extension' by first making the circle of radius 'r' and placing its center in one of the square's
   vertices. Then, we just move circle all over the circumference of the square, such that its center
   always lies on that circumference. All points touched by that circle, along with all points of
   original square, together make 'rth spherical extension'.

*/

int main() {
	std::cout << "Works!" << std::endl;
	
	return 0;
}
