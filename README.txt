* INTRODUCTION

This is BeagleHash - a family of hash functions developed using
genetic-algorithm techniques.

BeagleHash is named in honor of the the H.M.S. Beagle, the ship
which carried Charles Darwin to the Galapogos Islands.

Since this software was "evolved" using techniques very much like
that of natural selection, it seemed a fitting choice.

* WHAT IS IT FOR

The primary intention of BeagleHash is to provide a hash function
suitable for use in scripting languages such as Perl and other
contexts where there is a single  seed used to hash many keys,
including ones from untrusted sources,  and where there may be
"leakage" of details about how the hash behaves.

* WHAT DOES IT INCLUDE

BeagleHash is one core function exposed as a set of different
functions each intended for a different seed or hash size.

The core routine is the function beagle_hash_64_128_a() which
expects a 128 bit random seed which has been "prepared" for use.
Specifically the seed is expected to have certain properties:

    - Neither the first 64 bits nor the last may be either all
      zeros, nor all ones.
    - There must be a reasonable mix of ones and zeros.

This function is wrapped by various other functions:

    beagle_hash_64_127_a()      beagle_hash_32_127_a()
    beagle_hash_64_112_a()      beagle_hash_32_112_a()
    beagle_hash_64_96_a()       beagle_hash_32_96_a()
    beagle_hash_64_64_a()       beagle_hash_32_64_a()

The _96, _112 and _127 variants are exposed primarily for testing
and are NOT recommended for general use as they include key
preparation as part of their implementation. Instead one should
use the functions

    beagle_hash_seed_prep_127_128_a()
    beagle_hash_seed_prep_112_128_a()
    beagle_hash_seed_prep_96_128_a()
    beagle_hash_seed_prep_64_128_a()
    beagle_hash_seed_prep_32_128_a()

to prepare the seed for use with

    beagle_hash_64_128_a()
    beagle_hash_32_128_a()

both of which do no seed preparation at all.

Only the _64 variant is recommended for use with variable seeds,
as it uses a fast method for ensuring that seed is of sufficient
quality.

* STATUS

This project is a work in progress. It may change at any time until
this notice is removed. Please do not use this code in any production
systems without contacting me first.

* LICENSE

You may use this software either under the LGPL 3.0 or later, or under
express license after contacting the author.

* AUTHOR and COPYRIGHT

Yves Orton wrote and ran the software (currently not included in this
package) that evolved the internals used by BeagleHash, and wrote
the "skeleton" that the genetic-algorithm "filled in".

This software is copyright Yves Orton, 2017.

* ACKNOWLEDGEMENTS

The author would like to thank the following people who have one way or
another influenced the development of this software:

    Damian Gryski - A smart cookie who has interest in this area and who
                    has helped with some of the analysis.
    Brett Mulvey  - Author of http://papa.bretmulvey.com/post/124027987928/hash-functions
    JP Aumasson "veorq" - One of the authors of SipHash which has been very
                    influential in how this function was structured.
    Larry Wall    - Author of Perl, where I got my chops, and why I started
                    on this in the first place.

