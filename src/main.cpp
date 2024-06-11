#include <iostream>

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
		float max = -999999;
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
	default: {
		return v3(0, 0, 0);
	}
	}
}

v3 minkowski_diff_support(shape_2d s1, shape_2d s2, v3 direction) {
	return support(s1, direction) - support(s2, -direction);
}

void add_point_to_simplex(simplex *s, v3 point) {
	s->points[n++] = point;
}

bool update_simplex_and_direction_2d(simplex *s, v3 *d) {
	// One point case doesn't happen.

	// Two point case is determined because of how we found support point and
	// because of check if support point didn't reach origin (in the main loop).
	switch(s->n) {
	case 2: {
		v3 temp = s->points[0] - s->points[1];
		*d = v3::cross(v3::cross(temp, -(s->points[1])), temp);
	}
	case 3: {
		v3 temp1 = s->points[0] - s->points[2]; // AC
		v3 temp2 = s->points[1] - s->points[2]; // AB
		v3 N = v3::cross(temp2, temp1);

		if(v3::dot(v3::cross(N, temp1), -(s->points[2])) > 0) {
			s->n = 2;
			s->points[1] = s->points[2];
			*d = v3::cross(v3::cross(temp1, -(s->points[2])), temp1);
		}
		else {
			if(v3::dot(v3::cross(temp2, N), -(s->points[2])) > 0) {
				s->n = 2;
				s->points[0] = s->points[2];
				*d = v3::cross(v3::cross(temp2, -(s->points[2])), temp2);
			}
			else {
				return true;
			}
		}
	}
	}

	return false;
}

bool gjk_2d(shape_2d s1, shape_2d s2) {
	simplex s;
	v3 direction;
	// Start out with some point in minkowski diff.
	v3 maybe_closest = pick_from_md(s1, s2);
	add_point_to_simplex(&s, maybe_closest);

	direction = -maybe_closest;

	while(true) {
		// Furthest point in opposite direction (direction towards origin).
		v3 support_point = minkowski_diff_support(s1, s2, direction);

		// Support point didn't reach origin.
		if(v3::dot(support_point, direction) < 0) return false;

		add_point_to_simplex(&s, support_point);

		if (update_simplex_and_direction_2d(&s, &direction)) return true;
	}
}

int main() {
	
	return 0;
}
