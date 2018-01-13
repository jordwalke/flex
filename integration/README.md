
This integration code is the C that creates a library that makes ReLayout act
as a drop in replacement for Yoga library.
It is not built by default, and the API to Yoga might have changed, but this
acts as an example of how to bridge to a system that currently relies on Yoga.
The directories structures have changed, but the Makefile and original C code
are left here as a rough guide about what you need to do to make ReLayout a
drop in replacement for (an older version of) Yoga.
