# Shader Squisher

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
