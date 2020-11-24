#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Seg;
typedef K::Ray_2 Ray;

long floor_to_long(const K::FT& x) {
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}

// construct intersection of seg with the ray and return the closest point to the ray_source in the intersection
Point intersection(Ray& r, Seg& seg) { 
	auto o = CGAL::intersection(r, seg); // construct the intersection between ray and segment
	const Point* op = boost::get<Point>(&*o); // intersection can be a point
	const Seg* os = boost::get<Seg>(&*o); // intersection can be a segment
	if(op) { // intersection is a point, return it
		return *op;
	} else if(os) { // intersection is a segment, find the segment end point that is closest to ray source and return it
		K::FT dist_to_source = CGAL::squared_distance(r.source(), os->source());
		K::FT dist_to_target = CGAL::squared_distance(r.source(), os->target());
		if(dist_to_source < dist_to_target) {
			return os->source();
		} else {
			return os->target();
		}
	}
	else
		throw std::runtime_error("Strange intersection");
} 

int testcase() {
	// We have n segements and a ray. We want to find the closest intersection of the ray with any segment.
	// We could basically just check all segments, if a segment i intersects with the ray, then we construct the intersection
	// which might either be a point of a segment, if it is a segment, we take the end point of the segment that is closest to the
	// ray source. Once we have the closest point of intersection between the ray and segment i, then we keep it if it is the closest
	// point to the ray source over all previously checked segments.
	// The problem here is that we might need to construct n intersections (if the ray intersect with each segment) which is expensive
	// on the other hand, checking if the ray and the segment intersect is less expensive. So a faster algorithm would find the 1st
	// intersection p, then we limit our search in the segment [ray.source(), p] since we only want intersection points closer than
	// p. So we construct the segment limited_ray = [ray.source(), p] and for the next segments, we check if limited_ray intersects
	// with the segment, if so then we found a closer intersection point so we construct it limit again.
	// Another thing we do is to shuffle the segments read because the input could consist of segments ordered from furtherest to closest
	// to the ray source (in which case we hit the worst case where we construct every intersection). So we are not dependent on the
	// order by which the segment are given.
	
	int n; std::cin >> n;
	if(n == 0) return 0;
	
	long x, y, a, b; std::cin >> x >> y >> a >> b; // ray points coordinates
	
	Ray ray(Point(x,y), Point(a,b)); // construct ray
	std::vector<Seg> segs; // to store the n segments
	for(int i=0; i<n; i++) { // read segments
		long r, s, t, u; std::cin >> r >> s >> t >> u;
		segs.push_back(Seg(Point(r,s), Point(t,u)));
	}
	
	std::random_shuffle(segs.begin(), segs.end()); // shuffle the segments
	
	int i = 0;
	while(i<n) { // find the smallest index i such that segment i intersects with ray
		if (CGAL::do_intersect(ray, segs[i])) { // we only perform predicate checking which is fast
			break;
		}
		i++;
	}
	
	if(i == n) { // didn't find any intersection
		std::cout << "no" << std::endl;
		return 1;
	}
	
	Point closest_point = intersection(ray, segs[i]); // construct the closest intersection point
	// we now limit our search to the intersection points inside the segment [r.source(), closest_point]
	Seg ray_limited(ray.source(), closest_point);
	for(i=i+1;i<n; i++) { // check the next segments
		if (CGAL::do_intersect(ray_limited, segs[i])) { // if ray_limited and segs[i] intersect, then we found a closer intersection point
			closest_point = intersection(ray, segs[i]); // construct the closest intersection point
			ray_limited = Seg(ray.source(), closest_point); // limit the search the the closest point found
		}
	}
	
	std::cout << floor_to_long(closest_point.x()) << " " << floor_to_long(closest_point.y()) << std::endl;
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;
}
