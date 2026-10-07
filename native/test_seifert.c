#include "seifert.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define TEST_SEGMENTS 32u
#define TEST_VERTICES (SEIFERT_RIBBON_COUNT * (TEST_SEGMENTS + 1u) * 2u)
#define TEST_INDICES (SEIFERT_RIBBON_COUNT * TEST_SEGMENTS * 6u)
#define PI 3.14159265358979323846f

static SeifertVec3 vertices[TEST_VERTICES];
static uint16_t indices[TEST_INDICES];
static SeifertVec3 initial[TEST_VERTICES];
static SeifertVec3 blocks[SEIFERT_BLOCK_VERTEX_COUNT];
static SeifertVec3 initial_blocks[SEIFERT_BLOCK_VERTEX_COUNT];

static int near(float x, float y)
{
    return fabsf(x - y) < 0.0001f;
}

static int same(SeifertVec3 a, SeifertVec3 b)
{
    return near(a.x, b.x) && near(a.y, b.y) && near(a.z, b.z);
}

static int different(SeifertVec3 a, SeifertVec3 b)
{
    return !same(a, b);
}

static void sample(const SeifertScene *scene, SeifertRibbonMesh *mesh)
{
    assert(seifert_sample_ribbons(scene, TEST_SEGMENTS, mesh) == SEIFERT_OK);
    assert(mesh->vertex_count == TEST_VERTICES);
    assert(mesh->index_count == TEST_INDICES);
    for (size_t i = 0u; i < TEST_VERTICES; ++i) {
        assert(isfinite(vertices[i].x));
        assert(isfinite(vertices[i].y));
        assert(isfinite(vertices[i].z));
    }
}

int main(void)
{
    SeifertScene scene;
    seifert_scene_init(&scene);
    assert(seifert_scene_valid(&scene));
    for (unsigned i = 0u; i < SEIFERT_BLOCK_COUNT; ++i) {
        assert(scene.centers[i].x > 0.0f && scene.centers[i].y > 0.0f);
        assert(scene.centers[i].z == 0.0f);
    }

    SeifertRibbonMesh mesh = {
        .positions = vertices, .indices = indices,
        .vertex_capacity = TEST_VERTICES, .index_capacity = TEST_INDICES,
        .vertex_count = 0u, .index_count = 0u
    };
    sample(&scene, &mesh);
    memcpy(initial, vertices, sizeof(initial));
    assert(seifert_sample_blocks(&scene, initial_blocks) == SEIFERT_OK);
    assert(seifert_ribbon_vertex_count(TEST_SEGMENTS) == TEST_VERTICES);
    assert(seifert_ribbon_index_count(TEST_SEGMENTS) == TEST_INDICES);
    assert(seifert_ribbon_vertex_count(0) == 0u);
    assert(seifert_ribbon_vertex_count(SEIFERT_MAX_SEGMENTS + 1u) == 0u);

    /* Turning the left block changes the left root only, not the right ribbon. */
    assert(seifert_scene_turn(&scene, 0u, 0.5f * PI) == SEIFERT_OK);
    sample(&scene, &mesh);
    assert(different(vertices[0], initial[0]));
    assert(same(vertices[2u * TEST_SEGMENTS], initial[2u * TEST_SEGMENTS]));
    for (size_t i = TEST_VERTICES / 2u; i < TEST_VERTICES; ++i) {
        assert(same(vertices[i], initial[i]));
    }
    assert(seifert_sample_blocks(&scene, blocks) == SEIFERT_OK);
    assert(different(blocks[0], initial_blocks[0]));
    for (size_t i = 36u; i < SEIFERT_BLOCK_VERTEX_COUNT; ++i) {
        assert(same(blocks[i], initial_blocks[i]));
    }

    assert(seifert_scene_turn(&scene, 0u, -0.5f * PI) == SEIFERT_OK);
    sample(&scene, &mesh);
    for (size_t i = 0u; i < TEST_VERTICES; ++i) {
        assert(same(vertices[i], initial[i]));
    }

    /* Center block twists both attached ribbon endpoints; outer roots stay. */
    assert(seifert_scene_turn(&scene, 1u, 0.5f * PI) == SEIFERT_OK);
    sample(&scene, &mesh);
    assert(same(vertices[0], initial[0]));
    assert(different(vertices[2u * TEST_SEGMENTS], initial[2u * TEST_SEGMENTS]));
    assert(different(vertices[TEST_VERTICES / 2u], initial[TEST_VERTICES / 2u]));
    assert(same(vertices[TEST_VERTICES - 2u], initial[TEST_VERTICES - 2u]));
    assert(seifert_scene_turn(&scene, 1u, -0.5f * PI) == SEIFERT_OK);

    /* Right block has the complementary independent effect. */
    assert(seifert_scene_turn(&scene, 2u, PI) == SEIFERT_OK);
    sample(&scene, &mesh);
    for (size_t i = 0u; i < TEST_VERTICES / 2u; ++i) {
        assert(same(vertices[i], initial[i]));
    }
    assert(different(vertices[TEST_VERTICES - 2u], initial[TEST_VERTICES - 2u]));
    assert(seifert_scene_turn(&scene, 2u, -PI) == SEIFERT_OK);

    /* A full block turn returns the cube but leaves a visible ribbon twist. */
    assert(seifert_scene_turn(&scene, 0u, 2.0f * PI) == SEIFERT_OK);
    sample(&scene, &mesh);
    assert(seifert_sample_blocks(&scene, blocks) == SEIFERT_OK);
    for (size_t i = 0u; i < SEIFERT_BLOCK_VERTEX_COUNT; ++i) {
        assert(same(blocks[i], initial_blocks[i]));
    }
    assert(different(vertices[TEST_SEGMENTS], initial[TEST_SEGMENTS]));
    assert(seifert_scene_turn(&scene, 0u, -2.0f * PI) == SEIFERT_OK);
    sample(&scene, &mesh);
    for (size_t i = 0u; i < TEST_VERTICES; ++i) {
        assert(same(vertices[i], initial[i]));
    }

    /* Fail closed for invalid indices, angles, scene and buffer capacities. */
    assert(seifert_scene_turn(&scene, 3u, 1.0f) == SEIFERT_INVALID_ARGUMENT);
    assert(seifert_scene_turn(&scene, 0u, NAN) == SEIFERT_INVALID_ARGUMENT);
    float new_angles[3] = {1.0f, NAN, 2.0f};
    assert(seifert_scene_set_angles(&scene, new_angles) == SEIFERT_INVALID_ARGUMENT);
    assert(near(scene.angles[0], 0.0f));
    new_angles[1] = -3.0f;
    assert(seifert_scene_set_angles(&scene, new_angles) == SEIFERT_OK);
    assert(near(scene.angles[0], 1.0f));
    seifert_scene_init(&scene);
    mesh.vertex_capacity = TEST_VERTICES - 1u;
    mesh.vertex_count = 17u;
    assert(seifert_sample_ribbons(&scene, TEST_SEGMENTS, &mesh) == SEIFERT_BUFFER_TOO_SMALL);
    assert(mesh.vertex_count == 17u);
    mesh.vertex_capacity = TEST_VERTICES;
    assert(seifert_sample_ribbons(&scene, 0u, &mesh) == SEIFERT_LIMIT);
    scene.centers[1].y += 0.2f;
    assert(seifert_sample_ribbons(&scene, TEST_SEGMENTS, &mesh) == SEIFERT_INVALID_ARGUMENT);

    puts("Seifert host geometry and independent twist tests: PASS");
    return 0;
}
