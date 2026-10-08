# Seifert mathematical reading shelf

Bibliographic **metadata and source links only**, not redistributed books or scans.
The books.bib and papers.bib files provide stable citation keys for subsequent
Idriç function definitions and mathematical UI labels.

## Books

| Citation key | Work | Use in Seifert |
| --- | --- | --- |
| rolfsen2003 | Dale Rolfsen, *Knots and Links* (1976; AMS Chelsea 2003 reprint) | Knot/link topology, Seifert surfaces, linking, knot groups, Alexander invariants. Best initial geometry reference. |
| morishita2012 | Masanori Morishita, *Knots and Primes: An Introduction to Arithmetic Topology*, 1st ed. (2012) | Linking numbers vs. Legendre symbols; knot vs. Galois groups; Milnor vs. higher residue symbols. |
| morishita2024 | Morishita, *Knots and Primes*, 2nd ed. (2024) | Updated arithmetic topology; adds class field theory and Dijkgraaf–Witten material. |
| milnor1968 | John Milnor, *Singular Points of Complex Hypersurfaces* (1968) | Milnor fibration, links of singularities, fiber surfaces. |

## Papers

| Citation key | Work | Possible application |
| --- | --- | --- |
| seifert1935genus | Herbert Seifert, *Über das Geschlecht von Knoten* (1935) | Spanning surfaces and genus for certified topology. |
| seifert1935cyclic | Seifert, *Die Verschlingungsinvarianten der zyklischen Knotenüberlagerungen* (1935/36) | Knot invariants and cyclic coverings. |
| alexander1928 | J. W. Alexander, *Topological Invariants of Knots and Links* (1928) | Alexander polynomial for closed links. |
| milnor1954 | John Milnor, *Link Groups* (1954) | Link homotopy and link groups. |
| dennis2005 | M. R. Dennis and J. H. Hannay, *Geometry of Călugăreanu's theorem* (2005) | Linking = twist + writhe for appropriate **closed framed ribbons**. |
| ghys2007 | Étienne Ghys, *Knots and Dynamics* (ICM, 2007) | Knotted periodic orbits, dynamics and linking. |

## What could eventually appear on screen?

| Future operation | Grounding | Preconditions | Live display now? |
| --- | --- | --- | --- |
| relativeEndpointTurns | Current scene's two unwrapped endpoint angles | Both block angles available | **Yes**, but display as *relative commanded turns*, not a topological invariant. |
| ribbonTwist | dennis2005 | Regular centerline and continuous, transverse unit framing | Later: geometric measurement. |
| writhe | dennis2005 | Regular centerline, validated quadrature and self-contact policy | Later: geometric measurement; not a knot invariant. |
| linkingNumber | rolfsen2003, dennis2005 | **Disjoint closed oriented curves** | **No** for the current two open ribbons. |
| seifertGenus | seifert1935genus, rolfsen2003 | Certified connected orientable surface, Euler characteristic and boundary count | Not yet. |
| alexanderPolynomial | alexander1928, rolfsen2003 | Closed knot/link algebra, e.g. verified Seifert matrix | Not yet. |
| linkingResidueAnalogy | morishita2012, morishita2024 | Explicit number-theoretic inputs and a valid analogy | Not determined by a finger drag. |

The current geometry has **three independently rotating blocks and two open ribbons**.
The unwrapped angles preserve history: one full endpoint rotation can leave a
visible ribbon twist even when the cube's orientation returns. This makes
relative commanded turns useful. It does **not** certify closed curves, a
Seifert surface, conserved linking numbers, or non-self-intersection.

## Source and implementation policy

- Check in metadata and links; do not commit copyrighted volumes or scans.
- Publisher links may require paid or institutional access.
- Each new Idriç definition should cite a key and state **input preconditions**.
- If those preconditions are not yet met, a proposed invariant is *unavailable*,
  not zero. Reserve the word *invariant* for quantities that actually are one.
