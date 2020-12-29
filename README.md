# al5poly-color-test

![alt text](https://github.com/bambams/al5poly-color-test/raw/main/al5poly-color-test.png "Brandon's Result")

This repo has a submodule at deps\libal5poly. You will need to sync that
in order to compile this project. My submodule fu is still weak, but I
think something like `git clone --recursive' would do the job. Or there's
`git submodule update' if you've already cloned, I think.

The Makefile is based upon Edgar Reynaldo's MinGW binaries. Adjust as
necessary for your environment.

The sample program itself draws 3 circles. They are intended to all be
brown. Before drawing the brown circle though I draw a red one. That way
if brown isn't loaded as expected then you can visually see where it
should have been (to rule out other bugs in the sample).

Read src/main.cpp to understand what it's doing, and check out the
fprintf logs to better understand. To be clear, this is likely a bug in
libal5poly or in this program, not in Allegro, but I am currently at a
loss for finding it. :)
