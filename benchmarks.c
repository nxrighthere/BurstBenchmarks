#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#ifdef _WIN32
	#define EXPORT __declspec(dllexport)
#else
	#define EXPORT extern
#endif

// Fibonacci

EXPORT uint32_t benchmark_fibonacci(uint32_t number) {
	if (number <= 1)
		return 1;

	return benchmark_fibonacci(number - 1) + benchmark_fibonacci(number - 2);
}

// Mandelbrot

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

// NBody

typedef struct _NBody {
	double x, y, z, vx, vy, vz, mass;
} NBody;

inline static void benchmark_nbody_initialize_bodies(NBody* sun, NBody* end) {
	const double pi = 3.141592653589793;
	const double solarMass = 4 * pi * pi;
	const double daysPerYear = 365.24;

	sun[1] = (NBody){ // Jupiter
		4.84143144246472090e+00,
		-1.16032004402742839e+00,
		-1.03622044471123109e-01,
		1.66007664274403694e-03 * daysPerYear,
		7.69901118419740425e-03 * daysPerYear,
		-6.90460016972063023e-05 * daysPerYear,
		9.54791938424326609e-04 * solarMass
	};

	sun[2] = (NBody){ // Saturn
		8.34336671824457987e+00,
		4.12479856412430479e+00,
		-4.03523417114321381e-01,
		-2.76742510726862411e-03 * daysPerYear,
		4.99852801234917238e-03 * daysPerYear,
		2.30417297573763929e-05 * daysPerYear,
		2.85885980666130812e-04 * solarMass
	};

	sun[3] = (NBody){ // Uranus
		1.28943695621391310e+01,
		-1.51111514016986312e+01,
		-2.23307578892655734e-01,
		2.96460137564761618e-03 * daysPerYear,
		2.37847173959480950e-03 * daysPerYear,
		-2.96589568540237556e-05 * daysPerYear,
		4.36624404335156298e-05 * solarMass
	};

	sun[4] = (NBody){ // Neptune
		1.53796971148509165e+01,
		-2.59193146099879641e+01,
		1.79258772950371181e-01,
		2.68067772490389322e-03 * daysPerYear,
		1.62824170038242295e-03 * daysPerYear,
		-9.51592254519715870e-05 * daysPerYear,
		5.15138902046611451e-05 * solarMass
	};

	double vx = 0, vy = 0, vz = 0;

	for (NBody* planet = sun + 1; planet <= end; ++planet) {
		double mass = planet->mass;

		vx += planet->vx * mass;
		vy += planet->vy * mass;
		vz += planet->vz * mass;
	}

	sun->mass = solarMass;
	sun->vx = vx / -solarMass;
	sun->vy = vy / -solarMass;
	sun->vz = vz / -solarMass;
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
		benchmark_nbody_advance(sun, end, 0.01);
	}

	benchmark_nbody_energy(sun, end);

	return sun[0].x + sun[0].y;
}

// Sieve of Eratosthenes

EXPORT uint32_t benchmark_sieve_of_eratosthenes(uint32_t iterations) {
	const int size = 1024;

	uint8_t flags[size];
	uint32_t a, b, c, prime, count = 0;

	for (a = 1; a <= iterations; a++) {
		count = 0;

		for (b = 0; b <= size; b++) {
			flags[b] = 1; // True
		}

		for (b = 0; b <= size; b++) {
			if (flags[b] == 1) {
				prime = b + b + 3;
				c = b + prime;

				while (c <= size) {
					flags[c] = 0; // False
					c += prime;
				}

				count++;
			}
		}
	}

	return count;
}

// Pixar Raytracer

typedef struct _Vector {
	float x, y, z;
} Vector;

typedef enum _Hit {
	PIXAR_RAYTRACER_NONE = 0,
	PIXAR_RAYTRACER_LETTER = 1,
	PIXAR_RAYTRACER_WALL = 2,
	PIXAR_RAYTRACER_SUN = 3
} Hit;

static uint32_t marsagliaZ, marsagliaW;

inline static Vector benchmark_pixar_raytracer_multiply(Vector left, Vector right) {
	left.x *= right.x;
	left.y *= right.y;
	left.z *= right.z;

	return left;
}

