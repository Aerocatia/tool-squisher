# Tool Squisher

## Shader Squishing

Truncates shader types into 8-bit values. This is used to fix maps compiled
with the leaked 1.10 beta build, enabling them to work with the official
Halo Custom Edition release.

The reason these maps break on official releases is because the leaked 1.10
build reads and writes the shader type enum as an 8-bit value, with the
other 8 bits now padded with a 0xFF byte when stored.

Official releases read these enum fields as 16-bit, thus they will read a
shader value like 0x0005 (shader_transparent_generic) as 0xFF05, leading to
all sorts of fun undefined behavior, like non-shader_environment tags being
read as shader_environment, which leads to a crash.

Note that the shader struct exists in more tags than just shader tags, but
only shader tags are impacted by this.

## Scenario Squishing

Fixes variant numbers in conversations being incorrectly set to 0 when they
are unused. This was due to a bug in the leaked 1.10 beta build's tool.exe
where it didn't initialize the variant numbers to 0xFFFF. This breaks even
on the leaked 1.10 build (thus conversations will ALWAYS be broken on the tag
build which doesn't use cache files that could be fixed with this tool).

## Lens Flare Squishing

Fixes Halo Editing Kit's tool.exe setting rotation scaling to 360 radians,
which is likely higher (~58 times) than what the developer intended, 360
degrees.

## Sound Squishing

Fixes Halo Editing Kit's tool.exe not defaulting sound distance bounds.
This was not done by Xbox tool.exe either, but was stated by the developers
as intended behavior, and was fixed in both the leaked 1.10 build and MCC.

## Weapon HUD Interface Squishing

Fixes Weapon HUD Interface tags causing undefined behavior when they have a
crosshair overlay that is conditionally visible depending on if the weapon
is zoomed in.

The reason this occurs is due to an oversight in tool.exe. When processing
Weapon HUD Interface tags, tool.exe sets a bitfield that maps all crosshair
types, where its respective bit is true if that crosshair type is present
in the tag.

These flags toggle behavior in the overlay, and this includes the ability
to hide/show crosshair overlays depending on if the weapon is zoomed in,
even if that overlay is not a zoom crosshair. The fix is to detect these
overlays and also set the flag if they are present.

For reference, zoom overlays are just for displaying a different bitmap
depending on the current zoom level (e.g. current magnification level, a
smaller crosshair for higher zoom levels, etc.). You may want additional
bitmaps to be shown only when zoomed, but you might want it to be the same
on all zoom levels (otherwise you would have to have multiple copies of the
bitmap to avoid accessing out-of-bounds data). The stock sniper rifle even
does this, and if you were to remove the 2x/8x magnification level and then
build the map with tool.exe, other parts of the HUD will be broken since
the flag won't be set anymore.

## New Enum Squishing

Fixes enums added to Halo's definitions after the release of Halo PC and its
tools (e.g. CEA Unit metagame data and MCC's extra HUD anchors) that have
been corrupted in maps compiled by older versions of tool.exe. When an older
version of tool.exe is used with tags containing this data, the undefined
big-endian field data will be directly copied into the map as is. The fix is
to flip the endianness and check if the flipped value is within the bounds
for the given field. This is safe since corrupt fields will always be largely
out of bounds and can never be ambiguously considered valid.
