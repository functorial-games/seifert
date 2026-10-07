# Host-tested ribbon geometry

`seifert.[ch]` is a small checked C ABI with no Android dependencies.
It owns the three block centers, three independent signed/unwrapped angles,
block rotation, and both ribbon meshes. No renderer/Android data types cross
the boundary.

The reference block centers are (1.15, 1.5), (3.5, 1.5), and (5.85, 1.5)
in the positive x-y quadrant, with z=0. Blocks are 3D cubes whose
centers stay on that 2D line. Their twist axes are parallel to the x-axis,
the direction of each ribbon's centerline.

The two ribbon endpoints are fixed to the facing x-surfaces of their blocks.
For a ribbon between adjacent blocks with unwrapped angles a and b,
the cross-section at u between 0 and 1 is rotated through

```text
theta(u) = (1 - smoothstep(u)) * a + smoothstep(u) * b
smoothstep(u) = u*u*(3 - 2*u)
```

The sampled band is a simple ruled surface. This *does not* implement a
Spin(3) ambient contraction, isotopy, or Seifert-surface computation.
In particular, 2π leaves a full twist in the connecting band. That is
intentional in this initial multi-block experiment.

Buffers are caller-owned; capacity, finite angles, geometry constraints
and a 16-bit index bound are checked before writing. The renderer only
uploads the returned vertices and colors them.

Run `sh native/test-host.sh`. The tests cover the orthant layout,
independent end effects, both ribbons responding to the middle block,
untouched far endpoints, reversibility, 2π cube return with ribbon twist,
and invalid input/capacity failures.