inline static Vector benchmark_pixar_raytracer_multiply_float(Vector vector, float value) {
	vector.x *= value;
	vector.y *= value;
	vector.z *= value;

	return vector;
}

inline static float benchmark_pixar_raytracer_modulus(Vector left, Vector right) {
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

inline static float benchmark_pixar_raytracer_modulus_self(Vector vector) {
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

inline static Vector benchmark_pixar_raytracer_inverse(Vector vector) {
	return benchmark_pixar_raytracer_multiply_float(vector, 1 / sqrtf(benchmark_pixar_raytracer_modulus_self(vector)));
}

inline static Vector benchmark_pixar_raytracer_add(Vector left, Vector right) {
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;

	return left;
}

inline static Vector benchmark_pixar_raytracer_add_float(Vector vector, float value) {
	vector.x += value;
	vector.y += value;
	vector.z += value;

	return vector;
}

inline static Vector benchmark_pixar_raytracer_cross(Vector to, Vector from) {
	to.x *= from.z - to.z * from.y;
	to.y *= from.x - to.x * from.z;
	to.z *= from.y - to.y * from.x;

	return to;
}

inline static float benchmark_pixar_raytracer_min(float left, float right) {
	return left < right ? left : right;
}

inline static float benchmark_pixar_raytracer_box_test(Vector position, Vector lowerLeft, Vector upperRight) {
	lowerLeft = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(position, lowerLeft), -1);
	upperRight = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(upperRight, position), -1);

	return -benchmark_pixar_raytracer_min(benchmark_pixar_raytracer_min(benchmark_pixar_raytracer_min(lowerLeft.x, upperRight.x), benchmark_pixar_raytracer_min(lowerLeft.y, upperRight.y)), benchmark_pixar_raytracer_min(lowerLeft.z, upperRight.z));
}

inline static float benchmark_pixar_raytracer_random(void) {
	marsagliaZ = 36969 * (marsagliaZ & 65535) + (marsagliaZ >> 16);
	marsagliaW = 18000 * (marsagliaW & 65535) + (marsagliaW >> 16);

	return (((marsagliaZ << 16) + marsagliaW) + 1.0f) * 3.141592653589793f;
}

