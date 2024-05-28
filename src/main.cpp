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
	by simply applying rotation to base state and then translation. Formula is expressed in such a way that it
	also encompasses object that are made up of smaller objects.
(10)
    This is the same statement as (9), just for spherical extensions and objects that are made up of multiple
	spherical extension.
(11)
    
(12)
    Convex combination of points. Intuitively, if we have 3 points, then convex combination is the boundary of
	triangle made from those points. This one is important since distance between two objects is the distance
	between their boundaries.
(13)
    This defines near point of some set X to be the point in X that is closest to the origin of the space
	(for example 3D space).
(14)
    This just tells us that the near point of convex combination can be represented as convex combination for
	some coefficients.
(15),(16)
    This introduces support function which takes vector and spits out number. Intuitively, if we plug in unit vector
	that points in some direction, then the function will return us distance from the origin to the point of our set
	that is furthest away in that direction. If input vector is not unit, then the previous result is just scaled.
	Solution is just a point of our set, that is furthest away in chosen direction.
(17)
    This tells us that the value of support function and solution vector for some input direction vector is the same
	for the set of points and for convex boundary of those points. This makes sense, since furthest point, in any
	direction, will lie at that convex boundary. Additionally, this tells us that we can find value of support function
	by iterating over all points in our set, finding dot product between them and chosen direction vector. It is linear
	complexity procedure.
(18),(19),(20)
    These basically say that we can shift our perspective from viewing two objects and finding distance between them
	to viewing one object that is the difference between the two and finding its minimum length element. This element
	also represents origin of that new set. Since paper defines difference as a set containing differences between all
	possible combinations of elements from two sets, the resulting set will have 'M*N' if 'M' is the size of first set
	and 'N' the size of second. Main question now is "is there a way to solve problem without having complexity of M*N?".
(21)
    Here, paper makes a claim that in order to find distance from (18), we just need to worry about the value of support
	function and about a point for which this value is returned. Then, immediate question is how to compute support
	function value for set that is the difference of two original sets. If we have two convex sets 'K1' and 'K2', we can assume
	that we have support function values for them for some direction vector 'M', since we know how to compute that from (17).
	Looking at formula for support function, we can plug set 'x = x1 - x2' where 'x1' is from 'K1' and 'x2' is from 'K2'. This
	would be formula for support function of difference set 'K = K1 - K2'. We can try to express this via something that we know
	how to compute, which are support functions for 'K1' and 'K2'. Looking at case where 'x = x1 - x2', we can get maximum
	when 'x1*M' is maximum and 'x2*M' is minumum. In other words, we can write support function for 'K' as 'max{x1*M} - min{x2*M}'.
	First term, 'max{x1*M', is just support function for first set 'K1'. Problem is the second term where we would also like to
	get 'max' of something, since that would me that it is also support function of something, which we know how to compute.
	We can write '- min{x2*M}' as '+ max{x2*(-M)}' which can be easily imagined by seeing '-' as reflection of x-axis over 0.
	This gives us a way to calculate support function of difference set via support functions of two original sets.
	Another thing to calculate is actual point for which we get support function value. Let's mark support functions as 'S1(M)', 'S2(M)' and
	'S(M)' for two original sets and difference set. We will use 'M' for direction vector. Let's also mark points furthest in direction 'M'
	for given sets as 'P1(M)', 'P2(M)' and 'P(M)'. We know that 'S(M) = P(M) * M' must hold. Based on previous derivation, we know that
	'S(M) = S1(M) + S2(-M)'. From this, we get 'S1(M) + S2(-M) = P(M) * M', and then 'P1(M)*M + P2(-M)*(-M) = P(M)*M'. Our goal here is to
	find 'P(M)', but it is obvious that the two sides of equation are equal when 'P(M) = P1(M) - P2(-M)', which given us second formula
	in paper.
	From these formulas, we know that even though difference set size (N) is product of original sets sizes (N1*N2), we can calculate support
	function and corresponding point with complexity (N1 + N2).
