#include <iostream>

// TODO: Implement original GJK
// TODO: Implement optimizations (Erickson, Muratori, ...) [A Geometric Interpretation of the Boolean GJK Algorithm] https://arxiv.org/pdf/1505.07873
// TODO: 2D case, 3D case

// pseudocode
enum shape_2d_type { polygon, circle };
struct shape_2d {
	shape_2d_type type;
	v3 center;
	float radius;
	int n;
	v3 *points;
};

struct simplex {
	int n;
	v3 points[4];
};

v3 support(shape_2d s, v3 direction) {
	switch(s.type) {
	case circle: {
		return s.center + s.radius * direction.unit();
	}
	case polygon: {
		// ...
		break;
	}
	default: {
		// ...
	}
	}
}

v3 minkowski_diff_support(shape_2d s1, shape_2d s2, v3 direction) {
	return support(s1, direction) - support(s2, -direction);
}

v3 closest_simplex_point(simplex s) {
	switch(s.n) {
	case 1: {
		return s.points[0];
	}
	case 2: {
		
	}
	case 3: {

	}
	case 4: {

	}
	}
}

v3 gjk(shape_2d s1, shape_2d s2) {
	simplex s;
	// Start out with some point in minkowski diff.
	v3 maybe_closest = pick_from_md(s1, s2);
	add_point_to_simplex(s, maybe_closest);

	while(true) {
		// Furthest point in opposite direction (direction towards origin).
		v3 support_point = minkowski_diff_support(s1, s2, -maybe_closest);

		// We are moving towards origin and there is no point that is closer to origin than our starting point.
		// This means that starting point is on the edge of minkowski diff and is closest to origin, so we return it.
		if(support_point == maybe_closest /* up to error */) {
			return support_point;
		}
	
		add_point_to_simplex(s, support_point);
		maybe_closest = closest_simplex_point(s);
		// Only preserve points of sub-simplex that contains simplex point closest to origin.
		// We will build new simplex from piece of the old one that is closest to origin.
		cull_simplex(s);
	}
}



int main() {
	//std::cout << "Works!" << std::endl;
	
	return 0;
}
