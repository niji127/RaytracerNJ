#include <fstream>
#include <iostream>
#include <omp.h>
#include "renderer.h"
#include "light.h"
Eigen::Vector3f reflect(Eigen::Vector3f in, Eigen::Vector3f normal)
{
	Eigen::Vector3f wi = in.normalized();
	Eigen::Vector3f n = normal.normalized();
	return wi - 2.0 * wi.dot(n) * n;
}
Eigen::Vector3f refract(Eigen::Vector3f in, Eigen::Vector3f normal, float ratio)
{
	Eigen::Vector3f wi = in.normalized();
	Eigen::Vector3f n = normal.normalized();
	return wi; // -2.0 * wi.dot(n) * n;
}
void Renderer::render()
{
	int width = camera->width;
	int height = camera->height;
	int spp = 100;
	std::cout << "spp=" << spp << std::endl;
	std::vector<Eigen::Vector3f> framebuffer;
	framebuffer.resize(width * height);
	float progress = 0.0;
	omp_set_num_threads(20);
#pragma omp parallel for
	for (int j = 0; j < height; j++)
	{
		if (!(j % 10))
			std::cout << progress++ << "%" << std::endl;
		for (int i = 0; i < width; i++)
		{

			for (int k1 = 0; k1 < spp; k1++)
			{
				for (int k2 = 0; k2 < spp; k2++)
				{
					float ky = k1;
					float kx = k2;
					Eigen::Vector2f pix = {i + 0.5f / spp + ky / spp, height - (j + 0.5f / spp + kx / spp)};
					Ray ray = camera->cast_ray(pix);
					framebuffer[i + width * j] += get_color(ray, 0) / spp / spp;
				}
			}
		}
	}

	std::ofstream ofs; // 把帧缓存保存到文件中
	ofs.open("../out/out.ppm", std::ofstream::out | std::ofstream::binary);
	ofs << "P6\n"
		<< width << " " << height << "\n255\n";
	for (size_t i = 0; i < height * width; ++i)
	{
		for (size_t j = 0; j < 3; j++)
		{
			ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
		}
	}
	// float i = height / 2;
	// float j = width / 2;
	// Eigen::Vector2f pix = { j,i };
	// Ray ray = camera->cast_ray(pix);
	// Intersect inter = get_intersect(ray, this->bvh_root);
}

Intersect Renderer::get_intersect(const Ray &ray, BVH *bvh)
{
	if (bvh == nullptr)
		return Intersect();
	if (!bvh->is_intersect(ray))
		return Intersect();
	if (bvh->left_node != nullptr || bvh->right_node != nullptr)
	{
		Intersect int_left = get_intersect(ray, bvh->left_node);
		Intersect int_right = get_intersect(ray, bvh->right_node);
		if (int_left.dis < int_right.dis)
			return int_left;
		else
			return int_right;
	}
	else
	{
		Intersect inter_tri;
		for (auto iter : bvh->triangle_list)
		{
			Intersect t = iter->get_intersect(ray);
			if (t.dis < inter_tri.dis)
			{
				inter_tri = t;
			}
		}
		return inter_tri;
	}
}

Eigen::Vector3f Renderer::get_color(const Ray &ray, int depth)
{
	Intersect inter_p = get_intersect(ray, bvh_root);
	Eigen::Vector3f Light_dir = {0.0f, 0.0f, 0.0f};
	if (inter_p.happened == false)
		return Light_dir;
	if (inter_p.material.m_type == MaterialType::LIGHT)
	{
		if (depth == 0)
			return inter_p.material.m_emission;
		else
			return Light_dir;
	}
	if (inter_p.material.m_type == MaterialType::REFLECT)
	{
		Eigen::Vector3f Light_reflect{0.0f, 0.0f, 0.0f};
		if (get_random_float() < scene->RussianRoulette)
		{
			Eigen::Vector3f reflect_dir = reflect(ray.direction, inter_p.normal);
			Ray ray_reflect(inter_p.xyz, reflect_dir);
			// Intersect inter_reflect = get_intersect(ray_reflect, bvh_root);
			Light_reflect = get_color(ray_reflect, 0) / scene->RussianRoulette;
			// Light_reflect = get_color(ray_reflect, 0);// / scene->RussianRoulette;
			// printf("int\n");
		}
		return Light_reflect;
	}
	if (inter_p.material.m_type == MaterialType::TRANSPARENT)
	{
		Eigen::Vector3f Light_refract{0.0f, 0.0f, 0.0f};
		if (get_random_float() < scene->RussianRoulette)
		{
			Eigen::Vector3f refract_in_dir = refract(ray.direction, inter_p.normal, 1.5);
			Ray ray_refract_in(inter_p.xyz + 0.001 * refract_in_dir, refract_in_dir);
			Intersect inter_refract = get_intersect(ray_refract_in, bvh_root);
			Eigen::Vector3f refract_out_dir = refract(ray_refract_in.direction, -inter_refract.normal, 1.5);
			Ray ray_refract_out(inter_refract.xyz + 0.001 * refract_out_dir, refract_out_dir);
			Light_refract = get_color(ray_refract_out, 0) / scene->RussianRoulette;
			// Light_reflect = get_color(ray_reflect, 0);// / scene->RussianRoulette;
			// printf("int\n");
		}
		return Light_refract;
	}

	Intersect inter_light;
	float pdf_light = scene->sample_light(inter_light);

	Eigen::Vector3f x = inter_light.xyz;
	Eigen::Vector3f NN = inter_light.normal;
	Eigen::Vector3f emit = inter_light.material.m_emission;

	Eigen::Vector3f p = inter_p.xyz;
	Eigen::Vector3f N = inter_p.normal;
	Eigen::Vector3f wo = ray.direction;
	Eigen::Vector3f ws = (x - p).normalized();

	Ray ray_p2x(p, ws);
	Intersect inter_p2x = get_intersect(ray_p2x, bvh_root);
	float dis = (x - p).norm();
	if (inter_p2x.dis > dis - EPS)
	{
		if (ws.dot(N) > 0.0f)
		{
			Light_dir = emit.cwiseProduct(inter_p.material.m_color) / PI * ws.dot(N) * (-ws.dot(NN)) / dis / dis / pdf_light;
		}
	}
	// return { 0.0f,0.0f,0.0f };

	Eigen::Vector3f Light_indir{0.0f, 0.0f, 0.0f};
	if (get_random_float() < scene->RussianRoulette)
	{
		Eigen::Vector3f wi = sample(N);
		Ray ray_indir(p, wi);
		// Intersect inter_indir = get_intersect(ray_indir,bvh_root);
		Light_indir = get_color(ray_indir, depth + 1).cwiseProduct(inter_p.material.m_color) / PI * wi.dot(N) / (0.5f / PI) / scene->RussianRoulette;
	}
	return Light_dir + Light_indir;
	// return inter.material.m_color;
}