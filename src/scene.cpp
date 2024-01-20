#include "scene.h"
#include "global.h"
float Scene::sample_light(Intersect& pos)
{
	float emit_area_sum = 0;
	for (int k = 0; k < object_list.size(); ++k) {
		if (object_list[k]->material.m_type==MaterialType::LIGHT) {
			emit_area_sum += object_list[k]->area;
		}
	}
	float p = get_random_float() * emit_area_sum;
	float emit_area = 0;
	for (int k = 0; k < object_list.size(); ++k) {
		if (object_list[k]->material.m_type == MaterialType::LIGHT) {
			for (int i = 0; i < object_list[k]->triangle_num; i++) {
				emit_area+= object_list[k]->triangle_list[i]->area;
				if (p <= emit_area) {
					object_list[k]->triangle_list[i]->sample(pos);
					return 1.0f / emit_area_sum;
				}
			}
		}
	}
	return 0.0f;
}