(22)
    Output of the algorithm is near point for difference set and its length is distance between two original sets.
(23),(24)
    Input to the algorithm consists of points of initial sets and also of support functions and corresponding solution points for those sets.
	Also, we are given formula for calculating near point for difference set. It is easy to derive since we know that it lies on the
	convex boundary of difference set ie. it is convex combination of points in difference set. Additionally, since every point in
	difference set can be expressed as difference between two vectors, one from each initial set, we just need to plug this in
	convex combination and separate one sum to two sums. Now, two sum represent near points for initial sets.
(25)
    This introduces a measure for how close we are to near point of polytope. Function represents upper bound for quadratic difference between
	polytope point 'x' and polytope near point that we are trying to find. Function value is 0 when point 'x' is the near point. Important
	claim is that if function value is greater than 0 ie. if we haven't reached near point, then we can get closer to it by going
	from point 'x' in the direction that is opposite vector of 'x', reaching furthest polytope point in that direction. This property allows
	us to construct algorithm for finding near point.
(26)
    This tells us that distance of iterated near points from the origin decreases. To see why, we can imagine some arbitrary iteration. Within
	that iteration we found near point for current simplex. If this is a near point of the whole object ie. measure function is 0, then we are done.
	If this is not the case, we are looking for next near point of next simplex in the direction opposite to current near point vector.Let's
	suppose that we can't find such point. That would mean that our previous near point is the closest one to the origin in that direction.
	But this is in contradiction with the fact that measure function told us that it is not the closest one ie. value of measure function was not 0.
	This means that there must be some next near point that is closer to the origin that current near point, if the current near point is not
	the near point of the whole object.
(27),(28)
    These just illustrate steps of the algorithm and indicate that algorithm works when we start with a simplex of the same dimension as a whole object,
	but also when we start with simplex of lower dimension. In this case, we eventually get to iteration over simplices of the same dimension as object,
	in a certain number of steps that depends on the dimension of starting simplex.
(29)
    This just tells us that within some iteration 'k', we can represent near point of the simplex in that iteration as a convex combination of
	vertices of that simplex. It also states that any subset of vertices of that simplex is affinely independent. Intuitively, this means that if we were
	to move that subset in such a way that we put one of its elements in the coordinate origin, all vectors that represent other vertices would
	be linearly independent. Within the algorithm, this subset represents subset of current simplex's vertices that has at most 'dimension + 1'
	elements ie. we need to throw away one element when we include object's support vector in the direction opposite to current simplex near point vector.
	'Is' is an index set that we use to pick points from simplex. One particular pick is denoted by 'Ys' which is a set of points, where 's' just
	represents which index set was used.
(30)
    
	
    -----------------
    The goal of the algorithm is to find near point of some shape and then its norm. This is done by starting with a subset of points from this
	shape and finding near point for that convex combination of that subset. We then check if this is the near point of the whole shape by
	evaluating measure function, which is possible because we only need to know support function value for difference set in order to do this,
	and we can calculate it from two original objects. If the value is 0, we have solved the problem. If it is not, then we form new subset from
	old one by including point that is furthest one in the direction opposite to near point vector. We are guaranteed that we will find point
	closer to origin in that direction. This is because we arrive at contradiction if this is not the case. Consider that there is no point in
	that direction that is closer to origin. In that case, out last near value must lie on support plane ie. it must be near point of the whole
	set, which cotradicts previous step where measure function told us that it is not. Appart from including given point, we need to exclude point
	that is furthest away from current near point in current set. We repeat this process until measure function is zero.

	Why care about near point of some object? It is because we can apply this to a difference set. In that case, near point will represent distance
	between two objects and objects overlap if that value is zero ie. objects overlap if and only if difference set contains zero vector. The whole
	point of the algorithm is to try to efficiently find an answer to the question of whether the difference set contains zero vector and it does this
	by going over simplices of difference set in the direction that is supposed to contain near point.
*/

int main() {
	//std::cout << "Works!" << std::endl;
	
	return 0;
}
