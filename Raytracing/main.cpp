#include <iostream>
#include <limits>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"


const unsigned int width = 200;
const unsigned int height = 100;
const unsigned int channels = 3;
const unsigned int samples = 100;

double random()
{
	return (double)rand() / RAND_MAX;
}

vec3 color(const ray& r, hitable* world)
{
	hit_record rec;
	if (world->hit(r, 0.0f, std::numeric_limits<float>::max(), rec))
	{
		return 0.5f * vec3(rec.normal.x() + 1.0f, rec.normal.y() + 1.0f, rec.normal.z() + 1.0f);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}	
}

int main()
{
	hitable* list[2];
	list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);

	hitable* world = new hitable_list(list, 2);
	camera cam;

	unsigned char data[width * height * 3];
	unsigned idx = 0;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < samples; ++s)
			{
				float u = float(x + random() - 0.5f) / float(width);
				float v = float(y + random() - 0.5f) / float(height);
				ray r = cam.get_ray(u, v);
				col += color(r, world);
			}
			col /= float(samples);

			int ir = int(255.99f * col[0]);
			int ig = int(255.99f * col[1]);
			int ib = int(255.99f * col[2]);

			data[3 * (y * width + x) + 0] = ir;
			data[3 * (y * width + x) + 1] = ig;
			data[3 * (y * width + x) + 2] = ib;
		}
	}

	stbi_flip_vertically_on_write(true);
	stbi_write_png("output.png", width, height, channels, data, width * channels);

	return 0;
}