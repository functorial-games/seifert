# Seifert

A small interactive experiment: three independently twistable blocks arranged
left–middle–right in the positive `(x,y)` quadrant, with a ribbon joining
each outer block to the middle block.

The first implementation follows the architecture of
[functorial-games/spinor](https://github.com/functorial-games/spinor):
an Idriç semantic sketch, host-tested C geometry, a thin Android NativeActivity
adapter, and a GLES2 renderer. The geometry is independently implemented.

**First slice:** pick any block and drag horizontally to twist it about the
axis of its connecting ribbon. Both ribbons are recomputed from the three
signed, unwrapped block angles. Release holds the pose; reverse dragging
reverses the twist.

This is a ribbon-twist playground, **not** yet a Seifert-surface solver or a
Dirac belt-trick contraction. In particular, a 2π endpoint turn leaves a full
ribbon twist, instead of claiming that the entire field has returned.

See `android/README.md` for build/interaction and `native/README.md` for
geometry and host acceptance once those files are in place.

## Lineage

Jason Hise's belt-trick/antitwister visualizations are a principal visual
inspiration, as credited and researched in the upstream Spinor repository.
No Hise source code is included.
