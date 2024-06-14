#include <iostream>
#include <limits>

struct v3 {
	float x, y, z;

	v3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
	
	v3 operator-() {
		return v3(-x, -y, -z);
	}

	v3 operator-(v3 v) {
		return v3(x - v.x, y - v.y, z - v.z);
	}

	v3 operator+(v3 v) {
		return v3(x + v.x, y + v.y, z + v.z);
	}

	static float dot(v3 u, v3 v) {
		return u.x*v.x + u.y*v.y + u.z*v.z;
	}

	static v3 cross(v3 u, v3 v) {
		return v3(u.y*v.z - u.z*v.y,
				  u.z*v.x - u.x*v.z,
				  u.x*v.y - u.y*v.x);
	}
};

// Shapes don't need to be polygonal. In fact, they can be given in arbitrary format
// as long as it is possible to compute their support points in all directions.
// In the case of concave shapes, we first need to break them down into multiple convex
// shapes, and then apply the algorithm.
enum shape_type { polygon };
struct shape {
	shape_type type;
	int n;
	v3 *points;
};

// This is used to hold currently constructed simplex during the algorithm.
struct simplex {
	int n;
	v3 points[4];
};

// Support point computation is given only for convex shape. In that case, we just
// use dot product to find furthest point in the given direction. We need to include
// different cases for shapes that are given in a different form. For example, if we
// want the algorithm to work with analytic spheres, then we can find support point of
// a sphere in some direction by moving towards sphere center and then moving from center
// in the given direction by the amount given by sphere's radius.
v3 support(shape s, v3 direction) {
	switch(s.type) {
	case polygon: {
		float max = std::numeric_limits<float>::max();
		float temp = 0;
		int temp_i = 0;
		for(int i = 0; i < s.n; ++i) {
			temp = v3::dot(s.points[i], direction);
			if(temp > max) {
				max = temp;
				temp_i = i;
			}
		}

		return s.points[temp_i];
	}
	}

	return v3();
}

// This is a powerful result that allows us to calculate support point of minkowski difference
// by calculating support points of two shapes that are being subtracted. In other words, it
// allows us to operate on minkowski difference without the need to ever actually construct it.
v3 minkowski_diff_support(shape s1, shape s2, v3 direction) {
	return support(s1, direction) - support(s2, -direction);
}

// Just a utility function that adds new point to simplex.
void add_point_to_simplex(simplex *s, v3 point) {
	s->points[s->n++] = point;
}

// This function updates current simplex and search direction.
// It returns true only in the case that collision happened ie. when we succefully construct
// simplex containing origin. For 2D case, we need to find triangle, and for 3D case, we need
// to find tetrahedron.
bool update_simplex_and_direction(simplex *s, v3 *d) {
	switch(s->n) {
	case 2: {
		v3 temp = s->points[0] - s->points[1];
		*d = v3::cross(v3::cross(temp, -(s->points[1])), temp);
		break;
	}
	case 3: {
		v3 temp1 = s->points[0] - s->points[2];
		v3 temp2 = s->points[1] - s->points[2];
		v3 N = v3::cross(temp2, temp1);
		v3 minus_A = -(s->points[2]);

		if(v3::dot(v3::cross(N, temp1), minus_A) > 0) {
			s->n = 2;
			s->points[1] = s->points[2];
			*d = v3::cross(v3::cross(temp1, minus_A), temp1);
		}
		else {
			if(v3::dot(v3::cross(temp2, N), minus_A) > 0) {
				s->n = 2;
				s->points[0] = s->points[2];
				*d = v3::cross(v3::cross(temp2, minus_A), temp2);
			}
			else {
				// For 2D case, we just return true here, since we succesfully constructed triangle
				// that contains the origin.
				return true;
			}
		}
	}
	}

	return false;
}

bool gjk(shape s1, shape s2) {
	// It is enought to start with random point in minkowski difference.
	// Choice can impact convergence, but we don't worry about it here and we
	// choose starting point using existing support function in arbitrary direction.
	
	simplex s = {0};
	v3 direction(1, 0, 0);
	v3 support_point;

	// Add starting point to simplex, and start moving in the direction opposite of that point
	// since the origin must be in that direction (because chosen point vector goes from origin towards
	// that point).
	add_point_to_simplex(&s, minkowski_diff_support(s1, s2, direction));
	direction = -(s.points[0]);

	// We loop until we either construct simplex that contains origin, or we see that at some stage
	// during the iteration, support point (in the direction of origin) ends up before origin.
	while(true) {
		// Get support point of minkowski difference in the given direction.
		// This direction is always updated to point towards origin.
		support_point = minkowski_diff_support(s1, s2, direction);

		// If support point in the given direction didn't reach or get past the origin,
		// then we know that the minkowski difference doesn't contain origin ie. there is no collision.
		if(v3::dot(support_point, direction) < 0) return false;

		// Having passed previous check, we know that support point lies past the origin, so we
		// add it to our simplex, since in that case, there is a chance that new simplex will contain
		// the origin.
		add_point_to_simplex(&s, support_point);

		// If we successfully found simplex that contains the origin, this function will return
		// true, and we will stop the algorithm and indicate that the collision happened.
		if (update_simplex_and_direction(&s, &direction)) return true;
	}
}

int main() {
	
	return 0;
}
