# Tool Squisher

Tool Squisher fixes issues in Halo PC and Halo Custom Edition map files caused
by bugs in the `tool build-cache-file` process. It also fixes newer data fields
added to the game not present in the older versions of tool.exe used for
Custom Edition.

Issues fixed by this tool are:

## General
New fields added since MCC CEA are processed correctly so they do not end up
corrupt in the map

Examples are new enums like HUD child anchors, HUD meter `min_alpha` and
actor metagame types

## Bitmap

Bitmaps in Custom Edition MP maps that use direct file offsets in bitmaps.map
will be changed to use indices instead (if possible), to prevent out of bounds
data reads. This condition only happens due to a tool.exe bug when user tags
mismatch the tags in the resource maps. Consider re-building the map with
matching tags if this happens.

## Decal

The `sprite scale bug fix` flag can be used to have tool-squisher calculate the
correct `runtime maximum sprite extent` value. This will fix decal scaling when
sprite bitmaps are used.

## Lens flare
Fixes the Custom Edition HEK tool.exe setting a default rotation scaling to
360 radians instead of 360 degrees.

## Sound
Fixes the Custom Edition HEK tool.exe not setting default values for the
distance bounds.

Sounds in Custom Edition MP maps that use direct file offsets in sounds.map
will be changed to use tag paths instead (if possible), to prevent out of bounds
data reads. This condition only happens due to a tool.exe bug when user tags
mismatch the tags in the resource maps. Consider re-building the map with
matching tags if this happens.

## Weapon HUD Interface
Fixes Weapon HUD Interface tags causing undefined behavior when they have a
crosshair overlay that is conditionally visible depending on if the weapon
is zoomed in when the tag does not contain a crosshair type of
zoom_overlay (used to show zoom level).

## Shaders
Fixes the shader type being set in a way that causes the release client to
crash when the map is compiled with 1.10 tool_play.exe/tool_symbols.exe.

## Scenario tag
Fixes ai conversation variant numbers being incorrectly set to 0 when they
are unused in maps compiled with 1.10 tool_play.exe/tool_symbols.exe.
