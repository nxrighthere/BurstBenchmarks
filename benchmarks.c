#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#if defined (_WIN32)
	#define EXPORT __declspec(dllexport)
#else
	#define EXPORT extern
#endif

EXPORT uint32_t benchmark_fibonacci(uint32_t number) {
	if (number <= 1)
		return 1;

	return benchmark_fibonacci(number - 1) + benchmark_fibonacci(number - 2);
}

EXPORT float benchmark_mandelbrot(uint32_t width, uint32_t height, uint32_t iterations) {
	float data = 0.0f;

	for (int i = 0; i < iterations; i++) {
		float
			left = -2.1f,
			right = 1.0f,
			top = -1.3f,
			bottom = 1.3f,
			deltaX = (right - left) / width,
			deltaY = (bottom - top) / height,
			coordinateX = left;

		for (int x = 0; x < width; x++) {
			float coordinateY = top;

			for (int y = 0; y < height; y++) {
				float workX = 0;
				float workY = 0;
				int counter = 0;

				while (counter < 255 && sqrtf((workX * workX) + (workY * workY)) < 2.0f) {
					counter++;

					float newX = (workX * workX) - (workY * workY) + coordinateX;

					workY = 2 * workX * workY + coordinateY;
					workX = newX;
				}

				data = workX + workY;
				coordinateY += deltaY;
			}

			coordinateX += deltaX;
		}
	}

	return data;
}

typedef struct _NBody {
	double x, y, z, vx, vy, vz, mass;
} NBody;

inline static void benchmark_nbody_initialize_bodies(NBody* sun, NBody* end) {
	const double pi = 3.141592653589793;
	const double solarmass = 4 * pi * pi;
	const double daysPerYear = 365.24;

	sun[1] = (NBody){ // Jupiter
		4.84143144246472090e+00,
		-1.16032004402742839e+00,
		-1.03622044471123109e-01,
		1.66007664274403694e-03 * daysPerYear,
		7.69901118419740425e-03 * daysPerYear,
		-6.90460016972063023e-05 * daysPerYear,
		9.54791938424326609e-04 * solarmass
	};

	sun[2] = (NBody){ // Saturn
		8.34336671824457987e+00,
		4.12479856412430479e+00,
		-4.03523417114321381e-01,
		-2.76742510726862411e-03 * daysPerYear,
		4.99852801234917238e-03 * daysPerYear,
		2.30417297573763929e-05 * daysPerYear,
		2.85885980666130812e-04 * solarmass
	};

	sun[3] = (NBody){ // Uranus
		1.28943695621391310e+01,
		-1.51111514016986312e+01,
		-2.23307578892655734e-01,
		2.96460137564761618e-03 * daysPerYear,
		2.37847173959480950e-03 * daysPerYear,
		-2.96589568540237556e-05 * daysPerYear,
		4.36624404335156298e-05 * solarmass
	};

	sun[4] = (NBody){ // Neptune
		1.53796971148509165e+01,
		-2.59193146099879641e+01,
		1.79258772950371181e-01,
		2.68067772490389322e-03 * daysPerYear,
		1.62824170038242295e-03 * daysPerYear,
		-9.51592254519715870e-05 * daysPerYear,
		5.15138902046611451e-05 * solarmass
	};

	double vx = 0, vy = 0, vz = 0;

	for (NBody* planet = sun + 1; planet <= end; ++planet) {
		double mass = planet->mass;

		vx += planet->vx * mass;
		vy += planet->vy * mass;
		vz += planet->vz * mass;
	}

	sun->mass = solarmass;
	sun->vx = vx / -solarmass;
	sun->vy = vy / -solarmass;
	sun->vz = vz / -solarmass;
}

inline static void benchmark_nbody_energy(NBody* sun, NBody* end) {
	double e = 0.0;

	for (NBody* bi = sun; bi <= end; ++bi) {
		double
			imass = bi->mass,
			ix = bi->x,
			iy = bi->y,
			iz = bi->z,
			ivx = bi->vx,
			ivy = bi->vy,
			ivz = bi->vz;

		e += 0.5 * imass * (ivx * ivx + ivy * ivy + ivz * ivz);

		for (NBody* bj = bi + 1; bj <= end; ++bj) {
			double
				jmass = bj->mass,
				dx = ix - bj->x,
				dy = iy - bj->y,
				dz = iz - bj->z;

			e -= imass * jmass / sqrt(dx * dx + dy * dy + dz * dz);
		}
	}
}

inline static double benchmark_nbody_get_d2(double dx, double dy, double dz) {
	double d2 = dx * dx + dy * dy + dz * dz;

	return d2 * sqrt(d2);
}

inline static void benchmark_nbody_advance(NBody* sun, NBody* end, double distance) {
	for (NBody* bi = sun; bi < end; ++bi) {
		double
			ix = bi->x,
			iy = bi->y,
			iz = bi->z,
			ivx = bi->vx,
			ivy = bi->vy,
			ivz = bi->vz,
			imass = bi->mass;

		for (NBody* bj = bi + 1; bj <= end; ++bj) {
			double
				dx = bj->x - ix,
				dy = bj->y - iy,
				dz = bj->z - iz,
				jmass = bj->mass,
				mag = distance / benchmark_nbody_get_d2(dx, dy, dz);
				bj->vx = bj->vx - dx * imass * mag;
				bj->vy = bj->vy - dy * imass * mag;
				bj->vz = bj->vz - dz * imass * mag;
				ivx = ivx + dx * jmass * mag;
				ivy = ivy + dy * jmass * mag;
				ivz = ivz + dz * jmass * mag;
		}

		bi->vx = ivx;
		bi->vy = ivy;
		bi->vz = ivz;
		bi->x = ix + ivx * distance;
		bi->y = iy + ivy * distance;
		bi->z = iz + ivz * distance;
	}

	end->x = end->x + end->vx * distance;
	end->y = end->y + end->vy * distance;
	end->z = end->z + end->vz * distance;
}

EXPORT double benchmark_nbody(uint32_t advancements) {
	NBody sun[5] = { 0 };
	NBody* end = sun + 4;

	benchmark_nbody_initialize_bodies(sun, end);
	benchmark_nbody_energy(sun, end);

	while (advancements-- > 0) {
		benchmark_nbody_advance(sun, end, 0.01d);
	}

	benchmark_nbody_energy(sun, end);

	return sun[0].x + sun[0].y;
}