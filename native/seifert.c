#include "seifert.h"

#include <math.h>

#define SEIFERT_MAX_ANGLE 100000.0f

static int finite_angle(float angle)
{
    return isfinite(angle) && fabsf(angle) <= SEIFERT_MAX_ANGLE;
}

void seifert_scene_init(SeifertScene *scene)
{
    if (scene == NULL) {
        return;
    }

    scene->centers[0] = (SeifertVec3){1.15f, 1.50f, 0.0f};
    scene->centers[1] = (SeifertVec3){3.50f, 1.50f, 0.0f};
    scene->centers[2] = (SeifertVec3){5.85f, 1.50f, 0.0f};
    scene->block_half_extent = 0.46f;
    scene->ribbon_half_width = 0.30f;
    for (unsigned i = 0u; i < SEIFERT_BLOCK_COUNT; ++i) {
        scene->angles[i] = 0.0f;
    }
}

int seifert_scene_valid(const SeifertScene *scene)
{
    if (scene == NULL ||
        !isfinite(scene->block_half_extent) ||
        !isfinite(scene->ribbon_half_width) ||
        scene->block_half_extent <= 0.0f ||
        scene->ribbon_half_width <= 0.0f ||
        scene->ribbon_half_width > scene->block_half_extent) {
        return 0;
    }

    const float half = scene->block_half_extent;
    const float fixed_y = scene->centers[0].y;
    for (unsigned i = 0u; i < SEIFERT_BLOCK_COUNT; ++i) {
        const SeifertVec3 p = scene->centers[i];
        if (!isfinite(p.x) || !isfinite(p.y) || !isfinite(p.z) ||
            p.x <= half || p.y <= 1.42f * half || p.z != 0.0f ||
            p.y != fixed_y || !finite_angle(scene->angles[i])) {
            return 0;
        }
        if (i > 0u &&
            p.x - scene->centers[i - 1u].x <= 2.0f * half) {
            return 0;
        }
    }
    return 1;
}

SeifertStatus seifert_scene_turn(
    SeifertScene *scene, unsigned block_index, float delta_radians
)
{
    if (scene == NULL || block_index >= SEIFERT_BLOCK_COUNT ||
        !finite_angle(delta_radians) || !seifert_scene_valid(scene)) {
        return SEIFERT_INVALID_ARGUMENT;
    }

    const float next = scene->angles[block_index] + delta_radians;
    if (!finite_angle(next)) {
        return SEIFERT_LIMIT;
    }
    scene->angles[block_index] = next;
    return SEIFERT_OK;
}

SeifertStatus seifert_scene_set_angles(
    SeifertScene *scene, const float angles[SEIFERT_BLOCK_COUNT]
)
{
    if (scene == NULL || angles == NULL || !seifert_scene_valid(scene)) {
        return SEIFERT_INVALID_ARGUMENT;
    }
    for (unsigned i = 0u; i < SEIFERT_BLOCK_COUNT; ++i) {
        if (!finite_angle(angles[i])) {
            return SEIFERT_INVALID_ARGUMENT;
        }
    }
    for (unsigned i = 0u; i < SEIFERT_BLOCK_COUNT; ++i) {
        scene->angles[i] = angles[i];
    }
    return SEIFERT_OK;
}

size_t seifert_ribbon_vertex_count(unsigned segments)
{
    if (segments < 1u || segments > SEIFERT_MAX_SEGMENTS) {
        return 0u;
    }
    return SEIFERT_RIBBON_COUNT * 2u * ((size_t)segments + 1u);
}

size_t seifert_ribbon_index_count(unsigned segments)
{
    if (segments < 1u || segments > SEIFERT_MAX_SEGMENTS) {
        return 0u;
    }
    return SEIFERT_RIBBON_COUNT * 6u * (size_t)segments;
}