static float benchmark_pixar_raytracer_sample(Vector position, int* hitType) {
	const int size = 60;

	float distance = 1e9f;
	Vector f = position;
	uint8_t letters[size];

	// P              // I              // X              // A              // R
	letters[0]  = 53; letters[12] = 65; letters[24] = 73; letters[32] = 85; letters[44] = 97; letters[56] = 99;
	letters[1]  = 79; letters[13] = 79; letters[25] = 79; letters[33] = 79; letters[45] = 79; letters[57] = 87;
	letters[2]  = 53; letters[14] = 69; letters[26] = 81; letters[34] = 89; letters[46] = 97; letters[58] = 105;
	letters[3]  = 95; letters[15] = 79; letters[27] = 95; letters[35] = 95; letters[47] = 95; letters[59] = 79;
	letters[4]  = 53; letters[16] = 67; letters[28] = 73; letters[36] = 89; letters[48] = 97;
	letters[5]  = 87; letters[17] = 79; letters[29] = 95; letters[37] = 95; letters[49] = 87;
	letters[6]  = 57; letters[18] = 67; letters[30] = 81; letters[38] = 93; letters[50] = 101;
	letters[7]  = 87; letters[19] = 95; letters[31] = 79; letters[39] = 79; letters[51] = 87;
	letters[8]  = 53; letters[20] = 65;                   letters[40] = 87; letters[52] = 97;
	letters[9]  = 95; letters[21] = 95;                   letters[41] = 87; letters[53] = 95;
	letters[10] = 57; letters[22] = 69;                   letters[42] = 91; letters[54] = 101;
	letters[11] = 95; letters[23] = 95;                   letters[43] = 87; letters[55] = 95;

	f.z = 0.0f;

	for (int i = 0; i < size; i += 4) {
		Vector begin = benchmark_pixar_raytracer_multiply_float((Vector){ letters[i] - 79.0f, letters[i + 1] - 79.0f, 0.0f }, 0.5f);
		Vector e = benchmark_pixar_raytracer_add(benchmark_pixar_raytracer_multiply_float((Vector){ letters[i + 2] - 79.0f, letters[i + 3] - 79.0f, 0.0f }, 0.5f), benchmark_pixar_raytracer_multiply_float(begin, -1.0f));
		Vector o = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(f, benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(begin, e), benchmark_pixar_raytracer_min(-benchmark_pixar_raytracer_min(benchmark_pixar_raytracer_modulus(benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(begin, f), -1.0f), e) / benchmark_pixar_raytracer_modulus_self(e), 0.0f), 1.0f))), -1.0f);

		distance = benchmark_pixar_raytracer_min(distance, benchmark_pixar_raytracer_modulus_self(o));
	}

	distance = sqrtf(distance);

	Vector curves[2] = { 0 };

	curves[0] = (Vector){ -11.0f, 6.0f, 0.0f };
	curves[1] = (Vector){ 11.0f, 6.0f, 0.0f };

	for (int i = 2; i > 0; i--) {
		Vector o = benchmark_pixar_raytracer_add(f, benchmark_pixar_raytracer_multiply_float(curves[i], -1.0f));
		float m = 0.0f;

		if (o.x > 0.0f) {
			m = fabsf(sqrtf(benchmark_pixar_raytracer_modulus_self(o)) - 2.0f);
		} else {
			if (o.y > 0.0f)
				o.y += -2.0f;
			else
				o.y += 2.0f;

			o.y += sqrtf(benchmark_pixar_raytracer_modulus_self(o));
		}

		distance = benchmark_pixar_raytracer_min(distance, m);
	}

	distance = powf(powf(distance, 8.0f) + powf(position.z, 8.0f), 0.125f) - 0.5f;
	*hitType = PIXAR_RAYTRACER_LETTER;

	float roomDistance = benchmark_pixar_raytracer_min(-benchmark_pixar_raytracer_min(benchmark_pixar_raytracer_box_test(position, (Vector){ -30.0f, -0.5f, -30.0f }, (Vector){ 30.0f, 18.0f, 30.0f }), benchmark_pixar_raytracer_box_test(position, (Vector){ -25.0f, -17.5f, -25.0f }, (Vector){ 25.0f, 20.0f, 25.0f })), benchmark_pixar_raytracer_box_test((Vector){ fmodf(fabsf(position.x), 8), position.y, position.z }, (Vector){ 1.5f, 18.5f, -25.0f }, (Vector){ 6.5f, 20.0f, 25.0f }));

	if (roomDistance < distance) {
		distance = roomDistance;
		*hitType = PIXAR_RAYTRACER_WALL;
	}

	float sun = 19.9f - position.y;

	if (sun < distance) {
		distance = sun;
		*hitType = PIXAR_RAYTRACER_SUN;
	}

	return distance;
}

static int benchmark_pixar_raytracer_ray_marching(Vector origin, Vector direction, Vector* hitPosition, Vector* hitNormal) {
	int hitType = PIXAR_RAYTRACER_NONE;
	int noHitCount = 0;
	float distance = 0.0f;

	for (float i = 0; i < 100; i += distance) {
		*hitPosition = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(origin, direction), i);
		distance = benchmark_pixar_raytracer_sample(*hitPosition, &hitType);

		if (distance < 0.01f || ++noHitCount > 99) {
			*hitNormal = benchmark_pixar_raytracer_inverse((Vector){ benchmark_pixar_raytracer_sample(benchmark_pixar_raytracer_add(*hitPosition, (Vector){ 0.01f, 0.0f, 0.0f }), &noHitCount) - distance, benchmark_pixar_raytracer_sample(benchmark_pixar_raytracer_add(*hitPosition, (Vector){ 0.0f, 0.01f, 0.0f }), &noHitCount) - distance, benchmark_pixar_raytracer_sample(benchmark_pixar_raytracer_add(*hitPosition, (Vector){ 0.0f, 0.0f, 0.01f }), &noHitCount) - distance });

			return hitType;
		}
	}

	return PIXAR_RAYTRACER_NONE;
}

