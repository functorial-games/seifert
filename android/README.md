# Android three-block playground

The first implementation inherits the app architecture of
[functorial-games/spinor](https://github.com/functorial-games/spinor):

```text
types/Seifert.idric         semantic/type sketch (not executable yet)
       |
native/seifert.[ch]        host-tested C geometry and independent angles
       |
android/seifert_android.c NativeActivity, pointer IDs, lifecycle, saved state
       |
android/seifert_renderer.c GLES2 drawing and projected picking only
       |
android-NDK packager       canonical signed NativeActivity APK
```

Three colored cubes are arranged along a line in the positive x-y quadrant.
A wide orange band joins the left cube to the middle cube. A blue band joins
the middle cube to the right. The *ribbon direction* is the x-axis; twisting
rotates an individual cube about that axis.

**Control:** touch one cube and drag horizontally. One short-screen-width drag
produces a 2π turn. Release leaves the three angles unchanged. Grab another
cube to turn it without resetting the first; reverse dragging unwinds it.
The middle cube changes both ribbons because each has an endpoint there.
Touching the background does not alter any angle. Camera is fixed.

The Android app uses no DEX, no application Java/Kotlin, EGL and OpenGL ES 2.
It saves all three unwrapped angles across Activity state restoration and
retains them across ordinary window/surface recreation.

## Build and artifact policy

Like Spinor, this leaf build is ABI-parameterized. Cat Food owns target
facts; Flexible Pipes owns the paired MIRO A1/C67 orchestration.
A1 (`armeabi-v7a`) is the priority if only one ABI can be produced.

Host tests:
```sh
sh native/test-host.sh
```

Produce the native library:
```sh
ANDROID_ABI=armeabi-v7a ANDROID_NDK_HOME=/absolute/android-ndk-r27c \
  bash android/build-native.sh
```

Package with the canonical
[`isomorphisms/android-NDK`](https://github.com/isomorphisms/android-NDK)
NativeActivity packager. Set `ANDROID_NDK_CHECKOUT` and the
`SEIFERT_KEYSTORE`, `SEIFERT_KEYSTORE_TYPE`, `SEIFERT_KEY_ALIAS`,
`SEIFERT_STORE_PASSWORD`, `SEIFERT_KEY_PASSWORD` and
`SEIFERT_EXPECTED_CERT_SHA256` signer variables, then run:
```sh
ANDROID_ABI=armeabi-v7a bash android/build.sh
```

The GitHub Actions APK workflow follows Spinor's paired A1/C67 matrix and
uploads signed test APKs plus signing receipts. A successful host test or
cross-compile is **not** evidence of physical-device launch or touch success.

## Deferred

No camera orbit, physics cloth, knot/link invariants, Seifert-surface
construction, field contraction, or automatic 4π untangling in this slice.