SeifertStatus seifert_sample_ribbons(
    const SeifertScene *scene, unsigned segments, SeifertRibbonMesh *mesh
)
{
    if (!seifert_scene_valid(scene) || mesh == NULL ||
        mesh->positions == NULL || mesh->indices == NULL) {
        return SEIFERT_INVALID_ARGUMENT;
    }

    const size_t vertex_count = seifert_ribbon_vertex_count(segments);
    const size_t index_count = seifert_ribbon_index_count(segments);
    if (vertex_count == 0u) {
        return SEIFERT_LIMIT;
    }
    if (mesh->vertex_capacity < vertex_count ||
        mesh->index_capacity < index_count) {
        return SEIFERT_BUFFER_TOO_SMALL;
    }

    const float half = scene->block_half_extent;
    const float width = scene->ribbon_half_width;
    for (unsigned ribbon = 0u; ribbon < SEIFERT_RIBBON_COUNT; ++ribbon) {
        const float start_x = scene->centers[ribbon].x + half;
        const float end_x = scene->centers[ribbon + 1u].x - half;
        const float start_angle = scene->angles[ribbon];
        const float end_angle = scene->angles[ribbon + 1u];
        const float center_y = scene->centers[ribbon].y;
        const size_t base = (size_t)ribbon * 2u * ((size_t)segments + 1u);

        for (unsigned step = 0u; step <= segments; ++step) {
            const float u = (float)step / (float)segments;
            const float weight = u * u * (3.0f - 2.0f * u);
            const float angle = start_angle + (end_angle - start_angle) * weight;
            const float cosine = cosf(angle);
            const float sine = sinf(angle);
            const float x = start_x + (end_x - start_x) * u;
            const size_t vertex = base + 2u * (size_t)step;
            mesh->positions[vertex] =
                (SeifertVec3){x, center_y - width * cosine, -width * sine};
            mesh->positions[vertex + 1u] =
                (SeifertVec3){x, center_y + width * cosine, width * sine};
        }

        for (unsigned step = 0u; step < segments; ++step) {
            const size_t offset = ((size_t)ribbon * segments + step) * 6u;
            const uint16_t a = (uint16_t)(base + 2u * step);
            const uint16_t b = (uint16_t)(a + 1u);
            const uint16_t c = (uint16_t)(a + 2u);
            const uint16_t d = (uint16_t)(a + 3u);
            mesh->indices[offset] = a;
            mesh->indices[offset + 1u] = c;
            mesh->indices[offset + 2u] = b;
            mesh->indices[offset + 3u] = b;
            mesh->indices[offset + 4u] = c;
            mesh->indices[offset + 5u] = d;
        }
    }

    mesh->vertex_count = vertex_count;
    mesh->index_count = index_count;
    return SEIFERT_OK;
}

SeifertStatus seifert_sample_blocks(
    const SeifertScene *scene,
    SeifertVec3 positions[SEIFERT_BLOCK_VERTEX_COUNT]
)
{
    if (!seifert_scene_valid(scene) || positions == NULL) {
        return SEIFERT_INVALID_ARGUMENT;
    }

    static const unsigned indices[36] = {
        0, 1, 2, 0, 2, 3,
        4, 6, 5, 4, 7, 6,
        0, 4, 5, 0, 5, 1,
        3, 2, 6, 3, 6, 7,
        0, 3, 7, 0, 7, 4,
        1, 5, 6, 1, 6, 2
    };
    static const int8_t corners[8][3] = {
        {-1, -1, -1}, { 1, -1, -1}, { 1, 1, -1}, {-1, 1, -1},
        {-1, -1,  1}, { 1, -1,  1}, { 1, 1,  1}, {-1, 1,  1}
    };

    for (unsigned block = 0u; block < SEIFERT_BLOCK_COUNT; ++block) {
        const float cosine = cosf(scene->angles[block]);
        const float sine = sinf(scene->angles[block]);
        const SeifertVec3 center = scene->centers[block];
        const float half = scene->block_half_extent;
        for (unsigned i = 0u; i < 36u; ++i) {
            const unsigned corner = indices[i];
            const float x = half * (float)corners[corner][0];
            const float y = half * (float)corners[corner][1];
            const float z = half * (float)corners[corner][2];
            positions[block * 36u + i] = (SeifertVec3){
                center.x + x,
                center.y + cosine * y - sine * z,
                center.z + sine * y + cosine * z
            };
        }
    }
    return SEIFERT_OK;
}