static Vector benchmark_pixar_raytracer_trace(Vector origin, Vector direction) {
	Vector
		sampledPosition = { 1.0f, 1.0f, 1.0f },
		normal = { 1.0f, 1.0f, 1.0f },
		color = { 1.0f, 1.0f, 1.0f },
		attenuation = { 1.0f, 1.0f, 1.0f },
		lightDirection = benchmark_pixar_raytracer_inverse((Vector){ 0.6f, 0.6f, 1.0f });

	for (int bounceCount = 3; bounceCount > 0; bounceCount--) {
		int hitType = benchmark_pixar_raytracer_ray_marching(origin, direction, &sampledPosition, &normal);

		switch (hitType) {
			case PIXAR_RAYTRACER_NONE:
				break;

			case PIXAR_RAYTRACER_LETTER: {
				direction = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(direction, normal), benchmark_pixar_raytracer_modulus(normal, direction) * -2.0f);
				origin = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(sampledPosition, direction), 0.1f);
				attenuation = benchmark_pixar_raytracer_multiply_float(attenuation, 0.2f);

				break;
			}

			case PIXAR_RAYTRACER_WALL: {
				float
					incidence = benchmark_pixar_raytracer_modulus(normal, lightDirection),
					p = 6.283185f * benchmark_pixar_raytracer_random(),
					c = benchmark_pixar_raytracer_random(),
					s = sqrtf(1.0f - c),
					g = normal.z < 0 ? -1.0f : 1.0f,
					u = -1.0f / (g + normal.z),
					v = normal.x * normal.y * u;

				direction = benchmark_pixar_raytracer_add(benchmark_pixar_raytracer_add((Vector){ v, g + normal.y * normal.y * u, -normal.y * (cosf(p) * s) }, (Vector){ 1.0f + g * normal.x * normal.x * u, g * v, -g * normal.x }), benchmark_pixar_raytracer_multiply_float(normal, sqrtf(c)));
				origin = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(sampledPosition, direction), 0.1f);
				attenuation = benchmark_pixar_raytracer_multiply_float(attenuation, 0.2f);

				if (incidence > 0 && benchmark_pixar_raytracer_ray_marching(benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(sampledPosition, normal), 0.1f), lightDirection, &sampledPosition, &normal) == PIXAR_RAYTRACER_SUN)
					color = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_multiply(benchmark_pixar_raytracer_add(color, attenuation), (Vector){ 500.0f, 400.0f, 100.0f }), incidence);

				break;
			}

			case PIXAR_RAYTRACER_SUN: {
				color = benchmark_pixar_raytracer_multiply(benchmark_pixar_raytracer_add(color, attenuation), (Vector){ 50.0f, 80.0f, 100.0f });

				goto escape;
			}
		}
	}

	escape:

	return color;
}

EXPORT float benchmark_pixar_raytracer(uint32_t width, uint32_t height, uint32_t samples) {
	marsagliaZ = 666;
	marsagliaW = 999;

	Vector position = { -22.0f, 5.0f, 25.0f };
	Vector goal = { -3.0f, 4.0f, 0.0f };

	goal = benchmark_pixar_raytracer_add(benchmark_pixar_raytracer_inverse(goal), benchmark_pixar_raytracer_multiply_float(position, -1.0f));

	Vector left = { goal.z, 0, goal.x };

	left = benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_inverse(left), 1.0f / width);

	Vector up = benchmark_pixar_raytracer_cross(goal, left);
	Vector color = { 0 };
	Vector o = { 0 };

	for (uint32_t y = height; y > 0; y--) {
		for (uint32_t x = width; x > 0; x--) {
			for (uint32_t p = samples; p > 0; p--) {
				color = benchmark_pixar_raytracer_add(color, benchmark_pixar_raytracer_trace(position, benchmark_pixar_raytracer_add(benchmark_pixar_raytracer_inverse(benchmark_pixar_raytracer_multiply_float(benchmark_pixar_raytracer_add(goal, left), x - width / 2 + benchmark_pixar_raytracer_random())), benchmark_pixar_raytracer_multiply_float(up, y - height / 2 + benchmark_pixar_raytracer_random()))));
			}

			color = benchmark_pixar_raytracer_multiply_float(color, (1.0f / samples) + 14.0f / 241.0f);
			o = benchmark_pixar_raytracer_add_float(color, 1.0f);
			color = (Vector){
				color.x / o.x,
				color.y / o.y,
				color.z / o.z
			};

			color = benchmark_pixar_raytracer_multiply_float(color, 255.0f);
		}
	}

	return color.x + color.y + color.z;
}